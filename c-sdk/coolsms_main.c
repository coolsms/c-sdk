#include "coolsms.h"

/**
* Copyright (C) 2008-2016 NURIGO \n
* http://www.coolsms.co.kr
*/

/**
* @mainpage C SDK
* @section intro 소개
*     - 소개 : C SDK
*     - 버전 : 2.0
*     - 설명 : Coolsms REST API를 이용하여 보다 빠르고 안전하게 문자메시지를 보낼 수 있는 C SDK 입니다.\n
* @section CreateInfo 작성 정보
*     - 작성자 : Nurigo
*     - 작성일 : 2016/05/24
* @section Caution 주의 사항
*     - 해당 C SDK 는 SMS API v2 를 주로 개발되었음을 알려드립니다.
* @section common 기타 정보
*     - 저작권 GPL v2
*/

/**
* @brief  Initialize user infomation 
*/
user_opt user_opt_init(char *api_key, char *api_secret)
{
	user_opt user_info = { "\0", "\0", "\0", "\0", "\0", "\0" };
	char *uuid;
	char *salt = (char*)malloc(2 * 20 + 1);
	char *buf_ptr = salt;

	user_info.api_key = api_key;
	user_info.api_secret = api_secret;
	user_info.timestamp = get_timestamp();
	uuid = get_uuid();
	buf_ptr += sprintf(buf_ptr, "%s", uuid);
	user_info.salt = salt;
	user_info.signature = get_signature(user_info.timestamp, user_info.salt, user_info.api_secret);
	free(uuid);

	return user_info;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	response_struct *mem = (response_struct *)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

/**
* @brief  Process curl
*/
int curl_process(bool is_post, char *options, char *path, char *api_name, response_struct *output)
{
	CURL *curl;
	CURLcode res;
	char url[1024];
	struct curl_slist *headerlist = NULL;
	response_struct response;
	response.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	response.size = 0;    /* no data at this point */
	char *host = "http://api.coolsms.co.kr";
	char *api_version = "\0";

	if (api_name == "sms") {
		api_version = "2";
	} else {
		api_version = "1.1";
	}

	// Set url. is_pose true = POST, false = GET
	if (is_post) {
		sprintf(url, "%s/%s/%s/%s", host, api_name, api_version, path);
	}
	else {
		sprintf(url, "%s/%s/%s/%s?%s", host, api_name, api_version, path, options);
	}

	// initialize curl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		if (is_post)
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, options);
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		}
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // CURLOPT_VERBOSE shows infomation about connected server. USEFUL for debugging
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "REST SDK C&CPP/2.0");
		res = curl_easy_perform(curl);

		// Check for errors
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else
			printf("%lu bytes retrieved\n", (long)response.size);

		// always cleanup
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	*output = response;

	return res;
}

/**
* @brief Process curl ( Image )
*/
int multi_curl_process(const user_opt *u, const send_opt *s, const upload_image_opt *i, char *path, char *api_name, response_struct *output)
{
	CURL *curl;
	CURLcode res;
	char url[1024];
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	response_struct response;
	char *host = "http://api.coolsms.co.kr";
	char *api_version = "2";
	/* initialize response struct */
	response.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	response.size = 0;    /* no data at this point */

						  /* set url and initilize curl */
	sprintf(url, "%s/%s/%s/%s", host, api_name, api_version, path);
	curl = curl_easy_init();

	if (path == "send") {
		/* set values to curl_form */
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, u->api_key, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "salt", CURLFORM_COPYCONTENTS, u->salt, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "signature", CURLFORM_COPYCONTENTS, u->signature, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "timestamp", CURLFORM_COPYCONTENTS, u->timestamp, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "to", CURLFORM_COPYCONTENTS, s->to, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "from", CURLFORM_COPYCONTENTS, s->from, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "text", CURLFORM_COPYCONTENTS, s->text, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "charset", CURLFORM_COPYCONTENTS, s->charset, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "type", CURLFORM_COPYCONTENTS, s->type, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "image", CURLFORM_FILE, s->image, CURLFORM_CONTENTTYPE, "image/jpeg", CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "refname", CURLFORM_COPYCONTENTS, s->refname, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "datetime", CURLFORM_COPYCONTENTS, s->datetime, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "country_code", CURLFORM_COPYCONTENTS, s->country_code, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "subject", CURLFORM_COPYCONTENTS, s->subject, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "srk", CURLFORM_COPYCONTENTS, s->srk, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "mode", CURLFORM_COPYCONTENTS, s->mode, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "delay", CURLFORM_COPYCONTENTS, s->delay, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "force_sms", CURLFORM_COPYCONTENTS, s->force_sms, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "os_platform", CURLFORM_COPYCONTENTS, s->os_platform, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "dev_lang", CURLFORM_COPYCONTENTS, s->dev_lang, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "sdk_version", CURLFORM_COPYCONTENTS, s->sdk_version, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "app_version", CURLFORM_COPYCONTENTS, s->app_version, CURLFORM_END);
	} else {
		/* set values to curl_form */
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, u->api_key, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "salt", CURLFORM_COPYCONTENTS, u->salt, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "signature", CURLFORM_COPYCONTENTS, u->signature, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "timestamp", CURLFORM_COPYCONTENTS, u->timestamp, CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "image", CURLFORM_FILE, i->image, CURLFORM_CONTENTTYPE, "image/jpeg", CURLFORM_END);
		curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "encoding", CURLFORM_COPYCONTENTS, i->encoding, CURLFORM_END);
	}

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); /* CURLOPT_VERBOSE shows infomation about connected server. USEFUL for debugging */
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "REST SDK C&CPP/2.0");
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else
			printf("%lu bytes retrieved\n", (long)response.size);

		*output = response;
		/* then cleanup curl, formpost and headerlist */
		curl_easy_cleanup(curl);
		curl_formfree(formpost);
		curl_slist_free_all(headerlist);
	}
	return res;
}

