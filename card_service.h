#ifndef __CARD_SERVICE__H__
#define __CARD_SERVICE__H__

#define _CARD_TOTAL_ 50

#include "model.h"

int addCard(Card card);  //Ìí¼Ó¿¨

Card* queryCard(const char* cardno);								 //²éÑ¯¿¨

Card* queryCards(const char* pName, int* pIndex);

void releaseCardList();

Card* checkCard(const char* pName, const char* pPwd, int* pIndex);

#endif // !__CARD_SERVICE__H__

