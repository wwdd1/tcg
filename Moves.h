#ifndef __MOVES_H__
#define __MOVES_H__

#include "cocos2d.h"
#include "Card.h"
#include "Gamescene.h"

/**
* THIS CLASS NEEDS TO BE SINGLETON!!!
*/

class Moves {
public:
	bool init(float,float,cocos2d::Scene *);
	static Moves *getInstance();
	/*setters*/
	void set_deck_position(Vec2);
	bool deployCardsToPlayers(std::vector<cocos2d::Sprite *> *,int); //starting game to deploy cards to players with given card collection
	bool cardToEmptySlot(int,int,cocos2d::Sprite *);
	bool gameareaAnnounceTextSlide(cocos2d::Label *);
	bool play_PlayerOne(cocos2d::Sprite *); //player1 card to center animation
	bool play_PlayerTwo(cocos2d::Sprite *);	
	bool round_to(int); //cards in center to winner animation

	cocos2d::Scene *get_current_affecting_scene();
private:
	cocos2d::Scene *gamescene;
	static Moves *_mv;
	Moves();
	Size vs;
	Vec2 origin;
	std::vector<cocos2d::Vec2> player_slot_points;
	std::vector<cocos2d::Vec2> player2_slot_points;
	std::map<int, cocos2d::Vec2> mid_slot_points;
	Vec2 deck_position_points;
	float card_width = 0;
	float card_height = 0;
};


#endif // __HELLOWORLD_SCENE_H__
