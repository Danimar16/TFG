#define DEBUG(a) Serial.println(a);
//Variables de lectura por teclado
char lecturateclado;
String strcc;
int numero = 0;
//Pines analógicos
const int tensionMultiplexor = A0;    //vc1,ve2,vc2
const int corrienteMultiplexor = A1;  //ic1,ie2
const int tensionVB1 = A2;
const int tensionVB2 = A3;
const int tensionVC21 = A4;
const int tensionVC22 = A5;

//Pines digitales
const int pinRele1 = 2;
const int pin1Multiplex = 8;
const int pin2Multiplex = 9;
const int pin3Multiplex = 10;
const int pin4Multiplex = 11;

//Variables que almacenan tensiones o voltajes definitivos

float Igenac = 0.0;
float Ie2ac = 0.0;
float vb1 = 0.0;
float vc1 = 0.0;
float ic1 = 0.0;
float vb2 = 0.0;
float ve2 = 0.0;
float ie2 = 0.0;
float vc21 = 0.0;
float vc22 = 0.0;
float ic2 = 0.0;
float ve1 = 0.0;

//Variables intermedias
float corriente = 0.0;
float tension = 0.0;
int modificarRele = 0;
int turno = 0;

//Variable que indica si el pin de entrada del multiplex esta a 1 o 0
int var1multiplex = 0;  
int var2multiplex = 0;

const float Rescheq = 10;  // valor de las resistencias de chequeo

//Declaración de funciones
void leerdatos();
void leercorriente();
void leertension();
void rele();
void mostrarvalores();

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  pinMode(tensionMultiplexor, INPUT);
  pinMode(corrienteMultiplexor, INPUT);
  pinMode(tensionVB1, INPUT);
  pinMode(tensionVB2, INPUT);
  pinMode(tensionVC21, INPUT);
  pinMode(tensionVC22, INPUT);

  pinMode(pinRele1, OUTPUT);
  pinMode(pin1Multiplex, OUTPUT);
  pinMode(pin2Multiplex, OUTPUT);
  pinMode(pin3Multiplex, OUTPUT);
  pinMode(pin4Multiplex, OUTPUT);

  digitalWrite(pin1Multiplex, LOW);
  digitalWrite(pin2Multiplex, LOW);
  digitalWrite(pin3Multiplex, LOW);
  digitalWrite(pin4Multiplex, LOW);

  digitalWrite(pinRele1, LOW);
  delay(2000);
  Serial.print('\n');
  Serial.print(F("Conecte la alimentación del circuito, esta debe ser de 20V.\n"));
  Serial.print(F("Una vez hecho, presione 'c' y pulse ENTER.\n"));
  Serial.print('\n');
}

