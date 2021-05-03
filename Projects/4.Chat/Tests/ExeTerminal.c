#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ChatLauncher(char* _ip, int _port, char* _nameOfUser);

int main()
{
   /* system("gnome-terminal --geometry 100x40+700+700 -- ./a33.out"); */
   ChatLauncher("224.0.0.1", 1234, "jjjj");
   /*ChatLauncher("244.0.0.1", 1234, "vvbg"); */




    return 0;
}



void ChatLauncher(char* _ip, int _port, char* _nameOfUser)
{
    char commandLaunchChatSender[1024];
    char commandLaunchChatReceiver[1024];

    sprintf(commandLaunchChatSender, "%s %d %s %s", "gnome-terminal --geometry 30x20+400+300 -- ./send.out", _port, _ip, _nameOfUser);
    sprintf(commandLaunchChatReceiver, "%s %d %s", "gnome-terminal --geometry 30x20+900+300 -- ./receiv.out",_port , _ip);
   

    system(commandLaunchChatSender);
    system(commandLaunchChatReceiver);
}

