/***************************************************************************
 *
 * CURL copy right: http://curl.haxx.se/docs/copyright.html.
 * 
 ***************************************************************************/

#include <stdio.h>
#include "auth-client.h"

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