/**
* @brief Change the type from string
*/
char* time_to_string(const time_t time) {
	char *output;
	char *buf_str = malloc(sizeof(time) * 2 + 1);
	char *buf_ptr = buf_str;
	buf_ptr += sprintf(buf_ptr, "%d", (int)time);
	output = buf_str;

	return output;
}

/**
* @brief Get a timestamp
*/
char* get_timestamp()
{
	char *output;
	time_t timestamp;

	time(&timestamp);
	output = time_to_string(timestamp);

	return output;
}

/**
* @brief Get a uuid for salt
*/
char* get_uuid()
{
	struct tm tm_val;
	char *buf;

	buf = (char *)malloc(40);
	memset(buf, 0, sizeof(buf));
	memset(&tm_val, 0, sizeof(tm_val));
#ifdef _WIN32
	SYSTEMTIME st;
	GetLocalTime(&st);

	sprintf(buf, "%u%u%u%u%u%u%u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
	t = time(NULL);
	(void)localtime_r(&t, &tm_val);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tm_val);
#endif

	return buf;
}

/**
* @brief Get a signature with hash_hmac
*/
char* get_signature(const char *datetime, const char *salt, const char *api_secret)
{
	char *output;
	unsigned char signature[16];
	char data_unsigned[64];
	char * hash_data;
	hash_data = (char*)malloc(strlen(datetime) + strlen(salt) + 1);
	/* hash 예외처리 */
	sprintf(data_unsigned, "%s%s", datetime, salt);
	md5_hmac((unsigned char*)api_secret, strlen(api_secret), (unsigned char*)data_unsigned, strlen(data_unsigned), signature);

	char* buf_str = (char*)malloc(2 * sizeof(signature) + 1);
	char *buf_ptr = buf_str;
	for (int i = 0; i < sizeof(signature); i++)
	{
		buf_ptr += sprintf(buf_ptr, "%02x", signature[i]);
	}
	output = buf_str;
	
	return output;
}

/**
* @brief Return result
*/
void print_result(response_struct result)
{
	/* parsing json results */
	json_error_t error;
	json_t *root;
	root = json_loads(result.memory, 0, &error);
	if (!root) {
		fprintf(stderr, "error : root\n");
		fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
		exit(1);
	}

	/* print keys and its values */
	json_t *data, *obj, *array;
	const char * key;
	int i;
	json_object_foreach(root, key, obj) {
		printf("%s:", key);
		data = json_object_get(root, key);
		if (!json_is_array(data))
			printf("%s\n", json_dumps(data, JSON_ENCODE_ANY));

		json_array_foreach(obj, i, array) {
			printf("%s:\n", json_dumps(array, JSON_ENCODE_ANY));
		}
	}

	/* Jansson library is used for parsing JSON data
	More info found at https://jansson.readthedocs.org/en/2.5/ */
}