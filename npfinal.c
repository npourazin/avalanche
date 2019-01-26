#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#define INTERFACE 0
#define TERMINAL_CONSOLE 0

#define MAX_CHAR_LEN 1000
#define MAX_PROBLEM_N 1000
#define KNRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"
#define BLUE_5 "\x1b[34;5;1m" ///blink
#define BLUE_3 "\x1b[34;3;1m" ///thin font
#define BLUE_7 "\x1b[34;7;1m" ///back
#define BLUE_9 "\x1b[34;9;1m" ///line
#define BLUE_4 "\x1b[34;4;1m" ///underline
#define BLUE_S "\x1b[34;1m"
#define NONE        "\033[0m"

///global variables
int death_flag=0, quit_flag=0;
int game_started=1;
int n=0, num=0;
///structs
struct stats{
    int poeple;
    int court;
    int treasury;
} my;
struct solution{
    char ans[MAX_CHAR_LEN+10];
    int dif[3];
};
struct node{
    int number;
    int index;
    char problem[MAX_CHAR_LEN+10];
    struct solution choice[2];
    struct node* next;
};
struct usrdata{
    char name[MAX_CHAR_LEN+10];
    int pgame; ///is 1 if quit & is 0 if lost
    int problems_left[MAX_PROBLEM_N];
    struct stats now_my;
    int pnumber;
}usr_king;

///funtion prototypes
int min(int, int);
int max(int, int);
void clear();
void mysleep();
void file_name_pre(char*);
void add_front(struct node **, struct node *);
void add_end(struct node *, struct node *);
void deleter(struct node **, struct node *);
void print_list(struct node *);
void print_node_before_selection(struct node * );
void print_stats();
int find_usrs_number();
int cmper(struct usrdata, struct usrdata);
void sort_scoreboard_once(int);
void sort_scoreboard(int);
void show_scoreboard(int);
void show_menu(struct node*);
void print_node_after_selection(struct node**, struct node *, int);
void find_n_all();
struct usrdata get_usr_data();
void set_usr_data();
void set_usr_king(struct node*);
void print_exit_menu();
struct node * get_problem_files();
void random_node(struct node**);
void print_start_game();
void print_tutorial();
void print_break_point();
void save_usr_data();

