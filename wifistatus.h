#include <WiFi.h>
#include <EEPROM.h>

char chared[50];//Almacenaran los datos de la red que se uardarán en la eeprom
char charpass[50];


String red(int adr){//Funcion que leerá los datos en la epprom almacenados en la direccion dada (0 para ssid y 50 para pass)
  byte lect;
  String stred;
  for (int i=adr; i < adr+50; i++){
    lect = EEPROM.read(i);//Lectura de datos en la eeprom
    if (lect != 255){
      stred += (char)lect;    
    }
  }
  return stred;
}

bool wifistate(){ //Comprobará el estado de conexion y regresa bool para conexion correcta(true) o fallida(false)
  String ssid = red(0);
  String pass = red (50);
  ssid.toCharArray(chared, ssid.length());
  pass.toCharArray(charpass, pass.length());
  //WiFi.begin(chared, charpass);
  unsigned long timpopas=millis();
  while ((WiFi.status() != WL_CONNECTED)){
    if (millis() - timpopas >= 5000){  
      return false; 
    }
  }
  if (WiFi.status() == WL_CONNECTED){
    return true;    
  }
}

void savered(int addr, String a) {//Guarda los datos ingresados en la direccion dada en eeprom
  int tamano = a.length(); 
  char inchar[50]; 
  a.toCharArray(inchar, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, inchar[i]);
  }
  for (int i = tamano; i < 50; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}
