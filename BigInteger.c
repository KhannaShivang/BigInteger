#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Node{
    int data;
    struct Node* next;
};

struct BigInteger{
    struct Node* head;
    int negative;
};

void display(struct BigInteger);

struct BigInteger initialize(char str[]){
    struct BigInteger* num=(struct BigInteger*)malloc(sizeof(struct BigInteger));
    num->head=NULL;
    num->negative=0;
    int i=0;
    if(str[i]=='-'){
        num->negative=1;
        i++;
    }
    for(;str[i]!='\0';i++){
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->data=str[i]-'0';
        newNode->next=num->head;
        num->head=newNode;
    }
    return *num;
}


int length(struct Node*head){
    int l=0;
    while(head){
        l++;
        head=head->next;
    }
    return l;
}
void removeZero(struct Node*head){
    if(head==NULL||head->next==NULL) return;
    while(head->next){ 
        struct Node*curr=head;
        while(curr->next->next)curr=curr->next;
        if(curr->next->data==0) curr->next=NULL;
        else break;
    }
}
int compare(struct BigInteger n1,struct BigInteger n2){
    if(n1.negative&&!n2.negative) return -1;
    if(!n1.negative&&n2.negative) return 1;
    removeZero(n1.head);
    removeZero(n2.head);
    int l1=length(n1.head);
    int l2=length(n2.head);
    int flag=0;
    if(l1>l2) flag=1;
    else if(l1<l2) flag=-1;
    else{ 
        struct Node*p1=n1.head;
        struct Node*p2=n2.head;
        while(p1){
            if(p1->data<p2->data) flag=-1;
            if(p1->data>p2->data) flag=1;
            p1=p1->next;
            p2=p2->next;
        }
    }
    if(n1.negative&&n2.negative) return -flag;
    return flag;
}
struct BigInteger addBig(struct BigInteger num1,struct BigInteger num2){
    struct BigInteger* result =(struct BigInteger*)malloc(sizeof(struct BigInteger));
    result->head=NULL;
    result->negative=0;
    if(num1.negative && num2.negative) result->negative=1;
    struct Node*itr=result->head,*p1=num1.head,*p2=num2.head;
    int carry=0;
    while(p1 || p2||carry!=0){
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        int sum=carry;
        newNode->next=NULL;
        if(p1){
            sum+=p1->data;
            p1=p1->next;
        }
        if(p2){
            sum+=p2->data;
            p2=p2->next;
        }
        carry=sum/10;
        sum=sum%10;
        newNode->data=sum;
        if(result->head==NULL){
            itr=result->head=newNode;
        }
        else{
            itr=itr->next=newNode;
        }
    }removeZero(result->head);
    return *result;
}

struct BigInteger subBig(struct BigInteger num1,struct BigInteger num2){
    struct BigInteger* result=(struct BigInteger*)malloc(sizeof(struct BigInteger));
    result->negative=0;
    result->head=NULL;
    if(compare(num1,num2)==-1){
        struct BigInteger temp=num2;
        num2=num1;
        num1=temp;
        result->negative=1;
    }
    int borrow=0;
    struct Node* itr=NULL,*p1=num1.head,*p2=num2.head;
    while(p1){
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->next=NULL;
        int diff=borrow+p1->data;
            p1=p1->next;
        if(p2){
            diff-=p2->data;
            p2=p2->next;
        }
        if(diff<0){
            diff+=10;
            borrow=-1;
        }else borrow=0;
        newNode->data=diff;
        if(result->head==NULL){
            itr=result->head=newNode;
        }
        else{
            itr=itr->next=newNode;
        }
        
    }
    removeZero(result->head);
    if(result->head->data==0&&result->head->next==NULL) result->head=NULL;
    return *result;
}

struct BigInteger add(struct BigInteger num1,struct BigInteger num2){
    if(num1.negative && !num2.negative) return subBig(num2,num1);
    if(num2.negative && !num1.negative) return subBig(num1,num2);
    return addBig(num1,num2);
}

