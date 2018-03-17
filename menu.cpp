#include "stdafx.h"

#include "menu.h"
#include "service.h"
#include "global.h"
#include "tool.h"
#include "model.h"

void outputMenu() {
	char str[] = {
		"��ӭ����Ʒѹ���ϵͳ\n"
		"----------�˵�----------\n"
		"1.��ӿ�\n"
		"2.��ѯ��\n"
		"3.�ϻ�\n"
		"4.�»�\n"
		"5.��ֵ\n"
		"6.�˷�\n"
		"7.��ѯͳ��\n"
		"8.ע����\n"
		"9.����\n"
		"0.�˳�\n"
		"\n"
		"��ѡ��˵����ţ�"
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
		printf("����Ĳ˵�����������������룡\n");
		break;
	}
	printf("\n");

	return status;
}

void add() {
	char cardno[25], passwd[15];
	int len, status;
	float money;

	printf("----------��ӿ�----------\n");
	fflush(stdin);
	
	printf("�����뿨��(����Ϊ1~18)��");
	scanf("%20s", cardno);
	len = strlen(cardno);
	if (1 <= len && len <= 18) {
		printf("����������(����Ϊ1~8)��");
		scanf("%10s", passwd);
		len = strlen(passwd);
		if (1 <= len && len <= 8) {
			printf("�����뿪�����(RMB)��");
			scanf("%f", &money);
			if (0 <= money && TRUE == addCardInfo(cardno, passwd, money)) {
				printf("\n----------�����ɹ�----------\n");
				printf("����\t����\t״̬\t�������\n");
				printf("%s\t%s\t%d\t%.1f\n", cardno, passwd, 0, money);
				return;
			}
		}
	}
	printf("��������,����ʧ�ܣ�\n");
	fflush(stdin);
	return;
}

void query() {
	char cardno[25], tLast[_TIME_LENGTH_];
	Card* card = NULL;
	int i, total = 0, opt;

	printf("----------��ѯ��----------\n");
	printf("��ѡ��1.��׼��ѯ  2.ģ����ѯ\n��������ѡ�");
	scanf("%d", &opt);
	if (opt != 1 && opt != 2) {
		printf("��������\n");
		return;
	}
	printf("�������ѯ�Ŀ���(����Ϊ1~18)��");
	scanf("%20s", cardno);

	if (18 <= strlen(cardno)) {
		printf("��������,��ѯʧ�ܣ�\n");
		return;
	}
	if (opt == 1) {
		if (NULL == (card = queryCardInfo(cardno, 1, NULL))) {
			printf("û�иÿ�����Ϣ��\n");
			return;
		}
		timeToString(card->tLast, tLast);
		printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
		printf("%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", card->aName, card->nStatus, card->fBalance, card->fTotalUse, card->nUseCount, tLast);
	}
	else if (opt == 2) {
		card = queryCardInfo(cardno, 2, &total);
		if (card == NULL || total == 0) {
			printf("û�иÿ�����Ϣ��\n");
			return;
		}
		printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
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

	printf("----------�ϻ�----------\n");
	printf("�������ϻ�����(����Ϊ1~18)��");
	scanf("%20s", cardno);
	printf("�������ϻ�����(����Ϊ1~8)��");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("��������\n");
		return;
	}
	status = doLogon(cardno, passwd, &logonInfo);

	switch (status)
	{
	case FALSE:
		printf("�ϻ�ʧ�ܣ�\n");
		break;
	case TRUE:
		timeToString(logonInfo.tLogon, tLogon);
		printf("----------�ϻ���Ϣ����----------\n");
		printf("����\t���\t�ϻ�ʱ��\n");
		printf("%s\t%.1f\t%s\n", logonInfo.aCardName, logonInfo.fBalance, tLogon);
		break;
	case _UNUSE_:
		printf("�ÿ�����ʹ�û�����ע����\n");
		break;
	case _NOT_ENOUGH_MONEY:
		printf("�����㣡\n");
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

	printf("----------�»�----------\n");
	printf("�������»�����(����Ϊ1~18)��");
	scanf("%20s", cardno);
	printf("�������»�����(����Ϊ1~8)��");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("��������\n");
		return;
	}
	nResult = doSettle(cardno, passwd, &settleInfo);

	switch (nResult)
	{
	case _NOT_ENOUGH_MONEY:
		printf("�»�ʧ��,�����㣡\n");
		break;
	case FALSE:
		printf("�»�ʧ�ܣ�\n");
		break;
	case TRUE:
		timeToString(settleInfo.tStart, tStart);
		timeToString(settleInfo.tEnd, tEnd);
		printf("----------�»���Ϣ����----------\n");
		printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
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

	printf("----------ע����----------\n");
	printf("������ע������(����Ϊ1~18)��");
	scanf("%20s", cardno);
	printf("����������(����Ϊ1~8)��");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("��������\n");
		return;
	}
	strcpy(card.aName, cardno);
	strcpy(card.aPwd, passwd);

	nResult = annulCard(&card);
	printf("----------ע����Ϣ����----------\n");
	switch (nResult)
	{
	case TRUE:
		printf("����\t�˿���\n");
		printf("%s\t%.1f\n", card.aName, card.fBalance);
		break;
	case FALSE:
		printf("ע����ʧ�ܣ�\n");
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

	printf("----------��ֵ----------\n");
	printf("�������ֵ����(����Ϊ1~18)��");
	scanf("%20s", cardno);
	printf("�������ֵ����(����Ϊ1~8)��");
	scanf("%10s", passwd);
	printf("�������ֵ���(RMB)��");
	scanf("%f", &fMoney);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("��������\n");
		return;
	}
	moneyInfo.fMoney = fMoney;

	if (TRUE == doAddMoney(cardno, passwd, &moneyInfo)) {
		printf("----------��ֵ��Ϣ����----------\n");
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%.1f\t\t%.1f\n", moneyInfo.aCardName, moneyInfo.fMoney, moneyInfo.fBalance);
		return;
	}

	printf("��ֵʧ�ܣ�\n");
}

