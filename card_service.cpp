#include "stdafx.h"

#include "card_service.h"
#include "card_file.h"
#include "global.h"
#include "tool.h"
#include "model.h"

static lpCardNode cardlist = NULL;
static lpCardNode head = NULL;
static lpCardNode tail = NULL;
static Card searchResult[20];

int initCardList() {
	head = (lpCardNode)malloc(sizeof(CardNode));
	
	if (NULL != head) {
		head->next = NULL;
		cardlist = tail = head;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

int addCard(const char* cardno, const char* passwd, float money) {
	time_t nowTime;
	struct tm* tStart;
	struct tm* tEnd;

//	if (NULL != queryCard(cardno))
//		return _CS_EXISTED_;

//	lpCardNode p = (lpCardNode)malloc(sizeof(CardNode));
//	Card *aCard = &p->data;

	Card temp;
	Card *aCard = &temp;
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

//	p->next = NULL;
//	tail->next = p;
//	tail = p;

	if (FALSE == saveCard(aCard, _CARD_PATH_)) {
		return FALSE;
	}

	return TRUE;
}

void releaseCardList() {
	lpCardNode p = cardlist, temp;

	while (NULL != p) {
		temp = p;
		p = p->next;
		free(temp);
	}
}

static int getCard() {
	int nCount = 0, index;
	Card *pCard = NULL;
	lpCardNode p, q;

	releaseCardList();
	nCount = getCardCount(_CARD_PATH_);
	pCard = (Card*)calloc(nCount, sizeof(Card));
	
	if (nCount == 0 || FALSE == readCard(pCard, _CARD_PATH_)) {
		free(pCard);
		return FALSE;
	}

	initCardList();
	p = cardlist;
	for (index = 0; index < nCount; index++) {
		q = (lpCardNode)malloc(sizeof(CardNode));
		q->next = NULL;
		q->data = pCard[index];
		p->next = q;
		p = q;
	}
	free(pCard);
	return TRUE;
}

Card* queryCard(const char* cardno) {
	lpCardNode p;

	if (FALSE == getCard()) {
		return NULL;
	}

	for (p = cardlist->next; p != NULL; p = p->next) {
		if (0 == strcmp(p->data.aName, cardno))
			break;
	}
	if (p == NULL) {
		return NULL;
	}
	else {
		return &p->data;
	}
}

Card* queryCards(const char* pName, int* pIndex) {
	lpCardNode p;
	int n = 0;

	if (FALSE == getCard()) {
		(*pIndex) = 0;
		return NULL;
	}

	for (p = cardlist->next; p != NULL; p = p->next) {
		if (NULL != strstr(p->data.aName, pName) || 0 == strcmp(pName, "*")) {
			searchResult[n++] = p->data;
		}
	}
	(*pIndex) = n;
	return searchResult;
}

Card* doLogon(const char* pName, const char* pPwd) {
	Card* pCard = NULL;
	lpCardNode p;
	int nIndex = 0;

	if (FALSE == getCard()) {
		return NULL;
	}

	p = cardlist->next;
	while (NULL != p) {
		if (0 == strcmp(p->data.aName, pName) && 0 == strcmp(p->data.aPwd, pPwd)) {
			pCard = &p->data;

			if (0 != pCard->nStatus) {
				return NULL;
			}
			if (0 >= pCard->fBalance) {
				return NULL;
			}

			pCard->nStatus = 1;
			pCard->nUseCount++;
			pCard->tLast = time(NULL);
			
			if (FALSE == updateCard(pCard, _CARD_PATH_, nIndex)) {
				pCard = NULL;
			}
			break;
		}
		nIndex++;
		p = p->next;
	}

	return pCard;
}