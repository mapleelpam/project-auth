#include <stdlib.h>
#include <termios.h>		// for hinding password
#include <unistd.h>			// for hinding password
#include <string.h>
#include <curl/curl.h>

#include "auth-client.h"

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
		//printf("%s", content.ptr);
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
	return (int)(content_s[0] - '0');
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
