// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

int MaxFileLength = 32;
// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do
	{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i++;
	}while(i < len && oneChar != 0);
	return i;
}

void ExceptionHandler_ReadInt()
{
	char* buffer;
	int MAX_BUFFER = 255;
	buffer = new char[MAX_BUFFER + 1];
	int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);// doc buffer toi da MAX_BUFFER ki tu, tra ve so ki tu doc dc
	int number = 0; // so luu ket qua tra ve cuoi cung

	/* Qua trinh chuyen doi tu buffer sang so nguyen int */
			
	// Xac dinh so am hay so duong                       
	bool isNegative = false; // Gia thiet la so duong.
	int firstNumIndex = 0;
	if(buffer[0] == '-')
	{
		isNegative = true;
		firstNumIndex = 1;                       			   		
	}
	// Kiem tra tinh hop le cua so nguyen buffer
	for(int i = firstNumIndex; i < numbytes; i++)					
	{
		if(buffer[i] != ' ' && (buffer[i] < '0' && buffer[i] > '9'))
		{
			DEBUG('a', "\n The integer number is not valid");
			machine->WriteRegister(2, 0);
			delete buffer;
			return;
		} 
	}	
	// // La so nguyen hop le, tien hanh chuyen chuoi ve so nguyen
	for(int i = firstNumIndex; i < numbytes; i++)
	{
		number = number * 10 + (int)(buffer[i] - 48); 
	}
	// neu la so am thi * -1;
	if(isNegative)
	{
		number = number * -1;
	}
	machine->WriteRegister(2, number);
	delete buffer;
	return;	
}

void ExceptionHandler_PrintInt()
{
	int number = machine->ReadRegister(4);
	char* buffer = new char[11];
	char * temp = new char[11];
	int sign = 0;
	int len = 0;
	if(number == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	else
	{
		if(number < 0)
		{
			sign = 1;
			number = -number;
			buffer[0] = '-';
			len++;
		}
		while(number > 0)
		{
			temp[len] = number % 10 + '0';
			number /= 10;
			len++;
		}
		for(int i = len - 1; i >= sign; i--)
		{
			buffer[i] = temp[len - i - 1 + sign];
		}
		gSynchConsole->Write(buffer, len);
	}
}


void ExceptionHandler_ReadFloat()
{
	char* buffer;
	int MAX_BUFFER = 255;
	buffer = new char[MAX_BUFFER + 1];
	int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);// doc buffer toi da MAX_BUFFER ki tu, tra ve so ki tu doc dc
	float number = 0; // so luu ket qua tra ve cuoi cung

	int isNegative = 0, isAfterDot = 0, dotPos = 0;
	float temp = 0;
	// // Kiem tra tinh hop le cua so thuc buffer
	if(numbytes > 1 && buffer[0] == '-')
	{
		isNegative = 1;
	}
	for(int i = isNegative; i < numbytes; i++)
	{
		if(isAfterDot)
		{
			if(buffer[i] == '.' && (buffer[i] < '0' && buffer[i] > '9'))
			{
				DEBUG('a', "\n The float number is not valid");
				machine->WriteRegister(2, 0);
				delete buffer;
				return;
			}
			continue;
		}
		if(buffer[i] == '.')
		{
			dotPos = i;
			isAfterDot = 1;
			continue;
		}
		if(buffer[i] < '0' && buffer[i] > '9')
		{
			DEBUG('a', "\n The float number is not valid");
			machine->WriteRegister(2, 0);
			delete buffer;
			return;
		}
	}
	for(int i = isNegative; i < numbytes; i++)
	{
		if(buffer[i] == '.')
		{
			dotPos = i;
			break;
		}
		number = number * 10 + (int)(buffer[i] - 48);
	}
	for(int i = numbytes - 1; i > dotPos; i--)
	{
		temp = temp / 10 + (float)(buffer[i] - 48);
	} 
	temp /= 10;
	number += temp;
	if(isNegative)
	{
		number = -number;
	}
	machine->WriteRegister(2,*(int*)&number);
}

void ExceptionHandler_PrintFloat()
{
	int numberFloat = machine->ReadRegister(4);
	float* formatNumberFloat = (float*)&numberFloat;
	char* buffer = new char[50];
	char* temp = new char[50];
	int numberAfterDotInt = (int)((*formatNumberFloat - (int)*formatNumberFloat) * 10000000);
	int number = (int)*formatNumberFloat;
	int sign = 0;
	int len = 0;
	if(number == 0)
	{
		buffer[0] = '0';
		len++;
	}
	else
	{
		if(number < 0)
		{
			sign = 1;
			number = -number;
			numberAfterDotInt = -numberAfterDotInt;
			buffer[0] = '-';
			len++;
		}
		while(number > 0)
		{
			temp[len] = number % 10 + '0';
			number /= 10;
			len++;
		}
		for(int i = len - 1; i >= sign; i--)
		{
			buffer[i] = temp[len - i - 1 + sign];
		}
	}
	if(numberAfterDotInt != 0)
	{
		buffer[len] = '.';
		len++;
		int tempLen = 0;
		while(numberAfterDotInt > 0)
		{
			temp[tempLen] = numberAfterDotInt % 10 + '0';
			numberAfterDotInt /= 10;
			tempLen++;
		}
		for(int i = tempLen - 1; i >= 0; i--)
		{
			buffer[len] = temp[i];
			len++;
		}
	}
	gSynchConsole->Write(buffer, len);

}

