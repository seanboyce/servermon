#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <TM1640.h>
#include <TM16xxMatrix.h>
#include <WiFiClientSecureBearSSL.h>

unsigned long lastTime = 0;
// Check every 70 seconds
unsigned long timerDelay = 70000;



// Monitor these 8 servers + 1 for Internet connectivity check. First item in array is for internet connectivity (set to 1.1.1.1 clouflare, but you can use any server)."
#define ARRAYSIZE 9
String servers[ARRAYSIZE] = { "https://one.one.one.one/", "https://server1.com", "https://server2.com","https://server3.com", "https://server4.com", "https://server5.com", "https://server6.com","https://server7.com", "https://server8.com" };
// save last state to secide if RGB LED should be red or yellow on fail
bool laststate[ARRAYSIZE] = {true,true,true,true,true,true,true,true,true};

// TM1640 Settings. D5=14=SCLK, D6=12=DATA
TM1640 module(12, 14);
// We use 6 rows (SEG1-6) and 4 columns (GRID 1-4). This is 8 RGB LEDs
#define MATRIX_NUMCOLUMNS 6
#define MATRIX_NUMROWS 4

TM16xxMatrix matrix(&module, MATRIX_NUMCOLUMNS, MATRIX_NUMROWS);    // TM16xx object, columns, rows

ESP8266WiFiMulti WiFiMulti;


void setup() {
//Start LED tests!
// Wi-Fi LED
pinMode(5, OUTPUT);
digitalWrite(5, HIGH);
delay(250);
digitalWrite(5, LOW);
// Internet Connectivity LED
pinMode(4, OUTPUT);
digitalWrite(4, HIGH);
delay(250);
digitalWrite(4, LOW);
    // Blink all on startup as LED test
  for(int i=0; i<(MATRIX_NUMROWS-1); i++)
  {
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++)
    {
        matrix.setPixel(i,j, true);
        delay(200);
        matrix.setPixel(i,j, false);
    }
  }
// Last two LEDS are swapped columns to make the boards easier to route.
for(int i=3; i<MATRIX_NUMROWS; i++)
  {
    for(int j=0; j<MATRIX_NUMCOLUMNS; j++)
    {
        matrix.setPixel(i,5-j, true);
        delay(200);
        matrix.setPixel(i,5-j, false);
    }
  }
  Serial.begin(115200);

