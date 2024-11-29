#include <ESP32Servo.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <WebServer.h>

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0200r0.html

#define ll long long
#define ld long double
#define F_OR(i, a, b, s) for (int i=(a); (s)>0?i<(b):i>(b); i+=(s))
#define F_OR1(e) F_OR(i, 0, e, 1)
#define F_OR2(i, e) F_OR(i, 0, e, 1)
#define F_OR3(i, b, e) F_OR(i, b, e, 1)
#define F_OR4(i, b, e, s) F_OR(i, b, e, s)
#define GET5(a, b, c, d, e, ...) e
#define F_ORC(...) GET5(__VA_ARGS__, F_OR4, F_OR3, F_OR2, F_OR1)
#define FOR(...) F_ORC(__VA_ARGS__)(__VA_ARGS__)
#define EACH(x, a) for (auto& x: a)

#define MODS 100005
#define MOD 1000000007
#define MOD1 998244353
#define INF 1e18
#define pi 3.14159265359
const int maxn = 9e6;
const int M = int(1e9);
const int N = 1e6 + 5;

#define dw digitalWrite
#define pm pinMode

// WiFi credentials
const char* ssid = "xxxx"; //wifi name
const char* password = "xxxx"; //wifi password
Servo servo;
int pos = 0;
// Initialize web server on port 80
WebServer server(80);

// Define the LED pin
const int ledPin = 2;  // GPIO2 is often connected to the built-in LED
const int buzzerPin = 16;
int lights[9]={13,12,14,27,26,25,33,32};
const int light1=13; 
const int light2=12; 
const int light3=14; 
const int light4=27; 
const int light5=26; 
const int light6=25; 
const int light7=33; 
const int light8=32; 

