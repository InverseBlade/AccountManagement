#include "stdafx.h"

#include "card_service.h"
#include "card_file.h"
#include "global.h"
#include "tool.h"
#include "model.h"

static lpCardNode cardlist = NULL;
static lpCardNode head = NULL;
static lpCardNode tail = NULL;
static Card *cardRS = NULL;

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

int addCard(Card card) {

//	if (NULL != queryCard(cardno))
//		return _CS_EXISTED_;

//	lpCardNode p = (lpCardNode)malloc(sizeof(CardNode));
//	Card *aCard = &p->data;


//	p->next = NULL;
//	tail->next = p;
//	tail = p;

	if (FALSE == saveCard(&card, _CARD_PATH_)) {
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
		/**********将过期卡标注为失效*************/
		if (3 != pCard[index].nStatus && time(NULL) >= pCard[index].tEnd) {
			pCard[index].nStatus = 3;
			updateCard(&pCard[index], _CARD_PATH_, index);
		}

		q = (lpCardNode)malloc(sizeof(CardNode));
		q->next = NULL;
		q->data = pCard[index];
		p->next = q;
		p = q;
	}
	tail = p;
	free(pCard);
	return TRUE;
}

Card* queryCard(const char* cardno) {
	lpCardNode p;

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18)) {
		return NULL;
	}

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
	int n = 0, sum = 0;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18)) {
		return NULL;
	}
	if (FALSE == getCard()) {
		(*pIndex) = 0;
		return NULL;
	}
	for (p = cardlist->next; p != NULL; p = p->next) {
		if (NULL != strstr(p->data.aName, pName) || 0 == strcmp(pName, "*"))
			sum++;
	}
	if (NULL != cardRS)
		free(cardRS);
	cardRS = (Card*)calloc(sum, sizeof(Card));

	for (p = cardlist->next; p != NULL; p = p->next) {
		if (NULL != strstr(p->data.aName, pName) || 0 == strcmp(pName, "*")) {
			cardRS[n++] = p->data;
		}
	}
	(*pIndex) = sum;
	return cardRS;
}

Card* checkCard(const char* pName, const char* pPwd, int* pIndex) {
	Card* pCard = NULL;
	lpCardNode p;
	int nIndex = 0;

	if (!(1 <= strlen(pName) && strlen(pName) <= 18 && 1 <= strlen(pPwd) && strlen(pPwd) <= 8)) {
		return NULL;
	}

	if (FALSE == getCard()) {
		return NULL;
	}

	p = cardlist->next;
	while (NULL != p) {
		if (0 == strcmp(p->data.aName, pName) && 0 == strcmp(p->data.aPwd, pPwd)) {
			pCard = &p->data;
			(*pIndex) = nIndex;
			break;
		}
		nIndex++;
		p = p->next;
	}

	return pCard;
}