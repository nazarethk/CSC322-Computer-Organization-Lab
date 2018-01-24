#include <stdio.h>
#include <stdlib.h>
#include <str.h>

void converter (char *arg1, char *arg2);
int main(int argc, char **argv) {
    int i;
if (argc < 2) {
        printf("Program expects at least two numbers as inputs\n");
exit (1);
}
if (argc % 2 == 0)
{
printf("Program expects an even number of inputs\n"); exit (1);
}
for (i = 1; i < argc; i+=2)
    converter (argv[i], argv[i+1]);
}
 int htoi(char s[])
{
    int i;
    int c, n;

    n = 0;
    for (i = 0; (c = s[i]) != '\0'; ++i) {
        n *= 16;
        if (i == 0 && c == '0') {

            c = s[++i];
            if (c != 'x' && c != 'X')
                --i;
        } else if (c >= '0' && c <= '9')

            n += c - '0';
        else if (c >= 'a' && c <= 'f')

            n += 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F')

            n += 10 + (c - 'A');
        else //invalid

            return n;
    }
    return n;
}


void converter (char *arg1, char *arg2) {
    int n = htoi(arg1);
    int m = htoi(arg2);
printf("Argument 1: %d\nArgument 2: %d\n", n, m);
}

