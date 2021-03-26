/*
    Bind socket to port 8888 on localhost
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>

#include <process.h>

#include <cstring>
#include <vector>

#include <ctype.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

unsigned int _stdcall doWork(void *data);
unsigned int receive(SOCKET *s,char *message);
std::vector<char*> splitMsg(char *message);
char* toUpperCase(char *msg);

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    //char *message;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 1234 );


    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }

    puts("Bind done");
    //Listen to incoming connections

        listen(s , 3);

    //Accept and incoming connection
        puts("Waiting for incoming connections...");

        c = sizeof(struct sockaddr_in);
        //new_socket = accept(s , (struct sockaddr *)&client, &c);
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET ){
        _beginthreadex(0,0,doWork,(void*)&new_socket,0,0);

    }
    closesocket(s);
    WSACleanup();

    return 0;
}

unsigned int receive(SOCKET *s,char *message){
    int recv_size;

    if((recv_size = recv(*s , message , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
        return 0;
    }
    message[recv_size]='\0';
    return 1;
}

std::vector<char*> splitMsg(char *message){
    std::vector<char*> v;
    char* chars_array = strtok(message, ":");
    while(chars_array)
    {
        v.push_back(chars_array);
        chars_array = strtok(NULL, ":");
    }
    return v;
}

char* toUpperCase(char *msg){
    while(*msg){
        *msg=toupper((unsigned char) *msg);
        msg++;
    }
}

unsigned int _stdcall doWork(void *data){
    SOCKET* client =(SOCKET*)data;
    SOCKET new_socket = *client;
    char messageIN[2000], *messageOUT;
    int recv_size;



    if (new_socket == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d" , WSAGetLastError());
            return 0;
        }

    printf("Connection accepted\n");

    int doWork=1,isLoggedIn=0;
    std::vector<char*> parts;
    while(doWork){
        //Read from Client
        if(receive(&new_socket,messageIN)){
            printf("Message Received: %s\n",messageIN);
            parts=splitMsg(messageIN);
        }
        if(!strcmp(parts[0],"login")){
            if(!isLoggedIn){
                if(!strcmp(parts[1],"fero") && !strcmp(parts[2],"fera\r\n")){
                    isLoggedIn=1;
                    messageOUT="Login Sucessful!\n";
                }
                else{
                    messageOUT="Wrong Credentials!\n";
                }
            }
            else{
                messageOUT="Already logged in!\n";
            }
            send(new_socket , messageOUT , strlen(messageOUT) , 0);
        }
        else if(!strcmp(parts[0],"toUpper")){
            toUpperCase(parts[1]);
            send(new_socket , parts[1] , strlen(parts[1]) , 0);
        }
        else if(!strcmp(parts[0],"logout\r\n")){
            if(isLoggedIn){
                isLoggedIn=0;
                messageOUT="Logout Sucessful!\n";
                doWork=0;
            }
            else{
                messageOUT="You must login first!\n";
            }
            send(new_socket , messageOUT , strlen(messageOUT) , 0);
        }
        else{
            messageOUT="Wrong!!!";
            send(new_socket , messageOUT , strlen(messageOUT) , 0);
        }
    }
    printf("Connection closed\n");

    return 1;

}
