
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>

#define PIN            6
#define NUMPIXELS      15
Adafruit_NeoPixel jiminy = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

boolean inBody = false;
char resBody[15];
int bodyCur = 0;
char resBuffer[6];

EthernetClient client;

// Server running Jiminy agent
char server[] = "puppet.example.com";

unsigned long lastConnectionTime = 0;
boolean lastConnected = false;
const unsigned long postingInterval = 5*1000;

void setup() {
  jiminy.begin();
  for(int k=0; k < 15; k++) {
    jiminy.setPixelColor(k, jiminy.Color(0,0,0));
  }
  jiminy.show();
  Serial.begin(9600);
  
  delay(1000);
    
  for(int j=0; j < 15; j = j + 2) {
    jiminy.setPixelColor(j, jiminy.Color(0,0,0));
  }
  for(int j=1; j < 15; j = j + 2) {
    jiminy.setPixelColor(j, jiminy.Color(0,0,255));
  }
  jiminy.show();
  
  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    
    for(int i=0; i < 10; i++) {
      for(int j=0; j < 5; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,255,0));
      }
      for(int j=5; j < 10; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,0,0));
      }
      for(int j=10; j < 15; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,255,0));
      }
      jiminy.show();
      
      delay(1000);
    
      for(int j=0; j < 5; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,0,0));
      }
      for(int j=5; j < 10; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,255,0));
      }
      for(int j=10; j < 15; j++) {
        jiminy.setPixelColor(j, jiminy.Color(0,0,0));
      }
      jiminy.show();
        
      delay(1000);
    
      for(int j=0; j < 15; j = j + 2) {
        jiminy.setPixelColor(j, jiminy.Color(0,0,0));
      }
      for(int j=1; j < 15; j = j + 2) {
        jiminy.setPixelColor(j, jiminy.Color(0,0,255));
      }
      jiminy.show();
    }
  }
  
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {\
  if (client.available()) {
    char c = client.read();
    
    if (!inBody) {
      resBuffer[0] = resBuffer[1];
      resBuffer[1] = resBuffer[2];
      resBuffer[2] = resBuffer[3];
      resBuffer[3] = resBuffer[4];
      resBuffer[4] = resBuffer[5];
      resBuffer[5] = c;
    
      if (resBuffer[0]-0 == 10 && resBuffer[1]-0 == 13) {
        inBody = true;
      }
    } else if (inBody && bodyCur < 16) {
      resBody[bodyCur] = c;
      bodyCur++;
    }
  }

  if (!client.connected() && lastConnected) {
    client.stop();
    
    for(int i=0; i < 15; i++) {
      switch(resBody[i]) {
        case 'U':
          jiminy.setPixelColor(i, jiminy.Color(150,0,0));
          Serial.print('G');
          break;
        case 'C':
          jiminy.setPixelColor(i, jiminy.Color(200,0,200));
          Serial.print('C');
          break;
        case 'F':
          jiminy.setPixelColor(i, jiminy.Color(0,255,0));
          Serial.print('R');
          break;
        case 'N':
          jiminy.setPixelColor(i, jiminy.Color(127,255,0));
          Serial.print('O');
          break;
        default:
          jiminy.setPixelColor(i, jiminy.Color(0,0,0));
          Serial.print('x');
          break;
      }
    }
    jiminy.show();
    Serial.println();
    bodyCur = 0;
    inBody = false;
    resBuffer[0] == 0;
    resBuffer[1] == 0;
    resBuffer[2] == 0;
  }

  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  lastConnected = client.connected();
}
 
void httpRequest() {
  if (client.connect(server, 1492)) {
    client.println("GET /jiminy HTTP/1.1");
    client.println("Host: puppet.example.com"); //Server running Jiminy agent
    client.println("User-Agent: jiminy-bar");
    client.println("Connection: close");
    client.println();

    lastConnectionTime = millis();
  }
  else {
    Serial.println("connection failed");
    client.stop();
    
    for(int j=0; j < 500; j++) {
      for(int k=0; k < 15; k++) {
        jiminy.setPixelColor(k, jiminy.Color(0,map(j,0,500,0,255),0));
      }
      jiminy.show();
      
      delay(10);
    }
    for(int j=0; j < 500; j++) {
      for(int k=0; k < 75; k++) {
        jiminy.setPixelColor(k, jiminy.Color(0,map(500-j,0,500,0,255),0));
      }
      jiminy.show();
      
      delay(5);
    }
  }
}

