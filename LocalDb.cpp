#include "LocalDb.h"
//just for to_string() bug
#include <string>
#include <sstream>

LocalDb::LocalDb() {

}

int LocalDb::get_totalCount() {
	/*sqlite3 *sqlObj = NULL;
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "db/testomnia.db3";
	try {
		if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlObj, SQLITE_OPEN_READONLY, nullptr)) {
			return -1;
		}
		char **re;
		char *err;
		int r, c;
		if (SQLITE_OK != sqlite3_get_table(sqlObj, "SELECT * from tcg3_blue", &re, &r, &c, &err)) {
			CCLOG("%s", err);
			return -1;
		}
		std::string rrr = "";
		for (int i = c; i < c*2; i++) {
			rrr += re[i];
		}
		CCLOG("data has gotten!");
	}
	catch (exception ex) {
		CCLOG("%s", ex.what());
		return 0;
	}*/
	return 0;
}

bool LocalDb::f_check() {
	auto futils = FileUtils::getInstance();
	//CHECK LOCAL DATABASE CREATED OR NOT
	std::string db_dir = futils->getWritablePath() + "db/merchandise.db3";
	cocos2d::Data db_file = futils->getDataFromFile(db_dir);
	if (db_file.isNull()) {
		return false; // db not created yet.
	}
	db_dir = futils->getWritablePath() + "db/occupant.db3";
	db_file = futils->getDataFromFile(db_dir);
	if (db_file.isNull()) {
		return false;
	}

	db_dir = futils->getWritablePath() + "db/deal.db3";
	db_file = futils->getDataFromFile(db_dir);
	if (db_file.isNull()) {
		return false;
	}
	return true; //db already created.
}

bool LocalDb::f_init() {
	//check if local dbs up to date and update them.
	auto futils = FileUtils::getInstance();
	if (futils->createDirectory(FileUtils::getInstance()->getWritablePath() + "db") == false) {
		CCLOG("directory db cannot created at writablepath");
		return false;
	}
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "db/merchandise.db3";

	auto data = FileUtils::getInstance()->getDataFromFile("db/merchandise.db3");
	if (data.isNull()) {
		CCLOG("Resource dosyasindan data erisimi basarisiz.");
		return false;
	}

	FILE *dbfiletodest = fopen(path.c_str(), "wb");
	if (dbfiletodest != NULL) {
		fwrite(data.getBytes(), 1, data.getSize(), dbfiletodest);
		fclose(dbfiletodest);
		CCLOG("File in path db/testomnia.db3 ");
		CCLOG("has written to ... ");
		CCLOG("succesfully.");
	}
	else {
		CCLOG("error in localdb");
		return false;
	}

	path = cocos2d::FileUtils::getInstance()->getWritablePath() + "db/occupant.db3";
	data = FileUtils::getInstance()->getDataFromFile("db/occupant.db3");
	if (data.isNull()) {
		CCLOG("Resource dosyasindan data erisimi basarisiz.");
		return false;
	}

	dbfiletodest = fopen(path.c_str(), "wb");
	if (dbfiletodest != NULL) {
		fwrite(data.getBytes(), 1, data.getSize(), dbfiletodest);
		fclose(dbfiletodest);
		CCLOG("File in path db/occupant.db3 ");
		CCLOG("has written to ... ");
		CCLOG("succesfully.");
	}
	else {
		CCLOG("error 22");
		return false;
	}

	path = cocos2d::FileUtils::getInstance()->getWritablePath() + "db/deal.db3";
	data = FileUtils::getInstance()->getDataFromFile("db/deal.db3");
	if (data.isNull()) {
		CCLOG("Resource dosyasindan data erisimi basarisiz.");
		return false;
	}

	dbfiletodest = fopen(path.c_str(), "wb");
	if (dbfiletodest != NULL) {
		fwrite(data.getBytes(), 1, data.getSize(), dbfiletodest);
		fclose(dbfiletodest);
		CCLOG("File in path db/deal.db3 ");
		CCLOG("has written to ... ");
		CCLOG("succesfully.");
	}
	else {
		CCLOG("error 22");
		return false;
	}

	if (FileUtils::getInstance()->removeFile("db/merchandise.db3")) {
		CCLOG("db/merchandise.db3 removed succesfully.");
	}
	else {
		CCLOG("error 23");
		return false;
	}

	if (FileUtils::getInstance()->removeFile("db/occupant.db3")) {
		CCLOG("db/occupant.db3 removed succesfully.");
	}
	else {
		CCLOG("error 23");
		return false;
	}

	if (FileUtils::getInstance()->removeFile("db/deal.db3")) {
		CCLOG("db/merchandise.db3 removed succesfully.");
	}
	else {
		CCLOG("error 23");
		return false;
	}
	return true;
}

