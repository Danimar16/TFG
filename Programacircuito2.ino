#define DEBUG(a) Serial.println(a);
//Variables de lectura por teclado
char lecturateclado;
String strcc;
int numero = 0;
//Pines analógicos
const int tensionMultiplexor = A2;    //vc1,ve1,vc2,ve2,vc3,ve3
const int corrienteMultiplexor = A3;  //ic1,ie1,ic2,ie2,ic3,ie3
const int tensionVB1 = A0;

//Pines digitales
const int pinRele1 = 2;
const int pin1Multiplex = 8;
const int pin2Multiplex = 9;
const int pin3Multiplex = 10;
const int pin4Multiplex = 11;

//Variables que almacenan tensiones o voltajes definitivos
float Vgenac = 0.0;
float Vc3ac = 0.0;

float vb1 = 0.0;
float vc1 = 0.0;
float ve1 = 0.0;
float ic1 = 0.0;
float ie1 = 0.0;
float vb2 = 0.0;
float vc2 = 0.0;
float ve2 = 0.0;
float ic2 = 0.0;
float ie2 = 0.0;
float vb3 = 0.0;
float vc3 = 0.0;
float ve3 = 0.0;
float ic3 = 0.0;
float ie3 = 0.0;

//Variables intermedias
float corriente = 0.0;
float tension = 0.0;
int modificarRele = 0;
int turno = 0;
//Variables que indica si el pin de entrada del multiplex esta a 1 o 0
int var1multiplex = 0;
int var2multiplex = 0;
int var3multiplex = 0;
// valor de las distintas resistencias de chequeo
const float Rescheq = 12;
const float Rescheqic2 = 15.7;  // valor de la resistencia ic2 para calibración
const float Rescheqic3 = 16.1;  // valor de la resistencia ic3 para calibración
const float Rescheqie2 = 22.2;  // valor de la resistencia ie2 para calibración
int usoresistencia = 0;

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
  delay(2500);
  Serial.print('\n');
  Serial.print(F("Conecte la alimentación del circuito, esta debe ser de 6V.\n"));
  Serial.print(F("Conecta la alimentación de los multiplexores, esta debe ser de 5V, una vez hecho, presione 'c' y pulse ENTER.\n"));

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
  if (var1multiplex == 0 && var2multiplex == 0 && var3multiplex == 0)  //Trabajamos en el primer valor del multiplexor
  // por lo tanto es Vc1 e Ic1
  {
    leertension();
    vc1 = tension;
    vb2 = tension;

    leercorriente();
    ic1 = corriente * 100;
    digitalWrite(pin1Multiplex, HIGH);
    var1multiplex = 1;
  }

  if (var1multiplex == 1 && var2multiplex == 0 && var3multiplex == 0)  //Trabajamos en el segundo valor del multiplexor
  // por lo tanto es Ve1 e Ie1
  {
    leertension();
    ve1 = tension;
    leercorriente();
    ie1 = corriente * 1000;
    digitalWrite(pin1Multiplex, LOW);
    digitalWrite(pin2Multiplex, HIGH);
    var1multiplex = 0;
    var2multiplex = 1;
  }

  if (var1multiplex == 0 && var2multiplex == 1 && var3multiplex == 0)  //Trabajamos en el tercer valor del multiplexor
                                                                       // por lo tanto es Vc2 e Ic2
  {
    leertension();
    vc2 = tension;
    vb3 = tension;
    usoresistencia = 0;
    leercorriente();
    ic2 = corriente * 1000;
    digitalWrite(pin1Multiplex, HIGH);
    digitalWrite(pin2Multiplex, HIGH);
    var1multiplex = 1;
    var2multiplex = 1;
  }
  if (var1multiplex == 1 && var2multiplex == 1 && var3multiplex == 0)  //Trabajamos en el cuarto valor del multiplexor
                                                                       // por lo tanto es Ve2 e Ie2
  {
    leertension();
    ve2 = tension;
    usoresistencia = 1;
    leercorriente();
    ie2 = corriente * 1000;
    delay(100);
    digitalWrite(pin1Multiplex, LOW);
    digitalWrite(pin2Multiplex, LOW);
    digitalWrite(pin3Multiplex, HIGH);
    var1multiplex = 0;
    var2multiplex = 0;
    var3multiplex = 1;
  }
  if (var1multiplex == 0 && var2multiplex == 0 && var3multiplex == 1)  //Trabajamos en el cuarto valor del multiplexor
                                                                       // por lo tanto es Vc3 e Ic3
  {
    leertension();
    vc3 = tension;
    usoresistencia = 2;
    leercorriente();
    ic3 = corriente * 1000;
    delay(100);
    digitalWrite(pin1Multiplex, HIGH);
    digitalWrite(pin2Multiplex, LOW);
    digitalWrite(pin3Multiplex, HIGH);
    var1multiplex = 1;
    var2multiplex = 0;
    var3multiplex = 1;
  }
  if (var1multiplex == 1 && var2multiplex == 0 && var3multiplex == 1)  //Trabajamos en el cuarto valor del multiplexor
                                                                       // por lo tanto es Ve3 e Ie3
  {
    leertension();
    ve3 = tension;
    usoresistencia = 3;
    leercorriente();
    ie3 = corriente * 1000;
    delay(100);
    digitalWrite(pin1Multiplex, LOW);
    digitalWrite(pin2Multiplex, LOW);
    digitalWrite(pin3Multiplex, LOW);

    var1multiplex = 0;
    var2multiplex = 0;
    var3multiplex = 0;
  }
}

