#include "stdafx.h"

#include "billing_file.h"

int saveBilling(const Billing* pBilling, const char* pPath) {
	FILE *fp = NULL;

	if (NULL == (fp = fopen(pPath, "ab"))) {
		return FALSE;
	}

	if (0 >= fwrite(pBilling, sizeof(Billing), 1, fp)) {
		fclose(fp);
		return FALSE;
	}
	fclose(fp);
	return TRUE;
}

int readBilling(Billing* pBilling, const char* pPath) {
	FILE *fp = NULL;
	Billing bill;
	int nIndex = 0;

	if (NULL == (fp = fopen(pPath, "rb"))) {
		return FALSE;
	}
	while (!feof(fp)) {
		if (0 < fread(&bill, sizeof(Billing), 1, fp)) {
			pBilling[nIndex++] = bill;
		}
	}
	fclose(fp);
	return TRUE;
}

int getBillingCount(const char* pPath) {
	FILE *fp = NULL;
	Billing bill;
	int nIndex = 0;

	if (NULL == (fp = fopen(pPath, "rb"))) {
		return FALSE;
	}

	while (!feof(fp)) {
		if (0 < fread(&bill, sizeof(Billing), 1, fp)) {
			nIndex++;
		}
	}
	return nIndex;
}