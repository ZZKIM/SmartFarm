/* 동애등에 곤충 스마트팜 관리기 */
/* 들어가는 센서 종류 : 온습도 센서, 토양 수분 센서, 릴레이 모듈, LCD 모듈, 생장 LED */
/* 기능
 *  1. 온습도 센서를 이용해서 화분 주변 환경의 온습도를 측정한다.
 *  2. 토양 수분 센서를 이용해 토양의 수분을 측정한다.
 *  3. 발육최적온도 30도, 발육최적습도 40%를 유지하도록 한다.
 *  4. LCD 모듈을 통해서 온습도 센서와 토양 수분 센서 측정 값을 출력한다.
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>      // LCD 모듈 라이브러리
#include <DHT.h>                    // 온습도 센서 모듈 라이브러리
#include <Emotion_Farm.h>           // 특수 문자 및 이모티콘 라이브러리
 
// 센서 핀 설정
#define relayPin_LED 3    // 생장 LED 릴레이 모듈 핀
#define relayPin_Mist 5   // 분무 모듈 릴레이 모듈 핀
#define DHTPIN 4                    // 온습도센서 모듈 핀
#define DHTTYPE DHT11               // 온습도 센서타입 설정
#define soilmoisturePin A0          // 토양수분센서 핀

// 객체 생성
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD 초기화 (LCD 주소값, x축, y축)
DHT dht(DHTPIN, DHTTYPE);           //온습도 센서 모듈

// 문자열을 출력하기 위한 변수
char str_M[10];
char str_T[10];
char str_H[10];

void setup() {
  Serial.begin(9600);

  pinMode(relayPin_LED, OUTPUT);
  pinMode(relayPin_Mist, OUTPUT);
  pinMode(soilmoisturePin, INPUT);

  //LCD에 인트로 출력
  lcd.begin();
  lcd.clear();
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
  delay(500);
  lcd.setCursor(2,0);
  lcd.print("KOSAF");
  delay(1000);
  lcd.setCursor(8,0);
  lcd.print("SMART");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("FARM");
  delay(1000);
  lcd.clear();


  // 라이브러리로 추가한 특수 문자 및 이모티콘 추가
  lcd.createChar(0, temp);
  lcd.createChar(1, C);
  lcd.createChar(2, humi);  
  lcd.createChar(3, Qmark);
  lcd.createChar(4, water);
  lcd.createChar(5, good);
  lcd.createChar(6, wind);
}

void loop() {

  //센서값 측정
  int soilmoistureValue = analogRead(soilmoisturePin);               // 토양수분 값 측정: 0(습함) ~ 1023(건조)
  int soilmoisture_per = map(soilmoistureValue, 170, 1023, 100, 0);  // 센서 값을 퍼센트로 변경
  unsigned char h_Value = dht.readHumidity();                        // 공기 중 습도 값 측정
  unsigned char t_Value = dht.readTemperature();                     // 공기 중 온도 값 측정

  //LCD에 토양 습도값 출력
  lcd.setCursor(1,0);
  lcd.print("MOIST:");
  sprintf(str_M, "%03d", soilmoisture_per);
  lcd.print(str_M);
  lcd.setCursor(10,0);
  lcd.print("%");

  //LCD에 온도값 출력
  lcd.setCursor(1,1);
  lcd.write(0);
  sprintf(str_T, "%02d", t_Value);
  lcd.setCursor(3,1);
  lcd.print(str_T);
  lcd.write(1);

  //LCD에 습도값 출력
  lcd.setCursor(7,1);
  lcd.write(2);
  sprintf(str_H, "%02d", h_Value);
  lcd.setCursor(9,1);
  lcd.print(str_H);
  lcd.print("%");


  //토양습도 값에 따른 LCD에 이모티콘 띄우기
  if(soilmoisture_per >= 0 && soilmoisture_per < 30){
    lcd.setCursor(13,0);
    lcd.write(3);
    lcd.setCursor(14,0);
    lcd.write(4);
    digitalWrite(relayPin, HIGH); // 생장 LED 켜기
  }
  else if(soilmoisture_per >= 30 && soilmoisture_per < 70){
    lcd.setCursor(13,0);
    lcd.print(" ");
    lcd.setCursor(14,0);
    lcd.write(5);
    digitalWrite(relayPin, LOW); // 생장 LED 끄기
  }
  else if(soilmoisture_per >= 70){
    lcd.setCursor(13,0);
    lcd.write(3);
    lcd.setCursor(14,0);
    lcd.write(6);
     digitalWrite(relayPin, HIGH); // 생장 LED 켜기
  }
  Serial.println(soilmoisture_per);

// Check temperature and humidity thresholds
  if (t_Value > 30 || h_Value < 40) {
    lcd.setCursor(0, 0);
    lcd.print("Suboptimal Temp/Hum");

    if (h_Value < 30) {
      digitalWrite(relayPin_Mist, HIGH); // Turn on misting pump
      while (h_Value < 30) {
        h_Value = dht.readHumidity();
        delay(1000);
      }
      digitalWrite(relayPin_Mist, LOW); // Turn off misting pump
    }

    if (t_Value < 40) {
      digitalWrite(relayPin_LED, HIGH); // Turn on heater
      while (t_Value < 40) {
        t_Value = dht.readTemperature();
        delay(1000);
      }
      digitalWrite(relayPin_LED, LOW); // Turn off heater
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("                     "); // Clear the line
    digitalWrite(relayPin_Mist, LOW); // Turn off misting pump
    digitalWrite(relayPin_LED, LOW);  // Turn off heater
  }

  delay(500);
}
