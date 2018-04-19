#ifndef __CARD_SERVICE__H__
#define __CARD_SERVICE__H__

#define _CARD_TOTAL_ 50

#include "model.h"

int addCard(Card card);  //添加卡

Card* queryCard(const char* cardno);								 //查询卡

const Card* queryCards(const char* pName, int* pIndex);

void releaseCardList();

Card* checkCard(const char* pName, const char* pPwd, int* pIndex);

#endif // !__CARD_SERVICE__H__

