#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //

int Motor_R=6;
int Motor_L=9;

// Centro
const int Trigger_1 = 4;   //Pin digital 2 para el Trigger del sensor
const int Echo_1 = 5;   //Pin digital 3 para el echo del sensor

// Izquierdo
const int Trigger_2 = 6;
const int Echo_2 = 7;

// Derecho
const int Trigger_3 = 8;
const int Echo_3 = 9;

int pwmVar;

void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();

  //Iniciamos los parametros del ultrasonico
  pinMode(Trigger_1, OUTPUT); //pin como salida
  pinMode(Echo_1, INPUT);  //pin como entrada
  digitalWrite(Trigger_1, LOW);//Inicializamos el pin con 0

  pinMode(Trigger_2, OUTPUT);
  pinMode(Echo_2, INPUT);
  digitalWrite(Trigger_2, LOW);

  pinMode(Trigger_3, OUTPUT);
  pinMode(Echo_3, INPUT);
  digitalWrite(Trigger_3, LOW);

  pinMode(Motor_R, OUTPUT);
  pinMode(Motor_L, OUTPUT);
  
  lcd.print("Dist S1: ");
}

void loop() {
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(10, 0);
  // Escribimos el número de segundos trascurridos
  long distancia = getDistancia(Trigger_1, Echo_1);
  lcd.print(distancia);
  lcd.print(" CM");
  lcd.setCursor(0,1);
  lcd.print("VPWM: ");
  pwmVar=getPWM(distancia);
  analogWrite(Motor_R,pwmVar);
  analogWrite(Motor_L,pwmVar);
  lcd.print(pwmVar);

  if (pwmVar <= 0) {
    initBack();
  }
  
  delay(100);
}

long  getDistancia(int trigger, int echo)
{
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(trigger, LOW);
  
  t = pulseIn(echo, HIGH); //obtenemos el ancho del pulso
  d = (t/59) - 15;

  return d < 0 ? 0 : d;
}

float getPWM(int cm) {
  if(cm>45)
    return 255;
  const int pwm = cm * 8;
  return pwm <= 0 ? 0 : pwm;
}

void initBack() {
  lcd.setCursor(0,0); 
  lcd.print(" ");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Reiniciando");
  analogWrite(Motor_R,255);
  analogWrite(Motor_L,255);
  for(int i = 0; i < 3; i++) {
    lcd.print(".");
    delay(1000);
  }
}
