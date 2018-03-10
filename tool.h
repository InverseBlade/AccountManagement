#ifndef __TOOL__H__
#define __TOOL__H__

void clean_stdin();

void timeToString(time_t t, char* pBuf);

time_t stringToTime(const char* pTime);

#endif // !__TOOL__H__
