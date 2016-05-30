#include <stdio.h>
#include "csmd5.h"
#include <jansson.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define CURL_STATICLIB
#include <curl/curl.h>

typedef struct { char *api_key, *api_secret, *salt, *signature, *user_agent, *timestamp; } user_opt;
typedef struct { char *memory; size_t size; } response_struct;
/* Group message */
typedef struct { char *charset, *srk, *mode, *delay, *force_sms, *os_platform, *dev_lang, *sdk_version, *app_version; } new_group_opt;
typedef struct { char *group_id; } group_info_opt;
typedef struct { char *group_ids; } delete_groups_opt;
typedef struct { char *group_id, *to, *from, *text, *type, *image_id, *refname, *country_code, *datetime, *subject, *delay; } add_messages_opt;
typedef struct { char *group_id, *messages; } add_messages_json_opt;
typedef struct { char *group_id, *offset, *limit; } message_list_opt;
typedef struct { char *group_id, *message_ids; } delete_messages_opt;
typedef struct { char *group_id; } send_group_opt;
/* Message */
typedef struct { char *to, *from, *text, *type, *image, *image_encoding, *refname, *country_code, *datetime, *subject, *charset, *srk, *mode, *delay, *force_sms, *os_platform, *dev_lang, *sdk_version, *app_version; } send_opt;
typedef struct { char *offset, *limit, *rcpt, *start, *end, *status, *resultcode, *notin_resultcode, *message_id, *group_id; } sent_opt;
typedef struct { char *mid, *gid; } cancel_opt;
typedef struct { char *count, *unit, *date, *channel; } status_opt;
/* Image */
typedef struct { char *image, *encoding; } upload_image_opt;
typedef struct { char *offset, *limit; } image_list_opt;
typedef struct { char *image_id; } image_info_opt;
typedef struct { char *image_ids; } delete_images_opt;
/* SenderID */
typedef struct { char *phone, *site_user; } register_opt;
typedef struct { char *handle_key; } verify_opt;
typedef struct { char *handle_key; } delete_opt;
typedef struct { char *site_user; } list_opt;
typedef struct { char *site_user; } get_default_opt;
typedef struct { char *handle_key, *site_user; } set_default_opt;

int curl_process(bool, char *, char *, char *, response_struct *);
int multi_curl_process(const user_opt *, const send_opt *, const upload_image_opt *, char *, char *, response_struct *);

user_opt user_opt_init(char *, char *);
/* Group message */
new_group_opt new_group_opt_init();
group_info_opt group_info_opt_init();
delete_groups_opt delete_groups_opt_init();
add_messages_opt add_messages_opt_init();
add_messages_json_opt add_messages_json_opt_init();
message_list_opt message_list_opt_init();
delete_messages_opt delete_messages_opt_init();
send_group_opt send_group_opt_init();
static size_t WriteMemoryCallback(void *, size_t, size_t, void *);
/* Message */
send_opt send_opt_init();
sent_opt sent_opt_init();
cancel_opt cancel_opt_init();
status_opt status_opt_init();
/* Image */
upload_image_opt upload_image_opt_init();
image_list_opt image_list_opt_init();
delete_images_opt delete_images_opt_init();
image_info_opt image_info_opt_init();
/* SenderID */
register_opt register_opt_init();
verify_opt verify_opt_init();
delete_opt delete_opt_init();
list_opt list_opt_init();
get_default_opt get_default_opt_init();
set_default_opt set_default_opt_init();

/* Group message */
response_struct create_group(const user_opt *, const new_group_opt *);
response_struct get_group_list(const user_opt *);
response_struct get_group_info(const user_opt *, const group_info_opt *);
response_struct delete_groups(const user_opt *, const delete_groups_opt *);
response_struct add_messages(const user_opt *, const add_messages_opt *);
response_struct add_messages_json(const user_opt *, const add_messages_json_opt *);
response_struct get_message_list(const user_opt *, const message_list_opt *);
response_struct delete_messages(const user_opt *, const delete_messages_opt *);
response_struct send_group(const user_opt *, const send_group_opt *);
/* Message */
response_struct send_message(const user_opt *, const send_opt *);
response_struct sent(const user_opt *, const sent_opt *);
response_struct cancel(const user_opt *, const cancel_opt *);
response_struct balance(const user_opt *);
response_struct status(const user_opt *, const status_opt *);
/* Image */
response_struct upload_image(const user_opt *, const upload_image_opt *);
response_struct image_info(const user_opt *, const image_info_opt *);
response_struct image_list(const user_opt *, const image_list_opt *);
response_struct delete_images(const user_opt *, const delete_images_opt *);
/* SenderID */
response_struct register_sender(const user_opt *, const register_opt *);
response_struct verify(const user_opt *, const verify_opt *);
response_struct delete_sender(const user_opt *, const delete_opt *);
response_struct list(const user_opt *, const list_opt *);
response_struct set_default(const user_opt *, const set_default_opt *);
response_struct get_default(const user_opt *, const get_default_opt *);

char* time_to_string(const time_t);
char* get_timestamp();
char* get_uuid();
char* get_signature(const char*, const char *, const char *);
void print_result(response_struct);