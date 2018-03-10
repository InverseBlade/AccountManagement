#ifndef __CARD_SERVICE__H__
#define __CARD_SERVICE__H__

#define _CARD_TOTAL_ 50

#include "model.h"

int addCard(const char* cardno, const char* password, float money);  //Ìí¼Ó¿¨

Card* queryCard(const char* cardno);								 //²éÑ¯¿¨

Card* queryCards(const char* pName, int* pIndex);

void releaseCardList();

Card* doLogon(const char* pName, const char* pPwd);

#endif // !__CARD_SERVICE__H__

