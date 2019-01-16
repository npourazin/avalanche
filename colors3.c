#include <stdio.h>

#define KNRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

///FONT COLORS
#define FBLACK      "\033[30;"
#define FRED        "\033[31;"
#define FGREEN      "\033[32;"
#define FYELLOW     "\033[33;"
#define FBLUE       "\033[34;"
#define FPURPLE     "\033[35;"
#define D_FGREEN    "\033[6;"
#define FWHITE      "\033[7;"
#define FCYAN       "\x1b[36m"

///BACKGROUND COLORS
#define BBLACK      "40m"
#define BRED        "41m"
#define BGREEN      "42m"
#define BYELLOW     "43m"
#define BBLUE       "44m"
#define BPURPLE     "45m"
#define D_BGREEN    "46m"
#define BWHITE      "47m"
///END COLOR
#define NONE        "\033[0m"

int main(){

    printf("%s%sred\n",RED,"asb");
    /*//printf("\x1B[31mred\n", RED);
    printf("%sgreen\n", GREEN);
    printf("%syellow\n", YELOW);
    printf("%sblue\n", BLUE);
    printf("%smagenta\n", MAGENTA);
    printf("%scyan\n", CYAN);
    printf("%swhite\n", WHITE);
    printf("%snormal\n", KNRM);*/

    //printf(D_FGREEN BBLUE"%sChange color!\n"NONE, RED);
    printf(D_FGREEN BBLUE"%sChange color!\n"NONE, RED);

    return 0;
}
