#include "syscall.h"
int main()
{
    int x;
    x = ReadInt();
    PrintInt(x);
    Halt();
}

// int main()
// {
//     char c = 'd';
//     c = ReadChar();
//     PrintChar(c);
//     Halt();
// }

// int main()
// {
//     char ch[100];
//     ReadString(ch, 50);
//     PrintString(ch);
//     Halt();
// }

// int main()
// {
//     float a;
//     a = ReadFloat();
//     Halt();
// }