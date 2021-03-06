#include <ESP8266WiFi.h>
#include "dht11.h"

// WiFi setup - set wifiIndex to the index of the credentials you want
int wifiIndex = 1;
char wifi[][2][64] = {
  { "Holt-2.4Ghz", "67351932" },
  { "COG_MEL_MAC_WIFI", "Ct$Mel@345" }
};

// DHT11 (temp/humidity module setup)
dht11 dht;
const int dhtPin = 2;

// Request destination settings
const char* host = "lynq.me";
const int port = 8000;
String url = "/";

String request = String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n\r\n";

void setup() {
  Serial.begin(115200);

	// Set up LED
	pinMode(BUILTIN_LED, OUTPUT);

	// Connect to WiFi
	Serial.print("\nConnecting to WiFi");
	WiFi.begin(wifi[wifiIndex][0], wifi[wifiIndex][1]);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}
	Serial.println("WiFi connected");
	Serial.print("  IP: "); Serial.println(WiFi.localIP());

	// Connect to server
	WiFiClient client;
	Serial.print("Connecting to server ... ");
	if (!client.connect(host, port)) {
		Serial.println("Connection failed");
		return;
	}
	Serial.println("Connection succeeded");

  // Make request
	Serial.print("Requesting URL: "); Serial.println(url);
	client.print(request);
	Serial.println("Request:\n```\n" + request + "\n```");

	delay(1000);   // This is the amount of time to give the request to complete
                 // 50 is plenty for LAN under most conditions
                 // For internet it varies a lot - 1000 should be pretty safe
                 // unless your internet connection is bad

  // Receive response
	Serial.println("Response:\n```\n");
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.println(line);
	}
  Serial.println("\n```");

  Serial.println("== DONE ==");
}

void loop() {
	delay(5000);

  // Get latest status from DHT11
  int chk = dht.read(dhtPin);
  Serial.print("\nRead sensor: ");
  if (chk == DHTLIB_OK)   // I'm really not a fan of switch statements
    Serial.println("OK");
  else if (chk == DHTLIB_ERROR_CHECKSUM)
    Serial.println("Checksum error");
  else if (chk == DHTLIB_ERROR_TIMEOUT)
    Serial.println("Timed out");
  else
    Serial.println("Unknown error");

  Serial.print("Temperature: "); Serial.println((float) dht.temperature);
  Serial.print("Humidity: "); Serial.println((float) dht.humidity);
}
