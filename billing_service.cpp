#include "stdafx.h"

#include "billing_service.h"
#include "billing_file.h"
#include "global.h"

int addBilling(const char* pName, Billing* pBilling) {
	strcpy(pBilling->aCardName, pName);

	if (FALSE == saveBilling(pBilling, _BILLING_PATH_)) {
		return FALSE;
	}

	return TRUE;
}