///needed pre-functions
int min(int a, int b){ return (a<b) ? a : b;}
int max(int a, int b){ return (a>b) ? a : b;}
void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
void mysleep(){ ///sleeps 2sec
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        sleep(1);
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        Sleep(1000);
    #endif
}
///getting question files
void file_name_pre(char st[MAX_CHAR_LEN+10]){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        int len = strlen(st);
        int i=0;
        for(i=len;i>=0;i--){
            st[i+2]=st[i];
        }
        st[0]='.';
        st[1]='/';
        if(st[len+1]==10)
            st[len]=0;
    #endif // defined
}
void add_front(struct node **plist, struct node *new_node){
    ///this function is only called when the list is empty
    new_node->next = *plist;
    new_node->index=0;
    *plist = new_node;
}
void add_end(struct node *list, struct node * new_node){
    ///push_back
    struct node *current;
    for(current = list; current-> next != NULL; current = current->next);
    current->next = new_node;
    new_node->index=current->index+1;
    new_node->next = NULL;
}
void deleter(struct node **list, struct node *o_node){
    #if !INTERFACE
    printf("node deleted\n");
    #endif
    struct node *n1 =NULL ;
    struct node *nn1 = NULL;
    if((*list) == o_node){
        n1 = *list;
        *list = (*list)-> next;
        return;
    }
    nn1 = *list;
    while(nn1!=NULL){
        if(nn1==o_node){
            ///jumps over the head as it was taken care of
            n1->next = nn1->next;
            return;
        }
        n1=nn1;
        nn1=nn1->next;
    }
    return;
}
void print_list(struct node *list){
    struct node * current = list;
    while(current != NULL){
        printf("(%d) Q%d: \n", current->number, current->index);
        current = current->next;
    }
}
void print_node_before_selection(struct node * current){
    #if !INTERFACE
    printf("(%d) Q%d: ", current->number, current->index);
    #endif
    #if TERMINAL_CONSOLE
        printf(" ⚖️  ");
    #endif // TERMINAL_CONSOLE
    printf("%s%s\n",BLUE_3, current->problem);
    printf("%s", KNRM);
    printf("%s[1] %s\n",BLUE_4 ,current->choice[0].ans);
    printf("%s", KNRM);
    printf("%s[2] %s\n",BLUE_4, current->choice[1].ans);
    printf("%s", KNRM);
    printf("%s[3] menu\n\n", CYAN);
    printf("%s", KNRM);
    printf("%s[-1] Exit game!\n\n", RED);
    printf("%s", KNRM);

}
void print_node_after_selection(struct node** plist, struct node * current , int ind){
    if(ind==1 || ind==2){
        #if TERMINAL_CONSOLE
        printf(" ⚖️  ");
        #endif // TERMINAL_CONSOLE
        printf("%s%s\n",BLUE_3, current->problem);
        if(ind==1){
            printf("%s[1] %s\n",BLUE_7 ,current->choice[0].ans);
            printf("%s", KNRM);
            printf("%s[2] %s\n",BLUE_9, current->choice[1].ans);
        }
        if(ind==2){
            printf("%s[1] %s\n",BLUE_9 ,current->choice[0].ans);
            printf("%s", KNRM);
            printf("%s[2] %s\n",BLUE_7, current->choice[1].ans);
        }
        printf("%s", KNRM);
        current->number--; ///is atleast 1

        my.poeple = min(100, (my.poeple+current->choice[ind-1].dif[0]));
        if(my.poeple<=0) {
            my.poeple=0;
            death_flag =1;
        }
        my.court = min(100, (my.court + current->choice[ind -1 ].dif[1]));
        if(my.court<=0){
            my.court=0;
            death_flag =1;
        }
        my.treasury= min(100, (my.treasury + current->choice[ind -1].dif[2]));
        if(my.treasury<=0) {
            my.treasury=0;
            death_flag =1;
        }

        if((my.poeple+ my.court+ my.treasury)/3 <10) death_flag=1;
        if(current->number==0){
            deleter(plist, current);
            n--;
        }
        print_stats();
        #if INTERFACE
        mysleep();
        clear();
        #endif // INTERFACE
    }
    if(ind ==3){
        show_menu(*plist);
    }

}
struct node * get_problem_files(){
    FILE * fp, *fpin; ///whereas fp points to CHOICES.txt and fpin point to the question files. (ci.txt)
    char inname[MAX_CHAR_LEN+10];
    char buf_name[MAX_CHAR_LEN+10];
    char buf1[MAX_CHAR_LEN+10];
    int cnt=0;
    struct node *list = NULL;
    int tmp=0;

    strcpy(inname, "./CHOICES.txt");

