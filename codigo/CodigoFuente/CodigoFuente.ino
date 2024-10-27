//Motor A
const int In1A  = 12;    // IN1
const int In2A  = 11;    // IN2
//Motor B
const int In1B  = 10;    // IN3
const int In2B  = 9;    // IN4
int EN_A = 13;                 // ENA
int EN_B = 8;                 // ENB
//Motor C
const int In1C  = 6;    // IN1
const int In2C  = 5;    // IN2
//Motor D
const int In1D  = 4;    // IN3
const int In2D  = 3;    // IN4
int EN_C = 7;                 // ENA
int EN_D = 2;                 // ENB

//Infrarojo Izquierdo
const int IR_Izq = 52;
const int Vcc_IR_Izq = 53;
bool IR_Izq_D0;

//Infrarojo Derecho
const int IR_Der = 22;
const int Vcc_IR_Der = 24;
bool IR_Der_D0;

//Velocidad de los motores
int speed;

//contador de tiempo para los giros
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;
int contador_vueltas;
bool giro_brusco;

//Ultrasonico centro
const int Trig_Centro = 33;
const int Echo_Centro = 32;

//Ultrasonico izquierda
const int Trig_Izq = 35;
const int Echo_Izq = 34;

//Ultrasonico derecha
const int Trig_Der = 31;
const int Echo_Der = 30;

//Adafruit LED
const int Adress_LED = 38;

//Ultimo Giro detectado
bool Giro_Izq;

void setup() {
  Serial.begin(9600);
  speed = 0;
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(In1A, OUTPUT);
  pinMode(In2A, OUTPUT);
  pinMode(In1B, OUTPUT);
  pinMode(In2B, OUTPUT);

  pinMode(EN_C, OUTPUT);
  pinMode(EN_D, OUTPUT);
  pinMode(In1C, OUTPUT);
  pinMode(In2C, OUTPUT);
  pinMode(In1D, OUTPUT);
  pinMode(In2D, OUTPUT);


  pinMode(IR_Der, INPUT);
  pinMode(Vcc_IR_Der, OUTPUT);

  pinMode(IR_Izq, INPUT);
  pinMode(Vcc_IR_Izq, OUTPUT);

  digitalWrite(Vcc_IR_Izq, HIGH);
  digitalWrite(Vcc_IR_Der, HIGH);

  pinMode(Trig_Centro, OUTPUT);
  pinMode(Echo_Centro, INPUT);

  pinMode(Trig_Izq, OUTPUT);
  pinMode(Echo_Izq, INPUT);

  pinMode(Trig_Der, OUTPUT);
  pinMode(Echo_Der, INPUT);

  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, HIGH); //Direccion del motor C
  digitalWrite(In2C, LOW);

  digitalWrite(In1D, HIGH); //Direccion del motor D
  digitalWrite(In2D, LOW);

  tiempo1 = millis();
  contador_vueltas = 0;
  giro_brusco = false;

  Giro_Izq = false;
}

void loop() {
  tiempo1 = millis();
  if (deteccion_izq() && deteccion_der()) { // No hay ninguna linea, puede continuar
    Recto();
  } else if (!deteccion_izq() && !deteccion_der()) {
    if (Giro_Izq) {
      Giro_Izq = true;
      tiempo_transcurrido();
      Atras();
      Detenido();
      if (giro_brusco) {
        Mas_90();
        giro_brusco = false;
        contador_vueltas = 0;
      } else {
        Mas_45();
      }

    } else {
      Giro_Izq = false;
      tiempo_transcurrido();
      Atras();
      Detenido();
      if (giro_brusco) {
        Menos_90();
        giro_brusco = false;
        contador_vueltas = 0;
      } else {
        Menos_45();
      }
    }
  } else if (UltraSonico_Centro()) {
    //--------------------------------------------------------------------Aqui va el codigo del ultrasonico
  } else if (!deteccion_izq()) { //Detecto una linea a su izquierda
    Giro_Izq = false;
    tiempo_transcurrido();
    Atras();
    Detenido();
    if (giro_brusco) {
      Menos_90();
      giro_brusco = false;
      contador_vueltas = 0;
    } else {
      Menos_45();
    }
  } else if (!deteccion_der()) { //Detecto una linea a su derecha
    Giro_Izq = true;
    tiempo_transcurrido();
    Atras();
    Detenido();
    if (giro_brusco) {
      Mas_90();
      giro_brusco = false;
      contador_vueltas = 0;
    } else {
      Mas_45();
    }
  }
  delay(10);
}

