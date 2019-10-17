 /*
 * IOT 중간고사 과제
 * First commit: 2019-10-14
 * Author: 2019-13674 양현서
 * github link: https://github.com/KYHSGeekCode/SNU_IAB_IOT_Midterm_DiscoveryRobot
 * License: Apache License 2.0 (LGPL for WebSockets.h, etc..)
 * 
 * Part of this file is taken from Mudassar Tamboli's code.
 * OV7670을 제어하여 웹으로 스트리밍하는 파트의 코드는 Mudassar Tamoli가 작성한 것이며
 * Apache 라이선스로 배포되었습니다.
 * https://github.com/mudassar-tamboli/ESP32-OV7670-WebSocket-Camera/
 * 양현서(KYHSGeekCode)가 수정함 (디버그&모터 조종 기능 확장)
 */

#include "OV7670.h"

#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <WebSocketsServer.h>
//#include "WiFi.h// error: multiple libraries found://
  #include <WiFi.h>
  #include <esp_wifi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include "canvas_htm.h"

const char *ap_ssid     = "Esp32AP";
const char *ap_password = "thereisnospoon";

const char *ssid_AP_1 = "U+Net9ED3";
const char *pwd_AP_1  = "6000160842";

const char *ssid_AP_2 = "Engineering";
const char *pwd_AP_2  = "snuiab123";

const char *ssid_AP_3 = "KT_SNU";
const char *pwd_AP_3  = NULL;

const char * ssid = ssid_AP_1;
const char * password = pwd_AP_1;

const int SIOD = 21; //SDA
const int SIOC = 22; //SCL

const int VSYNC = 34;
const int HREF = 35;

const int XCLK = 32;
const int PCLK = 33;
//1716 -> 36 39
const int D0 = 27;////2;///24;//27;  10 안됨 파랑  A17 GPIO27
const int D1 = 26;////27;//17; 빨강 그냥 17
const int D2 = 4;////26;//16;  초록색 그냥 16
const int D3 = 39;////25;//15;  9안됨 91011 -> 0 2 15 갈색 15그냥
const int D4 = 14;    //노랑 A16  GPIO14
const int D5 = 13;////13;   11안됨 회색 A14 GPIO13
const int D6 = 12;////12;    빨강 A15 GPIO12
const int D7 = 36;////15;///4; 주황 그냥 4번핀


const int motor1PwmChannel = 293;
const int motor2PwmChannel = 294;
//새거 23, 8
const int motor1A = 17;//23;//39;//23;//39;       //흰선    //left motor input 1
const int motor1B =  16;//25; //36;//8;//36;//25;               //left motor input 2 DAC1
const int motor1Enable = -1;//36; //파란선  //left motor enable
//받은거
const int motor2A =  19;//5;               //right motor Input 3 VSPI SS
const int motor2B = 18;               //right motor Input 4 VSPI SCK
const int motor2Enable = -1;//19;  //31 30 29 검갈파 en A B //rignt motor enable VSPI MISO

#define BACK -1
#define STOP 0
#define STRAIGHT 1
#define TURNRIGHT 2
#define TURNLEFT 3


//const int TFT_DC = 2;
//const int TFT_CS = 5;
//DIN <- MOSI 23
//CLK <- SCK 18

#define ssid1        "YOUR_WIFI_SSID"
#define password1    "YOUR_PASSWORD"
//#define ssid2        ""
//#define password2    ""

OV7670 *camera;

WiFiMulti wifiMulti;
WiFiServer server(80);

unsigned char pix = 0;

//unsigned char bmpHeader[BMP::headerSize];

unsigned char start_flag = 0xAA;
unsigned char end_flag = 0xFF;
unsigned char ip_flag = 0x11;

WebSocketsServer webSocket(81);    // create a websocket server on port 81

//이벤트 핸들러를 등록하고 웹소켓을 시작한다.
void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

//웹서버를 시작한다.
void startWebServer()
{
   server.begin();
   Serial.println("Http web server started.");
}
// 스트리밍 웹서버의 본체
void serve() {
  WiFiClient client = server.available();
  if (client) 
  {
    //Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        //Serial.write(c);
        if (c == '\n') 
        {
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(canvas_htm);     // canvas_htm.h에 있음
            client.println();
            break;
          } 
          else 
          {
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {
          currentLine += c;
        }
        
      }
    }
    // close the connection:
    client.stop();

  }  
}

