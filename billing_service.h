#ifndef __BILLING_SERVICE__H__
#define __BILLING_SERVICE__H__

#include "model.h"

int addBilling(const char* pName, Billing* pBilling);

void releaseBillingList();

int initBillingList();

int getBilling();

Billing* queryBilling(const char* pName, int* pIndex);

#endif // !__BILLING_SERVICE__H__

