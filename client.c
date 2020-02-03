#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.c"

//Global variables

char User[1000],Pass[1000],AuthToken[1000],Extra[1000],Ch_Pass[1000];

//Functions

void EnterProgram();
int User_Pass();
char* PrepareToSend(char order[]);
int Menu();
int Chatting_Menu();
int Members(char*);
int Refresh(char*);
int Response(char*);
int Join_Check(char*);
int Create_Check(char*);
int Enter_Check(char*);
int Register_Check(char*);

//Main

int main()
{
    printf("................................\nHi\nWelcome to my chat application\nHope you Enjoy\n");

    //Preparing
    memset(AuthToken,0,1000);
    char Mess[1000];
    memset(Mess,0,strlen(Mess));
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);

    //Start

    EnterProgram(Mess);

    //End

    return 0;
}

void EnterProgram(char Mess[])
{
    int n;
    memset(Mess,0,strlen(Mess));
    printf("................................\n1- Log in\n2- Sign up\n3- Bye\n................................\nEnter Number: ");
    scanf("%d",&n);
    if(n==1)
    {
        User_Pass();
        strcpy(Mess,PrepareToSend("Log in"));
        memset(User,0,strlen(User));
        memset(Pass,0,strlen(Pass));
        Response(Mess);
        if(Enter_Check(Mess)==1)
        {
            Menu(Mess);
        }
        else
        {
            EnterProgram(Mess);
        }
    }
    else if(n==2)
    {
        User_Pass();
        strcpy(Mess,PrepareToSend("Register"));
        memset(User,0,strlen(User));
        memset(Pass,0,strlen(Pass));
        Response(Mess);
        Register_Check(Mess);
        EnterProgram(Mess);
    }
    else if(n==3)
        return;
    else
        EnterProgram(Mess);
}

//Giving Username and Password

int User_Pass()
{
    int i=0,j=0;
    char c='\0';
    printf("................................\nUsername:\n\t");
    scanf("%c",&c);
    while(1)
    {
        scanf("%c",&c);
        if(c=='\n')
            break;
        User[i]=c;
        i++;
    }
    printf("Password:\n\t");
    while(1)
    {
        scanf("%c",&c);
        if(c=='\n')
            break;
        Pass[j]=c;
        j++;
    }
}

//Prepare Strings to Send for Server

char* PrepareToSend(char order[])
{
    char Mess[1000];
    memset(Mess,0,strlen(Mess));
    if(strcmp(order,"Register")==0)
    {
        strcat(Mess,"register ");
        strcat(Mess,User);
        strcat(Mess,", ");
        strcat(Mess,Pass);
    }
    else if(strcmp(order,"Log in")==0)
    {
        strcat(Mess,"login ");
        strcat(Mess,User);
        strcat(Mess,", ");
        strcat(Mess,Pass);
    }
    else if(strcmp(order,"Log out")==0)
    {
        strcat(Mess,"logout ");
        strcat(Mess,AuthToken);
    }
    else if(strcmp(order,"Send Messege")==0)
    {
        int i=0;
        char c;
        char Messege[1000];
        memset(Messege,0,strlen(Messege));
        printf("Enter Messege: ");
        scanf("%c",&c);
        while(1)
        {
            scanf("%c",&c);
            if(c=='\n')
                break;
            Messege[i]=c;
            i++;
        }
        char *str = &Messege[i];
        memset(str,0,100);
        strcat(Mess,"send ");
        strcat(Mess,Messege);
        strcat(Mess,", ");
        strcat(Mess,AuthToken);
        memset(Messege,0,strlen(Messege));
    }
    else if(strcmp(order,"Create")==0)
    {
        int i=0;
        char c;
        char Channel_Name[1000];
        memset(Channel_Name,0,strlen(Channel_Name)+10);
        printf("Enter channel Name: ");
        scanf("%c",&c);
        while(1)
        {
            scanf("%c",&c);
            if(c=='\n')
                break;
            Channel_Name[i]=c;
            i++;
        }
        strcat(Mess,"create channel ");
        strcat(Mess,Channel_Name);
        strcat(Mess,", ");
        strcat(Mess,AuthToken);
        memset(Channel_Name,0,strlen(Channel_Name)+10);
    }
    else if(strcmp(order,"Create_P")==0)
    {
        int i=0,j=0;
        char c;
        char Channel_Name[1000],Channel_Pass[1000];
        memset(Channel_Name,0,strlen(Channel_Name)+10);memset(Channel_Pass,0,1000);
        printf("Enter channel Name: ");
        scanf("%c",&c);
        while(1)
        {
            scanf("%c",&c);
            if(c=='\n')
                break;
            Channel_Name[i]=c;
            i++;
        }
        printf("Enter channel password: ");
        while(1)
        {
            scanf("%c",&c);
            if(c=='\n')
                break;
            Channel_Pass[j]=c;
            j++;
        }
        strcat(Mess,"create channel_p ");
        strcat(Mess,Channel_Name);
        strcat(Mess,", ");
        strcat(Mess,Channel_Pass);
        strcat(Mess,", ");
        strcat(Mess,AuthToken);
        memset(Channel_Name,0,strlen(Channel_Name)+10);
    }
    else if(strcmp(order,"Join")==0)
    {
        int i=0;
        char c;
        char Channel_Name[1000];
        memset(Channel_Name,0,strlen(Channel_Name)+10);
        printf("Enter channel Name: ");
        scanf("%c",&c);
        while(1)
        {
            scanf("%c",&c);
            if(c=='\n')
                break;
            Channel_Name[i]=c;
            i++;
        }
        strcat(Mess,"join channel ");
        strcat(Mess,Channel_Name);
        strcat(Mess,", ");
        strcat(Mess,AuthToken);
        memset(Channel_Name,0,strlen(Channel_Name)+10);
    }
    else if(strcmp(order,"Channel Members")==0)
    {
        strcat(Mess,"channel members ");
        strcat(Mess,AuthToken);
    }
    else if(strcmp(order,"Leave")==0)
    {
        strcat(Mess,"leave ");
        strcat(Mess,AuthToken);
    }
    else if(strcmp(order,"Refresh")==0)
    {
        strcat(Mess,"refresh ");
        strcat(Mess,AuthToken);
    }
    else if(strcmp(order,"Channel Pass")==0)
    {
        strcat(Mess,"channel pass ");
        strcat(Mess,Ch_Pass);
        strcat(Mess,", ");
        strcat(Mess,AuthToken);
    }
    strcat(Mess,"\n");
    return Mess;
}

