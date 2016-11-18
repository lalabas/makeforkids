#include <ESP8266WiFi.h>
 
const char* ssid = "Mrspock_M1"; // 무선공유기 이름
const char* password = "12345678900"; // 무선공유기 접속 비밀번호
 
WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
   
  // 무선인터넷 접속
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // 서버 시작
  server.begin();
  Serial.println("Server started");
 
  // 무선인터넷 주소 출력
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
}
 
void loop() {
  // 무선인터넷에 연결돼었는지 확인
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // 명령을 기다림
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // 명령을 확인함
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // 명령을 실행
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED_BUILTIN, LOW);
    value = LOW;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(LED_BUILTIN,HIGH);
    value = HIGH;
  }
 
   
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<font size=30>");
   
  client.print("Led pin is now: ");
   
  if(value == LOW) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 

//무선으로 LED on/off로 해보는 예제입니다. 