struct BigInteger sub(struct BigInteger num1,struct BigInteger num2){
    if(num1.negative && num2.negative) return subBig(num2,num1);
    if(num1.negative){
        num2.negative=1;
        return addBig(num1,num2);
    }
    if(num2.negative) return addBig(num1,num2);
    return subBig(num1,num2);
}

struct BigInteger mul(struct BigInteger num1,struct BigInteger num2){
    if(num1.head==NULL) return num1; 
    if(num2.head==NULL) return num2;
    struct BigInteger* result=(struct BigInteger*)malloc(sizeof(struct BigInteger));
    result->head=NULL;
    int n=(num1.negative!=num2.negative);
    struct Node*p1,*p2=num2.head;
    int shift=0;
    while(p2){
        p1=num1.head;
        int pro=p2->data;
        struct BigInteger*proTill=(struct BigInteger*)malloc(sizeof(struct BigInteger));
        proTill->head=NULL;
        proTill->negative=result->negative;
        struct Node* itr=NULL;
        for(int i=0;i<shift;i++){
            struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
            newNode->data=0;
            newNode->next=NULL;
            if(proTill->head==NULL) itr=proTill->head=newNode;
            else itr=itr->next=newNode;
        }shift++;
        int carry=0;
        while(p1){
            struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
            int pro=carry+(p1->data)*(p2->data);
            carry=pro/10;
            pro=pro%10;
            newNode->data=pro;
            newNode->next=NULL;
            if(proTill->head==NULL) itr=proTill->head=newNode;
            else itr=itr->next=newNode;
            p1=p1->next;
        }
        if(carry>0){
            struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
            newNode->data=carry;
            newNode->next=NULL;
            itr->next=newNode;
        }
        if(result->head==NULL) result=proTill;
        else{ 
            *result=(addBig(*result,*proTill));
            free(proTill);
        }
        p2=p2->next;
    }result->negative=n;

    return *result;
}

// struct BigInteger divide(struct BigInteger num1,struct BigInteger num2){
//     if(num2.head==NULL||(num2.head->next==NULL && num2.head->data==0)) {
//         printf("ARTHMETIC EXCEPTION \"/byZero\"\n");
//         exit(1);
//         return num1;
//     }
//     struct BigInteger* result=(struct BigInteger*)malloc(sizeof(struct BigInteger));
//     result->head=NULL;
//     result->negative=!(num1.negative==num2.negative);
//     num1.negative=num2.negative=0;
//     int quotient=0;
//     while(1){
//         if(lessThan(num1,num2)||isZero(num1.head)) break;
//         num1=subBig(num1,num2);
//         quotient++;
//     }
    
//     struct Node*cu=NULL;
//      do{
//         struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
//         newNode->data=quotient%10;
//         newNode->next=NULL;
//         quotient/=10;
//         if(result->head==NULL)cu=result->head=newNode;
//         else cu=cu->next=newNode;
//     }while(quotient>0);
//     return *result;
// }