void refundMoney() {
	MoneyInfo moneyInfo;
	char cardno[25], passwd[15];
	float fMoney = 0;
	int nResult = 0;

	printf("----------�˷�----------\n");
	printf("�������˷ѿ���(����Ϊ1~18)��");
	scanf("%20s", cardno);
	printf("�������˷�����(����Ϊ1~8)��");
	scanf("%10s", passwd);

	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18 && 1 <= strlen(passwd) && strlen(passwd) <= 8)) {
		printf("��������\n");
		return;
	}
	nResult = doRefundMoney(cardno, passwd, &moneyInfo);

	switch (nResult) {
	case TRUE:
		printf("----------�˷���Ϣ����----------\n");
		printf("����\t�˷ѽ��\t���\n");
		printf("%s\t%.1f\t\t%.1f\n", moneyInfo.aCardName, moneyInfo.fMoney, moneyInfo.fBalance);
		break;
	case _NOT_ENOUGH_MONEY:
		printf("�����㣡\n");
		break;
	case _UNUSE_:
		printf("�˷�ʧ�ܣ�\n");
		break;
	default:
		printf("�˷�ʧ�ܣ�\n");
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

	printf("----------ͳ�Ʋ�ѯ----------\n");
	printf("1.��ѯ���Ѽ�¼\t2.��ѯ��ֵ��¼\n");
	printf("������ѡ�");
	scanf("%d", &opt);
	printf("�������ѯ����(����Ϊ1~18)��");
	scanf("%20s", cardno);
	//printf("����������(����Ϊ1~8)��");
	//scanf("%10s", passwd);
	if (!(1 <= strlen(cardno) && strlen(cardno) <= 18)) {
		printf("��������\n");
		return;
	}
	strcpy(card.aName, cardno);

	if (1 == opt) {
		if (NULL == (rs = queryBillingInfo(&card, &total))) {
			printf("��ѯʧ�ܣ�\n");
			return;
		}
		printf("����\t���ѽ��\t״̬\t�ϻ�ʱ��\t\t�»�ʱ��\n");
		for (index = 0; index < total; index++, rs++) {
			timeToString(rs->tStart, tStart);
			timeToString(rs->tEnd, tEnd);
			printf("%s\t%.1f\t\t%s\t%s\t%s\n", 
				rs->aCardName, rs->fAmount, rs->nStatus? "�ѽ���":"δ����", tStart, tEnd);
		}
	}
	else if (2 == opt) {
		if (NULL == (moneyRS = queryMoneyInfo(&card, &total))) {
			printf("��ѯʧ�ܣ�\n");
			return;
		}
		printf("����\t\tʱ��\t\t״̬\t���\tɾ����ʶ\n\n");
		for (index = 0; index < total; index++, moneyRS++) {
			timeToString(moneyRS->tTime, tStart);
			printf("%s\t%s\t%s\t%.1f\t%d\n",
				moneyRS->aCardName, tStart, moneyRS->nStatus ? "�˷�" : "��ֵ", moneyRS->fMoney, moneyRS->nDel);
		}
	}
}