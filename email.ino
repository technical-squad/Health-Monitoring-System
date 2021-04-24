
#include <Arduino.h>
#include "ESP32_MailClient.h"
#include <WiFi.h>
const char *SSID = "Megha";
const char *PWD = "94609460";
#define GMAIL_SMTP_SEVER "smtp.gmail.com"
#define GMAIL_SMTP_USERNAME "turugoriya@gmail.com"
#define GMAIL_SMTP_PASSWORD "Megha_Goriya"
#define GMAIL_SMTP_PORT 465  
// EMail Data
SMTPData data;
void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected - ");
//  Serial.println(WiFi.localIP);
}
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
void setup() {
  Serial.begin(9600);
  // Connect to WiFi
  connectToWiFi();
  // Send plain email 
  char *body = "<b>Hello World!</b><br><p>This is an <i>HTML email</i> body";
String result = sendEmail("Test", "ESP32", body, "meghagoriya067@gmail.com", true);
}




void loop() {
  // put your main code here, to run repeatedly:

}