// LED tests complete. You should have seen all LEDs light up sequentially.

  // the true work begins. We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to Wi-Fi");

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("your_network", "your_password");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Set Wi-Fi LED = on
  digitalWrite(5, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation, to save memory.
    client->setInsecure();

// Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      //WiFiClient client;
      for(int i=0; i<9; i++){
        HTTPClient https;
        // Yo ucan remove printing to serial here if you want, but it's useful for debug.
        Serial.println(servers[i]);
        https.begin(*client, servers[i].c_str());
        int httpResponseCode = https.GET();
        Serial.println(httpResponseCode);
        https.end();
        // The case/switch below is sort of gross. There are more elegant ways to do this, however this one is the easiest for beginenrs to modify.
        if (httpResponseCode==200) {
          switch (i){
          case 0:
              digitalWrite(4, HIGH);
              break;
          case 1:
              matrix.setPixel(0,1, false); // Red
              matrix.setPixel(0,0, false); // Blue
              matrix.setPixel(0,2, true); // Green
              laststate[i] = true;
              break;
          case 2:
              matrix.setPixel(0,4, false); // 
              matrix.setPixel(0,3, false); // 
              matrix.setPixel(0,5, true); // 
              laststate[i] = true;
              break;
          case 3:
              matrix.setPixel(1,1, false); // 
              matrix.setPixel(1,0, false); // 
              matrix.setPixel(1,2, true); // 
              laststate[i] = true;
              break;
          case 4:
              matrix.setPixel(1,4, false); // 
              matrix.setPixel(1,3, false); // 
              matrix.setPixel(1,5, true); // 
              laststate[i] = true;
              break;
          case 5:
              matrix.setPixel(2,1, false); // 
              matrix.setPixel(2,0, false); // 
              matrix.setPixel(2,2, true); // 
              laststate[i] = true;
              break;
          case 6:
              matrix.setPixel(2,4, false); // 
              matrix.setPixel(2,3, false); // 
              matrix.setPixel(2,5, true); // 
              laststate[i] = true;
              break;
          case 7:
              matrix.setPixel(3,1, false); // 
              matrix.setPixel(3,0, false); // 
              matrix.setPixel(3,2, true); // 
              laststate[i] = true;
              break;
          case 8:
              matrix.setPixel(3,4, false); // 
              matrix.setPixel(3,3, false); // 
              matrix.setPixel(3,5, true); // 
              laststate[i] = true;
              break;
          }
      }
      else {
        switch (i){
          case 0:
              digitalWrite(4, LOW);
              break;
          case 1:
              if (laststate[i] == true){
                  matrix.setPixel(0,1, true); // Red, Last state was HTTP 200 so we turn yellow
                  matrix.setPixel(0,0, false); // Blue
                  matrix.setPixel(0,2, true); // Green
                  laststate[i] = false; // save state
              }
              else{
                  matrix.setPixel(0,1, true);
                  matrix.setPixel(0,0, false); 
                  matrix.setPixel(0,2, false); // Last state was not HTTP 200, so we turn red.
              }

              break;  
          case 2:
              if (laststate[i] == true){
                  matrix.setPixel(0,4, true); // 
                  matrix.setPixel(0,3, false); // 
                  matrix.setPixel(0,5, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(0,4, true);
                  matrix.setPixel(0,3, false); 
                  matrix.setPixel(0,5, false); //   
              }

              break;  
          case 3:
              if (laststate[i] == true){
                  matrix.setPixel(1,1, true); // 
                  matrix.setPixel(1,0, false); // 
                  matrix.setPixel(1,2, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(1,1, true);
                  matrix.setPixel(1,0, false); 
                  matrix.setPixel(1,2, false); //   
              }

              break;  
          case 4:
              if (laststate[i] == true){
                  matrix.setPixel(1,4, true); // 
                  matrix.setPixel(1,3, false); // 
                  matrix.setPixel(1,5, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(1,4, true);
                  matrix.setPixel(1,3, false); 
                  matrix.setPixel(1,5, false); //   
              }

              break; 
          case 5:
              if (laststate[i] == true){
                  matrix.setPixel(2,1, true); // 
                  matrix.setPixel(2,0, false); // 
                  matrix.setPixel(2,2, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(2,1, true);
                  matrix.setPixel(2,0, false); 
                  matrix.setPixel(2,2, false); //   
              }

              break;  
          case 6:
              if (laststate[i] == true){
                  matrix.setPixel(2,4, true); // 
                  matrix.setPixel(2,3, false); // 
                  matrix.setPixel(2,5, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(2,4, true);
                  matrix.setPixel(2,3, false); 
                  matrix.setPixel(2,5, false); //   
              }

              break;  
          case 7:
              if (laststate[i] == true){
                  matrix.setPixel(3,1, true); // 
                  matrix.setPixel(3,0, false); // 
                  matrix.setPixel(3,2, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(3,1, true);
                  matrix.setPixel(3,0, false); 
                  matrix.setPixel(3,2, false); //   
              }

              break;  
          case 8:
              if (laststate[i] == true){
                  matrix.setPixel(3,4, true); // 
                  matrix.setPixel(3,3, false); // 
                  matrix.setPixel(3,5, true); // 
                  laststate[i] = false; // 
              }
              else{
                  matrix.setPixel(3,4, true);
                  matrix.setPixel(3,3, false); 
                  matrix.setPixel(3,5, false); //Red   
              }

              break;
          }
        
      }
      }}
      // We don't actually handle what to do on disconnect other than turn off the LEDs to alert you. Yo ucould write a handler for that here. Probably, just restarting the device would be an OK handler. However, I've never had it disconnect. 
    else {
      Serial.println("WiFi Disconnected");
      digitalWrite(5, LOW);
      digitalWrite(4, LOW); // If no Wi-Fi, there's no Internet.
    }
    lastTime = millis();
  }

}
