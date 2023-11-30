//Variable donde almacenaremos el valor del potenciometro
#include <MQ2.h>
int valor;
int valor2;
MQ2 mq2(A5);
 
void setup() {
  //Inicializamos la comunicación serial
  Serial.begin(9600);
  
  mq2.begin();
  
  //Escribimos por el monitor serie mensaje de inicio
  //Serial.println("Inicio de sketch - valores del potenciometro");
 
}
 
void loop() {

  calculoAzufre();

  calculoGas();



  delay(1000);
 
}

void calculoAzufre(){
// leemos del pin A0 valor
  valor = analogRead(A0);
 
  //Imprimimos por el monitor serie
  int azufre;

  azufre = map(valor, 0, 1023, 0, 100 );
  Serial.println("Medicion de azufre: ");
  Serial.println(azufre);

}

void calculoGas(){
// leemos del pin A5 valor
  float gas = mq2.readLPG();
 
  //Imprimimos por el monitor serie
  //gas = map(valor2, 0, 1023, 65, 255 );

  Serial.println("Medicion de gas: ");
  Serial.println(gas);

}
