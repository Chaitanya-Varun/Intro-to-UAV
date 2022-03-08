#include <WiFi.h>//https://www.arduino.cc/en/Reference/WiFi
//Constants
#define LED 2
#define L_in1 16
#define L_in2 17
#define R_in1 18
#define R_in2 19
#define R_enable HIGH
#define L_enable HIGH

//Parameters
String request ;
char* ssid  = "**********";
char* password  = "**********";

String nom  = "ESP32";
//Objects
WiFiServer server(80);
WiFiClient client;
void setup() {
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //Init ESP32Wifi
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Connect to Wifi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500); Serial.print(F("."));
  }
  server.begin();
  Serial.println();
  Serial.println(F("ESP32Wifi initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
  pinMode(LED, OUTPUT);
  pinMode(L_in1, OUTPUT);
  pinMode(L_in2, OUTPUT);
  pinMode(R_in1, OUTPUT);
  pinMode(R_in2, OUTPUT);
}
void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        handleRequest(request);
      }
      webpage(client);//Return webpage
      break;
    }
  }
}

void forward(){
  // Left
  digitalWrite(L_in1, HIGH);
  digitalWrite(L_in2, LOW);
  // Right
  digitalWrite(R_in1, HIGH);
  digitalWrite(R_in2, LOW);
}
void back(){
  // Left
  digitalWrite(L_in1, LOW);
  digitalWrite(L_in2, HIGH);
  // Right
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2, HIGH);
}
void left(){
  // Left
  digitalWrite(L_in1, LOW);
  digitalWrite(L_in2, HIGH);
  // Right
  digitalWrite(R_in1, HIGH);
  digitalWrite(R_in2, LOW);
}

void right(){
  // Left
  digitalWrite(L_in1, HIGH);
  digitalWrite(L_in2, LOW);
  // Right
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2, HIGH);  
}

void halt(){
  // Left
  digitalWrite(L_in1, LOW);
  digitalWrite(L_in2, LOW);
  // Right
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2, LOW);
}
void handleRequest(String request) { /* function handleRequest */
  ////Handle web client request
  String pwmCmd;
  //Digital Ouputs
  if (request.indexOf("/dig0on") > 0) {
    digitalWrite(LED, HIGH);
  }
  if (request.indexOf("/dig0off") > 0) {
    digitalWrite(LED, LOW);
  }
  if (request.indexOf("/forward") > 0){
    forward();
    delay(500);
  }
  if (request.indexOf("/back") > 0){
    back();
    delay(500);
  }
  
  if (request.indexOf("/left") > 0){
    left();
    delay(500);
  }
  
  if (request.indexOf("/right") > 0){
    right();
    delay(500);
  }
  if (request.indexOf("/stop") > 0){
    halt();
    delay(500);
  }
  if (request.indexOf("/rectangle") > 0){
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(2500);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(2500);
    //Right Turn
    right();
    delay(1000);
    //Stop
    halt();
    delay(500);
  }
  if (request.indexOf("/square") > 0){
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Forward
    forward();
    delay(5000);
    //Right Turn
    right();
    delay(1000);
    //Stop
    halt();
    delay(500);
  }
  
}
void webpage(WiFiClient client) { /* function webpage */
  ////Send webpage to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title> UGV_Wifi_Navigation </title>");
  client.println("<meta http-equiv='content-type' content='text/html; charset=UTF-8'>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("<meta http-equiv='refresh' content='5'>");
  client.println("</head>");
  client.println("<body bgcolor = '#0001230'> ");
  client.println("<hr/><hr>");
  client.println("<h1 style='color : #3AAA35;'><center> SCV UGV - Web Controller </center></h1>");
  client.println("<hr/><hr>");
  client.println("<br><br>");
  client.println("<br><center><h2 style='color : #3AAA35;'> LED </h2></center><center>");
  client.println("  <a href='/dig0on'><button>Turn On </button></a>");
  client.println("  <a href='/dig0off'><button>Turn Off </button></a><br /> ");
  client.println("  </center><center>");
  client.println("<table>");
  client.println("<tr>");
  client.println("</tr>");
  client.println("<br><h2 style='color : #3AAA35;'> Navigation </h2><center>");
  client.println("  <a href='/forward'><button>Forward </button></a><br><br>");
  client.println("  <a href='/left'><button>Left</button></a>&ensp ");
  client.println("  <a href='/stop'><button>Stop</button></a>&ensp ");
  client.println("  <a href='/right'><button>Right </button></a> <br>  <br>"); 
  client.println("  <a href='/back'><button>Back </button></a><br > ");
  client.println("<br><h2 style='color : #3AAA35;'> Fixed Path Control </h2><center>");
  client.println("  <a href='/circle'><button>Circle</button></a>&ensp ");
  client.println("  <a href='/rectangle'><button>Rectangle</button></a>&ensp ");
  client.println("  <a href='/square'><button>Square </button></a> <br>  <br>"); 
  client.println("</table></center></body></html>");  
  delay(1);
}
