#ifndef __MONEY__FILE__H__
#define __MONEY__FILE__H__

#include "model.h"

int saveMoney(const Money* pMoney, const char* pPath);

int readMoney(Money* pMoney, const char* pPath);

int getMoneyCount(const char* pPath);

int updateMoney(const Money* pMoney, const char* pPath, int nIndex);

#endif // !__MONEY__FILE__H__
