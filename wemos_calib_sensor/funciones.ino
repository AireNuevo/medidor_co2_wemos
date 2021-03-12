//-----------Led-RGB----------------------
void rgb(char color) {
  switch (color) {
    case 'r':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 255);
      break;
    case 'g':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'y':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'b':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 0);
      break;
  }
}
//-----------------Alarma----------------------------
void alarma(int veces, int duracionNota) {
  for(int i=0; i<veces; i++)
  {
    tone(pinBuzzer, 523, duracionNota);   // Hago sonar el buzzer, 523 corresponde a la nota C5
    delay(duracionNota);                  // Espero lo que dura la nota
    noTone(pinBuzzer);                    // Silencio el buzzer
    delay(duracionNota);                  // Delay entre alarmas
  }
}

// Creacion del simbolo de numeral para ser utilizado en el display i2c
byte numeral[] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
//-----------Print-por-display----------------------
void displayPrint(int posicion, int linea, String texto) {
  display.setCursor(posicion, linea);       // Ubicamos el cursor en la posicion y linea deseada
  display.print(texto);                     // Escribe primera linea del cartel
}
void imprimirCO2(int co2ppm) {
  // Print por serial  
  Serial.print("N° de serie 0000");
  Serial.print("CO2: " + String(co2ppm) + "ppm \n");  // Escribe CO2
  // Print por display
  // Muestra medición de CO2  
  display.clear();                                    // Borra pantalla
  // Print numero serie
  displayPrint(0, 0, "N/S: 0000");
  // Print CO2
  displayPrint(0, 1, "CO2: " + String(co2ppm) + "ppm");   
  logoUNAHUR();
}
void logoUNAHUR() {
  byte UNAHUR1[] = {
    B11100,
    B11110,
    B11111,
    B11111,
    B11111,
    B11111,
    B01111,
    B00111
  };
  byte UNAHUR2[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  };
  byte UNAHUR3[] = {
    B00111,
    B01111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11110,
    B11100
  };
  display.createChar(0, UNAHUR1);
  display.createChar(1, UNAHUR2);
  display.createChar(2, UNAHUR3);
  display.setCursor(13, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(0);
  display.setCursor(14, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(1);
  display.setCursor(15, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(2);
  display.setCursor(13, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(2);
  display.setCursor(14, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(1);
  display.setCursor(15, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(0);
}
//-----------Calibración----------------------
/*
void calibrar()
{
  const long segundosEspera = 1800;      // Cantidad de segundos a esperar
  long segundosPasados = 0;              // Cuenta segundos
  // Print por serial
  Serial.print("COMIENZA CALIBRACION \n");
  // Print por display
  display.clear();
  displayPrint(0, 0, "COMIENZA");
  displayPrint(0, 1, "CALIBRACION");
  delay(10000); // Espera 10 segundos
 
  while(segundosPasados <= segundosEspera) { // espera media hora
    if (++segundosPasados % 60 == 0) {                           // Si cnt es múltiplo de 60
      // Print por serial
      Serial.print(String(segundosPasados / 60) + " minutos \n"); // Cada minuto muestra el valor 
      Serial.print("CO2: " + String(sensor.getPPM()) + "ppm \n"); // Escribe CO2
      // Print por display
      display.clear();                                            // Borra pantalla  
      displayPrint(0, 0, String(segundosPasados / 60));           // Ubicamos el cursor en la primera posición(columna:0) de la primer línea(fila:0) y escribimos los minutos pasados
      displayPrint(7, 0, "minutos");
      displayPrint(0, 1, "CO2: ");                                // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) y escribimos el CO2
      displayPrint(8, 1, String(sensor.getPPM()));
      displayPrint(12, 1, "ppm");
    }
    else {
      // Print por display
      display.clear();                                            // Borra pantalla  
      displayPrint(0, 0, "CALIBRANDO");
      displayPrint(0, 1, String(segundosPasados / 60));           // Ubicamos el cursor en la primera posición(columna:0) de la primer línea(fila:0) y escribimos los minutos pasados
      displayPrint(7, 1, "minutos");
    }
    delay(1000); // Espera 1 segundo
  }
  sensor.calibrateZero();               // Calibra
  // Print por serial
  Serial.print("PRIMERA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla
  displayPrint(0, 0, "PRIMERA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
  alarma(1, 250);
  delay(60000);                         // Espera 1 minuto
  sensor.calibrateZero();               // Calibra por segunda vez por las dudas
  // Print por serial
  Serial.print("SEGUNDA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla     
  displayPrint(0, 0, "SEGUNDA");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "CALIBRACION");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1) 
  alarma(1, 250);
  delay(10000); // Espera 10 segundos
  // Print por serial
  Serial.print("CALIBRACION TERMINADA \n");
  // Print por display
  display.clear(); // borra pantalla  
  displayPrint(0, 0, "CALIBRACION");        // Ubicamos el cursor en la primera posición(columna:0) de la primera línea(fila:0)
  displayPrint(0, 1, "TERMINADA");    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  alarma(5, 250);
  delay(10000); // Espera 10 segundos 
}*/
