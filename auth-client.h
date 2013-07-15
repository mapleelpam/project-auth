#include "constants.h"

// data type
typedef struct {
	char *key;
	char *value;
} PARA;

struct string {
	char *ptr;
	size_t len;
};

// functions
char *convert_para(PARA paras[], int size);
void get_password(char *password);
int curl_post();

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
