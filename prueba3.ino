//Variable donde almacenaremos el valor del potenciometro
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h> 
float valorAzufre;
float valorSulf;
float valorGas;
int DS18S20_Pin = 2;
OneWire ds(DS18S20_Pin);  // on digital pin 2
//MQ2 mq2(A5);

// Registrar la Mac para su proyecto.
// Ingrese la MAC de su shell, para obtenerla, puede acceder a: Archivo->Ejemplos->Ethernet->DhcpAdressPrinter.
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char serverName[] = "10.58.16.220";    // name address of your domain

// Esta IP se asignará en caso de que el DHCP falle en asignar la ip al servidor
IPAddress ip(192,168,0,177);
int serverPort = 80;
EthernetClient client;
int totalCount = 0;
char pageAdd[64];

char tempString[] = "00.00";

// Setea el delay en milisegundos.
// 5 segundos.
#define delayMillis 5000UL

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

//DEFINIR DATOS SENSORES DHT11
// Incluimos librería
 
void setup() {
  //Inicializamos la comunicación serial
  Serial.begin(9600);
  
  //mq2.begin();

  // Iniciar ethernet
  Serial.println(F("Conectando a ethernet..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falla en la configuración de DHCP");

    Ethernet.begin(mac, ip);
  }

  digitalWrite(10,HIGH);

  Serial.println(Ethernet.localIP());
  Serial.println(F("Listo"));
 
}
 
void loop() {

  calculoSulf();

  calculoAzufre();

  calculoGas();

  delay(2000);
 
}

void calculoSulf(){
// leemos del pin A0 valor
  valorSulf = analogRead(A1);
 
  //Imprimimos por el monitor serie
  float acidoSulf;

  acidoSulf = map(valorSulf, 0, 1023, 0, 100 ); //2 - 5 Normal 5 - 20 Advertencia 20 - 100 Peligro
  Serial.println("Medicion de ácido sulfhídrico: ");
  //Al método ftoa, se le envía el arreglo llamado tempString, el valor del sensor, cantidad de decimales
  Serial.println(ftoa(tempString,acidoSulf,0));
  //Se envian los datos a las API creadas en el servidor, asegurandose de que los datos sean los mismos que recibirá la página php
  //en la variable $arduino_data = $_GET['data_temperatura']; por ejemplo.
  sprintf(pageAdd,"/arduino/acidoSulf.php?acidoSulfValor=%s",ftoa(tempString,acidoSulf,0));

  if(!getPage(serverName,serverPort,pageAdd)) Serial.print(F("Falla"));
  else Serial.print(F("OK "));
  totalCount++;
  Serial.println(totalCount,DEC);

}

void calculoAzufre(){
  
// leemos del pin A0 valor
  valorAzufre = analogRead(A0);
 
  //Imprimimos por el monitor serie
  float azufre;

  azufre = map(valorAzufre, 0, 1023, 0, 200 ); //0.5-2 Normal 2 - 100 Advertencia 100 - 200 Peligro
  Serial.println("Medicion de dióxido de azufre: ");
  //Al método ftoa, se le envía el arreglo llamado tempString, el valor del sensor, cantidad de decimales
  Serial.println(ftoa(tempString,azufre,0));
  //Se envian los datos a las API creadas en el servidor, asegurandose de que los datos sean los mismos que recibirá la página php
  //en la variable $arduino_data = $_GET['data_temperatura']; por ejemplo.
  sprintf(pageAdd,"/arduino/azufre.php?azufreValor=%s",ftoa(tempString,azufre,0));

  if(!getPage(serverName,serverPort,pageAdd)) Serial.print(F("Falla"));
  else Serial.print(F("OK "));
  totalCount++;
  Serial.println(totalCount,DEC);

}

void calculoGas(){ 
  
// leemos del pin A0 valor
  valorGas = analogRead(A5);
 
  //Imprimimos por el monitor serie
  float gas;

  gas = map(valorGas, 0, 1023, 0, 255 ); //0-10 Normal 10 - 20 Advertencia 20 - 255 Peligro

// leemos del pin A5 valor
  //float gas = mq2.readLPG();
  Serial.println("Medicion de gas: ");
  //Al método ftoa, se le envía el arreglo llamado tempString, el valor del sensor, cantidad de decimales
  Serial.println(ftoa(tempString,gas,0));
  //Se envian los datos a las API creadas en el servidor, asegurandose de que los datos sean los mismos que recibirá la página php
  //en la variable $arduino_data = $_GET['data_temperatura']; por ejemplo.
  sprintf(pageAdd,"/arduino/gas.php?gasValor=%s",ftoa(tempString,gas,0));

  if(!getPage(serverName,serverPort,pageAdd)) Serial.print(F("Falla"));
  else Serial.print(F("OK "));
  totalCount++;
  Serial.println(totalCount,DEC);

}


//Método que obiene la dirección de la página en la API que permite enviar los datos.
byte getPage(char *ipBuf,int thisPort, char *page)
{
  int inChar;
  char outBuf[128];

  Serial.print(F("Conectando..."));

  if(client.connect(ipBuf,thisPort))
  {
    Serial.println(F("Conectado"));

    sprintf(outBuf,"GET %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",serverName);
    client.println(outBuf);
    client.println(F("Connection: close\r\n"));
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  // Este loop controla si el hardware de desconecta
  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      // Si hay nuevos paquetes, se reinicia el contador
      connectLoop = 0;
    }

    connectLoop++;

    // si el loop tiene más de 10 segundos.
    if(connectLoop > 10000)
    {
      // Entonces cierra la conección por tiempo.
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
    
    delay(1);
  }

  Serial.println();

  Serial.println(F("Desconectado."));

  client.stop();

  return 1;
}

//Este código permite convertir los valores del sensor a string para su interpretación como valor al ser enviado a la API en PHP.
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}
