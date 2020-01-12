//처음 버젼에서 오류 수정(입력이 몇개든간에 단 1개만 처리하고 리턴하도록 되어 있었음
//직전 달까지 날짜 수를 1개월 단위로 더해서 계산하던 것을 누적값을 담은 배열로 한번에 처리
//openmp 도입하여 멀티코어 활용하도록 수정

#include <Rcpp.h>
#include <vector>
#include <omp.h>

using std::vector;
using namespace Rcpp;
using std::string;
using std::cout;
using std::endl;

// [[Rcpp::plugins(openmp)]]

unsigned int daysOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
unsigned int daysUntilLastMonth[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };


// [[Rcpp::export]]
IntegerVector ymd_hm_VWL(StringVector ymd, StringVector hm) { //입력 문자열 형식은, 20200101 , 1834
  
  unsigned int len = ymd.size();
  IntegerVector result(len);
  //omp_set_num_threads(4); //코어 숫자 명시 가능. 명시 안하면 최대 활용
  
#pragma omp parallel for
  for (unsigned int i=0  ; i<len ; i++)
  {
    unsigned int year = ymd[i][0] * 1000 + ymd[i][1]* 100 + ymd[i][2] * 10 + ymd[i][3] - '0' * 1111;
    unsigned int month = ymd[i][4] * 10 + ymd[i][5] - '0' * 11;
    unsigned int day = ymd[i][6] * 10 + ymd[i][7] - '0' * 11;
    
    /* 데이터 검증 부분
     if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
     if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //윤년 조건만족하고 2월일 때,
     if (day > 29) return 0;
     }
     else {
     if (daysOfMonth[month - 1] < day) return 0;
     }
     */
    
    day += daysUntilLastMonth[month - 1]; //직전 달까지의 일 수 누적
    
    if (month > 2) {
      if (year / 400 *400 == year || (year / 4 *4== year && year /100 *100 != year)) { //윤년 조건 만족하고 2월 이상이면 하루 추가
        day += 1;
      }
    }
    day += 365 * (year - 1);
    day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //윤년 일수 계산
    
    
    unsigned int secFrom19700101 = 0; //초단위
    
    secFrom19700101 += (day - 719163) * 86400; //719163은 1970년 1월 1일 까지의 날짜 수. 이것을 빼주면 1970.01.01부터 경과된 초를 계산할 수 있다.
    
    unsigned int hh = hm[i][0] * 10 + hm[i][1] - '0' * 11;
    unsigned int mm = hm[i][2] * 10 + hm[i][3]  - '0' * 11;
   //unsigned int ss = hm[i][4] * 10 + hm[i][5]  - '0' * 11;
    
    secFrom19700101 += hh * 3600 + mm * 60;// + ss;
    
    result[i] = secFrom19700101;
    
  }
  
  return result;
  
}

// [[Rcpp::export]]
IntegerVector ymdhmsVWL(StringVector ymdhms) { //입력 문자열 형식은, 2020-01-01 18:34:45 
  
  unsigned int len = ymdhms.size();
  IntegerVector result(len);
  //omp_set_num_threads(4); //코어 숫자 명시 가능. 명시 안하면 최대 활용
 
#pragma omp parallel for  
  for (unsigned int i=0  ; i<len ; i++)
  {
    unsigned int year = ymdhms[i][0] * 1000 + ymdhms[i][1]* 100 + ymdhms[i][2] * 10 + ymdhms[i][3] - '0' * 1111;
    unsigned int month = ymdhms[i][5] * 10 + ymdhms[i][6] - '0' * 11;
    unsigned int day = ymdhms[i][8] * 10 + ymdhms[i][9] - '0' * 11;
    
    /* 데이터 검증 부분
     if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
     if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //윤년 조건만족하고 2월일 때,
     if (day > 29) return 0;
     }
     else {
     if (daysOfMonth[month - 1] < day) return 0;
     }
     */
    
    day += daysUntilLastMonth[month - 1]; //직전 달까지의 일 수 누적
    if (month > 2) {
      if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //윤년 조건 만족하고 2월 이상이면 하루 추가
        day += 1;
      }
    }
    day += 365 * (year - 1);
    day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //윤년 일수 계산
    
    
    unsigned int secFrom19700101 = 0; //초단위
    
    secFrom19700101 += (day - 719163) * 86400; //719163은 1970년 1월 1일 까지의 날짜 수. 이것을 빼주면 1970.01.01부터 경과된 초를 계산할 수 있다.
    
    unsigned int hh = ymdhms[i][11] * 10 + ymdhms[i][12] - '0' * 11;
    unsigned int  mm = ymdhms[i][14] * 10 + ymdhms[i][15]  - '0' * 11;
    unsigned int  ss = ymdhms[i][17] * 10 + ymdhms[i][18]  - '0' * 11;
    
    secFrom19700101 += hh * 3600 + mm * 60 + ss;
    
    result[i] = secFrom19700101;

  }

  return result;
  
}

