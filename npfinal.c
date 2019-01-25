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

int death_flag=0, quit_flag=0, ng_flag=0;
int game_started=1;

int n=0;

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

int min(int a, int b){ return (a<b) ? a : b;}
int max(int a, int b){ return (a>b) ? a : b;}
void file_name_pre(char st[MAX_CHAR_LEN+10]){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        int len = strlen(st);
        int i=0;
        for(i=len;i>=0;i--){
            st[i+2]=st[i];
        }
        st[0]='.';
        ///unix is right what about windows?
        st[1]='/';
        if(st[len+1]==10)
            st[len]=0;
    #endif // defined
}
void add_front(struct node **plist, struct node *new_node){ ///!
    new_node->next = *plist;
    new_node->index=0;
    ///add alll other indexes?
    *plist = new_node;

}
void add_end(struct node *list, struct node * new_node){
    struct node *current;
    for(current = list; current-> next != NULL; current = current->next);
    current->next = new_node;
    new_node->index=current->index+1;
    new_node->next = NULL;
}
void deleter(struct node **list, struct node *o_node){
    printf("node deleted\n");
    struct node *n = NULL;
    struct node *nn = NULL;

    if((*list) == o_node){
        n = *list;
        *list = (*list)-> next;
        free(n);
        return;
    }
    nn = *list;
    while(nn!=NULL){
        if(nn==o_node){
            ///jumps over the head as it was taken care of
            n->next = nn->next;
            free(nn);
            break;
        }
        n=nn;
        nn=nn->next;
    }
    return;
}
void print_list(struct node *list){///!
    struct node * current = list;
    while(current != NULL){
        printf("(%d) Q%d: \n", current->number, current->index);
        current = current->next;
    }
}
void print_node_before_selection(struct node * current){ ///!
    printf("(%d) Q%d: ", current->number, current->index);
    printf("%s%s\n",BLUE_3, current->problem);
    printf("%s", KNRM);
    printf("%s[1] %s\n",BLUE_4 ,current->choice[0].ans);
    printf("%s", KNRM);
    printf("%s[2] %s\n",BLUE_4, current->choice[1].ans);
    printf("%s", KNRM);
    printf("%s[-1] Exit game!\n\n", RED);
    printf("%s", KNRM);

}
void show_menu(struct node* plist){
    int get_choice=0;
    printf("%sMENU:\n", KNRM);
    printf("[1] new game\n[2] leadership scoreboard\n[-1] exit\nelse, enter any integer to resume game.\n");
    if((!(scanf("%d", &get_choice)==1))){
        while ((getchar()) != '\n');
    }
    print_list(plist);
    printf("\n");
/*  if(get_choice==1){

    }
    if(get_choice==2){

    }
    if(get_choice ==-1){
        print_exit_menu();
    }
*/
}
void print_node_after_selection(struct node** plist, struct node * current , int ind){ ///!
    if(ind==1 || ind==2){
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
        my.treasury= min(100, (my.treasury + current->choice[ind -1].dif[1]));
        if(my.treasury<=0) {
            my.treasury=0;
            death_flag =1;
        }
        my.court = min(100, (my.court + current->choice[ind -1 ].dif[2]));
        if(my.court<=0){
            my.court=0;
            death_flag =1;
        }
        if((my.poeple+ my.court+ my.treasury)/3 <10) death_flag=1;
        if(current->number==0){
            deleter(plist, current);
            n--;
            ///u need the number of each node!
        }
        printf("%s", KNRM);
        printf("%sPoeple:%d, Court:%d, Treasury:%d\n----------------------------------\n\n",GREEN, my.poeple, my.court, my.treasury);
        printf("%s", KNRM);
    }
    if(ind ==3){
        show_menu(*plist);
        //clear();
    }

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
    printf("Number of Lines = %d\n", count);
    n = count;
}
void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
struct usrdata get_usr_data(){
    FILE* fp;
    struct usrdata temp, usr1;
    int usr_found=0, a1=0;
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
                ///this also should happen on ng_flag
                temp.now_my.poeple=temp.now_my.court= temp.now_my.treasury =50;
                temp.pgame= !death_flag;
                for(int i=0;i<MAX_PROBLEM_N;i++){
                    temp.problems_left[i] = (i<n) ? 3 : 0;
                }
                fseek(fp, -sizeof(struct usrdata), SEEK_CUR);
                fwrite(&temp, sizeof(struct usrdata), 1, fp);
            }
            printf("Welcome back, %s!\n\n%s", name1, KNRM);
            ///TODO: show menu
            usr1 = temp;
            usr_found=1;
            break;
        }
    }
    if(usr_found==0){
        printf("Welcome to 'melat dar hale sooghoot' game, %s!!\n\n%s", name1, KNRM );
        ///tutorial!!!
        ///TODO: show menu
        strcpy(usr1.name, name1);
        usr1.pgame =1;
        for(int j=0;j<n;j++)usr1.problems_left[j]=3;
        usr1.now_my.poeple=usr1.now_my.court= usr1.now_my.treasury =50;
        fseek(fp, 0, SEEK_END);
        fwrite(&usr1, sizeof(struct usrdata), 1, fp);
    }
    fclose(fp);
    return usr1;
}
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
void print_exit_menu(){
    if(!death_flag){
        char ans;
        printf("Do you want to save your current game? [y/n]\n");
        getchar();
        ans = getchar();
        if(ans=='y') set_usr_data();
        printf("ok! bye\n");
    }
    else{
        printf("You LOST!!!\n");
        set_usr_data();
    }
    //exit(1);
}

