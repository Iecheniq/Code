#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h> //for threading , link with lpthread


void *ConnectionHandler(void *);
int CreateSocket();
void BindPort(int ServerSocket);
void ListenRequest(int ServerSocket);
void AcceptRequest (int ServerSocket);
void MessageBackwards (int socket ,char *ClientMessage);


int main()
{
    int ServerSocket;

    //Create socket
    ServerSocket = CreateSocket();
    //Bind to port
    BindPort(ServerSocket);
    //Listen requests
    ListenRequest(ServerSocket);
    //Accept and incoming connection
    AcceptRequest(ServerSocket);


    return 0;
}


int CreateSocket(){

    int ServerSocket;

    ServerSocket = socket(AF_INET , SOCK_STREAM , 0);//create socket
    if (ServerSocket == -1)
        {
        printf("Could not create socket");
        }
    else{
        puts("Socket created");
        }
    return ServerSocket;
}

void BindPort(int ServerSocket){

    struct sockaddr_in server;
    const int port = 15000;
  //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );


    if( bind(ServerSocket,(struct sockaddr *)&server , sizeof(server)) < 0)//bind to port
    {

        perror("Bind failed. Error");
    }
    else{
    puts("Bind done");
    }
    return;
}

void ListenRequest(int ServerSocket){
listen(ServerSocket , 3);//listen for requests
return;
}

void AcceptRequest (int ServerSocket){
    int ClientSocket , c , *NewSocket;
    struct sockaddr_in client;

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (ClientSocket = accept(ServerSocket, (struct sockaddr *)&client, (socklen_t*)&c)) )//accept request from the client
    {
        puts("Connection accepted");

        pthread_t SnifferThread;
        NewSocket = malloc(1);
        *NewSocket = ClientSocket;

        if( pthread_create( &SnifferThread , NULL ,  ConnectionHandler , (void*) NewSocket) < 0)//create a thread
        {
            perror("Could not create thread");

        }

        else{
        puts("Handler assigned");
        }
    }

    if (ClientSocket < 0)
    {
        perror("Accept failed");
        return;
    }
}


void MessageBackwards(int socket ,char* ClientMessage){

        char j = 0;
        char temp;
        char *NewLine = "\\n";
        char *NewClientMessage;

        strtok(ClientMessage,"\\n");//Look for the new line
        char i = strlen(ClientMessage)-1;

        while(i > j){//Message backwards rutine
            temp = ClientMessage[i];
            ClientMessage[i] = ClientMessage[j];
            ClientMessage[j] = temp;
            i--;
            j++;
            }
        NewClientMessage = strcat(ClientMessage, NewLine);//Concatenate both strings

        write(socket ,NewClientMessage , strlen(NewClientMessage));//Send the new message back to the client
}
//This will handle connection for each client

void *ConnectionHandler(void *ServerSocket){
    //Get the socket descriptor
    int socket = *(int*)ServerSocket;
    int ReadSize;
    char *message , ClientMessage[2000];



    message = "Send a message and I will answer with the same message backwards. Just remember I take \\n as the end of the line";
    write(socket , message , strlen(message));

    //Receive a message from client
    while( (ReadSize = recv(socket , ClientMessage , 2000 , 0)) > 0 )
    {
       MessageBackwards(socket, ClientMessage);//Send message backwards
    }

    if(ReadSize == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(ReadSize == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(ServerSocket);

    return;
}