void ExceptionHandler_ReadChar()
{
	char buffer[2];
	int numBytes = gSynchConsole->Read(buffer, 2);

	if(numBytes == 2) //Neu nhap nhieu hon 1 ky tu thi khong hop le
	{
		DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
		machine->WriteRegister(2, 0);
	}
	else if(numBytes == 0) //Ky tu rong
	{
		DEBUG('a', "\nERROR: Ky tu rong!");
		machine->WriteRegister(2, 0);
	}
	else
	{
		//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
		char c = buffer[0];
		machine->WriteRegister(2, c);
		//char d = machine->ReadRegister(2);
	}
}

void ExceptionHandler_PrintChar()
{
	char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi r4
	gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
}

void ExceptionHandler_ReadString()
{
	int virtAddr = machine->ReadRegister(4); // Doc dia chi cua chuoi tu thanh ghi r4
	int length = machine->ReadRegister(5); // Doc do dai cua chuoi tu thanh ghi r5
	char* buffer = new char[length + 1]; // Tao buffer de chua chuoi
	gSynchConsole->Read(buffer, length); // Doc chuoi tu console
	buffer[length] = 0; // Gan ky tu ket thuc chuoi
	System2User(virtAddr, length, buffer); // Copy chuoi tu buffer sang vung nho cua user
	delete buffer; // Xoa buffer
}

void ExceptionHandler_PrintString()
{
	int virtAddr = machine->ReadRegister(4); // Doc dia chi cua chuoi tu thanh ghi r4
	char* buffer = new char[255]; // Tao buffer de chua chuoi
	buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho cua user sang buffer
	gSynchConsole->Write(buffer, strlen(buffer)); // In chuoi tu buffer
	delete buffer; // Xoa buffer
}



