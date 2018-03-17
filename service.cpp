#include "stdafx.h"

#include "service.h"
#include "global.h"
#include "card_service.h"
#include "card_file.h"
#include "billing_service.h"
#include "billing_file.h"
#include "money_service.h"
#include "money_file.h"

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo) {
	Card* pCard = NULL;
	Billing bill;
	int nIndex = 0;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18 && 1 <= strlen(pPwd) && strlen(pPwd) <= 8))
		return FALSE;
	if (NULL == (pCard = checkCard(pName, pPwd, &nIndex)))
		return FALSE;
	if (0 != pCard->nStatus) 
		return _UNUSE_;
	if (0 >= pCard->fBalance) 
		return _NOT_ENOUGH_MONEY;

	pCard->nStatus = 1;
	pCard->nUseCount++;
	pCard->tLast = time(NULL);

	if (FALSE == updateCard(pCard, _CARD_PATH_, nIndex))
		return FALSE;

	strcpy(bill.aCardName, pCard->aName);
	bill.tStart = time(NULL);
	bill.tEnd = 0;
	bill.fAmount = 0.f;
	bill.nStatus = 0;
	bill.nDel = 0;

	if (FALSE == addBilling(pCard->aName, &bill))
		return FALSE;

	strcpy(pInfo->aCardName, pCard->aName);
	pInfo->tLogon = bill.tStart;
	pInfo->fBalance = pCard->fBalance;

	return TRUE;
}

int addCardInfo(const char* cardno, const char* passwd, float money) {
	time_t nowTime;
	struct tm* tStart;
	struct tm* tEnd;
	Card card;
	Card *aCard = &card;

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8 && 0 <= money))
		return FALSE;

	strcpy(aCard->aName, cardno);
	strcpy(aCard->aPwd, passwd);

	aCard->nStatus = 0;
	aCard->fBalance = money;
	aCard->fTotalUse = money;
	aCard->nDel = 0;
	aCard->nUseCount = 0;

	nowTime = time(NULL);
	aCard->tStart = nowTime;
	aCard->tLast = nowTime;
	aCard->tEnd = nowTime;

	tStart = localtime(&aCard->tStart);
	tEnd = localtime(&aCard->tEnd);
	tEnd->tm_year = tStart->tm_year + 1;
	aCard->tEnd = mktime(tEnd);

	if (FALSE == addCard(card))
		return FALSE;

	return TRUE;
}

int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo) {
	Card *pCard = NULL;
	Billing *pBilling = NULL;
	int cardIndex, billingIndex;
	float fBalance;
	double dbAmount;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18 && 1 <= strlen(pPwd) && strlen(pPwd) <= 8))
		return FALSE;
	if (NULL == (pCard = checkCard(pName, pPwd, &cardIndex)))
		return FALSE;
	if (1 != pCard->nStatus)
		return FALSE;
	if (NULL == (pBilling = queryBilling(pName, &billingIndex)))
		return FALSE;

	dbAmount = getAmount(pBilling->tStart);
	if (0 > (fBalance = (float)(pCard->fBalance - dbAmount)))
		return _NOT_ENOUGH_MONEY;
	
	pCard->fBalance = fBalance;
	pCard->nStatus = 0;
	pCard->tLast = time(NULL);
	if (FALSE == updateCard(pCard, _CARD_PATH_, cardIndex))
		return FALSE;

	pBilling->fAmount = dbAmount;
	pBilling->nStatus = 1;
	pBilling->tEnd = time(NULL);
	if (FALSE == updateBilling(pBilling, _BILLING_PATH_, billingIndex))
		return FALSE;

	strcpy(pInfo->aCardName, pName);
	pInfo->fAmount = dbAmount;
	pInfo->fBalance = fBalance;
	pInfo->tEnd = time(NULL);
	pInfo->tStart = pBilling->tStart;

	return TRUE;
}

Card* queryCardInfo(const char* pName, int mode, int* pIndex) {
	Card *pCard = NULL;
	int total = 0;

	if (mode == 1 || pIndex == NULL) {
		if (NULL == (pCard = queryCard(pName)))
			return NULL;
		return pCard;
	}
	else if (mode == 2 && pIndex != NULL) {
		pCard = queryCards(pName, &total);
		if (pCard == NULL || total == 0) {
			*pIndex = 0;
			return NULL;
		}
		(*pIndex) = total;
		return pCard;
	}
	
	return NULL;
}

