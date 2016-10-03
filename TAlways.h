#ifndef __TALWAYS_H__
#define __TALWAYS_H__

#include "cocos2d.h"
#include "TMainMenu.h"
#include "ClientManager.h"
#include "LocalDb.h"
#include "PlayerOne.h"
#include <iostream>
#include <string>
#include <regex>
#include <tinyxml2\tinyxml2.h>
#include <ui/CocosGUI.h>
#include <thread>
#include <atomic>

class TAlways : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	bool ls_button_click(const char *uname, const char *pw);
	void go_to_mainmenu(float dt = 0);

    // implement the "static create()" method manually
    CREATE_FUNC(TAlways);
private:
	LocalDb *db;
	PlayerOne *player;
	void releaseSignupLayer();
	void server_connection();
	std::thread *connection;
	ClientManager *client;
	void update(float dt);
	bool authorize();
	bool check_files_updates();
	void check_localdb_files();
	void advance();
	void app_exit();
	Label *info;

	/**
	*	Func. call order :
	*	@update will be scheduled to check connection thread.
	*	Call @authorize , @check_files_update if connection established.
	*	Then call @login_locally either connected or not.
	*/
};

#endif