int Menu(char Mess[])
{
    int n,n2;
    memset(Mess,0,strlen(Mess));
    printf("................................\n1- Join Channel\n2- Create Channel\n3- Log out\n................................\nEnter number: ");
    scanf("%d",&n);
    if(n==1)
    {
        strcpy(Mess,PrepareToSend("Join"));
        Response(Mess);
        if(Join_Check(Mess)==1)
        {
            Chatting_Menu(Mess);
        }
        else
        {
            Menu(Mess);
        }
    }
    if(n==2)
    {
        printf("1- Public channel\n2- Private channel\n");
        scanf("%d",&n2);
        if(n2==1)
        {
            strcpy(Mess,PrepareToSend("Create"));
            Response(Mess);
            if(Create_Check(Mess)==1)
            {
                Chatting_Menu(Mess);
            }
            else
            {
                Menu(Mess);
            }
        }
        else if(n2==2)
        {
            strcpy(Mess,PrepareToSend("Create_P"));
            Response(Mess);
            if(Create_Check(Mess)==1)
            {
                Chatting_Menu(Mess);
            }
            else
            {
                Menu(Mess);
            }
        }
    }
    if(n==3)
    {
        strcpy(Mess,PrepareToSend("Log out"));
        Response(Mess);
        EnterProgram(Mess);
    }
    else
        Menu(Mess);
}

int Chatting_Menu(char Mess[])
{
    int n;
    memset(Mess,0,strlen(Mess));
    printf(".....................................................................\n 1: Send Messege \n 2: Refresh \n 3: Channel Members \n 4: Leave Channel \n..................................................................\nEnter number: ");
    scanf("%d",&n);
    if(n==1)
    {
        strcpy(Mess,PrepareToSend("Send Messege"));
        Response(Mess);
        Chatting_Menu(Mess);
    }
    if(n==2)
    {
        strcpy(Mess,PrepareToSend("Refresh"));
        Response(Mess);
        Refresh(Mess);
        Chatting_Menu(Mess);
    }
    if(n==3)
    {
        strcpy(Mess,PrepareToSend("Channel Members"));
        Response(Mess);
        Members(Mess);
        Chatting_Menu(Mess);
    }
    if(n==4)
    {
        strcpy(Mess,PrepareToSend("Leave"));
        Response(Mess);
        Menu(Mess);
    }
    else
    {
        Chatting_Menu(Mess);
    }
}

