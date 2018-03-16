#include "stdafx.h"

#include "money_file.h"
#include "global.h"

int saveMoney(const Money* pMoney, const char* pPath) {
	FILE *fp = NULL;

	if (NULL == (fp = fopen(pPath, "ab"))) {
		if (NULL == (fp = fopen(pPath, "wb"))) {
			return FALSE;
		}
	}

	if (0 >= fwrite(pMoney, sizeof(Money), 1, fp)) {
		fclose(fp);
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

int readMoney(Money* pMoney, const char* pPath) {
	FILE *fp = NULL;
	Money money;
	int nIndex = 0;

	if (NULL == (fp = fopen(pPath, "rb"))) {
		return FALSE;
	}
	while (!feof(fp)) {
		if (0 < fread(&money, sizeof(Money), 1, fp)) {
			pMoney[nIndex++] = money;
		}
	}
	fclose(fp);
	return TRUE;
}

int getMoneyCount(const char* pPath) {
	FILE *fp = NULL;
	Money money;
	int nIndex = 0;

	if (NULL == (fp = fopen(pPath, "rb"))) {
		return FALSE;
	}

	while (!feof(fp)) {
		if (0 < fread(&money, sizeof(Money), 1, fp)) {
			nIndex++;
		}
	}
	fclose(fp);
	return nIndex;
}

int updateMoney(const Money* pMoney, const char* pPath, int nIndex) {
	FILE *fp = NULL;
	size_t lPosition = 0;

	if (NULL == (fp = fopen(pPath, "rb+")))
		return FALSE;

	lPosition = nIndex * sizeof(Money);
	fseek(fp, lPosition, 0);

	if (0 >= fwrite(pMoney, sizeof(Money), 1, fp)) {
		return FALSE;
	}
	fclose(fp);
	return TRUE;
}