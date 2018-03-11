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
	char str[][50] = {
		"----------添加卡----------\n",
		"请输入卡号(长度为1~18)：",
		"请输入密码(长度为1~8)：",
		"请输入开卡金额(RMB)：",
		"输入有误,开卡失败！\n",
		"\n----------开卡成功----------\n",
		"卡号\t密码\t状态\t开卡金额\n",
		"%s\t%s\t%d\t%.1f\n",
		"开卡失败：已达开卡总数上限！\n",
		"开卡失败：卡号已存在！\n"
	};

	printf(str[0]);
	fflush(stdin);
	
	printf(str[1]);
	scanf("%20s", cardno);
	len = strlen(cardno);
	if (1 <= len && len <= 18) {
		printf(str[2]);
		scanf("%10s", passwd);
		len = strlen(passwd);
		if (1 <= len && len <= 8) {
			printf(str[3]);
			scanf("%f", &money);
			if (0 <= money) {
				if (TRUE == addCardInfo(cardno, passwd, money)) {
					printf(str[5]);
					printf(str[6]);
					printf(str[7], cardno, passwd, 0, money);
					return;
				}
			}
		}
	}
	printf(str[4]);
	fflush(stdin);
	return;
}

void query() {
	char str[][50] = {
		"----------查询卡----------\n",
		"请输入查询的卡号(长度为1~18)：",
		"没有该卡的信息！\n",
		"输入有误,查询失败！\n",
		"卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n",
		"%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n"
	};
	char cardno[25], tLast[20];
	Card* card = NULL;
	int i, total = 0, opt;

	printf(str[0]);
	printf("请选择：1.精准查询  2.模糊查询\n输入数字选项：");
	scanf("%d", &opt);
	if (opt != 1 && opt != 2) {
		printf("输入有误！\n");
		return;
	}
	printf(str[1]);
	scanf("%20s", cardno);

	if (18 <= strlen(cardno)) {
		printf(str[3]);
		return;
	}
	if (opt == 1) {
		if (NULL == (card = queryCardInfo(cardno, 1, NULL))) {
			printf(str[2]);
			return;
		}
		timeToString(card->tLast, tLast);
		printf(str[4]);
		printf(str[5], card->aName, card->nStatus, card->fBalance, card->fTotalUse, card->nUseCount, tLast);
	}
	else if (opt == 2) {
		card = queryCardInfo(cardno, 2, &total);
		if (card == NULL || total == 0) {
			printf(str[2]);
			return;
		}
		printf(str[4]);
		for (i = 0; i < total; i++, card++) {
			timeToString(card->tLast, tLast);
			printf(str[5], card->aName, card->nStatus, card->fBalance, card->fTotalUse, card->nUseCount, tLast);
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
	printf("下机成功!\n");
}

void annul() {
	printf("卡注销成功!\n");
}

void addMoney() {
	printf("充值成功!\n");
}

void refundMoney() {
	printf("退费成功!\n");
}

void statistic() {
	printf("查询统计信息如下:\n");
}