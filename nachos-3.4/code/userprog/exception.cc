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

void IncPC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

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

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch(which)
    {
    case NoException:
    	DEBUG('a', "Everything ok!\n");
    	printf("Everything ok!\n");
    	break;
    case SyscallException:
    {
    	switch(type)
    	{
    	case SC_Halt:
    		DEBUG('a', "Shutdown, initiated by user program.\n");
    		printf("Shutdown, initiated by user program.\n");
   			interrupt->Halt();
   			break;
   		case SC_Create:
   			//do something here
   			int virtAddr;
			char* filename;
			
			DEBUG('a',"\n SC_Create call ...");
			DEBUG('a',"\n Reading virtual address of filename");
			
			//check for exceptions
			virtAddr = machine->ReadRegister(4);
			DEBUG ('a',"\n Reading filename.");
			filename = User2System(virtAddr,MaxFileLength+1);
			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				DEBUG('a',"\n Not enough memory in system");
				machine->WriteRegister(2,-1);
				delete filename;
				break;
			}
			
			//DEBUG('a',"\n Finish reading filename.");
			DEBUG('a',"\n File name: '%s'", filename);
			
			if (!fileSystem->Create(filename,0))
			{
				printf("\n Error create file '%s'",filename);
				machine->WriteRegister(2,-1);
				delete filename;
				break;
			}
			
			machine->WriteRegister(2,0);
			delete filename;
   			break;
    	}
    	break;
    }
    case PageFaultException:
    	DEBUG('a', "No valid translation found\n");
    	printf("No valid translation found\n");
    	break;
    case ReadOnlyException:
    	DEBUG('a', "Write attempted to page marked \"read-only\"\n");
    	printf("Write attempted to page marked \"read-only\"\n");
    	break;
    case BusErrorException:
    	DEBUG('a', "Translation resulted in an invalid physical address\n");
    	printf("Translation resulted in an invalid physical address\n");
    	break;
    case AddressErrorException:
    	DEBUG('a', "Unaligned reference or one thatwas beyond the end of the address space\n");
    	printf("Unaligned reference or one thatwas beyond the end of the address space\n");
    	break;
    case OverflowException:
    	DEBUG('a', "Integer overflow in add or sub.\n");
    	printf("Integer overflow in add or sub.\n");
    	break;
    case IllegalInstrException:
    	DEBUG('a', "Unimplemented or reserved instr.\n");
    	printf("Unimplemented or reserved instr.\n");
    	break;
    case NumExceptionTypes:
    	DEBUG('a', "Num Exception Types\n");
    	printf("Num Exception Types\n");
    	break;
    default:
    	printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	IncPC();
    }
}