void loop() {
  lecturateclado = Serial.read();
  if ((lecturateclado == 'c') || (lecturateclado == 'a')) {
    if ((lecturateclado == 'a')) {
      modificarRele = 1;
    }
    if ((lecturateclado == 'c')) {
      modificarRele = 0;
    }
    lecturateclado = 'z';
    rele();
    delay(350);
    leerdatos();
    mostrarvalores();
    delay(500);
  }
}
//Esta función se encarga de leer los datos de las demás funciones
void leerdatos() {                              
  if (var1multiplex == 0 && var2multiplex == 0)  //Trabajamos en el primer valor del multiplexor
  {                                              // por lo tanto es Vc1 e Ic1
    leertension();
    vc1 = tension;
    leercorriente();
    ic1 = corriente;  
    digitalWrite(pin1Multiplex, HIGH);
    var1multiplex = 1;
  }

  if (var1multiplex == 1 && var2multiplex == 0)  //Trabajamos en el segundo valor del multiplexor
  {                                              // por lo tanto es Ve2 e Ie2
    leertension();
    ve2 = tension;
    leercorriente();
    ie2 = corriente;
    digitalWrite(pin1Multiplex, LOW);
    digitalWrite(pin2Multiplex, HIGH);
    var1multiplex = 0;
    var2multiplex = 0;
  }
}
//Esta función se encarga de transformar los datos obtenidos a través de los pines en valores de corriente
//gracias a las tensiones flotantes y las resistencias de chequeo
void leercorriente() {  
  if (modificarRele == 0) {
    corriente = 0.0;
    for (int i = 1; i <= 20; i++) {
      corriente = ((float)analogRead(corrienteMultiplexor) * (5 / 1023.0) / (Rescheq)) + corriente;
    }
    corriente = corriente / 20;
  }
}
//Esta función se encarga de transformar los datos obtenidos a través de los pines en valores de tensiones
void leertension() {  
  if (modificarRele == 0) {

    tension = 0.0;
    for (int i = 1; i <= 20; i++) {
      tension = (((float)analogRead(tensionMultiplexor) * (5 / 1023.0))) + tension;
    }
    tension = (tension) / 20;
    vb1 = 0.0;
    vb1 = ((float)analogRead(tensionVB1) * (5 / 1023.0));
    vb2 = 0.0;
    vb2 = ((float)analogRead(tensionVB2) * (5 / 1023.0));
    vc21 = 0.0;
    for (int i = 1; i <= 20; i++) {
      vc21 = (((float)analogRead(tensionVC21) * (5 / 1023.0)) * 6.92) + vc21;  // cambiar escala pq el divisor es de 100k y 680k
    }
    vc21 = ((vc21) / 20);
    vc22 = 0.0;
    for (int i = 1; i <= 20; i++) {
      vc22 = (((float)analogRead(tensionVC22) * (5 / 1023.0)) * 6.19) + vc22;  // cambiar escala pq el divisor es de 120k y 620k
    }
    vc22 = ((vc22) / 20);
    ic2 = ((vc22 - vc21) / Rescheq);
  }
}
//Esta función se encarga de mostrar por pantalla los valores de tensiones y corrientes que hemos obtenido
//a traves de los puertos analógicos del Arduino
void mostrarvalores() {
  if (modificarRele == 0) {
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.print(F("\t\t\t\t\t\t\tCONTINUA\n"));
    Serial.print(F("Se mostrarán valores de corriente, tensión y potencia. \n"));
    Serial.print(F("Las corrientes de base serán omitidas debido a la dificultad de obtener valores tan pequeños con Arduino. \n\n\n"));
    Serial.print(F("\t\t\t\t\t\t\tTENSIONES\n"));

    Serial.print('\n');
    Serial.print(F("Tensión VB1:\t"));
    Serial.print(vb1);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VC1:\t"));
    Serial.print(vc1);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VB2:\t"));
    Serial.print(vb2);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VC2:\t"));
    Serial.print(vc22);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VE2:\t"));
    Serial.print(ve2);
    Serial.print(F(" V \n"));
    Serial.print(F("\n"));

    Serial.print(F("Para visualizar las corrientes, por favor presione el numero '1' y pulse ENTER.\n"));
    if (Serial.available()) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }
    while (numero != 1) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }

    Serial.print(F("\n\n"));
    Serial.print(F("\t\t\t\t\t\t\tCORRIENTES\n"));

    Serial.print('\n');
    Serial.print(F("Corriente IC1:\t"));
    Serial.print(ic1 * 1000);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IC2:\t"));
    Serial.print(ic2 * 1000);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IE2:\t"));
    Serial.print(ie2 * 1000);
    Serial.print(F(" mA \n"));
    Serial.print(F("\n"));

    Serial.print(F("Para visualizar las potencias, por favor presione el numero '2' y pulse ENTER.\n"));
    if (Serial.available()) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }
    while (numero != 2) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }
    Serial.print(F("\n\n"));
    Serial.print(F("\t\t\t\t\t\t\POTENCIAS\n"));

    Serial.print('\n');
    Serial.print(F("Potencia transistor 1:\t"));
    Serial.print(ic1 * ((vc1 - vb1) + vb1));
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia transistor 2:\t"));
    Serial.print(ic2 * ((vc22 - vb2) + (vb2 - ve2)));
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Colector Q1:\t"));
    Serial.print(ic1 * ic1 * 3000);
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Colector Q2:\t"));
    Serial.print(ic2 * ic2 * 500);
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Emisor Q2:\t"));
    Serial.print(ie2 * ie2 * 50);
    Serial.print(F(" mW \n"));
    Serial.print('\n');

    Serial.print(F("Para visualizar los puntos de trabajo, por favor presione el numero '3' y pulse ENTER.\n"));
    if (Serial.available()) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }
    while (numero != 3) {
      strcc = Serial.readStringUntil('\n');
      numero = strcc.toFloat();
    }
    Serial.print(F("\n\n"));

    Serial.print(F("\t\t\t\t\t\t\PUNTOS DE TRABAJO\n"));

    if ((vb1 - ve1) == 0) {
      Serial.print(F("Transistor Q1 en CORTE \n"));
      Serial.print(F("Al encontrarse el transistor Q1 en CORTE, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo\n"));
      delay(100);
      exit(0);
    }

    else if ((vc1 - ve1) == 0) {
      Serial.print(F("Transistor Q1 en SATURACIÓN \n"));
      Serial.print(F("Al encontrarse el transistor Q1 en SATURACIÓN, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo\n"));
      delay(100);
      exit(0);
    }

    else {
      Serial.print(F("Transistor Q1 en ACTIVA \n"));
    }

    if ((vb2 - ve2) == 0) {
      Serial.print(F("Transistor Q2 en CORTE \n"));
      Serial.print(F("Al encontrarse el transistor Q2 en CORTE, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo\n"));
      delay(100);
      exit(0);
    }

    else if ((vc22 - ve2) == 0) {
      Serial.print(F("Transistor Q2 en SATURACIÓN \n"));
      Serial.print(F("Al encontrarse el transistor Q2 en SATURACIÓN, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo\n"));
      delay(100);
      exit(0);
    }

    else {
      Serial.print(F("Transistor Q2 en ACTIVA \n"));
    }

    Serial.print('\n');
    Serial.print(F("Acaban de mostrarse los valores de CONTINUA, para volver a ver estos valores, por favor, presione 'c' y pulse ENTER.\n"));
    Serial.print('\n');

    Serial.print(F("En otro caso, indicaremos los REQUERIMIENTOS para pasar a ALTERNA\n"));
    Serial.print('\n');
    Serial.print(F("CONFIGURACIÓN DE LOS MULTÍMETROS:\n"));
    Serial.print(F("- COMPROBAR QUE MIDEN AC\n"));
    Serial.print(F("- COMPROBAR QUE MIDEN CORRIENTE\n"));


    Serial.print('\n');

    Serial.print(F("CONFIGURACIÓN DEL GENERADOR DE FUNCIONES:\n"));
    Serial.print(F("- AMPLITUD: 0.5V-1.5V\n"));
    Serial.print(F("- SONDA CONECTADA AL TERMINAL OUTPUT Y BOTÓN OUTPUT ACTIVADO\n"));
    Serial.print(F("- FRECUENCIA: ~1KHz \n"));
    Serial.print('\n');

    Serial.print(F("Una vez comprobado esto, presione 'a' y pulse ENTER.\n"));

  } else {
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.print(F("\t\t\t\t\t\t\tALTERNA\n"));
    Serial.print("\n\n");

    Serial.print(F("Estamos en ALTERNA, por favor, conecte el multímetro al pin de la base del transistor Q1.\n"));
    Serial.print(F("Una vez comprobado, introduzca el valor de corriente ALTERNA en la base de Q1 expresado en mA y pulse ENTER.\n"));
    while (turno == 0) {
      if (Serial.available()) {
        String str = Serial.readStringUntil('\n');

        Igenac = str.toFloat();
      }
      if (Igenac > 0.0000) {
        turno = 1;
      }
    }
    Serial.print('\n');
    Serial.print(F("Ahora, por favor, conecte el multimetro entre el emisor de Q2 y la resistencia de chequeo.\n"));
    Serial.print(F("Una vez comprobado, introduzca ahora el valor de corriente ALTERNA en el emisor de Q2 expresada en mA.\n"));

    while (turno == 1) {
      if (Serial.available()) {
        String str2 = Serial.readStringUntil('\n');
        Ie2ac = str2.toFloat();
      }

      if (Ie2ac > 0.0000) {
        turno = 0;
      }
    }

    Serial.print(F("\n\n"));
    Serial.print(F("\t\t\t\t\t\t  GANANCIA EN ALTERNA\n"));

    Serial.print('\n');
    Serial.print(F("Corriente alterna IB1:\t"));
    Serial.print(Igenac, 4);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente alterna IC2:\t"));
    Serial.print(Ie2ac, 4);
    Serial.print(F(" mA \n"));

    Serial.print(F("Ganancia en corriente:\t"));
    Serial.print(Ie2ac / Igenac, 4);
    Serial.print(F(" A/A \n"));
    Igenac = 0;
    Ie2ac = 0;

    Serial.print('\n');
    Serial.print(F("Acaban de mostrarse los valores de ALTERNA, para volver a ver estos valores, por favor, presione 'a' y pulse ENTER. \n"));
    Serial.print(F("En caso de querer pasar a CONTINUA, presione 'c' y pulse ENTER.\n"));
  }
}

//Función que controla el funcionamiento del relé
void rele() {  
  if (modificarRele == 0 && ic2 < 40.00) {
    //rele abierto, estamos en continua

    digitalWrite(pinRele1, LOW);
  }

  else if (modificarRele == 1 && ic2 < 40.00) {
    //rele cerrado, estamos en alterna
    digitalWrite(pinRele1, HIGH);
  }

  else {
    digitalWrite(pinRele1, HIGH);
  }
}