double getAmount(time_t tStart) {
	time_t tEnd, nSec, nMinutes;
	int nCount;

	tEnd = time(NULL);
	nSec = tEnd - tStart;
	nMinutes = nSec / 60;

	nMinutes % _UNIT_ ? (nCount = 1 + nMinutes / _UNIT_) : (nCount = nMinutes / _UNIT_);
	
	return nCount * _CHARGE_;
}

void releaseList() {
	releaseCardList();
	releaseBillingList();
	releaseMoneyList();
}

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo *pMoneyInfo) {
	Money money;
	Card* card = NULL;
	int nIndex = 0;
	time_t nowTime;
	float fMoney = pMoneyInfo->fMoney;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18 && 1 <= strlen(pPwd) && strlen(pPwd) <= 8)) 
		return FALSE;
	if (0 >= fMoney) 
		return FALSE;
	if (NULL == (card = checkCard(pName, pPwd, &nIndex))) 
		return FALSE;
	if (2 == card->nStatus || 3 == card->nStatus) 
		return FALSE;

	nowTime = time(NULL);

	//card->nUseCount++;
	//card->tLast = nowTime;
	card->fTotalUse += fMoney;
	card->fBalance += fMoney;
	
	if (FALSE == updateCard(card, _CARD_PATH_, nIndex))
		return FALSE;

	strcpy(money.aCardName, pName);
	money.fMoney = fMoney;
	money.nDel = 0;
	money.nStatus = 0;
	money.tTime = nowTime;
	
	if (FALSE == addMoney(pName, &money))
		return FALSE;

	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	pMoneyInfo->fMoney = fMoney;

	return TRUE;
}

int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo *pMoneyInfo) {
	Money money;
	Card* card = NULL;
	int nIndex = 0;
	time_t nowTime;
	float fMoney = 0;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18 && 1 <= strlen(pPwd) && strlen(pPwd) <= 8))
		return FALSE;
	if (NULL == (card = checkCard(pName, pPwd, &nIndex)))
		return FALSE;
	if (1 == card->nStatus)
		return _UNUSE_;
	if (0 >= card->fBalance)
		return _NOT_ENOUGH_MONEY;

	fMoney = card->fBalance;
	nowTime = time(NULL);

	//card->nUseCount++;
	//card->tLast = nowTime;
	card->fBalance = 0;
	card->fTotalUse -= fMoney;

	if (FALSE == updateCard(card, _CARD_PATH_, nIndex))
		return FALSE;

	strcpy(money.aCardName, pName);
	money.fMoney = fMoney;
	money.nDel = 0;
	money.nStatus = 1;
	money.tTime = nowTime;

	if (FALSE == addMoney(pName, &money))
		return FALSE;

	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	pMoneyInfo->fMoney = fMoney;

	return TRUE;
}

int annulCard(Card* pCard) {
	Card *card = NULL;
	int nIndex = 0;
	float fMoney = 0;

	if (NULL == (card = checkCard(pCard->aName, pCard->aPwd, &nIndex)))
		return FALSE;
	if (0 != card->nStatus)
		return FALSE;

	fMoney = card->fBalance;
	card->fBalance = 0;
	card->nStatus = 2;
	card->tLast = time(NULL);
	card->fTotalUse -= fMoney;

	if (FALSE == updateCard(card, _CARD_PATH_, nIndex))
		return FALSE;

	strcpy(pCard->aName, card->aName);
	pCard->fBalance = fMoney;

	return TRUE;
}

Billing* queryBillingInfo(Card *pCard, int* pIndex) {
	int nIndex;
	Billing *result;

	if (NULL == (result = queryBillings(pCard->aName, &nIndex)))
		return NULL;
	*pIndex = nIndex;
	return result;
}

Money* queryMoneyInfo(Card *pCard, int *pIndex) {
	int nIndex;
	Money *result;

	if (NULL == (result = queryMoneys(pCard->aName, &nIndex)))
		return NULL;

	*pIndex = nIndex;
	return result;
}