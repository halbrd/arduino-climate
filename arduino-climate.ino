#include <ESP8266WiFi.h>

char ssid[] = "Holt-2.4Ghz";
char pass[] = "67351932";

void setup() {
  Serial.begin(115200);

	// Set up LED
	pinMode(BUILTIN_LED, OUTPUT);

	// Connect to WiFi
	Serial.print("Connecting to WiFi");
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}
	Serial.println("WiFi connected");
	Serial.print("  IP: "); Serial.println(WiFi.localIP());

	// Connect to server
	WiFiClient client;
	const char* host = "192.168.0.2";
	const int port = 8000;
	Serial.print("Connecting to server ");
	if (!client.connect(host, port)) {
		Serial.println("Connection failed");
		return;
	}
	Serial.println("Connection succeeded");

  // Make request
	String url = "/";
	Serial.print("Requesting URL: "); Serial.println(url);
	String request = String("GET ") + url + " HTTP/1.1\r\n" +
											"Host: " + host + "\r\n" +
											"Connection: close\r\n\r\n";
	client.print(request);
	Serial.println("Request: " + request);

	delay(10);

  // Receive response
	Serial.println("Response: ");
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.println(line);
	}
}

void loop() {
	digitalWrite(BUILTIN_LED, HIGH);
	delay(1000);
	digitalWrite(BUILTIN_LED, LOW);
	delay(1000);
}