CardList *LocalDb::deckDataFromLocal(std::map<int,int> *dst) {
	CardList *ddfl = new CardList();
#define CARD_COLOR_COUNT 5 //except green
#define EOA -10 //except green
	auto futils = FileUtils::getInstance();
	sqlite3 *sqlite = NULL;
	char **td;
	char *err;
	int r; //player count has gotten from db
	int c;
	int crs; //cursor for go through **td
	int card_color[CARD_COLOR_COUNT + 1] = {CARDINDEX_BRONZE,CARDINDEX_SILVER,CARDINDEX_GREEN,CARDINDEX_YELLOW,CARDINDEX_BLUE,-10};
	std::string dbpath = futils->getWritablePath() + CARD_DB_DIR;
	int result;
	if (SQLITE_OK != (result = sqlite3_open_v2(dbpath.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr))) {
		sqlite3_close(sqlite);
		return NULL;
	}

	for (int indx = 0;card_color[indx] != EOA;indx++) {
		int cclr_count = (*dst)[card_color[indx]];
		if (cclr_count != 0) {
			std::string query = this->get_queryofcolor(card_color[indx],cclr_count);
			if (SQLITE_OK != sqlite3_get_table(sqlite, query.c_str(), &td, &r, &c, &err)) {
				sqlite3_close(sqlite);
				return NULL;
			}
			for (int i = c,crs=1; i < (r + 1)*c; crs++, i = crs*c) {//add carts to ddfl(deck) stack
				CardList::oneCard * card_data = new CardList::oneCard;

				card_data->color = card_color[indx];
				card_data->avg = atoi(td[i]);
				card_data->name = td[i + 1];
				card_data->land = td[i + 2];
				card_data->position = td[i + 3];
				card_data->team = td[i + 4];
				card_data->rate = atoi(td[i + 5]);

				ddfl->add(card_data);
				//cocos2d::log("%s %s %s %s %s %s", td[i],td[i+1],td[i+2],td[i+3],td[i+4],td[i+5]);
			}
		}
	}
	sqlite3_close(sqlite);
	return (ddfl != NULL ) ? ddfl : NULL;
}

