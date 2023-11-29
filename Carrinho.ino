#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "Ultrasonic.h"
#include <MFRC522.h>
#include <StackArray.h>
#include <SPI.h>
// DEFINIÇÕES
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
StackArray<String> uidStack;

// DEFINIÇÕES DE PINOS
#define MotorDir 5
#define MotorEsq 6

#define SensorLDir A0
#define SensorLEsq A2

unsigned long tempo; //Variável global
const unsigned long period = 120000;  //Constante global em milissegundos
int distancia;

// DEFINIÇÕES
#define LINHA 0



#define FRENTE  1
#define PARADO  0
#define TRAS   -1

#define RST_PIN   9
#define SS_PIN    10

MFRC522 mfrc522(SS_PIN, RST_PIN);

// DECLARAÇÃO DE FUNÇÕES
void configMotor();
static boolean flag = true;

#define velocidade 255

Ultrasonic ultrassom(2,7);

LiquidCrystal_I2C lcd(endereco, colunas, linhas);

typedef struct NO{
  int dado;
  struct NO *prox;
}NO;

typedef struct PILHA{
  NO *topo;
}PILHA;

void inicializaPilha(PILHA *p){
  p->topo = NULL;
}

void empilha (int dado, PILHA *p){
  NO ptr = (NO) malloc(sizeof(NO));
  if(ptr == NULL){
    printf("Erro de alocação de nó. \n");
    return;
  } else {
    ptr->dado = dado;
    ptr->prox = p->topo;
    p->topo = ptr;
  }
}

int desempilha(PILHA *p){
  NO* ptr = p->topo;
  int dado;
  if(ptr == NULL){
    printf("Pilha vazia. \n");
    return;
  } else {
    p -> topo = ptr->prox;
    ptr->prox = NULL;
    dado = ptr->dado;
    free(ptr);
    return dado;
  }
}

void imprimePilha(PILHA *p){
  NO *ptr = p->topo;
  int tr;
  if(ptr == NULL){
    printf("Pilha vazia. \n");
    lcd.clear();
    lcd.print("Pilha vazia.");
    return;
  } else {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    while(ptr != NULL){
      printf("%d", ptr->dado);
      tr = ptr->dado;
      lcd.print(tr);
      lcd.print(" ");
      ptr = ptr->prox;
    }
    printf("\n");
  }
}


void setup() {
  Serial.begin(9600);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  lcd.init(); // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight(); // LIGA A ILUMINAÇÃO DO DISPLAY
  lcd.clear(); // LIMPA O DISPLAY
  tempo = 0;
  lcd.setCursor(0, 0);
  lcd.print("OBJ: COLHER DADOS");
  pinMode(MotorDir, OUTPUT);
  pinMode(MotorEsq, OUTPUT);
  analogWrite(MotorEsq, 0);
  analogWrite(MotorDir, 0);
  delay(5000);
  pinMode(SensorLEsq, INPUT);
  pinMode(SensorLDir, INPUT);
  Serial.println("Passe um cartão no leitor para obter algumas informações dele.");
}

void loop() {
  bool valE = digitalRead(SensorLEsq);
  bool valD = digitalRead(SensorLDir);
  distancia = ultrassom.Ranging(CM);

  if((distancia >  10)){
    tempo = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    if (tempo < period){
      if (valE == LINHA && valD == LINHA) {
        analogWrite(MotorEsq, 200);
        analogWrite(MotorDir, 200);
        lcd.print("OS DOIS NO BRANCO"); 
        lcd.setCursor(0,1);
        lcd.print(LINHA); 
        lcd.setCursor(3,1);
        lcd.print(valE);
        lcd.setCursor(5,1);
        lcd.print(valD);
        lcd.setCursor(7,1);
        lcd.print("t");
        lcd.print(tempo);
        delay(300);
      } 
      if (valE == LINHA && valD != LINHA) {
        analogWrite(MotorEsq, 200);
        analogWrite(MotorDir, 150);
        lcd.print("DIREITO NO BRANCO"); 
        lcd.setCursor(0,1);
        lcd.print(LINHA); 
        lcd.setCursor(3,1);
        lcd.print(valE);
        lcd.setCursor(5,1);
        lcd.print(valD);
        lcd.setCursor(7,1);
        lcd.print("t");
        lcd.print(tempo);
      } 
      if (valE != LINHA && valD == LINHA) {
        analogWrite(MotorEsq, 150);
        analogWrite(MotorDir, 200);
        lcd.print("ESQUERDO NO BRANCO"); 
        lcd.setCursor(0,1);
        lcd.print(LINHA); 
        lcd.setCursor(3,1);
        lcd.print(valE);
        lcd.setCursor(5,1);
        lcd.print(valD);
        lcd.setCursor(7,1);
        lcd.print("t");
        lcd.print(tempo);
      }
       if (valE != LINHA && valD != LINHA){
        analogWrite(MotorDir, 200);
        analogWrite(MotorEsq, 200);
        lcd.print("DOIS NO PRETO");
        lcd.setCursor(0,1);
        lcd.print(LINHA); 
        lcd.setCursor(3,1);
        lcd.print(valE);
        lcd.setCursor(5,1);
        lcd.print(valD);
        lcd.setCursor(7,1);
        lcd.print("t");
        lcd.print(tempo);
      }
    } else {
        analogWrite(MotorDir, 0);
        analogWrite(MotorEsq, 0);
        lcd.clear();
        lcd.print("Acabou o tempo");
        lcd.clear();
        PILHA p1 = (PILHA) malloc(sizeof(PILHA));
        if(p1 == NULL){
          printf("Erro de alocação da pilha. \n");
          exit(0);
        } else {
          inicializaPilha(p1);
          empilha(1, p1);
          empilha(2, p1);
          empilha(3, p1);
          empilha(4, p1);
          empilha(5, p1);

          imprimePilha(p1);
          delay(10000);
          desempilha(p1);
          imprimePilha(p1);
          delay(3000);
          desempilha(p1);
          imprimePilha(p1);
          delay(3000);
          desempilha(p1);
          imprimePilha(p1);
          delay(3000);
          desempilha(p1);
          imprimePilha(p1);
          delay(3000);
          desempilha(p1);
          imprimePilha(p1);
        } 
      }  
  } else{   
        delay(300);
        analogWrite(MotorDir, 50);
        analogWrite(MotorEsq, 50);
        Serial.println("PAROU PARA EVITAR A BATIDA"); 
        lcd.clear();
        lcd.print("PAROU"); 
        delay(500); // DELAY DE 5 SEGUNDOS
        lcd.setCursor(0, 1); // POSICIONA O CURSOR NA PRIMEIRA COLUNA DA LINHA 2
        lcd.print(LINHA); 
        lcd.setCursor(3,1);
        lcd.print(valE);
        lcd.setCursor(5,1);
        lcd.print(valD);
        lcd.setCursor(7,1);
        lcd.print("t");
        lcd.print(tempo);
        lcd.setCursor(7,0);
        lcd.print(distancia);
      }
  delay(255);      
}