void setup() {
  // Set up the LED pin
  pm(buzzerPin, OUTPUT);
  pm(ledPin, OUTPUT);
  pm(light1, OUTPUT);
  pm(light2, OUTPUT);
  pm(light3, OUTPUT);
  pm(light4, OUTPUT);
  pm(light5, OUTPUT);
  pm(light6, OUTPUT);
  pm(light7, OUTPUT);
  pm(light8, OUTPUT);

  dw(light1, LOW);
  dw(light2, LOW);
  dw(light3, LOW);
  dw(light4, LOW);
  dw(light5, LOW);
  dw(light5, LOW);
  dw(light6, LOW);
  dw(light7, LOW);
  dw(light8, LOW);
  // servo attach
  servo.attach(23);
  // Connect to WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Define endpoint to control LED
  server.on("/control", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      String body = server.arg("plain");
      if (body.indexOf("SERVO_") != -1) { // Servo control command
      int startIndex = body.indexOf("SERVO_") + 6; // Position after "SERVO_"
      String degreeStr = body.substring(startIndex); // Extract degree value as a string
      int degree = degreeStr.toInt(); // Convert to integer
      if (degree >= 0 && degree <= 180) { // Check if degree is valid
        servo.write(degree); // Set servo position
        server.send(200, "application/json", "{\"status\":\"SERVO ROTATED\", \"degree\":" + String(degree) + "}");}
      }
      
      else if (body.indexOf("ONlight1") != -1) {
        dw(light1, HIGH);
        server.send(200, "application/json", "{\"status\":\"light1 ON\"}");
      } else if (body.indexOf("OFFlight1") != -1) {
        dw(light1, LOW);
        server.send(200, "application/json", "{\"status\":\"light1 OFF\"}");}
        else if (body.indexOf("ONlight2") != -1) {
        dw(light2, HIGH);
        server.send(200, "application/json", "{\"status\":\"light2 ON\"}");
      } else if (body.indexOf("OFFlight2") != -1) {
        dw(light2, LOW);
        server.send(200, "application/json", "{\"status\":\"light2 OFF\"}");}
        else if (body.indexOf("ONlight3") != -1) {
        dw(light3, HIGH);
        server.send(200, "application/json", "{\"status\":\"light3 ON\"}");
      } else if (body.indexOf("OFFlight3") != -1) {
        dw(light3, LOW);
        server.send(200, "application/json", "{\"status\":\"light3 OFF\"}");}
        else if (body.indexOf("ONlight4") != -1) {
        dw(light4, HIGH);
        server.send(200, "application/json", "{\"status\":\"light4 ON\"}");
      } else if (body.indexOf("OFFlight4") != -1) {
        dw(light4, LOW);
        server.send(200, "application/json", "{\"status\":\"light4 OFF\"}");}
        else if (body.indexOf("ONlight5") != -1) {
        dw(light5, HIGH);
        server.send(200, "application/json", "{\"status\":\"light5 ON\"}");
      } else if (body.indexOf("OFFlight5") != -1) {
        dw(light5, LOW);
        server.send(200, "application/json", "{\"status\":\"light5 OFF\"}");}
        else if (body.indexOf("ONlight6") != -1) {
        dw(light6, HIGH);
        server.send(200, "application/json", "{\"status\":\"light6 ON\"}");
      } else if (body.indexOf("OFFlight6") != -1) {
        dw(light6, LOW);
        server.send(200, "application/json", "{\"status\":\"light6 OFF\"}");}
        else if (body.indexOf("ONlight7") != -1) {
        dw(light7, HIGH);
        server.send(200, "application/json", "{\"status\":\"light7 ON\"}");
      } else if (body.indexOf("OFFlight7") != -1) {
        dw(light7, LOW);
        server.send(200, "application/json", "{\"status\":\"light7 OFF\"}");}
        else if (body.indexOf("ONlight8") != -1) {
        dw(light8, HIGH);
        server.send(200, "application/json", "{\"status\":\"light8 ON\"}");
      } else if (body.indexOf("OFFlight8") != -1) {
        dw(light8, LOW);
        server.send(200, "application/json", "{\"status\":\"light8 OFF\"}");}
      else if (body.indexOf("ONlight") != -1) {
        dw(ledPin, HIGH);
        dw(light1, HIGH);
        dw(light2, HIGH);
        dw(light3, HIGH);
        dw(light4, HIGH);
        dw(light5, HIGH);
        dw(light5, HIGH);
        dw(light6, HIGH);
        dw(light7, HIGH);
        dw(light8, HIGH);
        server.send(200, "application/json", "{\"status\":\"LED ON\"}");
      } else if (body.indexOf("OFFlight") != -1) {
        dw(ledPin, LOW);
        dw(light1, LOW);
        dw(light2, LOW);
        dw(light3, LOW);
        dw(light4, LOW);
        dw(light5, LOW);
        dw(light5, LOW);
        dw(light6, LOW);
        dw(light7, LOW);
        dw(light8, LOW);
        server.send(200, "application/json", "{\"status\":\"LED OFF\"}");}
        else if (body.indexOf("ONbuzzer") != -1) {
        dw(buzzerPin, HIGH);
        server.send(200, "application/json", "{\"status\":\"BUZZER ON\"}");
      } else if (body.indexOf("OFFbuzzer") != -1) {
        dw(buzzerPin, LOW);
        server.send(200, "application/json", "{\"status\":\"BUZZER OFF\"}");
      }
      else if (body.indexOf("lightblink_") != -1) {
        int startIndex = body.indexOf("lightblink_") + 11; 
        String timesStr = body.substring(startIndex); 
        int times = timesStr.toInt();
        FOR(times){
          dw(light1, HIGH);
          dw(light2, HIGH);
          dw(light3, HIGH);
          dw(light4, HIGH);
          dw(light5, HIGH);
          dw(light5, HIGH);
          dw(light6, HIGH);
          dw(light7, HIGH);
          dw(light8, HIGH);
          
          delay(400);
          dw(light1, LOW);
          dw(light2, LOW);
          dw(light3, LOW);
          dw(light4, LOW);
          dw(light5, LOW);
          dw(light5, LOW);
          dw(light6, LOW);
          dw(light7, LOW);
          dw(light8, LOW);
          delay(400);
        }
        server.send(200, "application/json", "{\"status\":\"blink " + String(times) + " times\"}");
      }









       else {
        server.send(400, "application/json", "{\"status\":\"Unknown command\"}");
      }
    } else {
      server.send(400, "application/json", "{\"status\":\"No command received\"}");
    }
  });

  // Start the server
  server.begin();

}

void loop() {
  
  server.handleClient();
  
}
