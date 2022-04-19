String escanear(){//Funcion que regresará el nombre de las redes escaneadas en forma de botones HTML
  int n=WiFi.scanNetworks();//n Almacena el numero de redes detectadas
  if (n==0){
    return "<font size='3'>No se encontraron redes WiFi</font> <br>";
  }
  String redes="";
  if (n > 9){
    n=9;
  }
  for (int i = 0; i<n; ++i){
    redes += "<form action='' method='post'><input type='submit' name='ssid' value='"+WiFi.SSID(i)+"'/></form><br>";
  }
  return redes;
}
