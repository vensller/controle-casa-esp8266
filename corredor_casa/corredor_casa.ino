#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//Configuração do WIFI
const char* ssid = "Ivens 2.4G";
const char* password = "ID@X197Z";

//Configurações horário
static const char ntpServerName[] = "pool.ntp.br";
const int timeZone = -3; // Central European Time
unsigned int localPort = 8888; 
WiFiUDP Udp;
time_t prevDisplay = 0; 
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

ESP8266WebServer server(80);

// sensores de movimento
const int pinoPIR1 = D1;
const int pinoPIR2 = D2;
const int fim = 06;
const int inicio = 19;

// Relé
const int pinoRELE1 = D5;

// Configuração da lampada
int tempoLigado = 10; //minutos


// Comandos socket
const String ligar = "ligar";
const String desligar = "desligar";
const String mostrarLogs = "mostrarLogs";
const String reiniciar = "reiniciar";

// Variaveis controle
bool ligadoServidor = false;
bool ligadoSensor = false;
String logs = "";

void logInfo(String message) {
  Serial.println(message);
  logs += message + "\n";
}

void setup() {
  // put your setup code here, to run once:
  pinMode(pinoPIR1, INPUT);
  pinMode(pinoPIR2, INPUT);

  pinMode(pinoRELE1, OUTPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  server.on("/", verificarRequest); 
  server.begin();

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  digitalWrite(pinoRELE1, HIGH);  
}

void ligarLampadas() {
  digitalWrite(pinoRELE1, LOW);
  ligadoSensor = true;
}

void desligarLampadas() {
  digitalWrite(pinoRELE1, HIGH);
  ligadoSensor = false;
}

void verificarParaLigar(int valor1, int valor2) {
  if (ligadoSensor || ligadoServidor) {
    return;
  }
  
  if (valor1 == 1) {
    logInfo("Ativado pelo Sensor 1");
    ligarLampadas();
  }  
  
  if (valor2 == 1) {
    logInfo("Ativado pelo Sensor 2");
    ligarLampadas();
  }    
}

void verificarParaDesligar(int valor1, int valor2) {
  if (!ligadoSensor) {
    return;
  }
  
  if ((valor1 == 0) && (valor2 == 0)) {
    logInfo("Sem movimento, desligando lampadas");
    desligarLampadas();
  }
}

void verificarSensores() {
  int hora = hour();
  
  if (hora < inicio && hora >= fim) {
    return;
  }
  
  if (!ligadoServidor) {
    int valorSensor1 = digitalRead(pinoPIR1);  
    int valorSensor2 = digitalRead(pinoPIR2);

    verificarParaLigar(valorSensor1, valorSensor2);
    verificarParaDesligar(valorSensor1, valorSensor2);
  }
}

void verificarRequest() {
  if (server.hasArg("plain")== false){ //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }
  
  String request = server.arg("plain");  

  Serial.println(request);

  if (request == mostrarLogs) {
    server.send(200, "text/plain", logs);    
    return;
  }

  if (request == reiniciar) {
    server.send(200, "text/plain", "OK");       
    delay(1000); 
    ESP.restart();
    return;
  }

  if (request == ligar) {    
    logInfo("Ligado pelo server");
    digitalWrite(pinoRELE1, LOW);
    ligadoServidor = true;        
  }

  if (request == desligar) {
    logInfo("Desligado pelo server");
    digitalWrite(pinoRELE1, HIGH);
    ligadoServidor = false;        
  }

  server.send(200, "text/plain", "OK");  
}

void loop() {
//  if (timeStatus() != timeNotSet) {
//    if (now() != prevDisplay) { //update the display only if time has changed
//      prevDisplay = now();
//      digitalClockDisplay();
//    }
//  }
  
  verificarSensores();
  
  server.handleClient();  
  ArduinoOTA.handle();
  delay(100);
}

/*-------- NTP code ----------*/
void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay()
{
  // digital clock display of the timep
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
