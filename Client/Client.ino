#include <Arduino.h>
#include <ArduinoJson.h>
 
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include<SoftwareSerial.h>

SoftwareSerial mySUART(4, 5); 
 
WebSocketsClient webSocket;
DynamicJsonDocument doc(1024);
 
const char *ssid   = "POCO X3 Pro";
const char *password = "12345678";
const int puerto = 8001;
char* host = "192.168.118.87"; //pc
String texto = "";
 
unsigned long messageInterval = 5000;
bool connected = false;
 
#define DEBUG_SERIAL Serial
 
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    DynamicJsonDocument doc(1024);
    int tipo;
    const char* mensaje;

    switch(type) {
        case WStype_DISCONNECTED:
            DEBUG_SERIAL.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
 
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
            webSocket.sendTXT("Connected");
        }
            break;
        case WStype_TEXT:
            deserializeJson(doc, payload);
            tipo = doc["type"];
            Serial.print("[WSc] tipo: ");
            Serial.println(tipo);
            Serial.print("[WSc] tipo: ");
            Serial.println(tipo);
            switch (tipo)
            {
                case -1:
                    Serial.printf("[WSc] ERROR: ",mensaje);
                    break;
                case 0:
                    Serial.printf("[WSc] SERIAL ASIGNADO DE FORMA EXITOSA: ",mensaje);
                    break;
                case 1:
                    Serial.printf("[WSc] RESPUESTA: ",mensaje);
                    break;
            }
            break;
        case WStype_BIN:
            DEBUG_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
                case WStype_PING:
                        // pong will be send automatically
                        DEBUG_SERIAL.printf("[WSc] get ping\n");
                        break;
                case WStype_PONG:
                        // answer to a ping we send
                        DEBUG_SERIAL.printf("[WSc] get pong\n");
                        break;
    }
 
}
 
void setup() {
    DEBUG_SERIAL.begin(115200);
 
//  DEBUG_SERIAL.setDebugOutput(true);
 
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
 
    for(uint8_t t = 4; t > 0; t--) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }
 
    WiFi.begin(ssid, password);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    DEBUG_SERIAL.print("Local IP: "); DEBUG_SERIAL.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin(host, puerto, "/");
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();

void loop() {
    webSocket.loop();
    if(Serial.available()){
        texto = Serial.readStringUntil('\n');
        Serial.println(texto);
        webSocket.sendTXT(texto);
    }
}
