#include "coolsms.h"

/* Set 1 to test example */
#if 0
int main()
{
	/* api_key and api_secret can be obtained from http://www.coolsms.co.kr */
	response_struct result;
	char *api_key = "API_KEY";
	char *api_secret = "API_SECRET";
	user_opt user_info = user_opt_init(api_key, api_secret);
	status_opt status_info = status_opt_init();

	status_info.count = "1";				// 레코드 목록
	// status_info.unit = "minute";			// minute, day, hour, 중 한 단위로의 평균
	status_info.date = "20150101000000";	// 데이터를 읽어오는 시각
	// status_info.channel = "1";				// 1 : 1건 발송 채널, 2 : 대량 발송 채널

	result = status(&user_info, &status_info);
	print_result(result);

	return 0;
}
#endif