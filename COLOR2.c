#include <stdio.h>

#define KNRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

int main(){

    printf("%sred\n", RED);
    //printf("\x1B[31mred\n", RED);
    printf("%sgreen\n", GREEN);
    printf("%syellow\n", YELOW);
    printf("%sblue\n", BLUE);
    printf("%smagenta\n", MAGENTA);
    printf("%scyan\n", CYAN);
    printf("%swhite\n", WHITE);
    printf("%snormal\n", KNRM);

    return 0;
}
