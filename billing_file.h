#ifndef __BILLING_FILE__H__
#define __BILLING_FILE__H__

#include "model.h"

int saveBilling(const Billing* pBilling, const char* pPath);

int readBilling(Billing* pBilling, const char* pPath);

int getBillingCount(const char* pPath);

int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);

#endif // !__BILLING_FILE__H__