bool LocalDb::check_user()
{
#define NO_USER_SIGN_BIT_VALUE -10
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + "db/occupant.db3";
	cocos2d::log("wirtable path in check_user() : %s", path.c_str());
	sqlite3 *sqlite = NULL;
	char **td;
	char *err;
	int r;
	int c;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr)) {
		cocos2d::log("error opennig occupant.db3.");
		sqlite3_close(sqlite);
		return false;
	}
	const char * query = "SELECT uid,uname,tp,xp,level,win_count,lost_count,pts,slot_count,sign_byte FROM tcg3_hierarchist where sign_byte = -10";
	if (SQLITE_OK == sqlite3_get_table(sqlite,query,&td,&r,&c,&err)) {
		cocos2d::log("PLAYER INFO->>>");
		cocos2d::log("%s", td[10]);
		cocos2d::log("%s", td[11]);
		cocos2d::log("%s", td[12]);
		cocos2d::log("%s", td[13]);
		cocos2d::log("%s", td[14]);
		cocos2d::log("%s", td[15]);
		cocos2d::log("%s", td[16]);
		cocos2d::log("%s", td[17]);
		cocos2d::log("%s", td[18]);
		cocos2d::log("%s", td[19]);
		cocos2d::log("<<<-PLAYER INFO");
		if ( strcmp(td[10], "NOUSER") == 0 ) {
			cocos2d::log("no user data.");
			sqlite3_close(sqlite);
			return false;
		}
		else {
			//USER DATA FOUND. assign data to player object.
			auto player = PlayerOne::getInstance();
			//one by one
			/*
			unsigned int uid
			const char *uname
			unsigned int tp
			unsigned int xp
			unsigned int level
			unsigned int lost_count
			unsigned int win_count
			unsigned int pts
			*/
			player->init_data(atoi(td[10]), td[11], atoi(td[12]),atoi(td[13]),atoi(td[14]),atoi(td[15]), atoi(td[16]), atoi(td[17]),atoi(td[18]));
			sqlite3_close(sqlite);
			return true;
		}
	}
	else {
		cocos2d::log("%s",err);
		sqlite3_close(sqlite);
		return false;
	}
}

bool LocalDb::save_user_entry(bool update_required)
{
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + "db/occupant.db3";
	cocos2d::log("wirtable path in save_user_entry() : %s",path.c_str());
	sqlite3 *sqlite = NULL;
	char *err;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READWRITE, nullptr)) {
		cocos2d::log("error opennig occupant.db3. localdb->save_user_entry()");
		sqlite3_close(sqlite);
		return false;
	}
	auto player = PlayerOne::getInstance();
	
	unsigned int uid = player->get_uid();
	unsigned int tp = player->get_tp();
	std::string uname = player->get_username();
	unsigned int xp = player->get_xp();
	unsigned int level = player->get_level();
	unsigned int win_count = player->get_win_count();
	unsigned int lost_count = player->get_lost_count();
	unsigned int update_db = 0;
	unsigned int pts = player->get_pts();

	if (update_required) {
		update_db = 1;
	}

	std::ostringstream uids,tps,xps,levels,wins,losts,ptss,ureqs;
	uids << uid;
	tps << tp;
	xps << xp;
	levels << level;
	wins << win_count;
	losts << lost_count;
	ptss << pts;
	ureqs << update_db;

	std::string query = "UPDATE tcg3_hierarchist SET uid = "
		+ uids.str() +
		" , uname = '" 
		+ uname +
		"' , tp = " 
		+ tps.str() +
		" , xp = "
		+ xps.str() +
		" , level = "
		+ levels.str() +
		" , win_count = "
		+ wins.str() +
		" , lost_count = "
		+ losts.str() +
		" , update_required = "
		+ ureqs.str() +
		" , pts = "
		+ ptss.str() +
		" WHERE sign_byte = -10;";

	cocos2d::log("save_user_entry() 's query = %s",query.c_str());
	
	if (SQLITE_OK == sqlite3_exec(sqlite, query.c_str(), nullptr, 0, &err)) {
		sqlite3_close(sqlite);
		return true;
	}
	else {
		cocos2d::log("localdb->save_user_entry() error : %s", err);
		sqlite3_close(sqlite);
		return false;
	}
}

