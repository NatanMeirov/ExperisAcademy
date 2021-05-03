/* Zahi Greenblum */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TRUE 1
#define BUFFER_LENGTH 4096
#define MAX_VALID_NAME_LEN 100

/* Static: */

static int InitializeChatSenderSocket(int _port, char* _ip, struct sockaddr_in* sin);


/* TODO: Create a separate header file for this API function */
void RunChatSender(int _port, char* _ip, char* _name)
{
    struct sockaddr_in sin;
    int chatSenderSocket, bufferLen;
    ssize_t sentData = 0;
    char buffer[BUFFER_LENGTH];
    char bufferWithName[BUFFER_LENGTH + MAX_VALID_NAME_LEN];

    chatSenderSocket = InitializeChatSenderSocket(_port, _ip, &sin);

    while(TRUE)
    {
        strcpy(bufferWithName, _name);
        strcat(bufferWithName, ": ");

        fflush(stdin);
        fgets(buffer ,BUFFER_LENGTH, stdin);
        fflush(stdin);

        strcat(bufferWithName, buffer);
        strcat(bufferWithName, "\0");

        bufferLen = strlen(bufferWithName);

        sentData = sendto(chatSenderSocket, bufferWithName, bufferLen, 0, (const struct sockaddr*)&sin, sizeof(sin));
        if (sentData < 0)
        {
            perror("Failed while trying to send a message...\n");
            abort();
        }
    }
}

static int InitializeChatSenderSocket(int _port, char* _ip, struct sockaddr_in* sin)
{
    int chatSenderSocket = socket(AF_INET, SOCK_DGRAM, 0); /* UDP */
    if (chatSenderSocket  < 0)
    {
        perror("Socket creation failed...\n");
        abort();
    }

    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(_ip);
    sin->sin_port = htons(_port);

    return chatSenderSocket;
}


/*---------------------------------------------------------------------------------------------*/


/* TODO: Extract the main to other source file, and create a header file for the RunChatSender */
int main(int argc, char *argv[])
{
   FILE* PidSender;
   pid_t pid;
   if (argc != 5)
   {
       return 1;
   }

   printf("%s\n-----------------------\n", argv[4]);
   printf("%s, Enter text:\n", argv[3]);
   printf("-----------------------\n");
   pid = getpid();
   PidSender = fopen("pidSender","w");
   if (PidSender == NULL)
   {
       return 0;
   }
   fprintf(PidSender,"%d\n", pid);
   fclose(PidSender);
   RunChatSender(atoi(argv[1]),argv[2],argv[3]);

   return 0;
}
