#include <Ultrasonic.h>

//[B - Trás] [A - Frente]
const int motor1B = 2;
const int motor1A = 3;
const int motor2A = 4;
const int motor2B = 5;

const int dist_trigger = 8;
const int dist_echo = 9;
int distancia = 0;
//Classifica nome do sensor
Ultrasonic ultra(8,9);

const int led_vermelho = 10;
const int led_verde = 11;
extern bool v_led_vermelho = 0;
extern bool v_led_verde = 1;

const int senA = 6;
const int senB = 7;
bool v_senA = 0;
bool v_senB = 0;

const int vel = 250;

int cont = 0;

void setup() {
  Serial.begin(9600);
  //Divisão de pinos como Entrada e Saída de dados
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  pinMode(senA, INPUT);
  pinMode(senB, INPUT);

  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);

  //Delay de 5s para começar
  delay(5000);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor2A, HIGH);
  delay(3000);
}

void loop() {

  //Receber valores dos sensores como 0 e 1 (sendo 0 para cores claras)
  v_senA = digitalRead(senA); 
  v_senB = digitalRead(senB);

  //Receber valores dos LEDS
  v_led_vermelho = digitalRead(led_vermelho);    
  v_led_verde = digitalRead(led_verde);
    
  //Identificar no Monitor Serial os atuais valores dos Sensores
  Serial.print("Sensor 1 = ");
  Serial.print(v_senA);
  Serial.print(" | ");
  Serial.print("Sensor 2 = ");
  Serial.println(v_senB);


  digitalWrite(led_verde, HIGH);
  digitalWrite(led_vermelho, LOW);


  cont+=1;
  Serial.print("CONTADOR = ");
  Serial.println(cont);

  //Condições para Motores
    if((v_senA == 1) && (v_senB == 1)) {
    analogWrite(motor1A, LOW);
    analogWrite(motor2A, LOW);
    analogWrite(motor1B, vel);
    analogWrite(motor2B, vel);
    Serial.println("[RÉ]");
  }
  
  if((v_senA == 0) && (v_senB == 0)) {
    analogWrite(motor1B, LOW);
    analogWrite(motor2B, LOW);
    analogWrite(motor1A, vel);
    analogWrite(motor2A, vel);
    Serial.println("[RETA]");
  }
  
  if((v_senA == 0) && (v_senB == 1)) {
    analogWrite(motor1B, LOW);
    analogWrite(motor2B, LOW);
    analogWrite(motor1A, 0);
    analogWrite(motor2A, vel);
    Serial.println("[CURVA ESQUERDA]");
  }
  if((v_senA == 1) && (v_senB == 0)) {
    analogWrite(motor1B, LOW);
    analogWrite(motor2B, LOW);
    analogWrite(motor1A, vel);
    analogWrite(motor2A, 0);
    Serial.println("[CURVA DIREITA]");
  }

  distancia = ultra.Ranging(CM);
  
  if((distancia > 0) && (distancia <= 12)){
    digitalWrite(led_vermelho, HIGH);
    analogWrite(motor1B, LOW);    
    analogWrite(motor2B, LOW);    
    analogWrite(motor2A, LOW);    
    analogWrite(motor1A, LOW);
    Serial.print("OBJETO IDENTIFICADO | ");
    Serial.print(distancia);
    Serial.println(" CM");
    Serial.println(millis());
    Serial.println("  ");
    
    }
    else {
      Serial.print("NADA ENCONTRADO | ");
      Serial.print(distancia);
      Serial.println(" CM");
      Serial.println("  ");
    }




}

