#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.c"

//Functions

int PrepareToGet();
int PreProcess();
int AnsToClient(int,int);
int GetOrder(int,int);
int Process(int,int);
int Regist();
int Login();
int Members();
int Send();
int Logout();
int Leave();
int Create();
int Create_P();
int Join();
int Refresh();
int Channel_Pass();
int Tabdil(int);
int ANS(char[],char[]);
int ANS2(char[],cJSON*);
int find_char(char s[10000],char ch);
int Token();
int tok_eq(char[]);
int tok_check();
int tok_em(int);
int us_check(char[]);
int andis(char[]);

//Globals

char canal[10000][10000]; //clients are joined
char kanal[10000][10000]; //clients want to join
char user_online[10000][10000];
char user[10000];
char order[10000];
char Answer[10000];
char autoken[10000][10000];
int tokCount=0;
int usCount=0;
int refc[10000]={0};
//Main

int main()
{
    int i;
    for(i=0;i<10000;i++){
        memset(autoken,0,10000);memset(canal,0,10000);memset(user_online,0,10000);
    }
    srand(time(NULL));
    PreProcess();
    return 0;
}

// Kind 1
int PreProcess()
{
    PrepareToGet();
    PreProcess();
}
int PrepareToGet()
{
    int server_socket, client_socket;
    struct sockaddr_in server, client;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(12345);
    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 5);
    printf("IS LISTENING....\n");
    int len = sizeof(client);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &len);
    GetOrder(client_socket,server_socket);
}
int GetOrder(int client_socket,int server_socket)
{
    memset(order,0,10000);
    recv(client_socket, order, 10000, 0);
    printf("%s",order);
    Process(client_socket,server_socket);
}
int AnsToClient(int client_socket,int server_socket)
{
    printf("%s\n",Answer);
    send(client_socket,Answer,10000,0);
    closesocket(server_socket);
}
int Process(int client_socket,int server_socket)
{
    if(strncmp(order,"register",8)==0)
    {
        Regist();
    }
    else if(strncmp(order,"login",5)==0)
    {
        Login();
    }
    else if(strncmp(order,"create channel_p",16)==0)
    {
        Create_P();
    }
    else if(strncmp(order,"create channel",14)==0)
    {
        Create();
    }
    else if(strncmp(order,"join channel",12)==0)
    {
        Join();
    }
    else if(strncmp(order,"send",4)==0)
    {
        Send();
    }
    else if(strncmp(order,"refresh",7)==0)
    {
        Refresh();
    }
    else if(strncmp(order,"leave",5)==0)
    {
        Leave();
    }
    else if(strncmp(order,"channel members",15)==0)
    {
        Members();
    }
    else if(strncmp(order,"logout",6)==0)
    {
        Logout();
    }
    else if(strncmp(order,"channel pass",12)==0)
    {
        Channel_Pass();
    }
    else
    {
        ANS("Error","Undefined order");
    }
    AnsToClient(client_socket,server_socket);
}

