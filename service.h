#ifndef __SERVICE__H__
#define __SERVICE__H__

#include "model.h"

/**
 *�����ϻ�
 *return: TRUE:�ϻ��ɹ� FALSE:�ϻ�ʧ�� _UNUSE_:������ʹ�� _NOT_ENOUGH_MONEY:����
 */
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);

/**
 *��ӿ�
 *return: TRUE:�ɹ�  FALSE:ʧ��
 */
int addCardInfo(const char* cardno, const char* passwd, float money);

/**
 *queryCardInfo
 *��ģʽ��ѯ
 *param: mode: 1:��׼��ѯ 2.ģ����ѯ   pIndex: NULL:��׼��ѯ
 *return: Card* 
 */
Card* queryCardInfo(const char* pName, int mode, int* pIndex);

/**
 *�ͷſ���Ϣ����
 *param : none
 *return : void
 */
void releaseList();

/**
 *�����»�
 *return: TRUE:�ɹ�  FALSE:ʧ��
 */
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);

/**
 *�������
 *param: time_t tStart �ϻ�ʱ��
 *return: double �������ѽ��
 */
double getAmount(time_t tStart);

/**
 *��ֵ
 *param: 
 *return: TRUE : ��ֵ�ɹ�  FALSE : ��ֵʧ��
 */
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo *pMoneyInfo);

/**
 *�˷�
 *param: 
 *return: TRUE : �˷ѳɹ�  FALSE : �˷�ʧ��
 */
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo *pMoneyInfo);

#endif // !__SERVICE__H__

