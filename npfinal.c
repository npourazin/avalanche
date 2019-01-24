
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_CHAR_LEN 1000+10
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

int death_flag=0, quit_flag=0;
int n=0;

struct stats{
    int poeple;
    int court;
    int treasury;
} my;
struct solution{
    char ans[MAX_CHAR_LEN];
    int dif[3];
};
struct node{
    int number;
    int index;
    char problem[MAX_CHAR_LEN];
    struct solution choice[2];
    struct node* next;
};

void file_name_pre(char st[MAX_CHAR_LEN]){
    int len = strlen(st);
    int i=0;
    for(i=len;i>=0;i--){
        st[i+2]=st[i];
    }
    st[0]='.';
    ///
    st[1]='/';
    if(st[len+1]==10)
        st[len]=0;
}
void add_front(struct node **plist, struct node *new_node){
    new_node->next = *plist;
    new_node->index=0;
    *plist = new_node;

}
void add_end(struct node *list, struct node * new_node){
    struct node *current;
    for(current = list; current-> next != NULL; current = current->next);
    current->next = new_node;
    new_node->index=current->index+1;
    new_node->next = NULL;
}
void deleter(struct node** head, struct node* o_node){
    struct node* n= NULL;
    struct node* nn=NULL;

    if((*head)==o_node){
        n= *head;
        *head=(*head)->next;
        free(n);
        return;
    }
    nn=*head;
    while(nn!=NULL){
        if(nn==o_node){
            n->next=nn->next;
            free(nn);
            break;
        }
        n=nn;
        nn=nn->next;
    }
}
void print_node_before_selection(struct node * current){
    printf("%s%s\n",BLUE_3, current->problem);
    printf("%s", KNRM);
    printf("%s[1] %s\n",BLUE_4 ,current->choice[0].ans);
    printf("%s", KNRM);
    printf("%s[2] %s\n",BLUE_4, current->choice[1].ans);
    printf("%s", KNRM);
    printf("%s[-1] Exit game!\n\n", RED);
    printf("%s", KNRM);
    //print_node_after_selection(current, 1);
        //printf("%d%d%d\n", current->choice[0].dif[0], current->choice[0].dif[1], current->choice[0].dif[2]);
    //    current = current->next;
}
int min(int a, int b){ return (a<b) ? a : b;}
int max(int a, int b){ return (a>b) ? a : b;}

void print_node_after_selection(struct node* list, struct node * current , int i){
    printf("%s%s\n",BLUE_3, current->problem);
    ///if i==1 ||i==2
    if(i!=-1){
        if(i==1){
            printf("%s[1] %s\n",BLUE_7 ,current->choice[0].ans);
            printf("%s", KNRM);
            printf("%s[2] %s\n",BLUE_9, current->choice[1].ans);
        }
        if(i==2){
            printf("%s[1] %s\n",BLUE_9 ,current->choice[0].ans);
            printf("%s", KNRM);
            printf("%s[2] %s\n",BLUE_7, current->choice[1].ans);
        }
        printf("%s", KNRM);
        current->number--;

        my.poeple = min(100, my.poeple+current->choice[i-1].dif[0]);
        if(my.poeple<=0) {
            my.poeple=0;
            death_flag =1;
        }
        my.treasury= min(100, my.treasury + current->choice[i-1].dif[1]);
        if(my.treasury<=0) {
            my.treasury=0;
            death_flag =1;
        }
        my.court = min(100, my.court + current->choice[i-1].dif[2]);
        if(my.court<=0){
            my.court=0;
            death_flag =1;
        }
        if((my.poeple+ my.court+ my.treasury)/3 <10) death_flag=1;
        if(current->number==0){
            deleter((&list), current);
            n--;
            ///u need the number of each node!
        }
        printf("%s", KNRM);
        if(!death_flag){
            printf("%sPoeple:%d, Court:%d, Treasury:%d\n",GREEN, my.poeple, my.court, my.treasury);
        }
        printf("%s", KNRM);
    }

}
void print_list(struct node *list){///!
    struct node * current = list;
    while(current != NULL){
        //printf("%d \n", current->number);
        //printf("%d \n", current->index);
        print_node_before_selection(current);
        //printf("%d%d%d\n", current->choice[0].dif[0], current->choice[0].dif[1], current->choice[0].dif[2]);
        current = current->next;
    }
}
int find_n_all(struct node* mylist){
    struct node* c= mylist;
    while(c->next!=NULL) c= c->next;
    n = c->index+1;
    printf("n=%d\n", n);
    return n;
}
void clear(){
    system("clear");
}
void print_exit_menu(){
    printf("Exit Menu:\n");
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
void random_node(struct node** mylist){
    int rand_i, i;
    int get_choice=0;
    struct node* pt = *mylist;

    if(n==0){
        printf("boogh1\n");
        *mylist= get_problem_files();
        if(*mylist== NULL){printf("There seems to be a problem with getting input files!\n"); return -1;}
        n=find_n_all(*mylist);
        printf("boogh2\n");
    }
    pt = *mylist;
    //n = find_n_all(*mylist);
    srand(time(NULL));
    rand_i = rand()%n;
    for(i=0;i<rand_i;i++) pt= pt->next;
    print_node_before_selection(pt);
    scanf("%d", &get_choice);
    while(!((get_choice==1)||(get_choice==2) ||(get_choice==-1))){
        //clear();
        printf("%sWrong input!\nPlease try again!\n", RED);
        print_node_before_selection(pt);
        scanf("%d", &get_choice);
    }
    //clear();
    if(i!=-1) print_node_after_selection(*mylist, pt, get_choice);
    else  {    print_exit_menu();}
}
int main(){
    struct node *mylist=NULL;

    //mylist=get_problem_files();
    my.court=my.poeple=my.treasury=50;
   // if(mylist==NULL){printf("There seems to be a problem with getting input files!\n"); return -1;}
    //print_list(mylist);
    while(!death_flag && !quit_flag){
        random_node(&mylist);
    }
    //print_list(mylist);
    return 0;
}
