
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#include "connection.h"
#include "util.h"
#include "../config.h"

//-----------------------------------------------------------------------------------------------

WiFiSSLClient wifi;

//-----------------------------------------------------------------------------------------------

void connect(){

	// Get data
	int status = WL_IDLE_STATUS;
	char ssid[] = SECRET_SSID;
	char pass[] = SECRET_PASS;

	// Check for WiFi module
	if (WiFi.status() == WL_NO_MODULE) {
		exit("Communication with WiFi module failed!");
	}

	// Check Firmware
	String fv = WiFi.firmwareVersion();
	if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
		msgPrintln("Please upgrade the firmware!");
	}
	
	// Print state
	msgPrint("Connect to SSID: ");
	msgPrint(ssid);

	// Connect to network
	int connectionAttempts = 0;
	while (true) {
		
		// Connect
		status = WiFi.begin(ssid, pass);
		
		// Check state
		if(status == WL_CONNECTED){
			break;
		}
		else if(connectionAttempts > 100){
			exit("Can not connect to Wifi!");
		}
		else{
			delay(2000);
			msgPrint(".");
			connectionAttempts++;
		}
	}

	// Connection sucessful: Print state
	msgPrint("\nConnected to WiFi: ");
	msgPrintln(ssid);
	//printWifiStatus();
}

//-----------------------------------------------------------------------------------------------

Response request(String server, String path, int port, String header, String httpMethod, String postData){
  
	// Print state
	msgPrint("Request: ");
	msgPrint(httpMethod);
	msgPrint(" ");
	msgPrint(server);
	msgPrint(path);
	msgPrint(" - ");
	msgPrintln(postData);	

	// Make request
	HttpClient client = HttpClient(wifi, server, port);
	client.beginRequest();
	
	// Switch HTTP Method
	if(httpMethod.compareTo(HTTP_METHOD_GET) == 0){
		client.get(path);
	}
	else if(httpMethod.compareTo(HTTP_METHOD_POST) == 0){
		client.post(path);
	}
	else if(httpMethod.compareTo(HTTP_METHOD_PUT) == 0){
		client.put(path);
	}
	else if(httpMethod.compareTo(HTTP_METHOD_PATCH) == 0){
		client.patch(path);
	}
	else if(httpMethod.compareTo(HTTP_METHOD_DELETE) == 0){
		client.del(path);
	}
	
	// Set Headers and body
	client.sendHeader(header);
	client.sendHeader("Content-Type", "application/json");
	client.sendHeader("Content-Length", postData.length());
	client.beginBody();
    client.print(postData);
	client.endRequest();

	// Get response
	int status = client.responseStatusCode();
	String body = client.responseBody();
	
	// Print state
	msgPrint("Response: ");
	msgPrint(String(status));
	msgPrint(" ");
	msgPrintln(body);
	
	// Copy to response
	Response res = {status, body};
	return res;
}

//-----------------------------------------------------------------------------------------------

void printWifiStatus() {
	
	msgPrintln("\n\nWiFi connection state\n-------------------------------------");

	// Print the SSID of the network you're attached to:
	msgPrint("SSID: ");
	msgPrintln(String(WiFi.SSID()));

	// Print your board's IP address:
	IPAddress ip = WiFi.localIP();
	msgPrint("IP Address: ");
	msgPrintln(String(ip));

	// Print the received signal strength:
	long rssi = WiFi.RSSI();
	msgPrint("signal strength (RSSI):");
	msgPrint(String(rssi));
	msgPrintln(" dBm\n");
}