bool LocalDb::get_player_sh()
{
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + USER_DB_DIR;
	sqlite3 *sqlite = NULL;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr)) {
		cocos2d::log("error opennig occupant.db3 int function %s",__func__);
		sqlite3_close(sqlite);
		return false;
	}
	auto player = PlayerOne::getInstance();
	char **td;
	char *err;
	int r;
	int c;
	const char *query = "SELECT card_id,card_color FROM tcg3_sh ORDER BY seq ASC LIMIT 5";
	if (SQLITE_OK == sqlite3_get_table(sqlite, query, &td, &r, &c, &err)) {
		std::vector<CardList::oneCard> *sh_ref = player->get_sh_ref();
		
		for (int i = c, crs = 1; i < (r + 1)*c; crs++, i = crs*c) {
			CardList::oneCard *oc = new CardList::oneCard;
			oc->card_id = atoi(td[i]);
			oc->color = atoi(td[i+1]);
			int offset = crs - 1;
			if (oc->card_id != 0) {
				//get single card
				this->get_single_card(oc);
				sh_ref->erase(sh_ref->begin() + offset);
				sh_ref->emplace(sh_ref->begin() + offset, *oc);
				cocos2d::log("offset = %d",offset);
			} else {
				//push empty slot.
				sh_ref->erase(sh_ref->begin() + offset);
				sh_ref->emplace(sh_ref->begin() + offset, *oc);
				cocos2d::log("offset = %d", offset);
			}
		}
		sqlite3_close(sqlite);
		cocos2d::log("sh deployed.");
		return true;
	}
	cocos2d::log("sh cannot be deployed!1");
	sqlite3_close(sqlite);
	return false;
}

bool LocalDb::save_player_sh(std::vector<CardList::oneCard> *sh_ins)
{
	auto futils = FileUtils::getInstance();
	std::string dir = futils->getWritablePath() + USER_DB_DIR;
	sqlite3 *sqlite;
	char *err;
	if (SQLITE_OK != sqlite3_open_v2(dir.c_str(), &sqlite, SQLITE_OPEN_READWRITE, nullptr)) {
		cocos2d::log("cant open user database in function %s()",__func__);
		sqlite3_close(sqlite);
		return false;
	}

	const char *query_delete = "DELETE FROM tcg3_sh WHERE 1=1";
	std::vector<CardList::oneCard>::iterator oc = sh_ins->begin();
	ostringstream query_prep;
	query_prep << "INSERT INTO tcg3_sh(seq,card_id,card_color) VALUES";
	int i = 1;

	while (oc != sh_ins->end()) {
		if ((*oc).card_id != 0) {
			query_prep << " (" << i << "," << (*oc).card_id << "," << (*oc).color << ")";
		}
		else {
			query_prep << " (" << i << ",0,-1) ";
		}
		if (i < 5)
			query_prep << ",";
		oc++; i++;
	}

	if (SQLITE_OK != sqlite3_exec(sqlite, query_delete, nullptr, nullptr, &err)) {
		cocos2d::log("error on sh saving proccess in function %s() , ERROR MSG => %s",__func__,err);
		sqlite3_close(sqlite);
		return false;
	}
	//DIPNOT : eger delete query ok dondurur insert query false dondururse sictik demektir.
	if (SQLITE_OK != sqlite3_exec(sqlite, query_prep.str().c_str(), nullptr, nullptr, &err)) {
		cocos2d::log("error on sh saving proccess in function %s() , ERROR MSG => %s", __func__, err);
		sqlite3_close(sqlite);
		return false;
	}
	cocos2d::log("%s",query_prep.str().c_str());
	sqlite3_close(sqlite);
	return true;
}

bool LocalDb::get_single_card(CardList::oneCard *oc)
{
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + CARD_DB_DIR;
	sqlite3 *sqlite = NULL;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr)) {
		cocos2d::log("error opennig occupant.db3 int function %s", __func__);
		sqlite3_close(sqlite);
		return false;
	}
	auto player = PlayerOne::getInstance();
	char **td;
	char *err;
	int r;
	int c;
	std::string query = this->get_queryofcolor(oc->color, 1, oc->card_id);
	if (SQLITE_OK == sqlite3_get_table(sqlite, query.c_str(), &td, &r, &c, &err)) {
		oc->avg = atoi(td[9]);
		oc->name = td[10];
		oc->land = td[11];
		oc->position = td[12];
		oc->team = td[13];
		oc->rate = atoi(td[14]);
		oc->price = (unsigned int)atoi(td[15]);
		sqlite3_close(sqlite);
		return true;
	}
	cocos2d::log("%s",err);
	sqlite3_close(sqlite);
	return false;
}

