// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        38 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//--------------------------------------------------------------------

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

//Ultimo Giro detectado

int estado_color;
int color_R;
int color_G;
int color_B;

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

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
  color_R = 255;
  color_G = 0;
  color_B = 0;
  estado_color = 0;
}

void loop() {
  if (deteccion_izq() && deteccion_der()) { // No hay ninguna linea, puede continuar
    Recto();
    //pixels.setPixelColor(15, pixels.Color(255, 0, 0));
    //pixels.show();
  } else if (!deteccion_izq()) {
    //pixels.setPixelColor(15, pixels.Color(0, 255, 0));
    //pixels.show();
    Atras();
    Detenido();
    Giro_Der();
  } else if (!deteccion_der()) {
    //pixels.setPixelColor(15, pixels.Color(0, 0, 255));
    //pixels.show();

    Atras();
    Detenido();
    Giro_Izq();
  }
  rainbow();
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

void Giro_Der() {
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

void Giro_Izq() {
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
  delay(300);
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

  speed = 215;

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
  delay(100);
}

void rainbow() {
  switch (estado_color) {
    case 0: //RGB(255,0,B+1) hasta B = 255
      color_B = color_B + 1;
      Set_Color(color_R, color_G, color_B);
      if (color_B == 255) {
        estado_color = 1;
      }
      break;

    case 1: //RGB(R-1,0,255) hasta R = 0
    color_R = color_R - 1;
      Set_Color(color_R, color_G, color_B);
      if (color_R == 0) {
        estado_color = 2;
      }
      break;

    case 2: //RGB(0,G+1,255) hasta G = 255
    color_G = color_G + 1;
      Set_Color(color_R, color_G, color_B);
      if (color_G == 255) {
        estado_color = 3;
      }
      break;

    case 3: //RGB(0,255,B-1) hasta B = 0
    color_B = color_B - 1;
      Set_Color(color_R, color_G, color_B);
      if (color_B == 0) {
        estado_color = 4;
      }
      break;

    case 4: //RGB(R+1,255,0) hasta R = 255
      color_R = color_R + 1;
      Set_Color(color_R, color_G, color_B);
      if (color_R == 255) {
        estado_color = 5;
      }
      break;

    case 5:
    //RGB(255,G-1,0) hasta G = 0
      color_G = color_G - 1;
      Set_Color(color_R, color_G, color_B);
      if (color_G == 0) {
        estado_color = 0;
      }
      break;

    default:
    color_R = 255;
    color_G = 0;
    color_B = 0;
    estado_color = 0;
      break;
  }

  pixels.show();
}

void Set_Color(int R, int G, int B) {
  pixels.setPixelColor(0, pixels.Color(R, G, B));
  pixels.setPixelColor(1, pixels.Color(R, G, B));
  pixels.setPixelColor(2, pixels.Color(R, G, B));
  pixels.setPixelColor(3, pixels.Color(R, G, B));
  pixels.setPixelColor(4, pixels.Color(R, G, B));
  pixels.setPixelColor(5, pixels.Color(R, G, B));
  pixels.setPixelColor(6, pixels.Color(R, G, B));
  pixels.setPixelColor(7, pixels.Color(R, G, B));
  pixels.setPixelColor(8, pixels.Color(R, G, B));
  pixels.setPixelColor(9, pixels.Color(R, G, B));
  pixels.setPixelColor(10, pixels.Color(R, G, B));
  pixels.setPixelColor(11, pixels.Color(R, G, B));
  pixels.setPixelColor(12, pixels.Color(R, G, B));
  pixels.setPixelColor(13, pixels.Color(R, G, B));
  pixels.setPixelColor(14, pixels.Color(R, G, B));
  pixels.setPixelColor(15, pixels.Color(R, G, B));
}
