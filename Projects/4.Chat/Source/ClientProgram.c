#include "../Include/clientApp.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5555


int main(void)
{
    RunClientApp(SERVER_IP, SERVER_PORT);

    return 0;
}