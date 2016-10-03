#include "ClientManager.h"

ClientManager *ClientManager::instance;
std::atomic_int ClientManager::_STATE;

ClientManager::ClientManager() {
	memset(this->buffer, 0, 250);
}

bool ClientManager::sendAll(int size)
{
	int bytessent = 0;
	while (bytessent < size) {
		int s = send(this->sock, (this->buffer + bytessent), (size - bytessent), 0);
		if (s <= 0) {
			return false;
		}
		bytessent += s;
	}
	return true;
}

bool ClientManager::recvAll(int size)
{
	int bytesrecvd = 0;
	while (bytesrecvd < size) {
		int r = recv(this->sock,this->buffer,(size - bytesrecvd),0);
		if (r <= 0) {
			return false;
		}
		bytesrecvd += r;
	}
	return true;
}

bool ClientManager::isAuthorized()
{
	return this->authorized;
}

int ClientManager::state_what()
{
	return (int)(this->_STATE.load());
}

ClientManager * ClientManager::getInstance()
{
	if (!instance) {
		instance = new ClientManager();
	}
	return instance;
}

void ClientManager::connect_to_server()
{
	//if (connected)	return;
	_STATE.store(STATE_OFFLINE);
	int res;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	WSADATA wsad;
	res = WSAStartup(MAKEWORD(2, 0), &wsad);
	if (res != 0) {
		CCLOG("bad wsad!\n");
		exit(1);
	}
#endif

	int mysocket;
	struct sockaddr_in sockaddr;

	//create new socket
	if((mysocket = socket(AF_INET,SOCK_STREAM,0)) < 0){
		CCLOG("cant create socket");
		return;
	}

	if (!inet_pton(AF_INET, ADDR, &(sockaddr.sin_addr.s_addr))) {
		CCLOG("error while writing ip adress to config.");
		return ;
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(PORT);

	fd_set set;
	struct timeval tv;
	FD_ZERO(&set);
	FD_SET(mysocket, &set);
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	u_long iMode = 1;
	ioctlsocket(mysocket, FIONBIO, &iMode);
#else
	fcntl();
#endif*/
	auto c = ClientManager::getInstance();
	cocos2d::log("connect called..");
	if (connect(mysocket, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) != 0) {
		c->disconnect();
		cocos2d::log("disconnect occurred.");
		return;
	}
	char buf[50] = "hellow im client. :)";
	//send(mysocket, buf, strlen(buf), NULL);
	//recv(mysocket, buf, 50, NULL);

	cocos2d::log("recved = %s",buf);

	c->sock = mysocket;
	cocos2d::log("connected.. :)");
	//c->disconnect();
	_STATE.store(STATE_DISCONNECTED);
	//_STATE.store(STATE_ONLINE);
	
	/*if (select(mysocket + 1, NULL, &set, NULL, &tv) > 0) {
		int so_error;
		socklen_t len = sizeof(so_error);
		getsockopt(mysocket, SOL_SOCKET, SO_ERROR, (char *)&so_error, &len);
		if (so_error == 0) {
			this->connected = true;
			this->sock = mysocket;
			iMode = 0;
			ioctlsocket(mysocket, FIONBIO, &iMode);
			return true;
		}
	}*/
}

void ClientManager::disconnect()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(shutdown(sock, SHUT_RDWR) != 0)
		close(sock);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (shutdown(sock, SD_BOTH) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
	}
#endif
	_STATE.store(STATE_DISCONNECTED);
}

