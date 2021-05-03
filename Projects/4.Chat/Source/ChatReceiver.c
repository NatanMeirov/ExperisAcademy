/* Zahi Greenblum */

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TRUE 1
#define BUFFER_LENGTH 4096

/* Static: */

static int InitializedChatReceiverSocket(int _port, char* _ip,struct sockaddr_in* sin1,struct ip_mreq* mreq);


/* TODO: Create a separate header file for this API function */
int RunChatReceiver(int _port, char* _ip)
{
    struct sockaddr_in sin;
    struct ip_mreq mreq;
    int chatReceiverSocket;
    ssize_t readData = 0;
    char buffer[BUFFER_LENGTH];
    unsigned int sinLen1 = sizeof(sin);

    chatReceiverSocket = InitializedChatReceiverSocket(_port, _ip, &sin,&mreq);

    while(TRUE)
    {
        readData = recvfrom(chatReceiverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&sin, &sinLen1);
        if (readData < 0)
        {
            perror ("Failed while trying to receive a message...\n");
            abort();
        }

        buffer[readData] = '\0';
        printf("%s",buffer);
    }
}


static int InitializedChatReceiverSocket(int _port, char* _ip, struct sockaddr_in* sin,struct ip_mreq* mreq)
{
    int optval = 1;
    int chatReceiverSocket = socket(AF_INET, SOCK_DGRAM, 0); /* UDP */
    if (chatReceiverSocket  < 0)
    {
        perror ("Socket creation failed...\n");
        abort();
    }
    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(_ip);
    sin->sin_port = htons(_port);



    if (setsockopt(chatReceiverSocket, SOL_SOCKET, SO_REUSEADDR,
		    &optval, sizeof(optval)) < 0)
    {
	    perror("setsockopt mreq");
	    abort();
    }


    if (bind(chatReceiverSocket, (struct sockaddr*) sin, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind failed");
	    abort();
    }

    mreq->imr_multiaddr.s_addr = inet_addr(_ip);
    mreq->imr_interface.s_addr = htonl(INADDR_ANY);


    if (setsockopt(chatReceiverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		mreq, sizeof(*mreq)) < 0)
    {
	    perror("setsockopt mreq failed...");
	    abort();
    }

    return chatReceiverSocket;
}


/*---------------------------------------------------------------------------------------------*/


/* TODO: Extract the main to other source file, and create a header file for the RunChatSender */
int main(int argc, char *argv[])
{
   FILE* PidReceive;
   pid_t pid;
   if (argc != 4)
   {
       return 1;
   }

   printf("%s\n-----------------------\n", argv[3]);
   pid = getpid();
   PidReceive = fopen("pidReceiver","w");
   if (PidReceive == NULL)
   {
       return 0;
   }
   fprintf(PidReceive,"%d\n", pid);
   fclose(PidReceive);

   RunChatReceiver(atoi(argv[1]),argv[2]);

   return 0;
}