struct node * get_problem_files(){
    FILE * fp, *fpin;
    char inname[MAX_CHAR_LEN+10];
    char buf_name[MAX_CHAR_LEN+10];
    char buf1[MAX_CHAR_LEN+10];
    int cnt=0;
    struct node *list = NULL;
    int tmp=0;

    ///gets(inname);
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
void random_node(struct node** mylist){ ///! : list of cur nodes
    int rand_i, i;
    int get_choice=0;
    struct node* pt = NULL;
    struct node* cur = *mylist;
    //printf("in random node:\n");
    if(n==0 || game_started){

        printf("\n\n\n\n\n\n\n\n\nrefilled\n");
        *mylist = get_problem_files();
        //deleter(mylist, *mylist);
        //printf("bbb");
        if(game_started){
            struct node* pt = *mylist;
            while(pt->next!=NULL){ ///!
                pt->number = usr_king.problems_left[pt->index];
                //printf("%d : %d\n", pt->index, pt->number);
                if(pt->number==0) {
                    //printf("boogh %d\n", pt->index);
                    //deleter(mylist, pt);
                    n--;
                }
                pt = pt->next;
            }
            game_started=0;

        }

        //printf("---%d\n", n);
        /**/
        if(*mylist== NULL){printf("There seems to be a problem with getting input files!\n"); exit(-1);}
        find_n_all();
    }
    pt = *mylist;
    srand(time(NULL));
    rand_i = rand()%n;
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
//    clear();
    if(get_choice==1 || get_choice==2 || get_choice==3) print_node_after_selection(mylist, pt, get_choice);
    else  { quit_flag=1;}
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
    printf("Number of Lines = %d\n", count);
    return count;
}
void show_scoreboard(int num){
    char name1[MAX_CHAR_LEN];
    struct usrdata temp;
    int cnt=0;
    int n=7;
    //int cnt=0;
    FILE* fp;
    fp = fopen("./USER_NAMES.bin", "rb+");
    if(fp==NULL){
            printf("\nCannot open file\n");
            exit(-1);
    }
    while(cnt<num){
        cnt++;
        if( fread(&temp, sizeof(struct usrdata), 1, fp) <1) break;
        printf("Name: %s, People: %d,Court: %d,Treasury: %d,", temp.name, temp.now_my.poeple, temp.now_my.court, temp.now_my.treasury);
        printf(" lost or not: %s, n:%d\n", (temp.pgame)? "No": "Yes" , temp.pnumber);
    }
    fclose(fp);
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
    char name1[MAX_CHAR_LEN];
    struct usrdata temp1, temp2;
    int n=7;
    //int cnt=0;
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
            printf("\nboogh\n");
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
void sort_scoreboard(num){
    for(int i=0;i<num;i++)
        sort_scoreboard_once(num);
}

int main(){
    struct node *mylist=NULL;
    int num = find_usrs_number();

    sort_scoreboard(num);
    show_scoreboard(num);
    find_n_all();
    usr_king = get_usr_data();
    printf("got usr data\n");
    my = usr_king.now_my;
    printf("Your currennt stats:\n %sPoeple:%d, Court:%d, Treasury:%d\n----------------------------------\n\n%s",GREEN, my.poeple, my.court, my.treasury, KNRM);

    while(!death_flag && !quit_flag){
        //set_usr_data2();
        //set_usr_king(mylist);
        random_node(&mylist);

    }
    set_usr_king(mylist);
    print_exit_menu();
    //printf("%d", n);
    //print_list(mylist);
    return 0;
}
