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
	char str[][50] = {
		"----------��ӿ�----------\n",
		"�����뿨��(����Ϊ1~18)��",
		"����������(����Ϊ1~8)��",
		"�����뿪�����(RMB)��",
		"��������,����ʧ�ܣ�\n",
		"\n----------�����ɹ�----------\n",
		"����\t����\t״̬\t�������\n",
		"%s\t%s\t%d\t%.1f\n",
		"����ʧ�ܣ��Ѵ￪���������ޣ�\n",
		"����ʧ�ܣ������Ѵ��ڣ�\n"
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
		"----------��ѯ��----------\n",
		"�������ѯ�Ŀ���(����Ϊ1~18)��",
		"û�иÿ�����Ϣ��\n",
		"��������,��ѯʧ�ܣ�\n",
		"����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n",
		"%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n"
	};
	char cardno[25], tLast[20];
	Card* card = NULL;
	int i, total = 0, opt;

	printf(str[0]);
	printf("��ѡ��1.��׼��ѯ  2.ģ����ѯ\n��������ѡ�");
	scanf("%d", &opt);
	if (opt != 1 && opt != 2) {
		printf("��������\n");
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
	printf("�»��ɹ�!\n");
}

void annul() {
	printf("��ע���ɹ�!\n");
}

void addMoney() {
	printf("��ֵ�ɹ�!\n");
}

void refundMoney() {
	printf("�˷ѳɹ�!\n");
}

void statistic() {
	printf("��ѯͳ����Ϣ����:\n");
}