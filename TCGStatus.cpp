#include "TCGStatus.h"

TCGStatus * TCGStatus::instance;
TCGStatus::TCGStatus() {
	stats_general = new GeneralStatus;
	stats_general->isGameStarted = false;

	stats_ingame = new InGameStatus;
	stats_ingame->isCardPlayedByPlayer1 = false;
	stats_ingame->isCardPlayedByPlayer2 = false;
}

TCGStatus * TCGStatus::getInstance(){
	if (!instance) {
		instance = new TCGStatus();
	}
	return instance;
}

TCGStatus::GeneralStatus *TCGStatus::generalStatus() {
	return this->stats_general;
}

TCGStatus::InGameStatus *TCGStatus::inGameStatus() {
	return this->stats_ingame;
}

void TCGStatus::playerOnePlayed(bool d)
{
	this->stats_ingame->isCardPlayedByPlayer1 = d;
}

void TCGStatus::playerTwoPlayed(bool d)
{
	this->stats_ingame->isCardPlayedByPlayer2 = d;
}
