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
	char *device_str;
	PARA paras[PARA_N];

	// read
	printf("username >> ");
	scanf("%s", username);

	// read - password is hidden when typing
	printf("password >> ");
	get_password(password);
	printf("\n");

	//*****************************************
	// device str here
	device_str = "tF27432Bh0Xt26s468HQ";
	//*****************************************

	//
	paras[0].key = "log",			paras[0].value = username;
	paras[1].key = "pwd",			paras[1].value = password;
	paras[2].key = "device_str",	paras[2].value = device_str;
	paras[3].key = "action",		paras[3].value = "login";

	//
	int login_status;
	char *paras_str;
	paras_str = convert_para(paras, sizeof(paras)/sizeof(paras[0]));

	login_status = curl_post(url, paras_str);

	if (login_status == SUCCESS_STR) {
		printf("success\n");
	}
	else if (login_status == WRONG_DEVICE_STR) {
		printf("wrong device string\n");
	}
	else {
		printf("fail to login\n");
	}

	//
	return 0;
}
