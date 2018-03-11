#ifndef __SERVICE__H__
#define __SERVICE__H__

#include "model.h"

/**
 *处理上机
 *return: TRUE:上机成功 FALSE:上机失败 _UNUSE_:卡不能使用 _NOT_ENOUGH_MONEY:余额不足
*/
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);

/**
 *添加卡
 *return: TRUE:成功  FALSE:失败
*/
int addCardInfo(const char* cardno, const char* passwd, float money);

/**
 *queryCardInfo
 *多模式查询
 *param: mode: 1:精准查询 2.模糊查询   pIndex: NULL:精准查询
 *return: Card* 
*/
Card* queryCardInfo(const char* pName, int mode, int* pIndex);

/**
 *释放卡信息链表
 *param : none
 *return : void
*/
void releaseList();


#endif // !__SERVICE__H__