//ymd_hm_VWL 이 6초일때 이 함수는 95초 소요
// [[Rcpp::export]]
unsigned int ymdhmsVWL_old(vector<string> yyyymmdd, vector<string> hhmm) {
  
  unsigned int year = yyyymmdd[0].at(0) * 1000 + yyyymmdd[0].at(1) * 100 + yyyymmdd[0].at(2) * 10 + yyyymmdd[0].at(3) - '0' * 1111;
  unsigned int month = yyyymmdd[0].at(4) * 10 + yyyymmdd[0].at(5) - '0' * 11;
  unsigned int day = yyyymmdd[0].at(6) * 10 + yyyymmdd[0].at(7) - '0' * 11;
  
  /*
   if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
   if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //윤년 조건만족하고 2월일 때,
   if (day > 29) return 0;
   }
   else {
   if (daysOfMonth[month - 1] < day) return 0;
   }
   */
  
  for (unsigned int i = 0; i < (unsigned int)(month - 1); i++) day += daysOfMonth[i];
  if (month > 2) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //윤년 조건 만족하고 2월 이상이면 하루 추가
      day += 1;
    }
  }
  day += 365 * (year - 1);
  day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //윤년 일수 계산
  
  
  unsigned int result = 0; //초단위
  
  result += (day - 719163) * 86400; //719163은 1970년 1월 1일 까지의 날짜 수
  
  unsigned int hh = hhmm[0].at(0) * 10 + hhmm[0].at(1) - '0' * 11;
  unsigned int mm = hhmm[0].at(2) * 10 + hhmm[0].at(3)  - '0' * 11;
  
  result += hh * 3600 + mm * 60;
  
  return result;
  
}


//아래 ymdhmRcpp 함수에서 호출 
unsigned int getDateTime(const char* yyyymmdd, const char* hhmm) {
  
  unsigned int year = yyyymmdd[0] * 1000 + yyyymmdd[1] * 100 + yyyymmdd[2] * 10 + yyyymmdd[3] - '0' * 1111;
  unsigned int month = yyyymmdd[4] * 10 + yyyymmdd[5] - '0' * 11;
  unsigned int day = yyyymmdd[6] * 10 + yyyymmdd[7] - '0' * 11;
  
  /*
   if (year < 1 || year>9999 || month < 1 || month>12 || day < 1) return 0;
   if ((year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) && month == 2) { //윤년 조건만족하고 2월일 때,
   if (day > 29) return 0;
   }
   else {
   if (daysOfMonth[month - 1] < day) return 0;
   }
   */
  
  for (unsigned int i = 0; i < (unsigned int)(month - 1); i++) day += daysOfMonth[i];
  if (month > 2) {
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { //윤년 조건 만족하고 2월 이상이면 하루 추가
      day += 1;
    }
  }
  day += 365 * (year - 1);
  day += ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400); //윤년 일수 계산
  
  
  unsigned int result = 0; //초단위
  
  result += (day - 719163) * 86400; //719163은 1970년 1월 1일 까지의 날짜 수
  
  unsigned int hh = hhmm[0] * 10 + hhmm[1] - '0' * 11;
  unsigned int mm = hhmm[2] * 10 + hhmm[3] - '0' * 11;
  
  result += hh * 3600 + mm * 60;
  
  return result;
  
}

//ymd_hm_VWL 이 6초일때 이 함수는 54초 소요
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