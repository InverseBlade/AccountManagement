#ifndef __CARD_FILE__H__
#define __CARD_FILE__H__

#include"model.h"

#define _CARD_CHAR_NUM_ 256

int saveCard(const Card* pCard, const char* pPath);

int readCard(Card* pCard, const char* pPath);

int getCardCount(const char* pPath);

int updateCard(const Card* pCard, const char* pPath, int nIndex);

#endif // !__CARD__FILE__H__
