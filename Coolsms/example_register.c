#include "coolsms.h"

/*
example_register.c
- phone register
*/

void print_result(response_struct);

#if 0
int main()
{
	response_struct result;
	char *api_key = "API_KEY";
	char *api_secret = "API_SECRET";
	user_opt user_info = user_opt_init(api_key, api_secret);
	register_opt register_info = register_opt_init();
	 
	register_info.phone = "01000000000";
	register_info.site_user = "__private__";

	result = _register(&user_info, &register_info);
	print_result(result);

	return 0;
}

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

#endif
