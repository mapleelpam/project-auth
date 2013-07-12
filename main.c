/***************************************************************************
 *
 * CURL copy right: http://curl.haxx.se/docs/copyright.html.
 * 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>		// for hinding password
#include <unistd.h>			// for hinding password
#include <string.h>
#include <curl/curl.h>

#define KEY_SIZE				20
#define VALUE_SIZE				200
#define POST_STRING_MAX_LENGTH	300
#define PARA_N					4
#define CONTENT_SIZE			20000

#define LOGIN_SUCCESS			1
#define LOGIN_ERROR				(-1)

#define URL						"http://heron.nctucs.net/authcpp/custom-wp-login.php"
#define URL_TEST				"http://heron.nctucs.net/authcpp/post/post.php"

#define SUCCESS_STR				"SUCCESS"

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

// main
int main(void)
{
	char url[] = URL;
	char username[VALUE_SIZE];
	char password[VALUE_SIZE];
	char *device_code;
	PARA paras[PARA_N];

	// read
	printf("username >> ");
	scanf("%s", username);

	// read - password is hidden when typing
	printf("password >> ");
	get_password(password);
	printf("\n");

	//*****************************************
	// device code here
	device_code = "abcd1234";
	//*****************************************

	//
	paras[0].key = "log",			paras[0].value = username;
	paras[1].key = "pwd",			paras[1].value = password;
	paras[2].key = "device_code",	paras[2].value = device_code;
	paras[3].key = "action",		paras[3].value = "login";

	//
	printf("\n============= post response ==============\n");
	char *paras_str;
	paras_str = convert_para(paras, sizeof(paras)/sizeof(paras[0]));
	if( curl_post(url, paras_str) == LOGIN_SUCCESS){
		printf("successful login.\n");
	} else {
		printf("login error.\n");
	}

	//
	return 0;
}

void get_password(char *password){

	// set hidden
	struct termios term, term_orig;
	tcgetattr(STDIN_FILENO, &term);
	term_orig = term;
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	// read here
	scanf("%s", password);

	// set back
	tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
}

char *convert_para(PARA paras[], int size){

	// if input paras is empty
	if(sizeof(paras) == 0 || sizeof(paras[0]) == 0){
		fprintf(stderr, "convert_para() failed: input paras errer.\n");
		return NULL;
	}

	//
	char *result = calloc(POST_STRING_MAX_LENGTH, sizeof(char));
	int i;
	for(i=0 ; i<size ; i++){
		strcat(result, paras[i].key);
		strcat(result, "=");
		strcat(result, paras[i].value);
		if(i != size-1)	strcat(result, "&");
	}

	return result;
}

int curl_post(char url[], char *paras_str){

	// 
	int login_status = LOGIN_ERROR;	// default value

	//
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if(curl) {

		struct string content;
		init_string(&content);

		/* First set the URL that is about to receive our POST. This URL can
		   just as well be a https:// URL if that is what should receive the
		   data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);

		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paras_str);

		/* Save the result in custom string struct */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		/* Print Out Result */
		printf("%s", content.ptr);
		login_status = check_login_status(content.ptr);
		free(content.ptr);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	// cleanup
	curl_global_cleanup();

	// return
	return login_status;
}

int check_login_status(char *content_s){
	int len = sizeof(SUCCESS_STR);
	char sub_s[len];
	memcpy( sub_s, content_s, len );

	if (strcmp(sub_s, SUCCESS_STR) == 0){
		return LOGIN_SUCCESS;
	}

	return LOGIN_ERROR;
}

void init_string(struct string *s) {
	s->len = 0;
	s->ptr = malloc(s->len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {

	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}

	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}
