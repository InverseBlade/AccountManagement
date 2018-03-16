#include "stdafx.h"

#include "billing_service.h"
#include "billing_file.h"
#include "global.h"

static lpBillingNode billingList = NULL;
static lpBillingNode tail = NULL;
static Billing searchResult[20];

int addBilling(const char* pName, Billing* pBilling) {
	strcpy(pBilling->aCardName, pName);

	if (FALSE == saveBilling(pBilling, _BILLING_PATH_)) {
		return FALSE;
	}

	return TRUE;
}


Billing* queryBilling(const char* pName, int* pIndex) {
	lpBillingNode p = NULL;
	int nIndex = 0;

	if (FALSE == getBilling())
		return NULL;

	p = billingList->next;
	while (NULL != p) {
		if (0 == strcmp(pName, p->data.aCardName) && 0 == p->data.nStatus) {
			*pIndex = nIndex;
			return &p->data;
		}
		nIndex++;
		p = p->next;
	}

	return NULL;
}

void releaseBillingList() {
	lpBillingNode pBillingNode = billingList, temp;

	while (NULL != pBillingNode) {
		temp = pBillingNode;
		pBillingNode = pBillingNode->next;
		free(temp);
	}
}

int initBillingList() {
	if (NULL == (billingList = (lpBillingNode)malloc(sizeof(BillingNode))))
		return FALSE;

	memset(&billingList->data, 0, sizeof(Billing));
	billingList->next = NULL;
	tail = billingList;

	return TRUE;
}

int getBilling() {
	int nCount = 0, i;
	lpBillingNode p, tail;
	Billing *bill = NULL;

	releaseBillingList();
	if (FALSE == initBillingList()) 
		return FALSE;
	if (FALSE == (nCount = getBillingCount(_BILLING_PATH_))) 
		return FALSE;
	if (NULL == (bill = (Billing*)calloc(nCount, sizeof(Billing)))) 
		return FALSE;
	if (FALSE == readBilling(bill, _BILLING_PATH_)) 
		return FALSE;

	tail = billingList;
	for (i = 0; i < nCount; i++) {
		if (NULL == (p = (lpBillingNode)malloc(sizeof(BillingNode))))
			return FALSE;
		p->data = bill[i];
		p->next = NULL;
		tail->next = p;
		tail = p;
	}

	return TRUE;
}