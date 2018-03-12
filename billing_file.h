#ifndef __BILLING_FILE__H__
#define __BILLING_FILE__H__

#include "model.h"

int saveBilling(const Billing* pBilling, const char* pPath);

int readBilling(Billing* pBilling, const char* pPath);

int getBillingCount(const char* pPath);

#endif // !__BILLING_FILE__H__