bool LocalDb::get_single_card_po(CardList::oneCard *oc)
{
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + CARD_DB_DIR;
	sqlite3 *sqlite = NULL;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr)) {
		cocos2d::log("error opennig occupant.db3 int function %s", __func__);
		sqlite3_close(sqlite);
		return false;
	}
	auto player = PlayerOne::getInstance();
	char **td;
	char *err;
	int r;
	int c;
	ostringstream query_prepare;
	query_prepare << "SELECT id,avg,name,land,position,team,price FROM ";
	query_prepare << table_name(oc->color);
	query_prepare << " WHERE rate = ";
	query_prepare << oc->rate;
	query_prepare << " ORDER BY random() LIMIT 1;";
	std::string query = query_prepare.str();
	if (SQLITE_OK != sqlite3_get_table(sqlite, query.c_str(), &td, &r, &c, &err)) {
		cocos2d::log("%s", err);
		sqlite3_close(sqlite);
		return false;
		
	}
	oc->card_id = atoi(td[7]);
	oc->avg = atoi(td[8]);
	oc->name = td[9];
	oc->land = td[10];
	oc->position = td[11];
	oc->team = td[12];
	oc->price = static_cast<unsigned int>(atoi(td[13]));
	sqlite3_close(sqlite);
	return true;
}

bool LocalDb::get_marketItems_cardpack(std::map<int, MarketItemDbData*>* items)
{
	auto futils = FileUtils::getInstance();
	std::string path = futils->getWritablePath() + SHOP_DB_DIR;
	sqlite3 *sqlite = NULL;
	if (SQLITE_OK != sqlite3_open_v2(path.c_str(), &sqlite, SQLITE_OPEN_READONLY, nullptr)) {
		cocos2d::log("error opennig deal.db3 int function %s", __func__);
		sqlite3_close(sqlite);
		return false;
	}
	char **td;
	char *err;
	int r;
	int c;
	std::string query = "SELECT unique_id,name,price_tp FROM tcg3_market_cardpack ORDER BY price_tp DESC";
	if (SQLITE_OK != sqlite3_get_table(sqlite, query.c_str(), &td, &r, &c, &err)) {
		cocos2d::log("error opennig deal.db3 int function %s", __func__);
		sqlite3_close(sqlite);
		return false;
	}
	

	for (int i = c, crs = 1; i < (r + 1)*c; crs++, i = crs*c) {
		MarketItemDbData *item = new MarketItemDbData;

		item->unique_id = td[i];
		item->name = td[i + 1];
		item->price_tp = (unsigned int)atoi(td[i+2]);

		items->insert(std::pair<int, MarketItemDbData*>(crs-1, item));
	}
	sqlite3_close(sqlite);
	return true;
}

std::string LocalDb::get_queryofcolor(int color,int count,int id) {
	string cols = "id, avg, name, land, position, team, rate, price";
	string tbl = table_name(color);
	std::ostringstream stm;
	stm << count;
	if (id != -1) {
		std::ostringstream idc;
		idc << id;
		std::string q =  "SELECT " + cols + " FROM " + tbl + " WHERE id=" + idc.str() + " LIMIT " + stm.str();
		return q;
	}
	return "SELECT "+cols+" FROM "+tbl+" ORDER BY random() LIMIT "+stm.str();
}

std::string LocalDb::table_name(int color) {
	switch (color) {
	case CARDINDEX_BRONZE:
		return "tcg3_bronze_v1";
	case CARDINDEX_SILVER:
		return "tcg3_silver_v1";
	case CARDINDEX_GREEN:
		return "tcg3_green_v1";
	case CARDINDEX_YELLOW:
		return "tcg3_yellow_v1";
	case CARDINDEX_BLUE:
		return "tcg3_blue_v1";
	}
}