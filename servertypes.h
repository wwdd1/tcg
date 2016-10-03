#ifndef __SERVERTYPES_H__
#define __SERVERTYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <sys/types.h>

typedef enum{
	_FAIL_ = 0,					//failure.
	_SUCCESS_ = 1,				//everything fine.
	_USER_INFO_ = 2,			//player's information data
	_WAIT_ = 3,					//waiting on game queue to find a player to play with online.
	_CONNECT_ = 4,				//connected to server.(game app started.)
	_CARD_DATA_ = 5,			//one card's information data
	_YOUR_TURN_ = 6,			//who starts the round
	_DECK_DATA_COUNTS_ = 7,		//deck data of how many card from which color
	_DECK_DATA_ = 8,			//deck cards data.
	_ADD_FRIEND_ = 9,			//add friend to player's list.
	_ADD_INVENTORY_ = 10,		//add item to player's list.
	_REMOVE_INVENTORY_ = 11,	//remove item from player's inventory.
	_INVENTORY_GET_ = 12,		//get all player's inventory items.
	_FRIENDS_GET_ = 13,			//get all player's friends.
	_FRIEND_SEARCH_ = 14,		//search for a player by nickname with received keyword.
	_LOGIN_ = 15,
	_SIGN_UP_ = 16,
	_LADDER_ = 17
}REQUEST_T;

typedef enum{
	PACKET_ITEM_CARD = 600,
	PACKET_ITEM_PACK = 601
}ITEM_T;

struct item{
	uint32_t id;
	uint32_t type;
};

typedef struct item TITEM;

struct player{
	uint32_t player_id;		//4 byte
    char nickname[26];		//26 byte
    uint32_t tp;			//4 byte
    uint32_t win_c;			//4 byte
    uint32_t lost_c;		//4 byte
    uint32_t xp;			//4 byte
    uint32_t level;			//4 byte
    uint32_t pts;			//4 byte
};
typedef struct player TPLAYER;

struct player_sum{
	uint32_t player_id;		//4 byte
	char nickname[26];		//26 byte
};
typedef struct player_sum TPLAYER_LITTLE;

struct card{
	REQUEST_T type;	  //4 byte
	uint32_t _id;	  //4 byte
	uint16_t color;	  //2 byte
	uint16_t avg;	  //2 byte
	char name[26];    //26 byte
	char land[4];	  //4 byte
	char position[4]; //4 byte
	char team[4]; 	  //4 byte
	uint16_t rate;	  //2 byte
};

typedef struct card TCARD;

struct card_sum{
	uint16_t card_id;
	uint16_t card_color;
};

typedef struct card_sum TCARD_LITTLE;

struct stack_element{
	TCARD *the_card;
	char card_data_buffer[sizeof(TCARD)];
	int place; //to get size of the stack faster..
	struct stack_element *node_forward;
};
typedef struct stack_element STACK_NODE;

struct list_element{
	int socket;
	TPLAYER pl;
	struct list_element *node_forward;
};
typedef struct list_element LIST_NODE;

struct a_game_knowledge{
	int listener;
	int player_one_socket;
	int player_two_socket;
	TPLAYER player_one_data;
	TPLAYER player_two_data;
};
typedef struct a_game_knowledge AGN;

struct ladder_player {
	char nickname[26];
	uint32_t pts;
	uint32_t win_c;
	uint32_t lost_c;
};
typedef struct ladder_player TPLAYER_LADDER;

#define DECK_COUNT 42 //if you change 42 to different value, multiple it by 4 and change GAMESTART_BUF_LEN
#define GAMESTART_BUF_LEN 170 // 2 + (42 * 4)  2 means packet type.
#endif
