#include "wifistatus.h"
#include "escanear.h"
#include "pagina.h"
#include <WebServer.h>

const char *ssidap = "Naceli"; //Datos para generar el AP
const char *passap = "NaceliHome#";

String redselec="";//Variables donde se almacenará la red y password a probar
String passelec="";


WebServer server(80);

bool checkpass(String ssid, String pass){ //Funcion que verificará si la contraseña y red wifi son correctas y conectan con el AP
  ssid.toCharArray(chared, ssid.length()+1);
  pass.toCharArray(charpass, pass.length()+1);
  WiFi.begin(chared, charpass);
  unsigned long timpopas=millis(); //Utilizando millis en lugar de delay
  while ((WiFi.status() != WL_CONNECTED)){
    if (millis() - timpopas >= 10000){//Si no se conecta en 10 Seg
      return false; 
    }
  }
    return true;  
}

void webpage(){//Funcion que determina las respuestas que dará el servidor (paginas enviadas)
    String pagesend;
  if (server.hasArg("ssid")){//Si la informacion recivida contiene un ssid
    redselec=String(server.arg("ssid")); //Guarda en un String el valor de ssid recibido por botón o texto
    pagesend = password1+redselec+password2;
    server.send(200, "text/html", pagesend);//Envia la pagina para introducir contraseña
  }
  if (server.hasArg("pass")){ //Si la informacion recibida contiene un password
    passelec=String(server.arg("pass"));//Almacena en un string el valor de password recibida
    if (checkpass(redselec, passelec)){//Verifica si los datos (ssid y pass) son correctos y conectan a la red
      savered(0,redselec);
      savered(50, passelec);//Si los valores son correctos los almacena en eeprom
      ESP.restart();//Reiniciamos el ESP
    }
    else{
      ESP.restart();//Reiniciamos el ESP sin guardar nada para reiniciar el proceso
    }
  }
  else{//Si no se reciben datos de ssid o password enviará la primer pagina para iniciar el proceso
    pagesend = Index1+escanear()+Index2;
    server.send(200, "text/html", pagesend);//Envia la pagina principal (Index) y los botones con las redes detectadas
  }
}

void createap(){//Crea un AP para configurar
    WiFi.softAP(ssidap, passap);//Establece el ssid y contraseña del AP
    server.on("/", webpage);//Si se recibe la solicitud "/" se ejecuta el administrador de paginas
    server.begin();//Inicia el servidor
  while (!wifistate()){//Mientras no se conecte a la red permanece a la espera de clientes
    server.handleClient();
    delay(100);
  }
}
