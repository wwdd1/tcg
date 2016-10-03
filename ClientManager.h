#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__
//-p android compile için alttaki ilk endife ka
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>	
	#include <windows.h>
	#include <ws2tcpip.h>
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
#endif

#define SHIFTI32(cursor) (return (cursor + (int)sizeof(uint32_t)))
#define SHIFT(cursor,size) (return (cursor + size))
#define SIZEUI32 4

#define ADDR "127.0.0.1"
#define PORT 11444

#define STATE_ONLINE       0
#define STATE_OFFLINE	   1
#define STATE_DISCONNECTED 2

#include "cocos2d.h"
#include <sys\types.h>
#include "servertypes.h"
#include "PlayerOne.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cstdint>
#include <errno.h>
#include <fcntl.h>
#include <thread>
#include <atomic>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	#include <jni.h>
	#include <stdio.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <sys/un.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <stddef.h>
#endif

USING_NS_CC;


class ClientManager
{
private:
	int sock;
	bool authorized = false;
	char buffer[ 250 ];
	ClientManager();
	static ClientManager *instance;
	static std::atomic_int _STATE;
	bool sendAll(int size);
	bool recvAll(int size);
public:
	bool isAuthorized();
	int state_what();
	static ClientManager *getInstance();
	static void connect_to_server();
	void disconnect();
	bool AuthMyUser(bool isSignup , const char * username,const char * password, int buf_len_uname = 26, int buf_len_pwd = 26);
	void *request(REQUEST_T rq,void *vector);
};

#endif
