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