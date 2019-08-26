#ifndef _CONFIG_H
#define _CONFIG_H

// Common
#define APP_NAME "TimeTracker"

// Wifi configuration
#define SECRET_SSID "WIFI SSID Name"
#define SECRET_PASS "Password"

// Clockify
#define CLOCKIFY_API_URL "api.clockify.me"
#define CLOCKIFY_API_PATH "/api/v1"
#define CLOCKIFY_API_PORT 443
#define CLOCKIFY_API_KEY "XYZ"
#define CLOCKIFY_API_WORKSPACE_ID "XYZ"
#define CLOCKIFY_PROJECT_ID "XYZ"
#define CLOCKIFY_PROJECT_BILLABLE "false"
#define CLOCKIFY_DESCRIPTION_PREFIX "Anwesenheit"
#define CLOCKIFY_TAG_DELIMITER ":"
#define CLOCKIFY_MAX_NR_OF_USERS 300

// Time Definitions
#define TIMEFORMAT "Y-m-d~TH:i:s~Z"
#define DAY_OF_WEEK {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"}

// RFID RC522
#define RC522_RST_PIN 6
#define RC522_SS_PIN 11

#endif
