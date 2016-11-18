#include <ESP8266WiFi.h>
 
const char* ssid = "Mrspock_M1"; // 무선공유기 이름
const char* password = "123456"; // 무선공유기 접속 비밀번호
 
WiFiServer server(80);

const int in1 = 4; //모터제어1
const int in2 = 5; //모터제어2
const int mag = 14; // 자석센서
const int rain = 12; //레인센서
int value = 3; //기본상태 3,알수없음 

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(mag,INPUT);
  pinMode(rain,INPUT);
 
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
    if (value != 5 & value != 11) {
      if (digitalRead(rain) == LOW) {
      value = 5;
      eclose();
    }
    }

  if (digitalRead(mag) == LOW) {
    wstop();
    if (value == 0) {
      value=10;
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (value == 1) {
      value=11;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (value == 5) {
      value=5;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    delay(1000);
  }
  
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
  client.flush();
   
  // 명령을 실행
 



  
  if (request.indexOf("/OPEN") != -1) {   
    wopen();
  } 
  if (request.indexOf("/CLOSE") != -1){
    wclose();
  }
  if (request.indexOf("/STOP") != -1){
    wstop();
    if(value < 4) {
      value = 2;
    }
  }
 
   
  // Return the response
  Serial.println(request);
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><META charset=\"utf-8\"></head>");
  client.println("<font size=30>");
   
  client.print("");

   
  if(value == 10 | value == 0) {
    client.print("창문이 열렸습니다.");  
  }
  else if (value == 11 | value == 1) {
    client.print("창문이 닫혔습니다.");
  }
  else if (value == 2) {
    client.print("창문이 중간에서 멈췄습니다.");
  }
  else if (value == 3) {
    client.print("<font color=\"RED\">창문의 위치를 알 수 없습니다.</font>");
  }
  else if (value == 5) {
    client.println("<font color=\"RED\">비가와서 창문을 강제로 닫았습니다.</font>");
    }
  
  client.println("<br><br>");
  client.println("Click <a href=\"/OPEN\">here</a> 창문열기 <br>");
  client.println("Click <a href=\"/CLOSE\">here</a> 창문닫기 <br>");
  client.println("Click <a href=\"/STOP\">here</a> 멈춤 <br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}

void wopen() {
  if(value == 10) {
  }
  else {
    value = 0;
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    delay(500);
  }
}

void wclose() {
  if(value == 11 |value == 5) {
  }
  else {
    value = 1;
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    delay(500);
  }
}

void wstop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void eclose() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

