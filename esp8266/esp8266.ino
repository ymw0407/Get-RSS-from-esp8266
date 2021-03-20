#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Your_ssid";
const char* password = "your_password";
const String endpoint = "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=1141071000";
String line = "";

IPAddress ip (your_own); // 연결할, 고정 IP 주소
IPAddress gateway (your_own); // 게이트웨이 주소
IPAddress subnet (your_own); // 서브마스크 주소
IPAddress dns (your_own); // DNS 주소
int ledPin = D5;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.config (ip, dns ,gateway, subnet);
  WiFi.begin(ssid, password);
 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  get_weather();


}

void get_weather() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    Serial.println("Starting connection to server...");
    HTTPClient http;
    http.begin(endpoint);       //Specify the URL
    int httpCode = http.GET();  //Make the request
    if (httpCode > 0) {         //Check for the returning code
      line = http.getString();
    }
    else {
      Serial.println("Error on HTTP request");
    }
//    Serial.println(line); // 수신한 날씨 정보 시리얼 모니터 출력
    parsing();
    http.end(); //Free the resources
  }
}

// 온도 저장 변수
float temp0;
float temp1;
float temp2;
String wfEn;
String s_temp0 = String(temp0);
String s_temp1 = String(temp1);
String s_temp2 = String(temp2);


void parsing() {
  String announce_time;  
  int tm_start= line.indexOf(F("<tm>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int tm_end= line.indexOf(F("</tm>"));  
  announce_time = line.substring(tm_start + 4, tm_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("announce_time: ")); Serial.println(announce_time);
  String hour;
  int hour_start= line.indexOf(F("<hour>"));
  int hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  String temp;
  int temp_start= line.indexOf(F("<temp>"));
  int temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp0 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp0: ")); Serial.println(temp0);
 
  int wfEn_start= line.indexOf(F("<wfEn>"));
  int wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  int del_index = line.indexOf(F("</data>")); 
  line.remove(0, del_index + 7);                     // 시작 인덱스 부터 "</data>" 스트링 포함 삭제
  hour_start= line.indexOf(F("<hour>"));
  hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  temp_start= line.indexOf(F("<temp>"));
  temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp1 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp1: ")); Serial.println(temp1);
  wfEn_start= line.indexOf(F("<wfEn>"));
  wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  del_index = line.indexOf(F("</data>"));
  line.remove(0, del_index + 7);                     // 시작 인덱스 부터 "</data>" 스트링 포함 삭제
  hour_start= line.indexOf(F("<hour>"));
  hour_end= line.indexOf(F("</hour>"));
  hour = line.substring(hour_start + 6, hour_end);
  Serial.print(F("hour: ")); Serial.println(hour);
  temp_start= line.indexOf(F("<temp>"));
  temp_end= line.indexOf(F("</temp>"));
  temp = line.substring(temp_start + 6, temp_end);
  Serial.print(F("temp: ")); Serial.println(temp);
  temp1 = temp.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("temp2: ")); Serial.println(temp1);
  wfEn_start= line.indexOf(F("<wfEn>"));
  wfEn_end= line.indexOf(F("</wfEn>"));
  wfEn = line.substring(wfEn_start + 6, wfEn_end);
  Serial.print(F("weather: ")); Serial.println(wfEn);
  line = ""; // 스트링 변수 line 데이터 추출 완료 
}


void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 

  // Return the response
  client.println("<title>IoT</title>");;
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<html lang='ko'>");
  
  client.println("<head>");
  
  client.println("<meta charset='utf-8'>");
  client.println("<title>IoT</title>"); 
  
  client.println("<style>");

  client.println("@import url('https://fonts.googleapis.com/css2?family=Nanum+Myeongjo&family=Nanum+Pen+Script&display=swap');");
  client.println(".font {font-family: 'Nanum Pen Script'}");
  client.println(".footer-font {font-size: 15px}");
  client.println(".font-sizeup {font-family: 'Nanum Pen Script'; font-size: 23px}");
  
  client.println("p {font-size: 20px}");
  client.println("html, body {height: 100%; margin: 0;}");
  client.println("div {height: 100%; margin: 0;}");
  client.println("header {background-color: rgb(196, 255, 150); padding: 10px}");
  client.println("footer {border:1px solid Black; height: 100px;}");
  client.println("button {width:120px; height: 40px; color:#fff; background: rgb(196, 163, 255); font-size: 16px; border:none; border-radius: 20px; box-shadow: 0 4px 16px rgba(196, 163, 255, 0.3); transition: 0.3s; }");
  client.println("button:focus{outline:0;}");
  client.println("button:hover{background-color: rgba(196, 163, 255, 0.9); cursor: pointer; box-shadow: 0 2px 4px rgba(196, 163, 255, 0.6);}");
  
  client.println(".container {background-color:#F0F0F0; width:960px; margin:0 auto; padding:10px;}");
  client.println(".content {background-color:#F5F5F5; float:right; width:660px; height:400px; text-align: center;}");
  client.println(".sideinfo { background-color:#DCDAD9; float:left; width:300px; height:400px; }");
  client.println(".position {position: absolute; left: 35%; top: 50%}");
  
  client.println("</style>"); 

  client.println("</head>");
  
  client.println("<body>");
  client.println("<div class='container'>");
  client.println("<header class='font'><h1>Internet of Things</h1><h3>21214 윤민우</h3></header>");
  client.println("<div class='sideinfo'><p class='font'>&nbsp;&nbsp;현재 상태 : </p>"); if(value == HIGH) {client.print("<p class='font'>&nbsp;&nbsp;&nbsp;&nbsp;On</p>");} else {client.print("<p class='font'>&nbsp;&nbsp;&nbsp;&nbsp;Off</p>");}
  client.print ("<span class='font'>&nbsp;&nbsp온도 :&nbsp;&nbsp</span>" );
  client.print (temp1);
  client.print ("<span class='font'>℃</span><br /><br />");
  client.print ("<span class='font'>&nbsp;&nbsp날씨 :&nbsp;&nbsp</span>" );
  client.print (wfEn);
  client.print ("<br /></div>");

  client.println("<div class='content', 'postion'>");
  client.println("<br /><br /><br/><br /><p class='font'>릴레이 : <a href=\"/LED=ON\"><button class='font-sizeup'>ON</button></a></p>");
  client.println("<br /><br /><p class='font'>릴레이 : <a href=\"/LED=OFF\"><button class='font-sizeup'>OFF</button></a></p>");
  client.println("</div>");
  client.println("<footer><h5>© copyright 2020 by 윤민우</h5><p class='footer-font'>It's the practice of esp8266 + html, css</p></footer>");
  client.println("</div>");
  client.println("</body>");
  client.println("</html>");
  client.println("");

  

 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}