//Orders

int Refresh(char *Mess)
{
    char str1[1000],str2[1000];
    memset(str2,0,strlen(str2));memset(str1,0,strlen(str1));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    cJSON *sender;  cJSON *content_2;
    int i;
    printf("\n");
    for(i = 0 ; i < cJSON_GetArraySize(content) ; i++)
    {
        cJSON *submess=cJSON_GetArrayItem(content,i);
        sender = cJSON_GetObjectItem(submess,"sender");
        content_2 = cJSON_GetObjectItem(submess,"content");
        strcpy(str1,cJSON_Print(sender));
        strcpy(str2,cJSON_Print(content_2));
        printf("%s: %s\n",str1,str2);
    }
}

int Members(char *Mess)
{
    char str[1000];
    memset(str,0,strlen(str));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    cJSON *sender;  cJSON *content_2;
    int i;
    printf("Members: \n");
    for(i = 0 ; i < cJSON_GetArraySize(content) ; i++)
    {
        cJSON *submess=cJSON_GetArrayItem(content,i);
        strcpy(str,cJSON_Print(submess));
        printf("- %s\n",str);
    }
}

//Connection To Server

int Response(char *Mess)
{
    int Socket,i,j;
    Socket=socket(AF_INET,SOCK_STREAM,0);
    //Identifying Server
    struct sockaddr_in serveraddr;

    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serveraddr.sin_port=htons(12345);

    //Socket and Server

    connect(Socket,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    send(Socket,Mess,strlen(Mess),0);
    shutdown(Socket,1);
    strcpy(Mess,"");
    recv(Socket,Mess,1000,0);
    shutdown(Mess,0);
    closesocket(Socket);
}

//Checking Functions

int Create_Check(char *Mess)
{
    char str[1000];
    memset(str,0,strlen(str));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    strcpy(str,cJSON_Print(type));
    if(strcmp(str,"\"Error\"")==0)
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("%s\n",str);
        return 0;
    }
    else
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("..............................................................................\nYour Channel successfully created\n");
        return 1;
    }
}

int Join_Check(char *Mess)
{
    char str[1000];
    memset(str,0,strlen(str));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    strcpy(str,cJSON_Print(type));
    if(strcmp(str,"\"Error\"")==0)
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("%s\n",str);
        return 0;
    }
    else if(strcmp(str,"\"Private channel\"")==0)
    {
        int j=0;
        char pass[1000];
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("\nEnter channel password: ");
        scanf("%[^\n]s",pass);
        strcpy(Ch_Pass,pass);
        strcpy(Mess,PrepareToSend("Channel Pass"));
        Response(Mess);
        cJSON *pay=cJSON_Parse(Mess);
        cJSON *type2 = cJSON_GetObjectItem(pay,"type");
        cJSON *content2 = cJSON_GetObjectItem(pay,"content");
        strcpy(str,cJSON_Print(type2));
        if(strcmp(str,"\"Error\"")==0)
        {
            memset(str,0,strlen(str));
            strcpy(str,cJSON_Print(content));
            printf("%s\n",str);
            return 0;
        }
        else
        {
            printf("..................................................................\nSuccessfully enterd channel\n");
            return 1;
        }
    }
    else
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("......................................................................\nSuccessfully enterd channel\n");
        return 1;
    }
}

int Enter_Check(char *Mess)
{
    int i;
    char str[1000];
    memset(str,0,strlen(str));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    strcpy(str,cJSON_Print(type));
    if(strcmp(str,"\"Error\"")==0)
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("%s\n",str);
        return 0;
    }
    else
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf(".......................................................................\nSuccessfully logged in\n");
        for(i=1;i<strlen(str)-1;i++)
            AuthToken[i-1]=str[i];
        return 1;
    }
}

int Register_Check(char *Mess)
{
    char str[1000];
    memset(str,0,strlen(str));
    cJSON *payam = cJSON_Parse(Mess);
    cJSON *type = cJSON_GetObjectItem(payam,"type");
    cJSON *content = cJSON_GetObjectItem(payam,"content");
    strcpy(str,cJSON_Print(type));
    if(strcmp(str,"\"Error\"")==0)
    {
        memset(str,0,strlen(str));
        strcpy(str,cJSON_Print(content));
        printf("%s\n",str);
        return 0;
    }
    else
    {
        printf(".........................................................................\nYour account successfully created...\n");
        return 1;
    }
}
