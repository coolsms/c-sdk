#include "coolsms.h"

new_group_opt new_group_opt_init() {
	new_group_opt group_info = { "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0" };

	return group_info;
}

group_info_opt group_info_opt_init() {
	group_info_opt group_info = { "\0" };

	return group_info;
}

delete_groups_opt delete_groups_opt_init() {
	delete_groups_opt group_info = { "\0" };

	return group_info;
}

add_messages_opt add_messages_opt_init() {
	add_messages_opt message_info = { "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0" };

	return message_info;
}

add_messages_json_opt add_messages_json_opt_init() {
	add_messages_json_opt message_info = { "\0", "\0" };

	return message_info;
}

message_list_opt message_list_opt_init() {
	message_list_opt message_info = { "\0", "\0", "\0" };

	return message_info;
}

delete_messages_opt delete_messages_opt_init() {
	delete_messages_opt message_info = { "\0", "\0" };

	return message_info;
}

send_group_opt send_group_opt_init() {
	send_group_opt group_info = { "\0" };

	return group_info;
}

/**
* @brief    GET    Create group
*/
response_struct create_group(const user_opt *u, const new_group_opt *g)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&charset=%s&srk=%s&mode=%s&delay=%s&force_sms=%s&os_platform=%s&dev_lang=%s&sdk_version=%s&app_version=%s",
		u->api_key, u->salt, u->signature, u->timestamp, g->charset, g->srk, g->mode, g->delay, g->force_sms, g->os_platform, g->dev_lang, g->sdk_version, g->app_version);

	if (curl_process(false, options, "new_group", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get list of groups
*/
response_struct get_group_list(const user_opt *u)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s",
		u->api_key, u->salt, u->signature, u->timestamp);

	if (curl_process(false, options, "group_list", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get information of group
*/
response_struct get_group_info(const user_opt *u, const group_info_opt *g)
{
	char options[1024];
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s",
		u->api_key, u->salt, u->signature, u->timestamp, g->group_id);
	sprintf(param, "groups/%s", g->group_id);

	if (curl_process(false, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Delete groups
*/
response_struct delete_groups(const user_opt *u, const delete_groups_opt *g)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_ids=%s",
		u->api_key, u->salt, u->signature, u->timestamp, g->group_ids);

	if (curl_process(true, options, "delete_groups", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Add messages in group
*/
response_struct add_messages(const user_opt *u, const add_messages_opt *m)
{
	char options[1024];
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s&to=%s&from=%s&text=%s&type=%s&image_id=%s&refname=%s&country_code=%s&datetime=%s&subject=%s&delay=%s",
		u->api_key, u->salt, u->signature, u->timestamp, m->group_id, m->to, m->from, m->text, m->type, m->image_id, m->refname, m->country_code, m->datetime, m->subject, m->delay);
	sprintf(param, "groups/%s/add_messages", m->group_id);

	if (curl_process(true, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Add messages of json type in group
*/
response_struct add_messages_json(const user_opt *u, const add_messages_json_opt *m)
{
	char options[1024]; 
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s&messages=%s",
		u->api_key, u->salt, u->signature, u->timestamp, m->group_id, m->messages);
	sprintf(param, "groups/%s/add_messages.json", m->group_id);

	if (curl_process(true, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get message list in group
*/
response_struct get_message_list(const user_opt *u, const message_list_opt *m)
{
	char options[1024]; 
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s&offset=%s&limit=%s",
		u->api_key, u->salt, u->signature, u->timestamp, m->group_id, m->offset, m->limit);
	sprintf(param, "groups/%s/message_list", m->group_id);

	if (curl_process(false, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Delete messages in group
*/
response_struct delete_messages(const user_opt *u, const delete_messages_opt *m)
{
	char options[1024];
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s&message_ids=%s",
		u->api_key, u->salt, u->signature, u->timestamp, m->group_id,m->message_ids); 
	sprintf(param, "groups/%s/delete_messages", m->group_id);

	if (curl_process(true, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Send group
*/
response_struct send_group(const user_opt *u, const send_group_opt *g)
{
	char options[1024];
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&group_id=%s",
		u->api_key, u->salt, u->signature, u->timestamp, g->group_id);
	sprintf(param, "groups/%s/send", g->group_id);

	if (curl_process(true, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}