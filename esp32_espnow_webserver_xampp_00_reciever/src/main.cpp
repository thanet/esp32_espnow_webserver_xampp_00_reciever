/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-esp-now-wi-fi-web-server/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

#include <HTTPClient.h> 

// Replace with your network credentials (STATION)  enjoy office
// const char* ssid = "True Enjoy";
// const char* password = "enjoy7777777777";
// String URL = "http://127.0.0.1/esp8266/test.php";
// int temperature = 0; 
// int humidity = 0;


// Replace with your network credentials (STATION)  บ้านริมโขง
const char* ssid = "ENJMesh";
const char* password = "enjoy042611749";
String URL = "http://192.168.0.113/EspData/upload.php";
int temperature = 0; 
int humidity = 0;

void UploadData2Xampp();
//void connectWiFi();

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

struct_message incomingReadings;

JSONVar board;

AsyncWebServer server(80);
AsyncEventSource events("/events");

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  board["id"] = incomingReadings.id;
  board["temperature"] = incomingReadings.temp;
  board["humidity"] = incomingReadings.hum;
  board["readingId"] = String(incomingReadings.readingId);
  String jsonString = JSON.stringify(board);
  events.send(jsonString.c_str(), "new_readings", millis());
  
  Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  Serial.printf("t value: %4.2f \n", incomingReadings.temp);
  Serial.printf("h value: %4.2f \n", incomingReadings.hum);
  Serial.printf("readingID value: %d \n", incomingReadings.readingId);
  Serial.println();

// ++for upload data to xampp

    // ++get data from esp_now to temperator and humidity
    temperature = int(board["temperature"]);
    humidity = int(board["humidity"]);
    // --get data from esp_now to temperator and humidify
    //WiFi.mode(WIFI_STA);
    //delay (2000);
   
  // String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity); 

  // HTTPClient http; 
  // http.begin(URL);
  // http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  
  // int httpCode = http.POST(postData); 
  // String payload = http.getString(); 
  
  
  // if(httpCode > 0) {
  //   // file found at server
  //   if(httpCode == HTTP_CODE_OK) {
  //     String payload = http.getString();
  //     Serial.println(payload);
  //   } else {
  //     // HTTP header has been send and Server response header has been handled
  //     Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  //   }
  // } else {
  //   Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  // }
  
  // http.end();  //Close connection
  
  // Serial.print("URL : "); Serial.println(URL); 
  // Serial.print("Data: "); Serial.println(postData); 
  // Serial.print("httpCode: "); Serial.println(httpCode); 
  // Serial.print("payload : "); Serial.println(payload); 
  // Serial.println("--------------------------------------------------");

  // delay(10000);
// --for upload data to xampp

}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.humidity { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #1 - TEMPERATURE</h4><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt1"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #1 - HUMIDITY</h4><p><span class="reading"><span id="h1"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh1"></span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #2 - TEMPERATURE</h4><p><span class="reading"><span id="t2"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt2"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #2 - HUMIDITY</h4><p><span class="reading"><span id="h2"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh2"></span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  document.getElementById("t"+obj.id).innerHTML = obj.temperature.toFixed(2);
  document.getElementById("h"+obj.id).innerHTML = obj.humidity.toFixed(2);
  document.getElementById("rt"+obj.id).innerHTML = obj.readingId;
  document.getElementById("rh"+obj.id).innerHTML = obj.readingId;
 }, false);
}
</script>
</body>
</html>)rawliteral";



void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set the device as a Station and Soft Access Point simultaneously
  //WiFi.mode(WIFI_AP_STA);
  
// Set the device as a Station 
  WiFi.mode(WIFI_STA);
      WiFi.setSleep(WIFI_PS_NONE);
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
   
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}
 
void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
  }

UploadData2Xampp();
// ++for upload data to xampp

  //   // ++get data from esp_now to temperator and humidity
  //   temperature = int(board["temperature"]);
  //   humidity = int(board["humidity"]);
  //   // --get data from esp_now to temperator and humidify

  //   String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity); 

  //   HTTPClient http; 
  //   http.begin(URL);
  //   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
  //   int httpCode = http.POST(postData); 
  //   String payload = http.getString(); 

  //   if(httpCode > 0) {
  //   // file found at server
  //     if(httpCode == HTTP_CODE_OK) {
  //     String payload = http.getString();
  //     Serial.println(payload);
  //     } else {
  //       // HTTP header has been send and Server response header has been handled
  //       Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  //     }
  //     } else {
  //       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  //   }
  
  // http.end();  //Close connection
    
  //   Serial.print("URL : "); Serial.println(URL); 
  //   Serial.print("Data: "); Serial.println(postData); 
  //   Serial.print("httpCode: "); Serial.println(httpCode); 
  //   Serial.print("payload : "); Serial.println(payload); 
  //   Serial.println("--------------------------------------------------");
  //   delay(10000);
// --for upload data to xampp

}



//Function for upload data to xampp
void UploadData2Xampp() {
  Serial.println("Have Data to up Upload...");

  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity); 

  HTTPClient http; 
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(postData); 
  String payload = http.getString(); 
  
  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData); 
  Serial.print("httpCode: "); Serial.println(httpCode); 
  Serial.print("payload : "); Serial.println(payload); 
  Serial.println("--------------------------------------------------");
  delay(20000);

}