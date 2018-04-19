#include "stdafx.h"

#include "menu.h"
#include "service.h"
#include "global.h"
#include "tool.h"
#include "model.h"

void outputMenu() {
	char str[] = {
		"欢迎进入计费管理系统\n"
		"----------菜单----------\n"
		"1.添加卡\n"
		"2.查询卡\n"
		"3.上机\n"
		"4.下机\n"
		"5.充值\n"
		"6.退费\n"
		"7.查询统计\n"
		"8.注销卡\n"
		"9.清屏\n"
		"0.退出\n"
		"\n"
		"请选择菜单项编号："
	};
	//system("cls");
	printf(str);
}

int controller(int request) {
	int status = _RUN_;

	switch (request)
	{
	case 1:
		add();
		break;
	case 2:
		query();
		break;
	case 3:
		logon();
		break;
	case 4:
		settle();
		break;
	case 5:
		addMoney();
		break;
	case 6:
		refundMoney();
		break;
	case 7:
		statistic();
		break;
	case 8:
		annul();
		break;
	case 9:
		system("cls");
		break;
	case 0:
		exitApp();
		status = _EXIT_;
		break;
	default:
		printf("输入的菜单编号有误，请重新输入！\n");
		break;
	}
	printf("\n");

	return status;
}

void add() {
	char cardno[25], passwd[15];
	int len, status;
	float money;

	printf("----------添加卡----------\n");
	fflush(stdin);
	
	printf("请输入卡号(长度为1~18)：");
	scanf("%20s", cardno);
	len = strlen(cardno);
	if (1 <= len && len <= 18) {
		printf("请输入密码(长度为1~8)：");
		scanf("%10s", passwd);
		len = strlen(passwd);
		if (1 <= len && len <= 8) {
			printf("请输入开卡金额(RMB)：");
			scanf("%f", &money);
			if (0 <= money && TRUE == addCardInfo(cardno, passwd, money)) {
				printf("\n----------开卡成功----------\n");
				printf("卡号\t密码\t状态\t开卡金额\n");
				printf("%s\t%s\t%d\t%.1f\n", cardno, passwd, 0, money);
				return;
			}
		}
	}
	printf("输入有误,开卡失败！\n");
	fflush(stdin);
	return;
}

void query() {
	char cardno[25], tLast[_TIME_LENGTH_];
	const Card* card = NULL;
	int i, total = 0, opt;

	printf("----------查询卡----------\n");
	printf("请选择：1.精准查询  2.模糊查询\n输入数字选项：");
	scanf("%d", &opt);
	if (opt != 1 && opt != 2) {
		printf("输入有误！\n");
		return;
	}
	printf("请输入查询的卡号(长度为1~18)：");
	scanf("%20s", cardno);

	if (18 <= strlen(cardno)) {
		printf("输入有误,查询失败！\n");
		return;
	}
	if (opt == 1) {
		if (NULL == (card = queryCardInfo(cardno, 1, NULL))) {
			printf("没有该卡的信息！\n");
			return;
		}
		timeToString(card->tLast, tLast);
		printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
		printf("%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", card->aName, card->nStatus, card->fBalance, card->fTotalUse, card->nUseCount, tLast);
	}
	else if (opt == 2) {
		card = queryCardInfo(cardno, 2, &total);
		if (card == NULL || total == 0) {
			printf("没有该卡的信息！\n");
			return;
		}
		printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
		for (i = 0; i < total; i++, card++) {
			timeToString(card->tLast, tLast);
			printf("%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", card->aName, card->nStatus, card->fBalance, card->fTotalUse, card->nUseCount, tLast);
		}
	}

	printf("\n");
}

void exitApp() {
	releaseList();
	printf("\n");
}

void logon() {
	char cardno[25], passwd[15];
	char tLogon[_TIME_LENGTH_];
	LogonInfo logonInfo;
	int status;

	printf("----------上机----------\n");
	printf("请输入上机卡号(长度为1~18)：");
	scanf("%20s", cardno);
	printf("请输入上机密码(长度为1~8)：");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("输入有误！\n");
		return;
	}
	status = doLogon(cardno, passwd, &logonInfo);

	switch (status)
	{
	case FALSE:
		printf("上机失败！\n");
		break;
	case TRUE:
		timeToString(logonInfo.tLogon, tLogon);
		printf("----------上机信息如下----------\n");
		printf("卡号\t余额\t上机时间\n");
		printf("%s\t%.1f\t%s\n", logonInfo.aCardName, logonInfo.fBalance, tLogon);
		break;
	case _UNUSE_:
		printf("该卡正在使用或者已注销！\n");
		break;
	case _NOT_ENOUGH_MONEY:
		printf("卡余额不足！\n");
		break;
	default:
		break;
	}

	printf("\n");
}