//Esta función se encarga de transformar los datos obtenidos a través de los pines en valores de corriente
//gracias a las tensiones flotantes y las resistencias de chequeo
void leercorriente() {
  if (usoresistencia == 0) {  //corrienteic2
    corriente = 0.0;
    for (int i = 1; i <= 20; i++) {
      corriente = ((float)analogRead(corrienteMultiplexor) * (5 / 1023.0) / (Rescheqic2)) + corriente;
    }
    corriente = corriente / 20;
  } else if (usoresistencia == 1) {  //corrienteie2
    corriente = 0.0;
    for (int i = 1; i <= 20; i++) {
      corriente = ((float)analogRead(corrienteMultiplexor) * (5 / 1023.0) / (Rescheqie2)) + corriente;
    }
    corriente = corriente / 20;
  } else if (usoresistencia == 2) {  //corrienteic3
    corriente = 0.0;
    for (int i = 1; i <= 20; i++) {
      corriente = ((float)analogRead(corrienteMultiplexor) * (5 / 1023.0) / (Rescheqic3)) + corriente;
    }
    corriente = corriente / 20;
  } else {
    corriente = 0.0;
    for (int i = 1; i <= 20; i++) {
      corriente = ((float)analogRead(corrienteMultiplexor) * (5 / 1023.0) / (Rescheq)) + corriente;
    }
    corriente = corriente / 20;
  }
}
//Esta función se encarga de transformar los datos obtenidos a través de los pines en valores de tensiones
void leertension() {
  tension = 0.0;
  for (int i = 1; i <= 20; i++) {
    tension = (((float)analogRead(tensionMultiplexor) * (5 / 1023.0))) + tension;
  }
  tension = (tension) / 20;

  vb1 = 0.0;
  vb1 = ((float)analogRead(tensionVB1) * (5 / 1023.0));
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

    Serial.print(F("Tensión VE1:\t"));
    Serial.print(ve1);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VB2:\t"));
    Serial.print(vb2);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VC2:\t"));
    Serial.print(vc2);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VE2:\t"));
    Serial.print(ve2);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VB3:\t"));
    Serial.print(vb3);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VC3:\t"));
    Serial.print(vc3);
    Serial.print(F(" V \n"));

    Serial.print(F("Tensión VE3:\t"));
    Serial.print(ve3);
    Serial.print(F(" V \n"));
    Serial.print(F(" \n"));

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
    Serial.print(ic1);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IC2:\t"));
    Serial.print(ic2);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IC3:\t"));
    Serial.print(ic3);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IE1:\t"));
    Serial.print(ie1);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IE2:\t"));
    Serial.print(ie2);
    Serial.print(F(" mA \n"));

    Serial.print(F("Corriente IE3:\t"));
    Serial.print(ie3);
    Serial.print(F(" mA \n"));
    Serial.print(F(" \n\n"));

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
    Serial.print(F("Potencia Transistor 1: "));
    Serial.print(ic1 * ((vc1 - vb1) + vb1));
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Transistor 2: "));
    Serial.print(ic2 * ((vc2 - vb2) + (vb2 - ve2)));
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Transistor 3: "));
    Serial.print(ic3 * ((vc3 - vb3) + (vb3 - ve3)));
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Colector Q1: "));
    Serial.print((ic1)*12000);
    Serial.print(F(" uW \n"));

    Serial.print(F("Potencia Resistencia Colector Q2: "));
    Serial.print((((ic2 * ic2) * 2200) / 1000));  //dividimos entre 1000 para pasar de uW a mW
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Colector Q3: "));
    Serial.print(((ic3 * ic3) * 430) / 1000);  //dividimos entre 1000 para pasar de uW a mW
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Emisor Q1:\t"));
    Serial.print(ie1 * ie1 * 2700);
    Serial.print(F(" uW \n"));

    Serial.print(F("Potencia Resistencia Emisor Q2:\t"));
    Serial.print((ie2 * ie2 * 6200) / 1000);  //dividimos entre 1000 para pasar de uW a mW
    Serial.print(F(" mW \n"));

    Serial.print(F("Potencia Resistencia Emisor Q3:\t"));
    Serial.print((ie3 * ie3 * 1500) / 1000);  //dividimos entre 1000 para pasar de uW a mW
    Serial.print(F(" mW \n"));
    Serial.print(F(" \n\n"));

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
      Serial.print(F("Al encontrarse el transistor Q1 en CORTE, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else if ((vc1 - ve1) == 0) {
      Serial.print(F("Transistor Q1 en SATURACIÓN \n"));
      Serial.print(F("Al encontrarse el transistor Q1 en SATURACIÓN, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else {
      Serial.print(F("Transistor Q1 en ACTIVA \n"));
    }

    if ((vb2 - ve2) == 0) {
      Serial.print(F("Transistor Q2 en CORTE \n"));
      Serial.print(F("Al encontrarse el transistor Q2 en CORTE, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else if ((vc2 - ve2) == 0) {
      Serial.print(F("Transistor Q2 en SATURACIÓN \n"));
      Serial.print(F("Al encontrarse el transistor Q2 en SATURACIÓN, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else {
      Serial.print(F("Transistor Q2 en ACTIVA \n"));
    }

    if ((vb3 - ve3) == 0) {
      Serial.print(F("Transistor Q3 en CORTE \n"));
      Serial.print(F("Al encontrarse el transistor Q3 en CORTE, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else if ((vc1 - ve1) == 0) {
      Serial.print(F("Transistor Q3 en SATURACIÓN \n"));
      Serial.print(F("Al encontrarse el transistor Q3 en SATURACIÓN, no tiene sentido proseguir con el programa, por favor revise el circuito y vuelva a intentarlo.\n"));
      delay(100);
      exit(0);
    }

    else {
      Serial.print(F("Transistor Q3 en ACTIVA \n"));
    }

    Serial.print('\n');
    Serial.print(F("Acaban de mostrarse los valores de CONTINUA, para volver a ver estos valores, por favor, presione 'c' y pulse ENTER.\n"));
    Serial.print('\n');

    Serial.print(F("En otro caso, indicaremos los REQUERIMIENTOS para pasar a ALTERNA\n"));
    Serial.print('\n');
    Serial.print(F("CONFIGURACIÓN DE LOS MULTÍMETROS: COMPROBAR QUE MIDEN AC\n"));
    Serial.print('\n');

    Serial.print(F("CONFIGURACIÓN DEL GENERADOR DE FUNCIONES:\n"));
    Serial.print(F("- AMPLITUD: 0.5V-1.5V\n"));
    Serial.print(F("- SONDA CONECTADA AL TERMINAL OUTPUT Y BOTÓN OUTPUT ACTIVADO\n"));
    Serial.print(F("- FRECUENCIA: ~1KHz \n"));
    Serial.print('\n');

    Serial.print(F("DESCONECTAR COLECTOR Q3 DEL PIN C4 DEL MULTIPLEXOR\n"));
    Serial.print('\n');

    Serial.print(F("Una vez comprobado esto, presione 'a' y pulse ENTER.\n"));

  } else {
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.print(F("\t\t\t\t\t\t\tALTERNA\n"));
    Serial.print("\n\n");

    Serial.print(F("Estamos en ALTERNA, por favor, conecte el multímetro al pin de la base del transistor Q1.\n"));
    Serial.print(F("Una vez comprobado, introduzca el valor de tensión ALTERNA en la base de Q1 expresado en mV y pulse ENTER.\n"));

    while (turno == 0) {
      if (Serial.available()) {
        String str = Serial.readStringUntil('\n');

        Vgenac = str.toFloat();
      }
      if (Vgenac > 0.0000) {
        turno = 1;
      }
    }
    Serial.print('\n');
    Serial.print(F("Ahora, por favor, conecte el multimetro al pin del colector del transistor Q3.\n"));
    Serial.print(F("Una vez comprobado, introduzca el valor de tensión ALTERNA en el colector de Q3 expresado en mV y pulse ENTER.\n"));

    while (turno == 1) {
      if (Serial.available()) {
        String str2 = Serial.readStringUntil('\n');
        Vc3ac = str2.toFloat();
      }

      if (Vc3ac > 0.0000) {
        turno = 0;
      }
    }

    Serial.print(F("\n\n"));
    Serial.print(F("\t\t\t\t\t\t  GANANCIA EN ALTERNA\n"));

    Serial.print('\n');
    Serial.print(F("Tensión alterna IB1:\t"));
    Serial.print(Vgenac, 4);
    Serial.print(F(" mV \n"));

    Serial.print(F("Tensión alterna VC3:\t"));
    Serial.print(Vc3ac, 4);
    Serial.print(F(" mV \n"));

    Serial.print(F("Ganancia en tensión:\t"));
    Serial.print(Vc3ac / Vgenac, 4);
    Serial.print(F(" V/V \n"));
    Vgenac = 0;
    Vc3ac = 0;
    Serial.print('\n');
    Serial.print(F("Acaban de mostrarse los valores de ALTERNA, para volver a ver estos valores, por favor, presione 'a' y pulse ENTER. \n"));
    Serial.print(F("En caso de querer pasar a CONTINUA, por favor conecte el colector de Q3 al PIN C4 de ambos multiplexor, presione 'c' y pulse ENTER.\n"));
  }
}

//Función que controla el funcionamiento del relé
void rele() {
  if (modificarRele == 0) {
    //rele abierto, estamos en continua

    digitalWrite(pinRele1, LOW);
  }

  else if (modificarRele == 1) {
    //rele cerrado, estamos en alterna
    digitalWrite(pinRele1, HIGH);
  }

  else {
    digitalWrite(pinRele1, HIGH);
  }
}
