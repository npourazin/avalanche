#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR_LEN 1000+10

struct solution{
    char ans[MAX_CHAR_LEN];
    int dif[3];
};
struct node{
    int number;
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
    st[1]='/';
    if(st[len+1]==10)
        st[len]=0;
}
void add_front(struct node **plist, struct node *new_node){
    new_node->next = *plist;
    new_node->number=0;
    *plist = new_node;

}
void add_end(struct node *list, struct node * new_node){
    struct node *current;
    for(current = list; current-> next != NULL; current = current->next);
    current->next = new_node;
    new_node->number=current->number+1;
    new_node->next = NULL;
}
void print_list(struct node *list){///!
    struct node * current = list;
    while(current != NULL){
        //printf("%d ", current->number);

        printf("%s\n", current->problem);
        printf("[1] %s\n", current->choice[0].ans);
        printf("[2] %s\n", current->choice[1].ans);
        printf("[-1] Exit game!\n\n");
        //printf("%d%d%d\n", current->choice[0].dif[0], current->choice[0].dif[1], current->choice[0].dif[2]);
        current = current->next;
    }
}
void print_node(struct node * current){
    printf("%s\n", current->problem);
    printf("[1] %s\n", current->choice[0].ans);
    printf("[2] %s\n", current->choice[1].ans);
    printf("[-1] Exit game!\n\n");
        //printf("%d%d%d\n", current->choice[0].dif[0], current->choice[0].dif[1], current->choice[0].dif[2]);
    //    current = current->next;
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
        if(list == NULL)     add_front(&(list), new_node);
        else    add_end(list, new_node);

    }
    fclose(fp);
    return list;
}

int main(){
    struct node *mylist;

    mylist=get_problem_files();
    if(mylist==NULL){printf("There seems to be a problem with getting input files!\n"); return -1;}
    print_list(mylist);
    return 0;
}