//Kind 2
int Regist()
{
    int A=0;
    char User[10000],Pass[10000],s[10000],p[10000];
    memset(User,0,10000);memset(Pass,0,10000);memset(p,0,10000);memset(s,0,10000);
    Tabdil(9);
    strncpy(User,order,find_char(order,','));
    Tabdil(strlen(User)+2);
    strncpy(Pass,order,strlen(order)-1);
    FILE *UP;
    strcpy(s,"Resources/Users/");
    strcat(s,User);
    strcat(s,".txt");
    UP=fopen(s,"r");
    if(UP==NULL)
    {
        fclose(UP);
        cJSON *us;
        us= cJSON_CreateObject();
        cJSON_AddItemToObject(us,"Username",cJSON_CreateString(User));
        cJSON_AddItemToObject(us,"Password",cJSON_CreateString(Pass));
        UP=fopen(s,"w");
        fputs(cJSON_Print(us),UP);
        ANS("Successful","");
        fclose(UP);
    }
    else
    {
        fclose(UP);
        ANS("Error","The Username is not unique");
    }
}
int Login()
{
    int A=0;
    char User[10000],Pass[10000],s[10000],p[10000],b[10000];
    memset(User,0,10000);memset(Pass,0,10000);memset(p,0,10000);memset(s,0,10000);memset(b,0,10000);
    strcpy(Pass,"\"");
    Tabdil(6);
    strncpy(User,order,find_char(order,','));
    Tabdil(strlen(User)+2);
    strncat(Pass,order,strlen(order)-1);
    strcat(Pass,"\"");
    FILE *UP;
    strcat(s,"Resources/Users/");
    strcat(s,User);
    strcat(s,".txt");
    UP=fopen(s,"r");
    if(UP==NULL)
    {
        ANS("Error","The Username doesn't exist");
    }
    else
    {
        while(!feof(UP)){
            strcat(p,fgets(b,10000,UP));
            memset(b,0,10000);
        }
        cJSON *us= cJSON_Parse(p);
        fclose(UP);
        cJSON *ps;
        ps=cJSON_GetObjectItem(us,"Password");
        if(strcmp(cJSON_Print(ps),Pass)==0)
        {
            if(us_check(User)==0)
                ANS("Error","User hasn't log out");
            else{
                Token();
                strcpy(user_online[usCount],User);
                usCount++;
                ANS("AuthToken",autoken[tokCount-1]);
            }
        }
        else
            ANS("Error","Wrong Password");
    }

}
int Logout()
{
    char tok[10000];
    memset(tok,0,10000);
    Tabdil(7);
    strcpy(tok,order);
    int i,A=0;
    for(i=0;i<tokCount;i++)
    {
        if(strncmp(tok,autoken[i],strlen(autoken[i]))==0)
        {
            A=1;
            ANS("Successful","");
            memset(autoken[i],0,10000);
            memset(user_online[i],0,10000);
            tok_em(i);
            break;
        }
    }
    if(A==0)
        ANS("Error","Your token is wrong");
}
int Join()
{
    int A=0,i,k;
    char ch_N[10000],tok[10000],s[10000],p[10000],q[10000],b[10000];
    memset(tok,0,10000);memset(ch_N,0,10000);memset(p,0,10000);memset(s,0,10000);memset(q,0,10000);
    Tabdil(13);
    strncpy(ch_N,order,find_char(order,','));
    Tabdil(strlen(ch_N)+2);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,ch_N);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        ANS("Error","The channel doesn't exist");
    }
    else
    {
        while(!feof(ch)){
            strcat(p,fgets(b,10000,ch));
            memset(b,0,10000);
        }
        fclose(ch);
        cJSON *cn,*mem,*mes;
        cn=cJSON_Parse(p);
        mem=cJSON_GetObjectItem(cn,"Members");
        mes=cJSON_GetObjectItem(cn,"Messeges");
        int i,k=0;
        char us[10000];
        strcpy(us,"\"");
        strcat(us,user);
        strcat(us,"\"");
        for(i=0;i<cJSON_GetArraySize(mem);i++)
        {
            cJSON *submem=cJSON_GetArrayItem(mem,i);
            if(strcmp(cJSON_Print(submem),us)==0)
            {
                k=1;
                ANS("Error","The user has joined before");
                break;
            }
        }
        if(k==0)
        {
            cJSON *PS=cJSON_GetObjectItem(cn,"Password");
            if(strcmp(cJSON_Print(PS),"\"NULL\"")==0)
            {
                cJSON *submes;
                cJSON_AddItemToArray(mem,cJSON_CreateString(user));
                submes=cJSON_CreateObject();
                cJSON_AddItemToObject(submes,"sender",cJSON_CreateString(user));
                strcpy(q,user);strcat(q," joined the channel");
                cJSON_AddItemToObject(submes,"content",cJSON_CreateString(q));
                cJSON_AddItemToArray(mes,submes);
                ch=fopen(s,"w");
                fputs(cJSON_Print(cn),ch);
                fclose(ch);
                strcpy(canal[andis(user)],ch_N);
                ANS("Successful","");
            }
            else
            {
                strcpy(kanal[andis(user)],ch_N);
                ANS("Private channel","Enter Password");
            }
        }
    }
}
int Create()
{
    int A=0;
    char ch_N[10000],tok[10000],s[10000],p[10000],b[10000];
    memset(tok,0,10000);memset(ch_N,0,10000);memset(p,0,10000);memset(s,0,10000);
    Tabdil(15);
    strncpy(ch_N,order,find_char(order,','));
    Tabdil(strlen(ch_N)+2);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,ch_N);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        cJSON *cn,*mes,*mem,*submes;
        cn=cJSON_CreateObject();
        cJSON_AddItemToObject(cn,"Name",cJSON_CreateString(ch_N));
        cJSON_AddItemToObject(cn,"Password",cJSON_CreateString("NULL"));
        mes=cJSON_CreateArray();
        mem=cJSON_CreateArray();
        cJSON_AddItemToArray(mem,cJSON_CreateString(user));
        submes=cJSON_CreateObject();
        cJSON_AddItemToObject(submes,"sender",cJSON_CreateString(user));
        char q[10000];
        strcpy(q,user);
        strcat(q," created the channel");
        cJSON_AddItemToObject(submes,"content",cJSON_CreateString(q));
        cJSON_AddItemToArray(mes,submes);
        cJSON_AddItemToObject(cn,"Members",mem);
        cJSON_AddItemToObject(cn,"Messeges",mes);
        ch=fopen(s,"w");
        fputs(cJSON_Print(cn),ch);
        fclose(ch);
        strcpy(canal[andis(user)],ch_N);
        ANS("Successful","");
    }
    else
    {
        fclose(ch);
        ANS("Error","The channel has existed before");
    }
}
int Create_P()
{
    int A=0;
    char ch_N[10000],tok[10000],s[10000],p[10000],b[10000],pass[10000];
    memset(tok,0,10000);memset(ch_N,0,10000);memset(p,0,10000);memset(s,0,10000);memset(pass,0,10000);
    Tabdil(17);
    strncpy(ch_N,order,find_char(order,','));
    Tabdil(strlen(ch_N)+2);
    strncpy(pass,order,find_char(order,','));
    Tabdil(strlen(pass)+2);
    strcpy(tok,order);
    printf("%s\n",tok);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,ch_N);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        cJSON *cn,*mes,*mem,*submes;
        cn=cJSON_CreateObject();
        cJSON_AddItemToObject(cn,"Name",cJSON_CreateString(ch_N));
        cJSON_AddItemToObject(cn,"Password",cJSON_CreateString(pass));
        mes=cJSON_CreateArray();
        mem=cJSON_CreateArray();
        cJSON_AddItemToArray(mem,cJSON_CreateString(user));
        submes=cJSON_CreateObject();
        cJSON_AddItemToObject(submes,"sender",cJSON_CreateString(user));
        char q[10000];
        strcpy(q,user);
        strcat(q," created the channel");
        cJSON_AddItemToObject(submes,"content",cJSON_CreateString(q));
        cJSON_AddItemToArray(mes,submes);
        cJSON_AddItemToObject(cn,"Members",mem);
        cJSON_AddItemToObject(cn,"Messeges",mes);
        ch=fopen(s,"w");
        fputs(cJSON_Print(cn),ch);
        fclose(ch);
        strcpy(canal[andis(user)],ch_N);
        ANS("Successful","");
    }
    else
    {
        fclose(ch);
        ANS("Error","The channel has existed before");
    }
}
int Leave()
{
    char tok[10000],s[10000],p[10000],b[10000];
    memset(tok,0,10000);memset(p,0,10000);memset(s,0,10000);
    Tabdil(6);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,canal[andis(user)]);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        ANS("Error","The channel doens't exist");
    }
    else
    {
        int o=0;
        while(!feof(ch)){
            strcat(p,fgets(b,10000,ch));
            memset(b,0,10000);
        }
        cJSON *cn,*mem,*mes,*su;
        cn=cJSON_Parse(p);
        mem=cJSON_GetObjectItem(cn,"Members");
        mes=cJSON_GetObjectItem(cn,"Messeges");
        su=cJSON_CreateObject();
        int i;
        char us[10000];
        strcpy(us,"\"");
        strcat(us,user);
        strcat(us,"\"");
        for(i=0;i<cJSON_GetArraySize(mem);i++)
        {
            if(strcmp(us,cJSON_Print(cJSON_GetArrayItem(mem,i)))==0)
            {
               o=1;
               char h[10000];
               strcpy(h,user);
               strcat(h," leaved the channel");
               cJSON_AddItemToObject(su,"sender",cJSON_CreateString(user));
               cJSON_AddItemToObject(su,"content",cJSON_CreateString(h));
               cJSON_AddItemToArray(mes,su);
               cJSON_DeleteItemFromArray(mem,i);
               break;
            }
        }
        if(o==0)
            ANS("Error","The user is not joined in the channel");
        else{
            ch=fopen(s,"w");
            fputs(cJSON_Print(cn),ch);
            fclose(ch);
            memset(canal[andis(user)],0,10000);
            refc[andis(user)]=0;
            ANS("Successful","");
        }
    }
}
int Send()
{
    char tok[10000],s[10000],p[10000],payam[10000],b[10000];
    memset(tok,0,10000);memset(s,0,10000);memset(p,0,10000);memset(payam,0,10000);
    Tabdil(5);
    strncpy(payam,order,find_char(order,','));
    Tabdil(strlen(payam)+2);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,canal[andis(user)]);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        ANS("Error","The channel doesn't exist");
    }
    else
    {
        while(!feof(ch)){
            strcat(p,fgets(b,10000,ch));
            memset(b,0,10000);
        }
        cJSON *cn,*mes,*submes;
        cn=cJSON_Parse(p);
        mes=cJSON_GetObjectItem(cn,"Messeges");
        submes=cJSON_CreateObject();
        cJSON_AddItemToObject(submes,"sender",cJSON_CreateString(user));
        cJSON_AddItemToObject(submes,"content",cJSON_CreateString(payam));
        cJSON_AddItemToArray(mes,submes);
        fclose(ch);
        ch=fopen(s,"w");
        fputs(cJSON_Print(cn),ch);
        fclose(ch);
        ANS("Successful","");
    }
}
int Refresh()
{
    char tok[10000],s[10000],p[10000],b[10000];
    memset(tok,0,10000);memset(s,0,10000);memset(p,0,10000);
    Tabdil(8);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,canal[andis(user)]);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        ANS("Error","The channel doesn't exist");
    }
    else
    {
        while(!feof(ch)){
            strcat(p,fgets(b,10000,ch));
            memset(b,0,10000);
        }
        fclose(ch);
        cJSON *cn,*mes,*ms;
        int i,j;
        cn=cJSON_Parse(p);
        ms=cJSON_CreateArray();
        mes=cJSON_GetObjectItem(cn,"Messeges");
        cJSON_AddItemToArray(ms,cJSON_GetArrayItem(mes,refc[andis(user)]));
        refc[andis(user)]=cJSON_GetArraySize(mes);
        ANS2("List",ms);
    }
}
int Members()
{
    char tok[10000],s[10000],p[10000],b[10000];
    memset(tok,0,10000);memset(s,0,10000);memset(p,0,10000);
    Tabdil(16);
    strcpy(tok,order);
    if(tok_eq(tok)==0)
        return 0;
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,canal[andis(user)]);
    strcat(s,".txt");
    ch=fopen(s,"r");
    if(ch==NULL)
    {
        fclose(ch);
        ANS("Error","The channel doesn't exist");
    }
    else
    {
        while(!feof(ch)){
            strcat(p,fgets(b,10000,ch));
            memset(b,0,10000);
        }
        fclose(ch);
        cJSON *cn,*mem;
        int i;
        cn=cJSON_Parse(p);
        mem=cJSON_GetObjectItem(cn,"Members");
        ANS2("List",mem);
    }
}
int Channel_Pass()
{
    char tok[10000],pass[10000],pas2[10000],s[10000],p[10000],b[10000];
    memset(tok,0,10000);memset(pass,0,10000);memset(s,0,10000);memset(p,0,10000);
    strcpy(pas2,"\"");
    Tabdil(13);
    strncpy(pass,order,find_char(order,','));
    Tabdil(strlen(pass)+2);
    strcpy(tok,order);
    int i;
    if(tok_eq(tok)==0)
        return 0;
    strcat(pas2,pass);
    strcat(pas2,"\"");
    FILE *ch;
    strcat(s,"Resources/Channels/");
    strcat(s,kanal[andis(user)]);
    strcat(s,".txt");
    ch=fopen(s,"r");
    while(!feof(ch))
        strcat(p,fgets(b,10000,ch));
    fclose(ch);
    cJSON *cn=cJSON_Parse(p);
    if(strcmp(pas2,cJSON_Print(cJSON_GetObjectItem(cn,"Password")))==0)
    {
        char q[10000];
        cJSON *submes,*mem,*mes;
        mem=cJSON_GetObjectItem(cn,"Members");
        mes=cJSON_GetObjectItem(cn,"Messeges");
        cJSON_AddItemToArray(mem,cJSON_CreateString(user));
        submes=cJSON_CreateObject();
        cJSON_AddItemToObject(submes,"sender",cJSON_CreateString(user));
        strcpy(q,user);strcat(q," joined the channel");
        cJSON_AddItemToObject(submes,"content",cJSON_CreateString(q));
        cJSON_AddItemToArray(mes,submes);
        ch=fopen(s,"w");
        fputs(cJSON_Print(cn),ch);
        fclose(ch);
        strcpy(canal[andis(user)],kanal[andis(user)]);
        ANS("Successful","");
    }
    else
    {
        ANS("Error","Wrong password");
    }
}

