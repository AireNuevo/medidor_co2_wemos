/*----------------------------------------------------------
    Medidor de CO2 con placa WEMOS, pantalla i2c, led simple
  ----------------------------------------------------------*/
#include "pitches.h"
#include <MHZ19_uart.h>     
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//-----------------Pines----------------------------
/*
El pin D3/GPIO0 se usa para Flash de la placa, si el buzzer está conectado durante el inicio
o al cargar código, genera problemas. 
*/
const int rx_pin = 13;     // Serial rx
const int tx_pin = 15;     // Serial tx
const int pinLed = 16;     // Led
const int pinBuzzer = 14;   // Buzzer
const int pinCalib = 12;    // Pulsador
const String numeroSerie = "0208"; 
//--------------------------------------------------
long loops = 0;                         // Contamos las veces que se ejecutó el loop
MHZ19_uart sensor;                      // Creo el objeto del sensor
LiquidCrystal_I2C display(0x27,16,2);   // Creo el objeto display dirección 0x27 y 16 columnas x 2 filas
//-----------------Setup----------------------------
void setup() {
  pinMode(pinLed, OUTPUT);               // Inicia LED
  pinMode(pinBuzzer, OUTPUT);            // Inicia Buzzer
  pinMode(pinCalib, INPUT_PULLUP);       // Entrada pulsador para calibrar, setteada como pullup para poder conectar pulsador sin poner resistencia adicional
  Serial.begin(9600);                    // Iniciamos el serial
  display.begin();                       // Inicio el display            
  display.clear();                       // Limpio la pantalla 
  display.backlight();                   // Prendo la backlight
  alarma(1, 250);
  // Imprimimos un mensaje mostrando el número de serie del equipo y un cartel de que el equipo se está iniciando
  // Print por serial
  Serial.print("N° de serie " + numeroSerie + "\n");
  Serial.print("INICIANDO \n");
  // Print por display
  // Print numero serie
  displayPrint(0, 0, "N/S: " + numeroSerie);
  displayPrint(0, 1, "INICIANDO");       
  logoUNAHUR();                          // Dibuja el logo de la UNAHUR en el lado derecho de la pantalla.
  delay(10000);                          // Esperamos 10 segundos
  display.clear();                       // Limpio la pantalla
  // Calentamos 
  sensor.begin(rx_pin, tx_pin);          // Setteamos los pines y iniciamos el sensor
  sensor.setAutoCalibration(false);      //Se setea la autocalibracion en falso para poder calibrar cuando se desee        
  // Avisamos que el dispositivo se está calentando y que se debe esperar                       
  // Print por serial
  Serial.print("Calentando, espere 1 minuto \n");
  // Print por display
  displayPrint(0, 0, "Calentando");       
  displayPrint(0, 1, "Espere 1 minuto"); 
  delay(60000);                           // Esperamos 1 minuto
  display.clear();                        // Limpio la pantalla
  alarma(3, 250);                         // Alarma indicando que terminó el calentamiento
}

void loop() {
  if (digitalRead(pinCalib) == LOW) {           // Si el pulsador está presionado, se ejecuta la calibración
    alarma(1, 250);
    calibrar();
  }
  if(++loops % 30 == 0) {                       // Si loops es múltiplo de 30 
                                                // Imprimimos por pantalla un mensaje presentando el dispositivo
    // Print por serial
    Serial.print("AireNuevo UNAHUR \n"); 
    Serial.print("MEDIDOR de CO2 \n");  
    // Print por display
    display.clear();                            // Limpio la pantalla
    displayPrint(0, 0, "AireNuevo UNAHUR");     
    displayPrint(0, 1, "MEDIDOR de CO2");       
    delay(5000);
    loops = 0;                                  // Reinicio la cuenta
  } 
  display.clear();
  displayPrint(0, 0, "Aire Nuevo");
  //  Emite una alarma en función del resultado
  while(sensor.getPPM() >= 1200) {              // Acá todavía no podemos usar una variable porque tiene que ser 
    alarma(1, 250);                             // la medición en tiempo real para que suene constantemente
    imprimirCO2(sensor.getPPM());
  }
  int co2ppm = sensor.getPPM();                 // Guardamos el CO2 en una variable
  imprimirCO2(co2ppm);
  if(co2ppm >= 1000){
    alarma(4, 500);                         
  } 
  else if(co2ppm >= 800){
    alarma(2, 1000);
  }
  aireNuevo();                                  // Imprimimos un cartel que se va moviendo de derecha a izquierda con la leyenda "Aire Nuevo"
  delay(5000);                                  // Demora 10 seg entre mediciones, 5 segundos pasan con el mensaje de Aire Nuevo, y 5 se esperan
}
