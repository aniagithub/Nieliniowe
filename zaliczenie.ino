#include <LiquidCrystal.h>
//temperatura
#define TEMP0 13 
#define a -4.69511984002336e-07
#define b 0.000793445258411565
#define c -0.547618929211625
#define d 239.691651858058
#define HEATER_0 10
#define FAN 44    
LiquidCrystal lcd(16,17,23,25,27,29);
int k;
double temp,e,u;//temperatura,blad,sterowanie
double i=0;
double tempZadana=190;

//krancowki
#define X_MIN_PIN           3
#define X_MAX_PIN           2
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
//silniki
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

int x, y, z, xmin;
int kroki_x = 0, kroki_y = 0, kroki_z = 0;
int tmp;
bool wyslane = true;
String s;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(X_MAX_PIN, INPUT); //ustawienie pinu wejsciowego krancowki
  pinMode(Y_MAX_PIN, INPUT);
  pinMode(Z_MAX_PIN, INPUT);
  pinMode(X_MIN_PIN, INPUT);//krancowka dolna x jako przycisk awaryjny 
  digitalWrite(X_MAX_PIN, HIGH);
  digitalWrite(Y_MAX_PIN, HIGH);
  digitalWrite(Z_MAX_PIN, HIGH); //krancowki
  

  pinMode(X_ENABLE_PIN, OUTPUT); //ustawienie pinu wejsciowego silnika
  digitalWrite(X_ENABLE_PIN, LOW);
  pinMode(X_DIR_PIN, OUTPUT); //kierunek
  digitalWrite(X_DIR_PIN, LOW); //w gore
  pinMode(X_STEP_PIN, OUTPUT);

  pinMode(Y_ENABLE_PIN, OUTPUT); //ustawienie pinu wejsciowego silnika
  digitalWrite(Y_ENABLE_PIN, LOW);
  pinMode(Y_DIR_PIN, OUTPUT); //kierunek
  digitalWrite(Y_DIR_PIN, LOW); //w gore
  pinMode(Y_STEP_PIN, OUTPUT);

  pinMode(Z_ENABLE_PIN, OUTPUT); //ustawienie pinu wejsciowego silnika
  digitalWrite(Z_ENABLE_PIN, LOW);
  pinMode(Z_DIR_PIN, OUTPUT); //kierunek
  digitalWrite(Z_DIR_PIN, LOW); //w gore
  pinMode(Z_STEP_PIN, OUTPUT);

  
    lcd.begin(20,4);
    lcd.print("Hello");
    pinMode(HEATER_0, OUTPUT);
    digitalWrite(HEATER_0,LOW);
    pinMode(FAN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  x = digitalRead(X_MAX_PIN);
  y = digitalRead(Y_MAX_PIN);
  z = digitalRead(Z_MAX_PIN);
  xmin = digitalRead(X_MIN_PIN);//dodatkowa krancowka
  // Serial.print(x);
  // Serial.print(y);
  // Serial.println(z); //krancowki
  //robienie krokow
  digitalWrite(X_STEP_PIN, HIGH);
  digitalWrite(Y_STEP_PIN, HIGH);
  digitalWrite(Z_STEP_PIN, HIGH);
  digitalWrite(X_MIN_PIN, HIGH);
  delayMicroseconds(500);
  if (xmin == 0)
  {
    digitalWrite(X_ENABLE_PIN,HIGH);
    digitalWrite(Y_ENABLE_PIN,HIGH);
    digitalWrite(Z_ENABLE_PIN,HIGH);
  }
  else 
  {
    digitalWrite(X_ENABLE_PIN,LOW);
    digitalWrite(Y_ENABLE_PIN,LOW);
    digitalWrite(Z_ENABLE_PIN,LOW);
  }//dodatkowa krancowka - logika odwrotna, przy przycisnieciu przycisku silniki wylaczone
  
  if (x == 0 && kroki_x > 0)
  {
    digitalWrite(X_STEP_PIN, LOW);
    kroki_x--;
  }
  if (y == 0 && kroki_y > 0)
  {
    digitalWrite(Y_STEP_PIN, LOW);
    kroki_y--;
  }
  if (z == 0 && kroki_z > 0)
  {
    digitalWrite(Z_STEP_PIN, LOW);
    kroki_z--;
  }
  delayMicroseconds(500);
  //Serial.println(kroki_x);
  //Serial.println(kroki_y);
  //Serial.println(kroki_z);

  while (Serial.available() > 0)
  {
    s = Serial.readStringUntil('\n');
    sscanf(s.c_str(), "x %d y %d z %d \n", &kroki_x, &kroki_y, &kroki_z); //wczytywanie krokow z klawiatury
    wyslane = false;
    // .c_str = rzutowanie do string
    if (kroki_x < 0)
    {
      digitalWrite(X_DIR_PIN, HIGH);
      kroki_x = -kroki_x;
    }
    else
    {
      digitalWrite(X_DIR_PIN, LOW);
    }

    if (kroki_y < 0)
    {
      digitalWrite(Y_DIR_PIN, HIGH);
      kroki_y = -kroki_y;
    }
    else
    {
      digitalWrite(Y_DIR_PIN, LOW);
    }

    if (kroki_z < 0)
    {
      digitalWrite(Z_DIR_PIN, HIGH);
      kroki_z = -kroki_z;
    }
    else
    {
      digitalWrite(Z_DIR_PIN, LOW);
    }
   
    /*if(tmp>0)
      {
      kroki_x = tmp;
      kroki_y = tmp;
      kroki_z = tmp;
      }*/
  }
 if (kroki_x == 0 && kroki_y == 0 && kroki_z == 0 && wyslane == false)
    {
      Serial.println("OK");
      wyslane = true;
      
    }
 {
      k = analogRead(TEMP0);
  //y=ak^3+bk^2+ck+d
  temp = a*(double)pow(k,3)+b*(double)pow(k,2)+c*(double)k+d;
  
  e=tempZadana-temp;//blad
  u=250*e/(tempZadana-20) + i/7;//regulator P, gdzie P=250/80, I=1/7
  i=i+0.01*e;
  
  //Serial.println(k);
  lcd.setCursor(0,0);
  lcd.print("temp:");
  lcd.setCursor(6,0);
  lcd.print(temp);
  
  if (temp<tempZadana)
  { 
    analogWrite(HEATER_0, u>255 ? 255 : floor(u));//jesli u>255 to u=255, jesli nie to u
    // digitalWrite(FAN,LOW);
    
    lcd.setCursor(0,1);
    lcd.print("Grzalka  wlaczona");
    //lcd.setCursor(0,2);
    //lcd.print(u);
    //lcd.setCursor(0,3);
    //lcd.print(i);
  }
  else
  {
    digitalWrite(HEATER_0,LOW);
    // digitalWrite(FAN,HIGH);
    lcd.setCursor(0,1);
    lcd.print("Grzalka wylaczona");
  }
  delay(10);//calkuje co 10 ms
}
}

