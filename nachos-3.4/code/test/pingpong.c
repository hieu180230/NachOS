#include "syscall.h"
#include "copyright.h"

int main()
{
	int PingID, PongID;
	PrintString("PingPong Test Starting... \n");
	PingID = Exec("./test/a");
	PongID = Exec("./test/b");
	Join(PingID);
	Join(PongID);
	return 0;
}

