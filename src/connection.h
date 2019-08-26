#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <ArduinoHttpClient.h>

//-----------------------------------------------------------------------------

typedef struct {
	int status;
	String body;
} Response;

//-----------------------------------------------------------------------------

void     connect();
Response request(String url, String path = "/", int port = 80, String header = "", String httpMethod = HTTP_METHOD_GET, String postData = "");
void     printWifiStatus();

//-----------------------------------------------------------------------------

#endif