#include "stdafx.h"

#include "money_service.h"
#include "money_file.h"
#include "global.h"

static lpMoneyNode moneyList = NULL;
static lpMoneyNode tail = NULL;
static Money searchResult[20];

Money* queryMoney(const char* pName, int* pIndex) {
	lpMoneyNode p = NULL;
	int nIndex = 0;

	if (FALSE == getMoney())
		return NULL;

	p = moneyList->next;
	while (NULL != p) {
		if (0 == strcmp(pName, p->data.aCardName)) {
			*pIndex = nIndex;
			return &p->data;
		}
		nIndex++;
		p = p->next;
	}

	return NULL;
}

void releaseMoneyList() {
	lpMoneyNode pMoneyNode = moneyList, temp;

	while (NULL != pMoneyNode) {
		temp = pMoneyNode;
		pMoneyNode = pMoneyNode->next;
		free(temp);
	}
}

int initMoneyList() {
	if (NULL == (moneyList = (lpMoneyNode)malloc(sizeof(MoneyNode))))
		return FALSE;

	memset(&moneyList->data, 0, sizeof(Money));
	moneyList->next = NULL;
	tail = moneyList;

	return TRUE;
}

static int getMoney() {
	int nCount = 0, i;
	lpMoneyNode p;
	Money *money = NULL;

	releaseMoneyList();
	if (FALSE == initMoneyList())
		return FALSE;
	if (FALSE == (nCount = getMoneyCount(_MONEY_PATH_)))
		return FALSE;
	if (NULL == (money = (Money*)calloc(nCount, sizeof(Money))))
		return FALSE;
	if (FALSE == readMoney(money, _MONEY_PATH_))
		return FALSE;

	tail = moneyList;
	for (i = 0; i < nCount; i++) {
		if (NULL == (p = (lpMoneyNode)malloc(sizeof(MoneyNode))))
			return FALSE;
		p->data = money[i];
		p->next = NULL;
		tail->next = p;
		tail = p;
	}
	free(money);
	return TRUE;
}

int addMoney(const char* pName, Money* pMoney) {
	strcpy(pMoney->aCardName, pName);

	if (FALSE == saveMoney(pMoney, _MONEY_PATH_)) {
		return FALSE;
	}

	return TRUE;
}