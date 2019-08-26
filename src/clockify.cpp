
#include <SPI.h>
#include <ezTime.h>
#include <ArduinoJson.h>

#include "clockify.h"
#include "connection.h"
#include "util.h"
#include "../config.h"

//-----------------------------------------------------------------------------

String userNames[CLOCKIFY_MAX_NR_OF_USERS];
String userIds[CLOCKIFY_MAX_NR_OF_USERS];
String cardIds[CLOCKIFY_MAX_NR_OF_USERS];
int nrOfTags = 0;

//-----------------------------------------------------------------------------

void initTags(){
	
	// Get all tags
	Response res = getClockifyData("/workspaces/" CLOCKIFY_API_WORKSPACE_ID "/tags");
	
	//DynamicJsonDocument doc(res.body.length());
	StaticJsonDocument<3000> doc;
	
	 // Deserialize the JSON document
	DeserializationError error = deserializeJson(doc, res.body);

	// Test if parsing succeeds
	if (error) {
		msgPrint("Error while deserializeJson(): ");
		msgPrintln(error.c_str());
		return;
	}
	
	// Walk the JsonArray
	JsonArray arr = doc.as<JsonArray>();
	for(JsonObject jsonObject: arr) {
		
		const char* name = jsonObject["name"];
		String tag = String(name);
		
		int firstDelimiter = tag.indexOf(CLOCKIFY_TAG_DELIMITER);
		int lastDelimiter = tag.lastIndexOf(CLOCKIFY_TAG_DELIMITER);
		
		userNames[nrOfTags] = tag.substring(0, firstDelimiter);
		userIds[nrOfTags] = tag.substring(firstDelimiter+1, lastDelimiter);
		cardIds[nrOfTags] = tag.substring(lastDelimiter+1, tag.length());
		
		nrOfTags++;
		
		if(nrOfTags >= CLOCKIFY_MAX_NR_OF_USERS){
			break;
		}
	}
}

//-----------------------------------------------------------------------------

String getUserId(String cardId){
	for(int i=0; i<nrOfTags; i++){
		if(cardId.compareTo(cardIds[i]) == 0){
			return userIds[i];
		}
	}
	return String("");
}

//-----------------------------------------------------------------------------

String getUserName(String cardId){
	for(int i=0; i<nrOfTags; i++){
		if(cardId.compareTo(cardIds[i]) == 0){
			return userNames[i];
		}
	}
	return String("");
}

//-----------------------------------------------------------------------------

int startTimer(String userId){
	
	// Sync clock and get time
	waitForSync();
	String time = UTC.dateTime(TIMEFORMAT);
	
	// Build path	
	String path = "";
	path.concat("/workspaces/" CLOCKIFY_API_WORKSPACE_ID "/user/");
	path.concat(userId);
	path.concat("/time-entries");
	
	// Get day of week
	String dayOfWeek[] = DAY_OF_WEEK;
	int numericDayOfWeek = UTC.dateTime("w").toInt()-1;
	
	// Build request post data
	String timeEntryData = "";
	timeEntryData.concat("{\"start\":\"");
	timeEntryData.concat(time);
	timeEntryData.concat("\",\"projectId\":\"" CLOCKIFY_PROJECT_ID "\",\"billable\":\"" CLOCKIFY_PROJECT_BILLABLE "\",\"description\":\"" CLOCKIFY_DESCRIPTION_PREFIX " ");
	timeEntryData.concat(dayOfWeek[numericDayOfWeek]);
	timeEntryData.concat("\"}");
	
	// Make request
	Response res = getClockifyData(path, HTTP_METHOD_POST, timeEntryData);
	return res.status;
}

//-----------------------------------------------------------------------------

int stopTimer(String userId){
	
	// Sync clock
	waitForSync();
	
	// Get current time
	String time = UTC.dateTime(TIMEFORMAT);
	
	// Build path
	String path = "";
	path.concat("/workspaces/" CLOCKIFY_API_WORKSPACE_ID "/user/");
	path.concat(userId);
	path.concat("/time-entries");
	
	// Build request post data
	String timeEntryData = "";
	timeEntryData.concat("{\"end\":\"");
	timeEntryData.concat(time);
	timeEntryData.concat("\"}");
	
	// Make request
	Response res = getClockifyData(path, HTTP_METHOD_PATCH, timeEntryData);
	return res.status;
}

//-----------------------------------------------------------------------------

Response getClockifyData(String path, String httpMethod, String postData){
	
	// Merge API Path and request path
	String completePath = "";
	completePath.concat(CLOCKIFY_API_PATH);
	completePath.concat(path);
	
	// Get data
	return request(CLOCKIFY_API_URL, completePath, CLOCKIFY_API_PORT, "X-Api-Key: " CLOCKIFY_API_KEY, httpMethod, postData);
}