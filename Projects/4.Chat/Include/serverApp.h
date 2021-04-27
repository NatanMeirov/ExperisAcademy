#ifndef __SERVERAPP_H__
#define __SERVERAPP_H__

typedef struct ServerApp ServerApp;

ServerApp* ServerAppCreate();


void ServerAppDestroy(ServerApp** _serverApp);


void ServerAppRun(ServerApp* _serverApp);
#endif /*#include __SERVERAPP_H__*/