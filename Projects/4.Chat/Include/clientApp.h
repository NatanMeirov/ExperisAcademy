/**
 * @file clientApp.h
 * @author Rinat Mualem
 * @brief Client-side chat application
 * @version 1.0
 * 
 */
#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__


/**
 * @brief The function runs a chat application (Client-side)
 * @param[in] _serverIP - The IP address of the server
 * @param[in] _serverPort - The port of the server
 * @return void - no return value
 *
 */
void RunClientApp(char* _serverIP, int _serverPort);



#endif /*#ifndef __CLIENTAPP_H__*/