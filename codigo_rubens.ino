#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definir variaveis

#define carga         8           // Digital 8 vai carregar o capacitor
#define descarga      9           // Digital 9 vai descarregar o capacitor
#define resistor       10000.0F    // Resistor de carga utilizado no projeto                                  
#define capLoad        A2         // medida de tensão (carga no capacitor) no pino analógico 0
#define tenLoad        A0      // leitura de tensão
#define res_divisor1   10000.0F   //
#define res_divisor2   100000.0F  //
#define bot_modo       7      // botao de modo  
#define bot_escala     6      // botao de escala

byte ohm[8] = {
      B00000,
      B01110,
      B10001,
      B10001,
      B10001,
      B01010,
      B11011,
      B00000
};

double pulse, frequency, capacitance, inductance;

void setup() {
  lcd.createChar(0,ohm);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
  pinMode(carga,OUTPUT);
  digitalWrite(carga,0);

  lcd.home();
  lcd.print("Proj. Multimetro");
  lcd.setCursor(0,1);
  lcd.print("Feito por:");
  delay(1500);
  lcd.clear();
  lcd.home();
  lcd.print("Joao Paiva &");
  lcd.setCursor(0,1);
  lcd.print("Luan Crozatti");
  delay(1500);
  lcd.clear();
  
}

