/** 
 * @file serverApp.h
 * @author Rinat Mualem
 * @brief Server-side chat application
 * @version 1.0
 * 
 */
#ifndef __SERVERAPP_H__
#define __SERVERAPP_H__

typedef struct ServerApp ServerApp;

/**
 * @brief The function creates and initializes a ServerApp struct
 * @return ServerApp - pointer to ServerApp struct
 */
ServerApp* ServerAppCreate();

/**
 * @brief Unallocate a previously created struct ServerApp, provide double free protection
 * @param[in] _serverApp - the ServerApp struct to unallocate
 * @return void - no return value
 * @warning after destroy _serverApp == NULL
 */
void ServerAppDestroy(ServerApp** _serverApp);


/**
 * @brief The function runs a chat application (Server-side)
 * @param[in] _serverApp - the ServerApp struct
 * @return void - no return value
 */
void ServerAppRun(ServerApp* _serverApp);
#endif /*#include __SERVERAPP_H__*/