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
	sent_opt sent_info = sent_opt_init();

	sent_info.offset = "0";							// ������ ����� ���� ��ġ
	// sent_info.limit = "5";						// �⺻�� 20, 20���� ����� ���� �� ������ 40 �Է½� 40���� ����� ����
	// sent_info.rcpt = "01000000000";				// ���Ź�ȣ�� �˻�
	// sent_info.start = "201401070915";			// �˻� �����Ͻ�
	// sent_info.end = "201401071430";				// �˻� �����Ͻ�
	// sent_info.status = "00";						// �޼��� ���� ��
	// sent_info.resultcode = "54";					// ���۰�� ��
	// sent_info.notin_resultcode = "00";			// �Էµ� ���۰�� �� �̿��� ��
	// sent_info.messate_id = "MID52CA262EC49D8";	// �޼��� ID ��
	// sent_info.group_id = "GID24HL31K4H234";		// �׷� ID ��

	result = sent(&user_info, &sent_info);
	print_result(result);

	return 0;
}
#endif