    fp = fopen(inname, "r+");
    if(fp==NULL){
        printf("Cannot open file\n");
        return NULL;
    }
    while(fgets(buf_name, MAX_CHAR_LEN, fp) != NULL){
        cnt=0;
        file_name_pre(buf_name);
        fpin = fopen(buf_name, "r+");
        if(fpin==NULL){
            printf("\nCannot open file\n");
            return NULL;
        }
        struct node *new_node = (struct node *)malloc(sizeof(struct node));
        while(fgets(buf1, MAX_CHAR_LEN, fpin) != NULL){
            if(new_node== NULL){
                printf("Cannot create node\n");
                exit(-1);
            }
            switch(cnt%9){ ///set new_node values
                case 0: {
                    strcpy(new_node->problem, buf1);
                    break;
                }
                case 1: {
                    strcpy(new_node->choice[0].ans, buf1);
                    break;
                }
                case 2: {
                    new_node->choice[0].dif[0]=(int)atoi(buf1);
                    break;
                }
                case 3: {
                    new_node->choice[0].dif[1]=(int)atoi(buf1);
                    break;
                }
                case 4: {
                    new_node->choice[0].dif[2]=(int)atoi(buf1);
                    break;
                }
                case 5: {
                    strcpy(new_node->choice[1].ans, buf1);
                    break;
                }
                case 6: {
                    new_node->choice[1].dif[0]=(int)atoi(buf1);
                    break;
                }
                case 7: {
                    new_node->choice[1].dif[1]=(int)atoi(buf1);
                    break;
                }
                case 8: {
                    new_node->choice[1].dif[2]=(int)atoi(buf1);
                    break;
                }
            }
            cnt++;
        }
        new_node->number = 3;
        if(list == NULL)     add_front(&(list), new_node);
        else    add_end(list, new_node);

    }
    fclose(fp);
    return list;
}
void find_n_all(){
    FILE* fp;
    int count =0;
    char buf2[MAX_CHAR_LEN+10];

    fp = fopen("CHOICES.txt", "r+");
    if(fp==NULL){
        printf("Cannot open file\n");
        exit(-1);
    }
    while(fgets(buf2, MAX_CHAR_LEN, fp) != NULL)
        count++;
    #if !INTERFACE
    printf("Number of Lines = %d\n", count);
    #endif
    n = count;
}
///choosing a random questoin
void random_node(struct node** mylist){ ///! : list of cur nodes
    int rand_i=0, i;
    int get_choice=0;
    struct node* pt = NULL;
    if(n==0 || game_started){
        ///filling(if (game_started)) or refilling (if (n==0)) the list
        #if !INTERFACE
        printf("\n\nrefilled\n");
        #endif
        *mylist = get_problem_files();
        if(game_started){
            ///loading the nodes from the previous game
            game_started=0;
            #if !INTERFACE
            for(int k=0;k<n;k++) printf("%d ", usr_king.problems_left[k]);
            printf("\n");
            #endif
             pt = *mylist;
            while(pt->next!=NULL){ ///!
                pt->number = usr_king.problems_left[pt->index];
                if(pt->number==0) {
                    deleter(mylist, pt);
                    n--;
                }
                pt = pt->next;
            }
        }
        if(*mylist== NULL){printf("There seems to be a problem with getting input files!\n"); exit(-1);}

    }
    ///finding a random node from the list
    pt = *mylist;
    srand(time(NULL));
    if(n!=0)
        rand_i = rand()%n;
    if(n==0){
        *mylist = get_problem_files();
        find_n_all();
        rand_i = rand()%n;
    }
    for(i=0;i<rand_i;i++) pt= pt->next;
    print_node_before_selection(pt);

    while(!((get_choice==1)||(get_choice==2) ||(get_choice==-1) || (get_choice==3))){
        if((!(scanf("%d", &get_choice)==1))){
            while ((getchar()) != '\n');
        }
        if(!((get_choice==1)||(get_choice==2) ||(get_choice==-1) || (get_choice==3))){
            printf("%sInvalid input!\nPlease try again!\n", RED);
        }
        printf("%s", KNRM);
    }
    clear();
    if(get_choice==1 || get_choice==2 || get_choice==3) {print_node_after_selection(mylist, pt, get_choice);}
    else  { quit_flag=1;}
}
///scoreboard and status
void print_start_game(){
    clear();
    printf("\n\n");
    printf("            ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️\n\n          ");
    printf("  ☺️      WELCOME    ️  ☺️\n\n          ");
    printf("  ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️ ☺️\n\n          ");
    printf("press ENTER to continue...");
    getchar();
    clear();
}
void print_tutorial(){
    printf("\n--Use your wisdom  to make choices and rule your kingdom!!--\n");
}
void print_break_point(){
    printf("Press ENTER to continue...\n");
    getchar();
    clear();
}
void print_stats(){
    printf("%s", KNRM);
    #if !TERMINAL_CONSOLE
    printf("\n %sPoeple:%d, Court:%d, Treasury:%d\n----------------------------------\n\n%s",GREEN, my.poeple, my.court, my.treasury, KNRM);
    #endif
    #if TERMINAL_CONSOLE
    printf("%s⛹  Poeple:%d%s  ||%s ",GREEN, my.poeple, KNRM, GREEN);
    printf(" ⚜️  Court:%d%s  ||%s  ", my.court, KNRM, GREEN);
    printf(" ⛪  Treasury:%d\n", my.treasury);
    printf("------------------------------------------------------\n\n%s", KNRM);
    #endif // TERMINAL_CONSOLE
    printf("%s", KNRM);
}
int find_usrs_number(){
    FILE* fp;
    int count =0;
    struct usrdata temp;

    fp = fopen("USER_NAMES.bin", "rb+");
    if(fp==NULL){
        printf("Cannot open file\n");
        exit(-1);
    }
    while( fread(&temp, sizeof(struct usrdata), 1, fp) ==1)
        count++;
    #if !INTERFACE
    printf("Number of Users = %d\n", count);
    #endif
    return count;
}
int cmper(struct usrdata a, struct usrdata b){
    if(a.now_my.poeple>b.now_my.poeple) return 1;
    if(a.now_my.poeple==b.now_my.poeple){
        if(a.now_my.court>b.now_my.court) return 1;
        if(a.now_my.court==b.now_my.court){
            if(a.now_my.treasury>b.now_my.treasury) return 1;
            if(a.now_my.treasury==b.now_my.treasury) return 0;
            return -1;
        }
        return -1;
    }
    return -1;
}
void sort_scoreboard_once(int size){
    struct usrdata temp1, temp2;
    FILE* fp;

    fp = fopen("./USER_NAMES.bin", "rb+");
    if(fp==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    for(int i=0; i<size-1;i++){
        fseek(fp, sizeof(struct usrdata)*i, SEEK_SET);
        fread(&temp1, sizeof(struct usrdata), 1, fp);
        fread(&temp2, sizeof(struct usrdata), 1, fp);
        if(cmper(temp1, temp2)==-1) {
            struct usrdata tt;
            tt = temp2;
            temp2 = temp1;
            temp1 = tt;
            fseek(fp, - sizeof(struct usrdata)*2 , SEEK_CUR);
            fwrite(&temp1, sizeof(struct usrdata), 1, fp);
            fwrite(&temp2, sizeof(struct usrdata), 1, fp);
        }
    }
    fclose(fp);
}
void sort_scoreboard(int num1){
    for(int i=0;i<num1;i++)
        sort_scoreboard_once(num1);
}
void show_scoreboard(int num){
    struct usrdata temp;
    int cnt=0;
    FILE* fp;

    fp = fopen("./USER_NAMES.bin", "rb+");
    if(fp==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    while(cnt<num){
        cnt++;
        if( fread(&temp, sizeof(struct usrdata), 1, fp) <1) break;
        #if !INTERFACE
        printf("Name: %s, People: %d,Court: %d,Treasury: %d,", temp.name, temp.now_my.poeple, temp.now_my.court, temp.now_my.treasury);
        printf(" lost or not: %s, n:%d\n", (temp.pgame)? "No": "Yes" , temp.pnumber);
        #endif
        #if INTERFACE
        printf("%sRank %d: %s => %d-%d-%d\n",(strcmp(temp.name, usr_king.name)==0) ? RED : YELOW, cnt, temp.name, temp.now_my.poeple, temp.now_my.court, temp.now_my.treasury);
        printf("%s", KNRM);
        #endif // INTERFACE
    }
    fclose(fp);
}
void show_menu(struct node* plist){
    int get_choice=0;
    char ex_p;
    printf("%sMENU:\n", KNRM);

    printf(" [1] New game");
    #if TERMINAL_CONSOLE
    printf(" ♻️ \n ");
    #endif // TERMINAL_CONSOLE
    printf("[2] Leadership scoreboard");
    #if TERMINAL_CONSOLE
    printf(" ⭐️ \n ");
    #endif // TERMINAL_CONSOLE
    printf("[3] Save");
    #if TERMINAL_CONSOLE
    printf(" ⚡️ \n ");
    #endif // TERMINAL_CONSOLE
    printf("[-1] Exit\nelse, Press any key to resume game.\n");
    if((!(scanf("%d", &get_choice)==1))){
        while ((getchar()) != '\n');
    }
    #if !INTERFACE
    print_list(plist);
    printf("\n");
    #endif
    struct usrdata temp;
    if(get_choice==1){
        #if TERMINAL_CONSOLE
        printf(" ⛔  ");
        #endif // TERMINAL_CONSOLE
        printf("This feature is unfortunatly not available in this version, Stay tuned for further releses!!\n");
      /*  FILE* fp;
        fp = fopen("./USER_NAMES.bin", "rb+");
        if(fp==NULL){
                printf("\nCannot open file\n");
                exit(-1);
        }
        while(1){
            if( fread(&temp, sizeof(struct usrdata), 1, fp) <1) break;
            if(strcmp(usr_king.name, temp.name)==0){
                temp.now_my.poeple=temp.now_my.court= temp.now_my.treasury =50;
                my.poeple=my.treasury=my.court=50;
                temp.pgame= !death_flag;
                for(int i=0;i<MAX_PROBLEM_N;i++){
                    temp.problems_left[i] = (i<n) ? 3 : 0;
                }
                usr_king=temp;
                fseek(fp, -sizeof(struct usrdata), SEEK_CUR);
                fwrite(&temp, sizeof(struct usrdata), 1, fp);
                break;
            }
        }
        for(int i=0;i<10;i++) printf("%d ", usr_king.problems_left[i]);
        printf("\n");
        print_stats();*/
    }
    if(get_choice==2){
        sort_scoreboard(num);
        show_scoreboard(min(10, num));
    }
    if(get_choice ==3){
        set_usr_king(plist);
        print_exit_menu();
        printf("\nExit? [y/n]\n");
        getchar();
        ex_p = getchar();
        if(ex_p =='y'){
            printf("ok! bye\n");
            exit(0);
        }
    }
    if(get_choice==-1){
        print_exit_menu();
        printf("ok! bye\n");
        exit(0);
    }
    printf("Press any key to resume game.\n");
    getchar();
    getchar();
    #if INTERFACE
    clear();
    #endif // INTERFACE
}
void print_exit_menu(){
    if(!death_flag){
        char ans;
        printf("Do you want to save your current game? [y/n]\n");
        getchar();
        ans = getchar();
        if(ans=='y') set_usr_data();
    }
    else{
        #if TERMINAL_CONSOLE
        printf("\n\n\n           ️☠️  ☠️  ☠️ \n ️");
        #endif // TERMINAL_CONSOLE
        printf("         You LOST!!!\n\n\n\n");
        set_usr_data();
    }
}
///getting usr data
struct usrdata get_usr_data(){
    FILE* fp;
    struct usrdata temp, usr1;
    int usr_found=0;
    char name1[MAX_CHAR_LEN];

    fp = fopen("./USER_NAMES.bin", "rb+");
    if(fp==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    printf("Enter your name: ");
    gets(name1);
    while(1){
        if( fread(&temp, sizeof(struct usrdata), 1, fp) <1) break;
        if(strcmp(name1, temp.name)==0){
            if(temp.pgame==0 ){
                temp.now_my.poeple=temp.now_my.court= temp.now_my.treasury =50;
                temp.pgame= !death_flag; ///1
                for(int i=0;i<MAX_PROBLEM_N;i++){
                    temp.problems_left[i] = (i<n) ? 3 : 0;
                }
                fseek(fp, -sizeof(struct usrdata), SEEK_CUR);
                fwrite(&temp, sizeof(struct usrdata), 1, fp);
            }
            printf("%sWelcome back, %s!\n\n%s",MAGENTA, name1, KNRM);
            usr1 = temp;
            usr_king = temp;
            usr_found=1;
            break;
        }
    }
    if(usr_found==0){
        printf("%sWelcome to 'Bahram who caught zebras for a lifetime...' game, %s!!\n\n%s", MAGENTA, name1, KNRM );
        print_tutorial();

        strcpy(usr1.name, name1);
        usr1.pgame =1;
        //for(int j=0;j<n;j++)usr1.problems_left[j]=3;
        for(int i=0;i<MAX_PROBLEM_N;i++){
            usr1.problems_left[i] = (i<n) ? 3 : 0;
        }
        usr1.now_my.poeple=usr1.now_my.court= usr1.now_my.treasury =50;
        usr1.pnumber = n;
        fseek(fp, 0, SEEK_END);
        fwrite(&usr1, sizeof(struct usrdata), 1, fp);
    }
    fclose(fp);
    return usr1;
}
///setting usr data
void set_usr_data(){ //! : list of cur nodes
    FILE* fp;
    struct usrdata temp;
    int usr_found=0;

    fp = fopen("./USER_NAMES.bin", "rb+");
    if(fp==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    while(1){
        if(fread(&temp, sizeof(struct usrdata), 1, fp) <1) break;
        if(strcmp(usr_king.name, temp.name)==0){
            ///check for usr_king pnumbber;
            //struct node* pt;
            ///usr damn this shit
            ///come here u little peice of shit
            fseek(fp, -sizeof(struct usrdata), SEEK_CUR);
            fwrite(&usr_king, sizeof(struct usrdata), 1, fp);
            break;
        }
    }
    fclose(fp);
}
void set_usr_king(struct node* list){
    struct node* pt = list;
    int i;

    usr_king.now_my=my;
    usr_king.pgame = !death_flag;
    usr_king.pnumber = n;

    for(i=0;i<MAX_PROBLEM_N;i++){
        usr_king.problems_left[i] = 0;
    }
    while(pt!=NULL){
        usr_king.problems_left[pt->index] = pt->number;
        pt = pt->next;
    }
}
void save_usr_data(){ //! : list of cur nodes
    FILE* fp1;
    struct usrdata temp;
    printf("in saver\n");
    fp1 = fopen("./USER_LOGS.bin", "rb+");
    if(fp1==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    while(1){
        if(fread(&temp, sizeof(struct usrdata), 1, fp1) <1) break;
        if(strcmp(usr_king.name, temp.name)==0){
            printf("%s: ", temp.name);
            print_stats();
            fseek(fp1, -sizeof(struct usrdata), SEEK_CUR);
            fwrite(&usr_king, sizeof(struct usrdata), 1, fp1);
            return;
        }
    }
    fseek(fp1, 0, SEEK_END);
    fwrite(&usr_king, sizeof(struct usrdata), 1, fp1);
    fclose(fp1);
}

int main(){
    struct node *mylist=NULL;

    print_start_game();

    num = find_usrs_number();
    find_n_all();

    usr_king = get_usr_data();

    #if !INTERFACE
    printf("got usr data\n");
    #endif
    my = usr_king.now_my;
    save_usr_data();
    printf("Your current stats:\n ");
    print_stats();
    print_break_point();

    while(!death_flag && !quit_flag){
        set_usr_king(mylist);
        save_usr_data();
        random_node(&mylist);
    }
    set_usr_king(mylist);
    save_usr_data();
    print_exit_menu();
    if(!death_flag)
        printf("ok! bye\n");
    return 0;
}
