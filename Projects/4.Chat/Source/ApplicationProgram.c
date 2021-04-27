#include "../Include/serverApp.h"


int main(void)
{
    ServerApp* application = ServerAppCreate();
    if(!application)
    {
        return 1;
    }

    ServerAppRun(application);

    ServerAppDestroy(&application);

    return 0;
}