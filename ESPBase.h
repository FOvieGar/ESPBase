#include "wifiap.h"
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "naceliupdate";

void base() { //Ejecutar en el loop para mantener a la espera de conexion wifi
  while (!wifistate()){//wifistate es Bool regresará false si no se conecta a la red de la eeprom
    createap();//Crea el AccesPoint en que se configura la red wifi
  }// codigo que se ejecutará normalmente
}

void updateota(bool usar=false){
  if (usar and wifistate()){
    //WebServer server(80);
    if (!MDNS.begin(host)) {
      usar=false;
    }
    server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", updatecharge);
  });
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
  while (usar){
    server.handleClient();
  }
  }
}
