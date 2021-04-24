//ALL THE HEADER FILES AND LIBERARY ARE INCLUDED HERE

#include <Arduino.h>
#include "time.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "ESP32_MailClient.h"
#include <WiFi.h>
#include <WebServer.h>

//ALL THE CONSTANT 
const char *SSID = "Megha";
const char *PWD = "94609460";

#define GMAIL_SMTP_SEVER "smtp.gmail.com"
#define GMAIL_SMTP_USERNAME "turugoriya@gmail.com"
#define GMAIL_SMTP_PASSWORD "Megha_Goriya"
#define GMAIL_SMTP_PORT 465

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 16200;
const int   daylightOffset_sec = 3600;
float BPM, bodytemperature;

OneWire oneWire(21);
DallasTemperature tempSensor(&oneWire);
int PulseSensorPurplePin = 34;        
int Signal;                
int Threshold = 3450;

// EMail Data
SMTPData data;


//web serever
// Object of WebServer(HTTP port, 80 is defult)
WebServer server(80); 


// METHOD TO CONNECT WIFI
void connectToWiFi() {
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}


// METHOD TO SEND MAIL
String sendEmail(char *subject, char *sender, char *body, char *recipient, boolean htmlFormat) {
  data.setLogin(GMAIL_SMTP_SEVER, GMAIL_SMTP_PORT, GMAIL_SMTP_USERNAME, GMAIL_SMTP_PASSWORD);
  data.setSender(sender, GMAIL_SMTP_USERNAME);
  data.setSubject(subject);
  data.setMessage(body, htmlFormat);
  data.addRecipient(recipient);
  if (!MailClient.sendMail(data))
    return MailClient.smtpErrorReason();

  return "";
}


// METHOD TO PRINT LOCAL TIME
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, " %H:%M:%S");
}



void setup(void)
{

 // Serial.println("LABEL,temperature,pulse,time");
  Serial.begin(115200);
 
  connectToWiFi();

  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
  server.on("/", handle_OnConnect);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);

  tempSensor.begin();

}

void loop(void)
{
  tempSensor.requestTemperaturesByIndex(0);
  Serial.print(tempSensor.getTempCByIndex(0));


  Serial.print(",");
  Signal = analogRead(PulseSensorPurplePin);
  Serial.print(Signal);

  Serial.print(",");

 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 printLocalTime();


if(Signal > 5000 || Signal< 10  )
{
  connectToWiFi();
  // Send plain email
  char *body = "<b>Patient pulse rate is not normal</b><br><p>It is suggested to call a doctor </p>";
String result = sendEmail("ALERT!!! ALERT!!! ALERT!!!", "Patient Monitoring System", body, "meghagoriya067@gmail.com", true);
}

if(tempSensor.getTempCByIndex(0) < 34 || tempSensor.getTempCByIndex(0) > 37)
{
   connectToWiFi();
  // Send plain email
  char *body = "<b>Patient temperature is not normal</b><br><p>It is suggested to call a doctor</p>";
String result = sendEmail("ALERT!!! ALERT!!! ALERT!!!", "Patient Monitoring System", body, "meghagoriya067@gmail.com", true);
}

delay(15000);


//Web Server
server.handleClient();
}


void handle_OnConnect() {

  bodytemperature = tempSensor.getTempCByIndex(0);
  BPM = analogRead(PulseSensorPurplePin);
  server.send(200, "text/html", SendHTML(BPM,bodytemperature));
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}



// HTML & CSS contents which display on web server
String SendHTML(float BPM,float bodytemperature){
  String ptr = "<!DOCTYPE html>";
  ptr +="<html>";
  ptr +="<head>";
  ptr +="<title>ESP32 Patient Health Monitoring</title>";
  ptr +="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr +="<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr +="<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>";
  ptr +="<link rel='preconnect' href='https://fonts.gstatic.com'>";
  ptr +="<link href='https://fonts.googleapis.com/css2?family=Headland+One&display=swap' rel='stylesheet'>";
  ptr +="<style>";
  ptr +="html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #ffffff;}";
  ptr +="body{margin: 0px; padding: 5%; background: rgb(0,0,0); background: linear-gradient(0deg, rgba(0,0,0,1) 2%, rgba(7,13,74,1) 72%); height: 100vh;} ";
  ptr +="h1 {margin: 50px auto 30px; font-family: 'Headland One', serif;} ";
  ptr +=".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr +=".text{font-weight: 600;font-size: 25px;width: 200px;}";
  ptr +=".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr +=".BPM .reading{color: #ffffff;}";
  ptr +=".bodytemperature .reading{color: #ffffff;}";
  ptr +=".superscript{font-size: 20px;font-weight: 600;position: absolute;top: 10px;}";
  ptr +=".data{padding: 10px;}";
  ptr +=".container{display: table;margin: 0 auto;}";
  ptr +=".icon{width:65px;font-size: 3.0rem;}";
  ptr +="</style>";
  ptr +="</head>";
  ptr +="<body>";
  ptr +="<h1>Patient Health Monitoring</h1>";
  ptr +="<div class='container'>";

  ptr +="<div class='data Heart Rate'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<i class='fas fa-heartbeat' style='color: rgb(255, 0, 0);'>";
  ptr +="</i>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Pulse Rate</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=(int)BPM;
  //ptr +="<span class='superscript'></span></div>";
  ptr +="</div>";


  ptr +="<div class='data Body Temperature'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<i class='fas fa-thermometer-half' style='color: rgb(0, 255, 242);'>";
  ptr +="</i>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Body Temperature</div>";
  ptr +="<div class='side-by-side reading'>";
  ptr +=(int)bodytemperature;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="</div>";

  ptr +="</div>";
  ptr +="</body>";
  ptr +="</html>";
  return ptr;
}
