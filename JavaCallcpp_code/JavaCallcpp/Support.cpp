#include "stdafx.h"
#include "Support.h"
#include <time.h>

// 年月日转日期
time_t YMDToDate(int nYear, int nMonth, int nDay)
{
	tm tLic;
	tLic.tm_year = nYear - 1900;
	tLic.tm_mon = nMonth -1;
	tLic.tm_mday = nDay;
	tLic.tm_hour = 0;
	tLic.tm_sec = 0;
	tLic.tm_min = 0;
	return mktime(&tLic);
}

// 日期转年月日
void DateToYMD(time_t& time, int& nYear, int& nMonth, int& nDay)
{
	tm t;
	localtime_s(&t,&time);
	nYear  = t.tm_year + 1900;
	nMonth = t.tm_mon + 1;
	nDay   = t.tm_mday;
}

void DeleteAllMark(string &s, const string &mark) {
	size_t nSize = mark.size();
	while (1) {
		size_t pos = s.find(mark);
		if (pos == string::npos) {
			return;
		}

		s.erase(pos, nSize);
	}
}