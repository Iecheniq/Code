#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

//the thread function
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

    ServerSocket = socket(AF_INET , SOCK_STREAM , 0);
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

    //Bind
    if( bind(ServerSocket,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Bind failed. Error");
    }
    else{
    puts("Bind done");
    }
    return;
}

void ListenRequest(int ServerSocket){
listen(ServerSocket , 3);
return;
}

void AcceptRequest (int ServerSocket){
    int ClientSocket , c , *NewSocket;
    struct sockaddr_in client;

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (ClientSocket = accept(ServerSocket, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t SnifferThread;
        NewSocket = malloc(1);
        *NewSocket = ClientSocket;

        if( pthread_create( &SnifferThread , NULL ,  ConnectionHandler , (void*) NewSocket) < 0)
        {
            perror("Could not create thread");

        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
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
        char* message = strtok(ClientMessage,"\\n");
        char i = strlen(message)-1;
        char j = 0;
        char temp;
        char *n = "\\n";
        while(i > j){
        temp = message[i];
        message[i] = message[j];
        message[j] = temp;
        i--;
        j++;
        }

        //Send the message back to client
        write(socket ,message , strlen(message));
          write(socket ,n , strlen(n));

}
//This will handle connection for each client

void *ConnectionHandler(void *ServerSocket){
    //Get the socket descriptor
    int socket = *(int*)ServerSocket;
    int ReadSize;
    char *message , ClientMessage[2000];



    message = "Send a message and I will answer with the same message backwards. Just remember to put an \\n at the end of the line\n\n";
    write(socket , message , strlen(message));

    //Receive a message from client
    while( (ReadSize = recv(socket , ClientMessage , 2000 , 0)) > 0 )
    {
       MessageBackwards(socket, ClientMessage);
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

    return 0;
}