//웹소켓의 이벤트 핸들러
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t payloadlength) { // When a WebSocket message is received
 
  int blk_count = 0;

  char canvas_VGA[] = "canvas-VGA";
  char canvas_Q_VGA[] = "canvas-Q-VGA";
  char canvas_QQ_VGA[] = "canvas-QQ-VGA";
  char canvas_QQQ_VGA[] = "canvas-QQQ-VGA";

  char MOTOR_TURNRIGHT[] = "motor_turnright";
  char MOTOR_TURNLEFT[] = "motor_turnleft";
  char MOTOR_STRAIGHT[] = "motor_straight";
  char MOTOR_STOP[] = "motor_stop";
  char MOTOR_BACK[] = "motor_back";
  
  char ipaddr[26] ;
  IPAddress localip;
  
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
           webSocket.sendBIN(0, &ip_flag, 1);
           localip = WiFi.localIP();
           sprintf(ipaddr, "%d.%d.%d.%d", localip[0], localip[1], localip[2], localip[3]);
           webSocket.sendTXT(0, (const char *)ipaddr);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
    if(payload[0]!='n'){
      Serial.print("payload:");
      Serial.printf("%100s",payload);
      Serial.println();
    }
    if ((payloadlength == sizeof(MOTOR_STRAIGHT)-1) && (memcmp(MOTOR_STRAIGHT, payload, payloadlength) == 0)) {
              Serial.printf("Go straight");
              //webSocket.sendBIN(0, &end_flag, 1);
              StartMove(STRAIGHT);
              return;
        }
     else if ((payloadlength == sizeof(MOTOR_TURNLEFT)-1) &&(memcmp(MOTOR_TURNLEFT, payload, payloadlength) == 0)) {
              Serial.printf("Turn left");
              //webSocket.sendBIN(0, &end_flag, 1);
              StartMove(TURNLEFT);
              return;
        }
      else if ((payloadlength == sizeof(MOTOR_TURNRIGHT)-1) && (memcmp(MOTOR_TURNRIGHT, payload, payloadlength) == 0)) {
              Serial.printf("Turn right");
              //webSocket.sendBIN(0, &end_flag, 1);
              StartMove(TURNRIGHT);
              return;
        }
      else if ((payloadlength == sizeof(MOTOR_STOP)-1) && (memcmp(MOTOR_STOP, payload, payloadlength) == 0)) {
              Serial.printf("Stop motors");
              //webSocket.sendBIN(0, &end_flag, 1);
              StartMove(STOP);
              return;
        }
      else if ((payloadlength == sizeof(MOTOR_BACK)-1) && (memcmp(MOTOR_BACK, payload, payloadlength) == 0)) {
              Serial.printf("Move Backward");
              //webSocket.sendBIN(0, &end_flag, 1);
              StartMove(BACK);
              return;
        }
      //이하 if nest는 페이로드 길이가 다 다를 때만 사용해야 한다. 안 그러면 버그 생긴다.
      else if (payloadlength == sizeof(canvas_QQQ_VGA)-1) {
        if (memcmp(canvas_QQQ_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_QQQ_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              camera = new OV7670(OV7670::Mode::QQQVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_QQ_VGA)-1) {
        if (memcmp(canvas_QQ_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_QQ_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              camera = new OV7670(OV7670::Mode::QQVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_Q_VGA)-1) {
        if (memcmp(canvas_Q_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_Q_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              camera = new OV7670(OV7670::Mode::QVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_VGA)-1) {
        if (memcmp(canvas_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              camera = new OV7670(OV7670::Mode::VGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      }

      blk_count = camera->yres/I2SCamera::blockSlice;//30, 60, 120
      for (int i=0; i<blk_count; i++) {

          if (i == 0) {
              camera->startBlock = 1;
              camera->endBlock = I2SCamera::blockSlice;
              webSocket.sendBIN(0, &start_flag, 1);
          }

          if (i == blk_count-1) {
              webSocket.sendBIN(0, &end_flag, 1);
          }
        
          camera->oneFrame();
          webSocket.sendBIN(0, camera->frame, camera->xres * I2SCamera::blockSlice * 2);
          camera->startBlock += I2SCamera::blockSlice;
          camera->endBlock   += I2SCamera::blockSlice;
      }
      
      break;
    case WStype_ERROR:                     // if new text data is received
      Serial.printf("Error \n");
    default:
      Serial.printf("WStype %x not handled \n", type);

  }
}

//모터를 제어하여 움직이는 함수
//오른쪽 바퀴 : Out3, out4
// 2A, 2B를 High low: 오른쪽 바퀴 뒤로
// 1A, 1B를 컨트롤 하려면
void StartMove(int action)
{
  Serial.print("Move action=");
  Serial.println(action);
  switch(action)
  {
    case STOP:
     //ledcWrite(motor1PwmChannel, 0);
     //ledcWrite(motor2PwmChannel, 0);
     //digitalWrite(motor1Enable,LOW);
     //digitalWrite(motor2Enable,LOW);   
     digitalWrite(motor1A,LOW);
     digitalWrite(motor1B,LOW);
     digitalWrite(motor2A,LOW);
     digitalWrite(motor2B,LOW);
     break;
    case STRAIGHT: //오른쪽 바퀴는  앞으로 가려 한다. 왼쪽 바퀴는 앞으로 가려한다.
    //ledcWrite(motor1PwmChannel, 250);
    //ledcWrite(motor2PwmChannel, 250);
    // digitalWrite(motor1Enable,HIGH);
    // digitalWrite(motor2Enable,HIGH);
     digitalWrite(motor1A,HIGH);
     digitalWrite(motor1B,LOW);
     digitalWrite(motor2A,LOW);
     digitalWrite(motor2B,HIGH);
     break;
    case TURNRIGHT: // 양쪽 다 앞으로 가려 한다.
     //ledcWrite(motor1PwmChannel, 250);
     //ledcWrite(motor2PwmChannel, 250);
    //digitalWrite(motor1Enable,HIGH);
    // digitalWrite(motor2Enable,HIGH); 
     digitalWrite(motor1A,HIGH);
     digitalWrite(motor1B,HIGH);
     digitalWrite(motor2A,LOW);
     digitalWrite(motor2B,HIGH);
     break;
    case TURNLEFT:  //오른쪽 바퀴만 뒤로 가려 한다.
    //ledcWrite(motor1PwmChannel, 250);
     //ledcWrite(motor2PwmChannel, 250);
    // digitalWrite(motor1Enable,HIGH);
    // digitalWrite(motor2Enable,HIGH);
     digitalWrite(motor1A,HIGH);
     digitalWrite(motor1B,LOW);
     digitalWrite(motor2A,HIGH);
     digitalWrite(motor2B,LOW);
     break;
    case BACK:  //오른쪽 바퀴만 뒤로 가려 한다. 왼쪽 바퀴는 앞으로 가려 한다.
      //LOW LOW는 정지시킨다.
      //LOW HIGH는 정지시킨다.
      //HIGH LOW는 앞으로 가게 한다.
      //HIGH HIGH는 앞으로 가게 한다.
    ///ledcWrite(motor1PwmChannel, 250);
     //ledcWrite(motor2PwmChannel, 250);
    // digitalWrite(motor1Enable,HIGH);
    // digitalWrite(motor2Enable,HIGH);
     digitalWrite(motor1A,LOW);
     digitalWrite(motor1B,HIGH);
     digitalWrite(motor2A,HIGH);
     digitalWrite(motor2B,LOW);
     break;
  }
}
/*
void initWifiStation() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);    
    Serial.print("\nConnecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
       delay(5000);        
       Serial.print(".");
    }
    Serial.println(String("\nConnected to the WiFi network (") + ssid + ")" );

    Serial.print("\nStation IP address: ");
    Serial.println(WiFi.localIP()); 

}
*/
void initWifiMulti()
{
    wifiMulti.addAP(ssid_AP_1, pwd_AP_1);
    wifiMulti.addAP(ssid_AP_2, pwd_AP_2);
    wifiMulti.addAP(ssid_AP_3, pwd_AP_3);

    Serial.println("Connecting Wifi...");
    while(wifiMulti.run() != WL_CONNECTED) {
       delay(5000);        
       Serial.print(".");
    }
    
    Serial.print("\n");
    Serial.print("WiFi connected : ");
    Serial.print("IP address : ");
    Serial.println(WiFi.localIP());
}

void initWifiAP() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void initMotors() {

  //pinMode(motor1Enable,OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  //pinMode(motor2Enable,OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
   const int freq = 30000;
  const int resolution = 8;
  ///ledcSetup(motor1PwmChannel, freq, resolution);
 // ledcSetup(motor2PwmChannel, freq, resolution);
  
  //ledcAttachPin(motor1Enable, motor1PwmChannel);
  //ledcAttachPin(motor2Enable, motor2PwmChannel);
  
  //ledcWrite(motor1PwmChannel, 0);
  //ledcWrite(motor2PwmChannel, 0);
 // digitalWrite(motor1Enable,LOW);
 // digitalWrite(motor2Enable,LOW);
  digitalWrite(motor1A,LOW);
  digitalWrite(motor1B,LOW);
  digitalWrite(motor2A,LOW);
  digitalWrite(motor2B,LOW);
}

void setup() {
  Serial.begin(115200);
  initMotors();
  esp_wifi_set_ps(WIFI_PS_NONE);
  initWifiMulti();
  initWifiAP();
  startWebSocket();
  startWebServer();
}

void loop()
{
  webSocket.loop();
  serve();
}
