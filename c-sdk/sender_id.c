#include "coolsms.h"

register_opt register_opt_init() {
	register_opt senderid_info = { "\0", "\0" };

	return senderid_info;
}

verify_opt verify_opt_init() {
	verify_opt senderid_info = { "\0" };

	return senderid_info;
}

delete_opt delete_opt_init() {
	delete_opt senderid_info = { "\0" };

	return senderid_info;
}

list_opt list_opt_init() {
	list_opt senderid_info = { "\0" };

	return senderid_info;
}

get_default_opt get_default_opt_init() {
	get_default_opt senderid_info = { "\0" };

	return senderid_info;
}

set_default_opt set_default_opt_init() {
	set_default_opt senderid_info = { "\0", "\0" };

	return senderid_info;
}

/**
* @brief    POST    Register
*/
response_struct register_sender(const user_opt *u, const register_opt *r)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&phone=%s&site_user=%s",
		u->api_key, u->salt, u->signature, u->timestamp, r->phone, r->site_user);

	if (curl_process(true, options, "register", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Verify
*/
response_struct verify(const user_opt *u, const verify_opt *v)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&timestamp=%s&salt=%s&signature=%s&handle_key=%s",
		u->api_key, u->timestamp, u->salt, u->signature, v->handle_key);

	if (curl_process(true, options, "verify", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Delete
*/
response_struct delete_sender(const user_opt *u, const delete_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&handle_key=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->handle_key);

	if (curl_process(true, options, "delete", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    List
*/
response_struct list(const user_opt *u, const list_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&site_user=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->site_user);

	if (curl_process(false, options, "list", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Set default
*/
response_struct set_default(const user_opt *u, const set_default_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&handle_key=%s&site_user=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->handle_key, s->site_user);

	if (curl_process(true, options, "set_default", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get default
*/
response_struct get_default(const user_opt *u, const get_default_opt *s)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&site_user=%s",
		u->api_key, u->salt, u->signature, u->timestamp, s->site_user);

	if (curl_process(false, options, "get_default", "senderid", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}