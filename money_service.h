#ifndef __MONEY__SERVICE__H__
#define __MONEY__SERVICE__H__

#include "model.h"

int addMoney(const char* pName, Money* pMoney);

Money* queryMoney(const char* pName, int* pIndex);

void releaseMoneyList();

int initMoneyList();

int getMoney();

#endif