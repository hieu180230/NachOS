#include "syscall.h"
#include "copyright.h"

int main()
{
	int openFileID;
	int fileSize = 512;
	char c[512];
	PrintString("\n\n----HELP----\n\n");
	openFileID = Open("mota.txt", 1);
	
	if (openFileID != -1)
	{
		
		while (fileSize == 512)
		{
			fileSize = Read(c, 512, openFileID);
			PrintString(c);
		}
		Close(openFileID);
	}
	else
	{ PrintString("\n Cannot open file!\n");}
    return 0;
}
