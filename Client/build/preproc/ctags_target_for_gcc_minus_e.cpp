# 1 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino"
# 2 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino" 2
# 3 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino" 2

# 5 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino" 2
# 6 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino" 2
# 7 "c:\\Users\\carmlago\\Documents\\Cesar\\Client\\Client.ino" 2

SoftwareSerial mySUART(4, 5);

WebSocketsClient webSocket;
DynamicJsonDocument doc(1024);

const char *ssid = "ASUS_X018D";
const char *password = "f07576531f17";
const int puerto = 8001;
char* host = "192.168.125.72";
String texto = "";

unsigned long messageInterval = 5000;
bool connected = false;



void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    DynamicJsonDocument doc(1024);
    int tipo;
    const char* mensaje;

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;

            // send message to server when Connected
            Serial.println("[WSc] SENT: Connected");
            webSocket.sendTXT("Connected");
        }
            break;
        case WStype_TEXT:
            deserializeJson(doc, payload);
            tipo = doc["type"];
            mensaje = doc["mesaje"];
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
            Serial.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
                case WStype_PING:
                        // pong will be send automatically
                        Serial.printf("[WSc] get ping\n");
                        break;
                case WStype_PONG:
                        // answer to a ping we send
                        Serial.printf("[WSc] get pong\n");
                        break;
    }

}

void setup() {
    Serial.begin(115200);

//  DEBUG_SERIAL.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
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