//Kind 3
int ANS(char type[],char content[])
{
    cJSON *awer;
    awer=cJSON_CreateObject();
    cJSON_AddItemToObject(awer,"type",cJSON_CreateString(type));
    cJSON_AddItemToObject(awer,"content",cJSON_CreateString(content));
    strcpy(Answer,cJSON_Print(awer));
}
int ANS2(char type[],cJSON *content)
{
    cJSON *awer;
    awer=cJSON_CreateObject();
    cJSON_AddItemToObject(awer,"type",cJSON_CreateString(type));
    cJSON_AddItemToObject(awer,"content",content);
    strcpy(Answer,cJSON_Print(awer));
}

//Kind 4
int Token()
{
    int i;
    do{
        for(i=0;i<20;i++)
            autoken[tokCount][i]=rand()%41+50;
    }
    while(tok_check()==0);
    tokCount++;
}
int tok_eq(char s[10000])
{
    int i;
    s[strlen(s)-1]=NULL;
    for(i=0;i<tokCount;i++)
    {
        if(strcmp(s,autoken[i])==0)
        {
            memset(user,0,10000);
            strcpy(user,user_online[i]);
            return 1;
        }
    }
    ANS("Error","Your token is wrong");
    return 0;
}
int tok_em(int i)
{
    int j;
    for(j=i;j<tokCount;j++)
    {
        strcpy(autoken[j],autoken[j+1]);
        strcpy(user_online[j],user_online[j+1]);
        strcpy(canal[j],canal[j+1]);
        strcpy(kanal[j],kanal[j+1]);
        refc[j]=refc[j+1];
    }
    tokCount--;
    usCount--;
}
int tok_check()
{
    int i=0,j=0;
    for(;i<tokCount;i++)
        for(j=i+1;j<tokCount;j++)
            if(strcmp(autoken[i],autoken[j])==0)
                return 0;
    return 1;
}
int us_check(char s[10000])
{
    int i=0;
    for(;i<usCount;i++)
        if(strcmp(s,user_online[i])==0)
            return 0;
    return 1;
}
int andis(char s[])
{
    int i;
    for(i=0;i<usCount;i++)
        if(strcmp(s,user_online[i])==0)
            return i;
}

//kind 5
int find_char(char s[10000],char ch)
{
    int i;
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]==ch)
            return i;
    }
    return -1;
}
int Tabdil(int n)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<strlen(order);j++)
            order[j]=order[j+1];
}