bool ClientManager::AuthMyUser(bool isSignup, const char * username,const char * password, int buf_len_uname, int buf_len_pwd)
{
	auto plOne = PlayerOne::getInstance();
	REQUEST_T type;
	if (isSignup)
		type = _SIGN_UP_;
	else
		type = _LOGIN_;
	uint32_t Ival;
	Ival = htonl(type);
	int sizei32 = (int)sizeof(Ival);
	char *cursor = this->buffer;
	memcpy(cursor, &Ival, sizei32);
	cursor += sizei32;
	memcpy(cursor, username, buf_len_uname);
	cursor += buf_len_uname;
	memcpy(cursor,password,buf_len_pwd);
	if (sendAll(sizei32 + buf_len_pwd + buf_len_uname)) {
		memset(this->buffer, 0, 250);
		if (recvAll(sizei32 + sizeof(TPLAYER))) {
			cursor = this->buffer;
			memcpy(&Ival, cursor, sizei32);
			if (ntohl(Ival) == _SUCCESS_) {
				cursor += sizei32;
				TPLAYER p;
				memcpy(&Ival,cursor,sizei32);
				p.player_id = ntohl(Ival);
				cursor += sizei32;
				memcpy(&p.nickname, cursor, buf_len_uname);
				cursor += buf_len_uname;
				memcpy(&Ival,cursor,sizei32);
				p.tp = ntohl(Ival);
				cursor += sizei32;
				memcpy(&Ival, cursor, sizei32);
				p.win_c = ntohl(Ival);
				cursor += sizei32;
				memcpy(&Ival, cursor, sizei32);
				p.lost_c = ntohl(Ival);
				cursor += sizei32;
				memcpy(&Ival, cursor, sizei32);
				p.xp = ntohl(Ival);
				cursor += sizei32;
				memcpy(&Ival, cursor, sizei32);
				p.level = ntohl(Ival);
				cursor += sizei32;
				memcpy(&Ival, cursor, sizei32);
				p.pts = ntohl(Ival);
				memset(this->buffer,0,sizeof(this->buffer));
				cocos2d::log("AUTH OK.");
				plOne->init_data(p.player_id, p.nickname, p.tp, p.xp, p.level, p.win_c, p.lost_c, p.pts,8);
				return true;
			}
			//login or signup failed.
			return false;
		}
		//data cannot be received from server.
		return false;
	}
	//data cannot be sent to a server.
	cocos2d::log("AUTH FAIL.");
	return false;
}

void * ClientManager::request(REQUEST_T rq,void *vector)
{
	if (!rq)
		return nullptr;
	switch (rq) {
		case _LADDER_: {

			memset(this->buffer, 0, sizeof(this->buffer));
			uint32_t Ival = htonl(rq);
			memcpy(this->buffer,&Ival,sizeof(uint32_t));
			if (!sendAll(sizeof(uint32_t) + 1)) {
				cocos2d::log("request cannot be sent.");
				return nullptr;
			}
			int recved_bytes;
			char buffer[500];
			if ((recved_bytes = recv(this->sock, buffer, sizeof(buffer), 0)) <= 0) {
				cocos2d::log("request data cannot be recvd..");
				return nullptr;
			}
			memcpy(&Ival, buffer, SIZEUI32);
			Ival = ntohl(Ival);
			if (Ival == _FAIL_) {
				return nullptr;
			}

			std::vector<TPLAYER_LADDER> *lads = (std::vector<TPLAYER_LADDER> *)vector;
			char *cursor = buffer + sizeof(rq);
			uint32_t count;
			memcpy(&count, cursor, SIZEUI32);
			count = ntohl(count);
			cursor += SIZEUI32;

			for (unsigned int ix = 0; ix < count; ix++) {
				TPLAYER_LADDER *p = (TPLAYER_LADDER *)malloc(sizeof(TPLAYER_LADDER));
				TPLAYER_LADDER &pl = *p;
				memcpy(pl.nickname,cursor,26);
				cursor += 26;
				memcpy(&Ival,cursor,SIZEUI32);
				pl.pts = ntohl(Ival);
				cursor += SIZEUI32;
				memcpy(&Ival, cursor, SIZEUI32);
				pl.win_c = ntohl(Ival);
				cursor += SIZEUI32;
				memcpy(&Ival, cursor, SIZEUI32);
				pl.lost_c = ntohl(Ival);
				cursor += SIZEUI32;
				lads->push_back(*p);
			}
			return lads;
		}break;
	}
	return nullptr;
}
