#include "stdafx.h"

#include "tool.h"

void clean_stdin()
{
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
	fflush(stdin);
}

void timeToString(time_t t, char* pBuf) {
	struct tm *temp;

	temp = localtime(&t);

	if (NULL == temp) {
		strcpy(pBuf, "error");
		return;
	}

	strftime(pBuf, 25, "%Y-%m-%d %H:%M", temp);
}

time_t stringToTime(const char* pTime) {
	struct tm tm1;

	sscanf(pTime, "%d-%d-%d %d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min);
	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	tm1.tm_isdst = -1;
	tm1.tm_sec = 0;

	return mktime(&tm1);
}