void settle() {
	int nResult;
	char cardno[25], passwd[15];
	char tStart[_TIME_LENGTH_], tEnd[_TIME_LENGTH_];
	SettleInfo settleInfo;

	printf("----------下机----------\n");
	printf("请输入下机卡号(长度为1~18)：");
	scanf("%20s", cardno);
	printf("请输入下机密码(长度为1~8)：");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("输入有误！\n");
		return;
	}
	nResult = doSettle(cardno, passwd, &settleInfo);

	switch (nResult)
	{
	case _NOT_ENOUGH_MONEY:
		printf("下机失败,卡余额不足！\n");
		break;
	case FALSE:
		printf("下机失败！\n");
		break;
	case TRUE:
		timeToString(settleInfo.tStart, tStart);
		timeToString(settleInfo.tEnd, tEnd);
		printf("----------下机信息如下----------\n");
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
		printf("%s\t%.1f\t%.1f\t%s\t%s\n", settleInfo.aCardName, settleInfo.fAmount, settleInfo.fBalance, tStart, tEnd);
		break;
	default:
		break;
	}
}

void annul() {
	Card card;
	char cardno[25], passwd[15];
	int nResult = 0;

	printf("----------注销卡----------\n");
	printf("请输入注销卡号(长度为1~18)：");
	scanf("%20s", cardno);
	printf("请输入密码(长度为1~8)：");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("输入有误！\n");
		return;
	}
	strcpy(card.aName, cardno);
	strcpy(card.aPwd, passwd);

	nResult = annulCard(&card);
	printf("----------注销信息如下----------\n");
	switch (nResult)
	{
	case TRUE:
		printf("卡号\t退款金额\n");
		printf("%s\t%.1f\n", card.aName, card.fBalance);
		break;
	case FALSE:
		printf("注销卡失败！\n");
		break;
	default:
		break;
	}
	return;
}

void addMoney() {
	MoneyInfo moneyInfo;
	char cardno[25], passwd[15];
	float fMoney = 0;

	printf("----------充值----------\n");
	printf("请输入充值卡号(长度为1~18)：");
	scanf("%20s", cardno);
	printf("请输入充值密码(长度为1~8)：");
	scanf("%10s", passwd);
	printf("请输入充值金额(RMB)：");
	scanf("%f", &fMoney);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("输入有误！\n");
		return;
	}
	moneyInfo.fMoney = fMoney;

	if (TRUE == doAddMoney(cardno, passwd, &moneyInfo)) {
		printf("----------充值信息如下----------\n");
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n", moneyInfo.aCardName, moneyInfo.fMoney, moneyInfo.fBalance);
		return;
	}

	printf("充值失败！\n");
}

void refundMoney() {
	MoneyInfo moneyInfo;
	char cardno[25], passwd[15];
	float fMoney = 0;
	int nResult = 0;

	printf("----------退费----------\n");
	printf("请输入退费卡号(长度为1~18)：");
	scanf("%20s", cardno);
	printf("请输入退费密码(长度为1~8)：");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("输入有误！\n");
		return;
	}
	nResult = doRefundMoney(cardno, passwd, &moneyInfo);

	switch (nResult) {
	case TRUE:
		printf("----------退费信息如下----------\n");
		printf("卡号\t退费金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n", moneyInfo.aCardName, moneyInfo.fMoney, moneyInfo.fBalance);
		break;
	case _NOT_ENOUGH_MONEY:
		printf("卡余额不足！\n");
		break;
	case _UNUSE_:
		printf("退费失败！\n");
		break;
	default:
		printf("退费失败！\n");
		break;
	}
	return;
}

void statistic() {
	char cardno[25], passwd[15];
	char tStart[_TIME_LENGTH_], tEnd[_TIME_LENGTH_];
	int opt, index, total;
	Billing *rs = NULL;
	Money *moneyRS = NULL;
	Card card;

	printf("----------统计查询----------\n");
	printf("1.查询消费记录\t2.查询充值记录\n");
	printf("请输入选项：");
	scanf("%d", &opt);
	printf("请输入查询卡号(长度为1~18)：");
	scanf("%20s", cardno);
	//printf("请输入密码(长度为1~8)：");
	//scanf("%10s", passwd);
	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18)) {
		printf("输入有误！\n");
		return;
	}
	strcpy(card.aName, cardno);

	if (1 == opt) {
		if (NULL == (rs = queryBillingInfo(&card, &total))) {
			printf("查询失败！\n");
			return;
		}
		printf("卡号\t消费金额\t状态\t上机时间\t\t下机时间\n");
		for (index = 0; index < total; index++, rs++) {
			timeToString(rs->tStart, tStart);
			timeToString(rs->tEnd, tEnd);
			printf("%s\t%.1f\t\t%s\t%s\t%s\n", 
				rs->aCardName, rs->fAmount, rs->nStatus? "已结算":"未结算", tStart, tEnd);
		}
	}
	else if (2 == opt) {
		if (NULL == (moneyRS = queryMoneyInfo(&card, &total))) {
			printf("查询失败！\n");
			return;
		}
		printf("卡号\t\t时间\t\t状态\t金额\t删除标识\n\n");
		for (index = 0; index < total; index++, moneyRS++) {
			timeToString(moneyRS->tTime, tStart);
			printf("%s\t%s\t%s\t%.1f\t%d\n",
				moneyRS->aCardName, tStart, moneyRS->nStatus ? "退费" : "充值", moneyRS->fMoney, moneyRS->nDel);
		}
	}
}