#include <Rcpp.h>
#include <vector>

using std::vector;
using namespace Rcpp;
using std::string;

unsigned int daysOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// [[Rcpp::export]]
unsigned int ymdhmsVWL(StringVector yyyymmdd, StringVector hhmmss) {
  
  
  unsigned int year = yyyymmdd[0][0] * 1000 + yyyymmdd[0][1]* 100 + yyyymmdd[0][2] * 10 + yyyymmdd[0][3] - '0' * 1111;
  unsigned int month = yyyymmdd[0][4] * 10 + yyyymmdd[0][5] - '0' * 11;
  unsigned int day = yyyymmdd[0][6] * 10 + yyyymmdd[0][7] - '0' * 11;
  
  /* ������ ���� �κ�
   if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
   if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //���� ���Ǹ����ϰ� 2���� ��,
   if (day > 29) return 0;
   }
   else {
   if (daysOfMonth[month - 1] < day) return 0;
   }
   */
  
  for (unsigned int i = 0; i < (unsigned int)(month - 1); i++) day += daysOfMonth[i];
  if (month > 2) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //���� ���� �����ϰ� 2�� �̻��̸� �Ϸ� �߰�
      day += 1;
    }
  }
  day += 365 * (year - 1);
  day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //���� �ϼ� ���
  
  
  unsigned int result = 0; //�ʴ���
  
  result += (day - 719163) * 86400; //719163�� 1970�� 1�� 1�� ������ ��¥ ��. �̰��� ���ָ� 1970.01.01���� ����� �ʸ� ����� �� �ִ�.
  
  unsigned int hh = hhmmss[0][0] * 10 + hhmmss[0][1] - '0' * 11;
  unsigned int mm = hhmmss[0][2] * 10 + hhmmss[0][3]  - '0' * 11;
  //unsigned int ss = hhmmss[0][4] * 10 + hhmmss[0][5]  - '0' * 11;
  
  result += hh * 3600 + mm * 60;// + ss;
  
  return result;
  
}


//ymdhmsVWL �� 0.15���϶� �� �Լ��� 95�� �ҿ�
// [[Rcpp::export]]
unsigned int ymdhmsVWL_old(vector<string> yyyymmdd, vector<string> hhmm) {
  
  unsigned int year = yyyymmdd[0].at(0) * 1000 + yyyymmdd[0].at(1) * 100 + yyyymmdd[0].at(2) * 10 + yyyymmdd[0].at(3) - '0' * 1111;
  unsigned int month = yyyymmdd[0].at(4) * 10 + yyyymmdd[0].at(5) - '0' * 11;
  unsigned int day = yyyymmdd[0].at(6) * 10 + yyyymmdd[0].at(7) - '0' * 11;
  
  /*
   if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
   if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //���� ���Ǹ����ϰ� 2���� ��,
   if (day > 29) return 0;
   }
   else {
   if (daysOfMonth[month - 1] < day) return 0;
   }
   */
  
  for (unsigned int i = 0; i < (unsigned int)(month - 1); i++) day += daysOfMonth[i];
  if (month > 2) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //���� ���� �����ϰ� 2�� �̻��̸� �Ϸ� �߰�
      day += 1;
    }
  }
  day += 365 * (year - 1);
  day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //���� �ϼ� ���
  
  
  unsigned int result = 0; //�ʴ���
  
  result += (day - 719163) * 86400; //719163�� 1970�� 1�� 1�� ������ ��¥ ��
  
  unsigned int hh = hhmm[0].at(0) * 10 + hhmm[0].at(1) - '0' * 11;
  unsigned int mm = hhmm[0].at(2) * 10 + hhmm[0].at(3)  - '0' * 11;
  
  result += hh * 3600 + mm * 60;
  
  return result;
  
}


//�Ʒ� ymdhmRcpp �Լ����� ȣ�� 
unsigned int getDateTime(const char* yyyymmdd, const char* hhmm) {
  
  unsigned int year = yyyymmdd[0] * 1000 + yyyymmdd[1] * 100 + yyyymmdd[2] * 10 + yyyymmdd[3] - '0' * 1111;
  unsigned int month = yyyymmdd[4] * 10 + yyyymmdd[5] - '0' * 11;
  unsigned int day = yyyymmdd[6] * 10 + yyyymmdd[7] - '0' * 11;
  
  /*
   if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
   if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //���� ���Ǹ����ϰ� 2���� ��,
   if (day > 29) return 0;
   }
   else {
   if (daysOfMonth[month - 1] < day) return 0;
   }
   */
  
  for (unsigned int i = 0; i < (unsigned int)(month - 1); i++) day += daysOfMonth[i];
  if (month > 2) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //���� ���� �����ϰ� 2�� �̻��̸� �Ϸ� �߰�
      day += 1;
    }
  }
  day += 365 * (year - 1);
  day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //���� �ϼ� ���
  
  
  unsigned int result = 0; //�ʴ���
  
  result += (day - 719163) * 86400; //719163�� 1970�� 1�� 1�� ������ ��¥ ��
  
  unsigned int hh = hhmm[0] * 10 + hhmm[1] - '0' * 11;
  unsigned int mm = hhmm[2] * 10 + hhmm[3] - '0' * 11;
  
  result += hh * 3600 + mm * 60;
  
  return result;
  
}

//ymdhmsVWL �� 0.15���϶� �� �Լ��� 54�� �ҿ�
// [[Rcpp::export]]
vector<unsigned int> ymdhmRcpp(vector<string> yyyymmdd, vector<string> hhmm)
{
  unsigned int count = yyyymmdd.size();
  vector<unsigned int> result(count);
  
  for (unsigned int i=0 ; i<count ; i++) 
  {
    string& ymd_ = yyyymmdd[i];
    string& hm_ = hhmm[i];
    result[i] = getDateTime(ymd_.c_str(), hm_.c_str());
  }
  
  return result;
  
}