void loop(){
  int modo = 0;
  int escala = 0;
  while(1){
    if (modo == 0){
      float tensao = ((analogRead(A1)*5.0*110000)/(1023.0*10000));
      Serial.println(tensao);
      delay(200);
      if (tensao*10 < 10){
        lcd.clear();
        while(tensao*10<10){
          tensao = ((analogRead(A1)*5.0*110000)/(1023.0*10000));
          lcd.home();
          lcd.print("-----Tensao-----");  
          lcd.setCursor(0,1);
          lcd.print(tensao*1000);
          lcd.print(" mV");
          if (!digitalRead(bot_modo)){
            lcd.clear();
            modo++;
            Serial.println(modo);
            delay(500);
            break;
          }
        }
      }
      else{
        lcd.clear();
        while(tensao*10>=10){
          tensao = ((analogRead(A1)*5.0*110000)/(1023.0*10000));
          lcd.home();
          lcd.print("-----Tensao-----");
          lcd.setCursor(0,1);
          lcd.print(tensao);
          lcd.print(" V");
          if (!digitalRead(bot_modo)){
            lcd.clear();
            modo++;
            Serial.println(modo);
            delay(500);
            break;
          }
        }
      }
    }

    else if(modo == 1){
      float vin = analogRead(A0)*5.0/1023.0;  
      float resistencia = (((50000*5.0)- (vin*50000))/vin);
      Serial.println(resistencia);
      if(resistencia/1000 > 1 && resistencia/1000000 < 1){
        lcd.clear();
        while(1){
          float vin = analogRead(A0)*5.0/1023.0;  
          float resistencia = (((50000*5.0)- (vin*50000))/vin);
          lcd.home();
          lcd.print("--Resistencia---");
          lcd.setCursor(0,1);
          lcd.print(resistencia/1000);
          lcd.write(" k");
          lcd.write(byte(0));
          if (!digitalRead(bot_modo)){
            lcd.clear();
            modo++;
            delay(100);
            break;
          }
          else if(resistencia/1000000 >= 1 || resistencia/1000 < 1){
            break;
          }
        }
      }
      else if(resistencia/1000000 >= 1){
        lcd.clear();
        while(1){
          float vin = analogRead(A0)*5.0/1023.0;  
          float resistencia = (((50000*5.0)- (vin*50000))/vin);
          lcd.home();
          lcd.print("--Resistencia---");
          lcd.setCursor(0,1);
          lcd.print(resistencia/1000000);
          lcd.write(" M");
          lcd.write(byte(0));
          if (!digitalRead(bot_modo)){
            lcd.clear();
            modo++;
            delay(100);
            break;
          }
          else if(resistencia/1000000 < 1){
            break;
          }
          
        }
      }
      else if(resistencia < 1000){
        float vin = analogRead(A0)*5.0/1023.0;  
        float resistencia = (((50000*5.0)- (vin*50000))/vin);
        lcd.clear();
        while(1){
          float vin = analogRead(A0)*5.0/1023.0;  
          float resistencia = (((50000*5.0)- (vin*50000))/vin);
          Serial.println(resistencia);
          lcd.home();
          lcd.print("Resistencia");
          lcd.setCursor(0,1);
          lcd.print(resistencia);
          lcd.print(" ");
          lcd.write(byte(0));
          if (!digitalRead(bot_modo)){
            lcd.clear();
            modo++;
            delay(100);
            break;
          }
          else if(resistencia>1000){
            break;
          }
        }
      }
    }

    else if(modo == 2){
      Serial.println(modo);
      lcd.home();
      lcd.print("--Capacitancia--");
      // Inicia o contador e carrega o capacitor ate chegar em 63% da carga
      int Time01 = millis();   
      digitalWrite(carga, 1);
      lcd.setCursor(0,1);
      lcd.print("Aguarde...");
      while(analogRead(capLoad) < 648){}  
      int Time02 = millis() - Time01;
      float microFarads = ((float)Time02 / resistor) * 1000;   
      // Descarrega o capacitor
      digitalWrite(carga, 0);               
      pinMode(descarga, OUTPUT);             
      digitalWrite(descarga, 0);             
      while(analogRead(capLoad) > 0){ }   
      pinMode(descarga, INPUT);                
      delay(300); 
      if(microFarads < 1)
        while (digitalRead(bot_modo)){
          lcd.setCursor(0,1);              
          lcd.print(microFarads*1000);   
          lcd.print(" nF  ");   
        }
      else{
        while (digitalRead(bot_modo)){
          lcd.setCursor(0,1);              
          lcd.print(microFarads);   
          lcd.print(" uF  ");   
        }
      }
      lcd.clear();
      delay(300);
      modo++; 
    }
    else if(modo == 3){
      Serial.println(modo);
      float sensibilidade = 0.2;
      float leitura = (analogRead(A3)/1023.)*5.;
      float corrente = (leitura - 2.5)/sensibilidade;
      lcd.clear();
      lcd.print("----Corrente----");
      Serial.println(corrente);
      while(1){
           float leitura = (analogRead(A3)/1024.)*5.;
           float corrente = (leitura - 2.5)/sensibilidade;
           if(corrente > 1){
              break;
           }
           lcd.setCursor(0,1);              
           lcd.print(corrente*1000);   
           lcd.print(" mA ");  
           if(!digitalRead(bot_modo)){
              lcd.clear();
              delay(300);
              modo++;
              break;
            }
          }
       while(corrente > 1){
            float leitura = (analogRead(A3)/1024.)*5.;
            float corrente = (leitura - 2.5)/sensibilidade;
            if(corrente < 1){
              break;
            }
            lcd.setCursor(0,1);              
            lcd.print(corrente);   
            lcd.print(" A ");  
            if(!digitalRead(bot_modo)){
              lcd.clear();
              delay(300);
              modo++;
              break;
            }
          }
    }
    else if(modo == 4){
        lcd.clear();
        lcd.home();
        lcd.print("---Indutancia---"); 
        digitalWrite(13, HIGH);
        delay(5);
        digitalWrite(13,LOW);
        delayMicroseconds(100); 
        pulse = pulseIn(10,HIGH,5000);
        if(pulse > 0.1){ 
        capacitance = 2.E-6;
        
        
        frequency = 1.E6/(2*pulse);
        inductance = 1./(capacitance*frequency*frequency*4.*3.14159*3.14159);
        inductance *= 1E6;

        while(inductance < 1000){
           lcd.setCursor(0,1);              
           lcd.print(inductance);   
           lcd.print(" uH ");  
           if(!digitalRead(bot_modo)){
              lcd.clear();
              delay(300);
              modo = 0;
              break;
            }
          }
        while(inductance > 1000){
            lcd.setCursor(0,1);              
            lcd.print(inductance/1000);   
            lcd.print(" mH ");  
            if(!digitalRead(bot_modo)){
              lcd.clear();
              delay(300);
              modo = 0;
              break;
            }
          }
      }
    }
  }
}
