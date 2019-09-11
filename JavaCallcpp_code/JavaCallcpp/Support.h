#include <string>

using namespace std;

// 年月日转日期
time_t YMDToDate(int nYear, int nMonth, int nDay);
// 日期转年月日
void DateToYMD(time_t& time, int& nYear, int& nMonth, int& nDay);

void DeleteAllMark(string &s, const string &mark);