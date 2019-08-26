#ifndef _CLOCKIFY_H
#define _CLOCKIFY_H

#include <ArduinoHttpClient.h>

#include "connection.h"

//-----------------------------------------------------------------------------

void     initTags();
String   getUserId(String cardId);
String   getUserName(String cardId);
int      startTimer(String userId);
int      stopTimer(String userId);
Response getClockifyData(String path, String httpMethod = HTTP_METHOD_GET, String postData = "");

//-----------------------------------------------------------------------------

#endif