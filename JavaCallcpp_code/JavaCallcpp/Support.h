#include <string>

using namespace std;

// ������ת����
time_t YMDToDate(int nYear, int nMonth, int nDay);
// ����ת������
void DateToYMD(time_t& time, int& nYear, int& nMonth, int& nDay);

void DeleteAllMark(string &s, const string &mark);