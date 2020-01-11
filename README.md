# R_exercise


R 관련 내용을 기록해두는 곳입니다.

# ymdhmsVWL
- ymdhmsVWL은 문자열로 된 날짜와 시간을 파싱한 후 integer로 변환하는 내용을 담고 있습니다. rcpp를 이용했습니다.

- test.R에 관련 코드와 비교하는 내용이 있습니다.
- 비교내용 중 dt9 를 제외한 나머지 부분은 Sang Woo Ham(https://www.facebook.com/ecosang)님이 만들어주셨습니다.

- 페이스북의 R Korea - KRUG 그룹에서 질문을 올린 후, 답변내용을 참고하여 기존 rcpp 함수를 업데이트 한 것입니다.
- https://www.facebook.com/groups/KoreaRUsers/

- ymdhmdVWL.cpp 안에는 세 개의 함수가 있습니다.
- 두 개는 기존 코드이고, 제일 상단의 ymdhmsVWL가 최종 함수입니다. lubridate의 ymdhms 계열보다 100배 정도 빠릅니다.
- 함수 내용은 간단한 연산작업입니다. 정상적인 날짜나 시간값을 벗어나는 부분에 대한 검증이 없으므로 데이터의 품질이 좋을때만 사용해야 합니다.
- 입력 포맷이 다를 경우 문자열의 위치를 읽는 부분들만 수정하면 됩니다.

- 세가지 함수의 성능 차이는 대부분 한 가지 이유 때문입니다.
- Rcpp에서 R 데이터를 읽어들이는 방식이 후속 처리 방식에 영향을 미칩니다.
- 문자를 숫자로 변환하는 등의 파싱을 할 경우 StringVector를 사용해서 받아오면 좋습니다.
- 이 방식은 cpp에서 char 를 다룰 때만큼의 속도가 나옵니다.