// Ham xu ly ngoai le runtime Exception va system call
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch(which)
    {
    case NoException:
    	DEBUG('a', "Everything ok!\n");
    	printf("Everything ok!\n");
    	return;
    case SyscallException:
    {
    	switch(type)
    	{
    	case SC_Halt:
    		DEBUG('a', "Shutdown, initiated by user program.\n");
    		printf("\nShutdown, initiated by user program!.\n");
   			interrupt->Halt();
   			break;
		case SC_ReadInt:
			ExceptionHandler_ReadInt();
			break;
		case SC_PrintInt:
			ExceptionHandler_PrintInt();
			break;
		case SC_ReadFloat:
			ExceptionHandler_ReadFloat();
			break;
		case SC_PrintFloat:
			ExceptionHandler_PrintFloat();
			break;
		case SC_ReadChar:
			ExceptionHandler_ReadChar();
			break;
		case SC_PrintChar:
			ExceptionHandler_PrintChar();
			break;
		case SC_ReadString:
			ExceptionHandler_ReadString();
			break;
		case SC_PrintString:
			ExceptionHandler_PrintString();
			break;
		case SC_CreateFile:
		{
			// Input: Dia chi tu vung nho user cua ten file
			// Output: -1 = Loi, 0 = Thanh cong
			// Chuc nang: Tao ra file voi tham so la ten file
			int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call ...");
			DEBUG('a', "\n Reading virtual address of filename");

			virtAddr = machine->ReadRegister(4); //Doc dia chi cua file tu thanh ghi R4
			DEBUG('a', "\n Reading filename.");
			
			//Sao chep khong gian bo nho User sang System, voi do dang toi da la (32 + 1) bytes
			filename = User2System(virtAddr, MaxFileLength + 1);
			if (strlen(filename) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				//IncreasePC();
				//return;
				break;
			}
			
			if (filename == NULL)  //Neu khong doc duoc
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				delete filename;
				//IncreasePC();
				//return;
				break;
			}
			DEBUG('a', "\n Finish reading filename.");
			
			if (!fileSystem->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
			{
				//Tao file that bai
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				delete filename;
				//IncreasePC();
				//return;
				break;
			}
			
			//Tao file thanh cong
			machine->WriteRegister(2, 0);
			delete filename;
			//IncreasePC(); //Day thanh ghi lui ve sau de tiep tuc ghi
			//return;
			break;
		}

		case SC_Open:
		{
			// Input: arg1: Dia chi cua chuoi name, arg2: type
			// Output: Tra ve OpenFileID neu thanh, -1 neu loi
			// Chuc nang: Tra ve ID cua file.
	 
			//OpenFileID Open(char *name, int type)
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
			int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
			char* filename;
			filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
			//Kiem tra xem OS con mo dc file khong
			
			
			
			if (fileSystem->index <= 9 && fileSystem->index >= 0) //Chi xu li khi khi index thuoc [0, 9]
			{
			
				if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
				{
					
					if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
					{
						machine->WriteRegister(2, fileSystem->index - 1); //tra ve OpenFileID
					}
				}
				else if (type == 2) // xu li stdin voi type quy uoc la 2
				{
					machine->WriteRegister(2, 0); //tra ve OpenFileID
				}
				else // xu li stdout voi type quy uoc la 3
				{
					machine->WriteRegister(2, 1); //tra ve OpenFileID
				}
				delete[] filename;
				IncreasePC();
				return;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file return -1
			
			delete[] filename;
			IncreasePC();
			return;
		}

		case SC_Close:
		{
			//Input id cua file(OpenFileID)
			// Output: 0: thanh cong, -1 that bai
			int fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
			if (fid >= 0 && fid <= 9) //Chi xu li khi fid nam trong [0, 14]
			{
				if (fileSystem->openf[fid]) //neu mo file thanh cong
				{
					delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
					fileSystem->openf[fid] = NULL; //Gan vung nho NULL
					machine->WriteRegister(2, 0);
					IncreasePC();
					break;
				}
			}
			machine->WriteRegister(2, -1);
			IncreasePC();
			break;
		}

		case SC_Read:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte read thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Doc file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 9)
			{
				printf("\nKhong the read vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the read vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop doc file stdout (type quy uoc la 3) thi tra ve -1
			{
				printf("\nKhong the read file stdout.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop doc file stdin (type quy uoc la 2)
			if (fileSystem->openf[id]->type == 2)
			{
				// Su dung ham Read cua lop SynchConsole de tra ve so byte thuc su doc duoc
				int size = gSynchConsole->Read(buf, charcount); 
				System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
				machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
				delete buf;
				IncreasePC();
				return;
			}
			// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				// So byte thuc su = NewPos - OldPos
				NewPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
				System2User(virtAddr, NewPos - OldPos, buf); 
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				// Truong hop con lai la doc file co noi dung la NULL tra ve -2
				//printf("\nDoc file rong.");
				machine->WriteRegister(2, -2);
			}
			delete buf;
			IncreasePC();
			return;
		}

		case SC_Write:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte write thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Ghi file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the write vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("into this?\n");
				printf("\nKhong the write vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Xet truong hop ghi file only read (type quy uoc la 1) hoac file stdin (type quy uoc la 2) thi tra ve -1
			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
			{
				printf("\nKhong the write file stdin hoac file only read.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
				{
					// So byte thuc su = NewPos - OldPos
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					IncreasePC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
				{
					gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole 
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
				machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
				delete buf;
				IncreasePC();
				return;
			}
		}
    	
		case SC_Exec:
		{

			int virtAddr;
			virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
			char* name;
			name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
	
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OpenFile *oFile = fileSystem->Open(name);
			if (oFile == NULL)
			{
				printf("\nExec:: Can't open this file.");
				machine->WriteRegister(2,-1);
				IncreasePC();
				return;
			}

			delete oFile;

			// Return child process id
			int id = pTab->ExecUpdate(name); 
			machine->WriteRegister(2,id);

			delete[] name;	
			IncreasePC();
			return;
		}	
			break;
		case SC_Join:
		{

			int id = machine->ReadRegister(4);
			
			int res = pTab->JoinUpdate(id);
			
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Exit:
		{
			int exitStatus = machine->ReadRegister(4);

			if(exitStatus != 0)
			{
				IncreasePC();
				return;
				
			}			
			
			int res = pTab->ExitUpdate(exitStatus);
			machine->WriteRegister(2, res);

			currentThread->FreeSpace();
			currentThread->Finish();
			IncreasePC();
			return;
		} 
    	IncreasePC();
    	break;
		}
    }
    case PageFaultException:
    	DEBUG('a', "No valid translation found\n");
    	printf("No valid translation found\n");
    	interrupt->Halt();
    	break;
    case ReadOnlyException:
    	DEBUG('a', "Write attempted to page marked \"read-only\"\n");
    	printf("Write attempted to page marked \"read-only\"\n");
    	interrupt->Halt();
    	break;
    case BusErrorException:
    	DEBUG('a', "Translation resulted in an invalid physical address\n");
    	printf("Translation resulted in an invalid physical address\n");
    	interrupt->Halt();
    	break;
    case AddressErrorException:
    	DEBUG('a', "Unaligned reference or one thatwas beyond the end of the address space\n");
    	printf("Unaligned reference or one thatwas beyond the end of the address space\n");
    	interrupt->Halt();
    	break;
    case OverflowException:
    	DEBUG('a', "Integer overflow in add or sub.\n");
    	printf("Integer overflow in add or sub.\n");
    	interrupt->Halt();
    	break;
    case IllegalInstrException:
    	DEBUG('a', "Unimplemented or reserved instr.\n");
    	printf("Unimplemented or reserved instr.\n");
    	interrupt->Halt();
    	break;
    case NumExceptionTypes:
    	DEBUG('a', "Num Exception Types\n");
    	printf("Num Exception Types\n");
    	interrupt->Halt();
    	break;
    default:
		break;
	IncreasePC();
    }

}
