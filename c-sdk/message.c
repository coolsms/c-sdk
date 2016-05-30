#include "coolsms.h"

send_opt send_opt_init() {
	send_opt message_info = { "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"};

	return message_info;
}

sent_opt sent_opt_init() {
	sent_opt message_info = { "0", "20", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0" };

	return message_info;
}

cancel_opt cancel_opt_init() {
	cancel_opt message_info = { "\0", "\0" };

	return message_info;
}

status_opt status_opt_init() {
	status_opt message_info = { "1", "minuit", "\0", "1" };

	return message_info;
}

/**
* @brief    POST    Send
*/
response_struct send_message(const user_opt *u, const send_opt *s)
{
	char options[1024];
	response_struct output;
	
	if (strcmp(s->type, "MMS")) {
		sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&to=%s&from=%s&text=%s&type=%s&image=%s&refname=%s&country_code=%s&datetime=%s&subject=%s&charset=%s&srk=%s&mode=%s&delay=%s&force_sms=%s&os_platform=%s&dev_lang=%s&sdk_version=%s&app_version=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->to, s->from, s->text, s->type, s->image, s->refname, s->country_code, s->datetime, s->subject, s->charset, s->srk, s->mode, s->delay, s->force_sms, s->os_platform, s->dev_lang, s->sdk_version, s->app_version);

		if (curl_process(true, options, "send", "sms", &output) == CURLE_OK)
			printf("\nSuccess!\n");
		else
			printf("\nError!\n");
	}
	else {
		if (multi_curl_process(u, s, NULL, "send", "sms", &output) == CURLE_OK)
			printf("\nSuccess!\n");
		else
			printf("\nError!\n");
	}

	return output;
}

/**
* @brief    GET    Sent
*/
response_struct sent(const user_opt *u, const sent_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&offset=%s&limit=%s&rcpt=%s&start=%s&end=%s&status=%s&resultcode=%s&notin_resultcode=%s&message_id=%s&group_id=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->offset, s->limit, s->rcpt, s->start, s->end, s->status, s->resultcode, s->notin_resultcode, s->message_id, s->group_id);

	if (curl_process(false, options, "sent", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Cancel
*/
response_struct cancel(const user_opt *u, const cancel_opt *c)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&mid=%s&gid=%s",
		u->api_key, u->salt, u->signature, u->timestamp, c->mid, c->gid);

	if (curl_process(true, options, "cancel", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Balance
*/
response_struct balance(const user_opt *u)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s",
		u->api_key, u->salt, u->signature, u->timestamp);

	if (curl_process(false, options, "balance", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Status
*/
response_struct status(const user_opt *u, const status_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&count=%s&unit=%s&read_date=%s&channel=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->count, s->unit, s->date, s->channel);
	printf("%s\n", options);
	if (curl_process(false, options, "status", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}