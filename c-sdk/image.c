#include "coolsms.h"

upload_image_opt upload_image_opt_init() {
	upload_image_opt image_info = { "\0", "\0" };

	return image_info;
}

image_list_opt image_list_opt_init() {
	image_list_opt image_info = { "\0", "\0" };

	return image_info;
}

delete_images_opt delete_images_opt_init() {
	delete_images_opt image_info = { "\0" };

	return image_info;
}

image_info_opt image_info_opt_init() {
	image_info_opt image_info = { "\0" };

	return image_info;
}

/**
* @brief    POST    Upload image in FTP
*/
response_struct upload_image(const user_opt *u, const upload_image_opt *i)
{
	response_struct output;

	if (multi_curl_process(u, NULL, i, "upload_image", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get infomation of image
*/
response_struct image_info(const user_opt *u, const image_info_opt *i)
{
	char options[1024];
	char param[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&image_id=%s",
		u->api_key, u->salt, u->signature, u->timestamp, i->image_id);
	sprintf(param, "images/%s", i->image_id);

	if (curl_process(false, options, param, "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    GET    Get list of images
*/
response_struct image_list(const user_opt *u, const image_list_opt *i)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&offset=%s&limit=%s",
		u->api_key, u->salt, u->signature, u->timestamp, i->offset, i->limit);

	if (curl_process(false, options, "image_list", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}

/**
* @brief    POST    Delete images
*/
response_struct delete_images(const user_opt *u, const delete_images_opt *i)
{
	char options[1024];
	response_struct output;

	sprintf(options, "api_key=%s&salt=%s&signature=%s&timestamp=%s&image_ids=%s",
		u->api_key, u->salt, u->signature, u->timestamp, i->image_ids);

	if (curl_process(true, options, "delete_images", "sms", &output) == CURLE_OK)
		printf("\nSuccess!\n");
	else
		printf("\nError!\n");

	return output;
}