bool UltraSonico_Centro() {
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(Trig_Centro, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trig_Centro, LOW);

  t = pulseIn(Echo_Centro, HIGH); //obtenemos el ancho del pulso
  d = t / 59;           //escalamos el tiempo a una distancia en cm
  if (d > 1) {
    return false;
  } else {
    return true;
  }
}

void tiempo_transcurrido() {
  tiempo2 = millis();
  if ((tiempo2 - tiempo1) < 1400) {
    tiempo1 = millis();
    contador_vueltas++;
  }
  if (contador_vueltas == 2) {
    giro_brusco = true;
  }
}

bool deteccion_izq() {
  IR_Izq_D0 = digitalRead(IR_Izq);
  return IR_Izq_D0;
}

bool deteccion_der() {
  IR_Der_D0 = digitalRead(IR_Der);
  return IR_Der_D0;
}

void Mas_45() {
  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, LOW); //Atras motor C
  digitalWrite(In2C, HIGH);

  digitalWrite(In1D, LOW); //Atras motor D
  digitalWrite(In2D, HIGH);
  speed = 255;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(175);
}

void Menos_45() {
  digitalWrite(In1A, HIGH); //Direccion del motor A
  digitalWrite(In2A, LOW);

  digitalWrite(In1B, HIGH); //Direccion del motor B
  digitalWrite(In2B, LOW);

  digitalWrite(In1C, HIGH); //Atras motor C
  digitalWrite(In2C, LOW);

  digitalWrite(In1D, HIGH); //Atras motor D
  digitalWrite(In2D, LOW);
  speed = 255;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(175);
}

void Menos_90() {
  digitalWrite(In1A, HIGH); //Direccion del motor A
  digitalWrite(In2A, LOW);

  digitalWrite(In1B, HIGH); //Direccion del motor B
  digitalWrite(In2B, LOW);

  digitalWrite(In1C, HIGH); //Atras motor C
  digitalWrite(In2C, LOW);

  digitalWrite(In1D, HIGH); //Atras motor D
  digitalWrite(In2D, LOW);
  speed = 255;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(275);
}

void Mas_90() {
  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, LOW); //Atras motor C
  digitalWrite(In2C, HIGH);

  digitalWrite(In1D, LOW); //Atras motor D
  digitalWrite(In2D, HIGH);
  speed = 255;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(275);
}

void Detenido() {
  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, LOW); //Atras motor C
  digitalWrite(In2C, HIGH);

  digitalWrite(In1D, LOW); //Atras motor D
  digitalWrite(In2D, HIGH);
  speed = 0;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(500);
}

void Recto() {
  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, HIGH); //Direccion del motor C
  digitalWrite(In2C, LOW);

  digitalWrite(In1D, HIGH); //Direccion del motor D
  digitalWrite(In2D, LOW);

  speed = 70;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
}

void Lento() {
  digitalWrite(In1A, LOW); //Direccion del motor A
  digitalWrite(In2A, HIGH);

  digitalWrite(In1B, LOW); //Direccion del motor B
  digitalWrite(In2B, HIGH);

  digitalWrite(In1C, HIGH); //Direccion del motor C
  digitalWrite(In2C, LOW);

  digitalWrite(In1D, HIGH); //Direccion del motor D
  digitalWrite(In2D, LOW);

  speed = 80;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor

  delay(1000);
}

void Atras() {
  digitalWrite(In1A, HIGH); //Direccion del motor A
  digitalWrite(In2A, LOW);

  digitalWrite(In1B, HIGH); //Direccion del motor B
  digitalWrite(In2B, LOW);

  digitalWrite(In1C, LOW); //Direccion del motor C
  digitalWrite(In2C, HIGH);

  digitalWrite(In1D, LOW); //Direccion del motor D
  digitalWrite(In2D, HIGH);

  speed = 225;

  analogWrite(EN_A, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_B, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_C, speed);      //Setea el valor de la velocidad del motor
  analogWrite(EN_D, speed);      //Setea el valor de la velocidad del motor
  delay(150);
}
