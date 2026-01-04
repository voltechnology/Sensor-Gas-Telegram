#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

int ambang = 1900;

#define gas 34
#define led 18

#define ssid "KOS KEJAWAN 24"
#define password "cleanhouse"

#define token "GANTI TOKEN KAMU DISINI"
#define id "GANTI ID KAMU DISINI"

bool kirimaman = true;
unsigned long awal = 0;
const unsigned long terakhir = 500;

LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClientSecure client;
UniversalTelegramBot bot(token, client);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(gas, INPUT);
  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

// KONEKSI WIFI
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("MENGHUBUNGKAN...");
    lcd.setCursor(0, 1);
  for(int L = 0; L < 16; L++){
    lcd.write(byte(255));
    delay(100);
    }
  }

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("MENGIRIM PESAN");
  lcd.setCursor(4, 1);
  lcd.print("TELEGRAM");

  digitalWrite(led, HIGH);
  delay(700);
  digitalWrite(led, LOW);
  delay(80);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(80);

// KIRIM PESAN AWAL KE TELEGRAM
  bot.sendMessage(id, "HALLO VOLTECH!!🫡");
  bot.sendMessage(id, "SENSOR SIAP DIGUNAKAN😉😁");
  lcd.clear();
}

void loop() {
  const int total = 30;
  int rata = 0;

// HASIL RATA-RATA NILAI OUTPUT
  for(int i = 0; i < total; i++){
    rata += analogRead(gas);
    delay(5);
  }

  int snsr = rata/total;
  char wadah[20];
  unsigned long sekarang = millis();

  if(snsr > ambang){
    for(int i = 0; i < 4; i++){
      digitalWrite(led, HIGH);
      delay(200);
      digitalWrite(led, LOW);
      delay(100);

// TAMPILAN LCD (OPSIONAL) 
    }
      lcd.setCursor(0, 0);
      lcd.print("AWAS GAS BOCOR!!");
      lcd.setCursor(0, 1);
      sprintf(wadah, "HATI - HATI %d   ", snsr);
      lcd.print(wadah);
      lcd.print("           ");
      
// KIRIM PESAN KE TELEGRAM JIKA ADA GAS BOCOR
    if(sekarang - awal >= terakhir){
      String anu = "AWAS GAS BOCOR!!!⚠️\nNILAI : " + String(snsr);
      bot.sendMessage(id, anu);
      awal = sekarang;
    }
    kirimaman = false;

  }else{
    lcd.setCursor(0, 0);
    lcd.print("MONITORING GAS  ");
    lcd.setCursor(0, 1);
    lcd.print("NILAI GAS : ");
    lcd.print(snsr);
    digitalWrite(led, LOW);

//KIRIM PESAN AMAN JIKA TIDAK ADA GAS
    if(!kirimaman){
    bot.sendMessage(id, "SUDAH AMAN😁👍");
    kirimaman = true;
    }
  }

  delay(500);
}
