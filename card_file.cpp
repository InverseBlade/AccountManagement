#include "stdafx.h"

#include "card_file.h"
#include "model.h"
#include "global.h"
#include "tool.h"

int saveCard(const Card* pCard, const char* pPath) {
	FILE *fp;
	char tStart[20], tEnd[20], tLast[20];

	if (NULL == (fp = fopen(pPath, "a"))) {
		if (NULL == (fp = fopen(pPath, "w"))) {
			return FALSE;
		}
	}
	timeToString(pCard->tStart, tStart);
	timeToString(pCard->tEnd, tEnd);
	timeToString(pCard->tLast, tLast);
	if (fprintf(
		fp,
		"%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n",
		pCard->aName, pCard->aPwd, pCard->nStatus,
		tStart, tEnd, pCard->fTotalUse,
		tLast, pCard->nUseCount, pCard->fBalance, pCard->nDel
	) <= 0) {
		fclose(fp);
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

static Card parseCard(char* pBuf) {
	Card card;
	char *buf, *str, data[10][50];
	const char *delims = "##";
	int index = 0;

	buf = pBuf;
	while (NULL != (str = strtok(buf, delims))) {
		strcpy(data[index++], str);
		buf = NULL;
	}
	strcpy(card.aName, data[0]);
	strcpy(card.aPwd, data[1]);
	card.nStatus = atoi(data[2]);
	card.tStart = stringToTime(data[3]);
	card.tEnd = stringToTime(data[4]);
	card.fTotalUse = (float)atof(data[5]);
	card.tLast = stringToTime(data[6]);
	card.nUseCount = atoi(data[7]);
	card.fBalance = (float)atof(data[8]);
	card.nDel = atoi(data[9]);

	return card;
}

int readCard(Card* pCard, const char* pPath) {
	FILE *fp;
	char aBuf[_CARD_CHAR_NUM_];
	int nIndex = 0;

	if (NULL == (fp = fopen(pPath, "r"))) {
		return FALSE;
	}
	while (NULL != fgets(aBuf, _CARD_CHAR_NUM_, fp)) {
		if (0 < strlen(aBuf)) {
			pCard[nIndex++] = parseCard(aBuf);
		}
	}
	fclose(fp);
	return TRUE;
}

int getCardCount(const char* pPath) {
	char aBuf[_CARD_CHAR_NUM_];
	int nIndex = 0;
	FILE *fp;

	if (NULL == (fp = fopen(pPath, "r"))) {
		return FALSE;
	}
	while (NULL != fgets(aBuf, _CARD_CHAR_NUM_, fp)) {
		nIndex++;
	}
	fclose(fp);
	if (nIndex == 0) {
		return FALSE;
	}
	return nIndex;
}

int updateCard(const Card* pCard, const char* pPath, int nIndex) {
	FILE *fp = NULL;
	char aBuf[_CARD_CHAR_NUM_] = { 0 };
	char tStart[_TIME_LENGTH_] = { 0 };
	char tLast[_TIME_LENGTH_] = { 0 };
	char tEnd[_TIME_LENGTH_] = { 0 };
	int nLine = 0;
	long lPosition = 0L;

	timeToString(pCard->tEnd, tEnd);
	timeToString(pCard->tLast, tLast);
	timeToString(pCard->tStart, tStart);

	if (NULL == (fp = fopen(pPath, "r+"))) {
		return FALSE;
	}
	while (nLine < nIndex && NULL != fgets(aBuf, _CARD_CHAR_NUM_, fp)) {
		lPosition = ftell(fp);
		nLine++;
	}
	fseek(fp, lPosition, 0);
	if (fprintf(
		fp,
		"%s##%s##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n",
		pCard->aName, pCard->aPwd, pCard->nStatus,
		tStart, tEnd, pCard->fTotalUse,
		tLast, pCard->nUseCount, pCard->fBalance, pCard->nDel
	) <= 0) {
		fclose(fp);
		return FALSE;
	}
	fclose(fp);
	return TRUE;
}