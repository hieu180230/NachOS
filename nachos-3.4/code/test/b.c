#include "syscall.h"
#include "copyright.h"

int main()
{
	int i;
	i = 0;
	for (i; i<1000; i++)
	{
		PrintChar('B');
	}
	return 0;
}