struct BigInteger div1(struct BigInteger n1, struct BigInteger num2) {
    if (num2.head == NULL || (num2.head->next == NULL && num2.head->data == 0)) {
        printf("ARITHMETIC EXCEPTION \"/byZero\"\n");
        exit(1);
    }
    struct BigInteger result=initialize("0");
    int com=compare(n1,num2);
    if(com==-1)return result;
    if(com==0) return initialize("1");
    result.negative=n1.negative!=num2.negative;
    n1.negative=num2.negative=0;
    while(num2.head&&num2.head->data==0) {n1.head=n1.head->next;num2.head=num2.head->next;}
    if(num2.head->data==1&&num2.head->next==NULL) return n1;
    int l2=length(num2.head);
    int shift=0;
    struct Node*itr=NULL;
    struct BigInteger num1;
    num1.head=NULL;
    int l=0;
    num1.negative=n1.negative;
    while(n1.head){
        struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->data=n1.head->data;
        newNode->next=NULL;
        if(num1.head==NULL)num1.head=itr=newNode;
        else itr=itr->next=newNode;
        n1.head=n1.head->next;
    }
    while(compare(num1,num2)!=-1){
        struct Node*h=num1.head;
        struct Node*temp=NULL;
        struct BigInteger quotient=initialize("0");
        int l1=length(num1.head);
        for(int i=1;i<l1-l2;i++){
            temp=num1.head;
            num1.head=num1.head->next;
        }
        for(int i=1;i<l;i++){
            struct Node*zero=(struct Node*)malloc(sizeof(struct Node));
            zero->data=0;
            zero->next=result.head;
            result.head=zero;
        }
            struct Node*t=num1.head;
            num1.head=num1.head->next;
            if(compare(num1,num2)==-1){
                num1.head=t;
            }
            else{
                if(temp)temp=temp->next;
                else temp=t;
            }
            while(compare(num1,num2)!=-1){
                num1=subBig(num1,num2);
                quotient=addBig(quotient,initialize("1"));
            }
        if(shift!=0){
            struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
            newNode->data=0;
            newNode->next=result.head;
            result.head=newNode;
        }if(temp&& (num1.head==NULL||num1.head->data==0&&num1.head->next==NULL)){
            temp->next=NULL;
            l=length(h);
        }else l=0;
        result=addBig(result,quotient);
        if(temp){
            temp->next=num1.head;
            num1.head=h;
        }
        shift=1;
    }
    for(int i=0;i<l;i++){
            struct Node*zero=(struct Node*)malloc(sizeof(struct Node));
            zero->data=0;
            zero->next=result.head;
            result.head=zero;
        }
    return result;
}
struct BigInteger mod(struct BigInteger n1, struct BigInteger num2) {
    if (num2.head == NULL || (num2.head->next == NULL && num2.head->data == 0)) {
        printf("ARITHMETIC EXCEPTION \"/byZero\"\n");
        exit(1);
    }
    struct BigInteger result=initialize("0");
    int com=compare(n1,num2);
    if(com==-1)return n1;
    if(com==0) return initialize("0");
    int n=n1.negative!=num2.negative;
    n1.negative=num2.negative=0;
    while(num2.head&&num2.head->data==0) {n1.head=n1.head->next;num2.head=num2.head->next;}
    if(num2.head->data==1&&num2.head->next==NULL) return initialize("0");
    int l2=length(num2.head);
    int shift=0;
    struct Node*itr=NULL;
    struct BigInteger num1;
    num1.head=NULL;
    num1.negative=n1.negative;
    while(n1.head){
        struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
        newNode->data=n1.head->data;
        newNode->next=NULL;
        if(num1.head==NULL)num1.head=itr=newNode;
        else itr=itr->next=newNode;
        n1.head=n1.head->next;
    }
    while(compare(num1,num2)!=-1){
        struct Node*h=num1.head;
        struct Node*temp=NULL;
        struct BigInteger quotient=initialize("0");
        int l1=length(num1.head);
        for(int i=1;i<l1-l2;i++){
            temp=num1.head;
            num1.head=num1.head->next;
        }
            struct Node*t=num1.head;
            num1.head=num1.head->next;
            if(compare(num1,num2)==-1){
                num1.head=t;
               
            }
            else{
                if(temp)temp=temp->next;
                else temp=t;
            }
            while(compare(num1,num2)!=-1){
                num1=subBig(num1,num2);
                quotient=addBig(quotient,initialize("1"));
            }
        if(temp!=0){
            struct Node*newNode=(struct Node*)malloc(sizeof(struct Node));
            newNode->data=0;
            newNode->next=result.head;
            result.head=newNode;
        }
        result=addBig(result,quotient);
        if(temp){
            temp->next=num1.head;
            num1.head=h;
        }
        shift++;
    }num1.negative=n;
    return num1;
}



void print(struct Node* head){
    if(head==NULL){
        return;
    }
    print(head->next);
    printf("%d",head->data);    
}
void display(struct BigInteger num){
    if(num.head==NULL){
        printf("0");
        return;
    }
    if(num.negative) printf("-");
    // removeZero(num.head);
    print(num.head);
    printf("\n");
}

