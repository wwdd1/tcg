#ifndef __TCGSTATUS_H__
#define __TCGSTATUS_H__

class TCGStatus
{
public:
	typedef struct general_status {
		bool isGameStarted;
	}GeneralStatus;

	typedef struct in_game_status {
		bool isCardPlayedByPlayer1;
		bool isCardPlayedByPlayer2;
	}InGameStatus;
private:
	TCGStatus();
	GeneralStatus *stats_general;
	InGameStatus *stats_ingame;
	static TCGStatus *instance;
public:
	static TCGStatus *getInstance();
	GeneralStatus *generalStatus();
	InGameStatus *inGameStatus();
	void playerOnePlayed(bool);
	void playerTwoPlayed(bool);
};

#endif
