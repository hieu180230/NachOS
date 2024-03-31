#include "syscall.h"

int main()
{
    char buffer[100];
    ReadString(buffer, 50);
    PrintString(buffer);
    Halt();
}