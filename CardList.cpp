#include "CardList.h"

bool CardList::add(CardList::oneCard *newdata) {
	if (this->firstNode == NULL) {
		newdata->prevNode = NULL;
		this->firstNode = newdata;
		this->topNode = newdata;
		this->sizeoflist++;
		return true;
	}
	oneCard *tmp = this->topNode;
	this->topNode = newdata;
	this->topNode->prevNode = tmp;
	this->sizeoflist++;
	return true;
}

CardList::oneCard *CardList::top() {
	return this->topNode;
}

CardList::oneCard *CardList::pop() {
	if (this->topNode != NULL) {
		oneCard *tmp = this->topNode;
		this->topNode = this->topNode->prevNode;
		this->sizeoflist--;
		return tmp;
	}
	return NULL;
}

void CardList::copy(CardList::oneCard *s1, CardList::oneCard *s2) {
	s1->avg = s2->avg;
	s1->color = s2->color;
	s1->land = s2->land;
	s1->name = s2->name;
	s1->position = s2->position;
	s1->rate = s2->rate;
	s1->team = s2->team;
}
bool CardList::isEmpty() {
	if (this->topNode == NULL) {
		return true;
	}
	return false;
}
int CardList::size() {
	return this->sizeoflist;
}

void CardList::disorder()
{
	int size = this->size();
	vector<oneCard*> *list = new vector<oneCard *>();

	while (!this->isEmpty()) {
		list->push_back(this->pop());
	}

	for (int i = 0; i < size; i++) {
		int selected_indx = RandomHelper::random_int(0,(int)(list->size()-1));
		this->add(*(list->begin() + selected_indx));
		list->erase(list->begin() + selected_indx);
	}
}
