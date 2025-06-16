#define BLYNK_TEMPLATE_ID "TMPL68PCnuec0"
#define BLYNK_TEMPLATE_NAME "Kebersihan Air"
#define BLYNK_AUTH_TOKEN "7LnHfk-i4kazPzvzVkYfROjWCOPgc31-"

// Aktifkan baris ini untuk menampilkan detail di Serial Monitor
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
//Library untuk LCD
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//library untuk Sensor Suhu DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

//pakai library DIY (Do It Yourself, atau library yang di buat sendiri)
#include "fuzzy.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "GAS";
char pass[] = "1sampai8";

const int ph_Pin  = A0; //pin pH sensor
const int oneWireBus = 32; //pin suhu sensor
const int turbidityPin = 34; //pin turbidty sensor
const int ledRed = 12; // pin LED Merah
const int ledGreen = 13; // pin LED Hijau
const int ledYellow = 14; // pin LED Biru
const int buzzer = 27; //pin Buzzer

//Object LCD
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//Object Suhu
OneWire oneWire(oneWireBus);
DallasTemperature suhu(&oneWire);
int temperatureC,temperatureF;


//variable ph
int Po = 0;
float PH_step;
int nilai_analog_PH;
double TeganganPh;

//variabel Turbidity
int ntu;
// untuk kalibrasi esp32
float PH4 = 3.150;
float PH7 = 2.555;

// untuk kalibrasi esp8266
// float PH4 = 3.216;
// float PH7 = 2.748;
// Fungsi baru untuk menangani koneksi WiFi secara terpisah

int random_Number = random(1,100);

BlynkTimer timer;

void checkBlynkStatus(){ // called every 2 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    Serial.println("Blynk Not Connected");
  }
  if (isconnected == true)
  {
    Serial.println(" Blynk IoT Connected ");
    
    
  }

}

void connect() {
    lcd.setCursor(0, 0);
    lcd.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(11 + i, 0); // posisi titik mulai dari kolom ke-11
      lcd.print(".");
      delay(650); // delay untuk efek animasi
    }
    
    lcd.setCursor(11, 0);
    lcd.print("    "); // hapus semua titik

    delay(250); // delay sebelum memulai ulang animasi
  }
 
}

void rgb (bool red, bool green, bool yellow){
  digitalWrite(ledRed, red);
  digitalWrite(ledGreen, green);
  digitalWrite(ledYellow, yellow);
}


//======================Function Sensor======================
void temperature() { 
  suhu.requestTemperatures();
  temperatureC = suhu.getTempCByIndex(0);
  // temperatureF = suhu.getTempFByIndex(0);
  // temperatureC = 20;
  Serial.println("📡 Sensor Suhu");
  Serial.print  ("  Suhu (°C)  : "); Serial.println(temperatureC);
  // Serial.print  ("  Temp (°F)  : "); Serial.println(temperatureF);
  Serial.println();
  lcd.setCursor(0,0);
  lcd.print("S:");
  lcd.print(temperatureC);
  // lcd.setCursor(7,0);
  // lcd.print("|");
  Blynk.virtualWrite(V0, temperatureC);
}

void pH_Sensor() {
  nilai_analog_PH = analogRead(ph_Pin);
  TeganganPh = 3.3 / 4096.0 * nilai_analog_PH;
  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - TeganganPh) / PH_step);     

  // Po = 7;
  Serial.println("🧪 Sensor pH");
  Serial.print  ("  Tegangan   : "); Serial.println(TeganganPh, 3);
  Serial.print  ("  ADC Value  : "); Serial.println(nilai_analog_PH);
  Serial.print  ("  pH         : "); Serial.println(Po );
  Serial.println();
  lcd.setCursor(6,0);
  lcd.print("P:");
  lcd.print(Po);
  Blynk.virtualWrite(V1, Po);
}

void tbdy() {
  int rawValue = analogRead(turbidityPin);
  ntu = map(rawValue, 1630, 0, 1, 100);
  // ntu = 20;
  if (ntu < 0) ntu = 0;

  Serial.println("🌫️ Sensor Kekeruhan");
  Serial.print  ("  ADC Value  : "); Serial.println(rawValue);
  Serial.print  ("  NTU        : "); Serial.println(ntu);
  Serial.println();
  lcd.setCursor(11,0);
  lcd.print("T:");
  lcd.print(ntu);
  // lcd.setCursor(5,1);
  // lcd.print("|");
  Blynk.virtualWrite(V2, ntu);
}
//============================================================



//======================= TAHAP FUZZIFIKASI =========================
void fuzzyfikasi(){
        shDingin = sh_Dingin(temperatureC);
        shSedang = sh_Sedang(temperatureC);
        shPanas = sh_Panas(temperatureC);

        phAsam = ph_Asam(Po);
        phNormal = ph_Normal(Po);
        phBasa = ph_Basa(Po);

        tbTinggi = tb_Tinggi(ntu);
        tbRendah = tb_Rendah(ntu);
        tbSedang = tb_Sedang(ntu);

        Serial.println("\n==================== TAHAP FUZZIFIKASI ====================");

        // Suhu
        Serial.println("Suhu (°C)");
        Serial.println("| Dingin | Sedang | Panas  |");
        Serial.println("|--------|--------|--------|");
        Serial.print ("|   "); Serial.print(shDingin); Serial.print("    |   ");
        Serial.print(shSedang); Serial.print("|   ");
        Serial.print(shPanas); Serial.println("    |");
        Serial.println();

        // pH
        Serial.println("pH");
        Serial.println("| Asam   | Netral | Basa   |");
        Serial.println("|--------|--------|--------|");
        Serial.print ("|   "); Serial.print(phAsam); Serial.print("    |   ");
        Serial.print(phNormal); Serial.print("|   ");
        Serial.print(phBasa); Serial.println("    |");
        Serial.println();

        // Kekeruhan
        Serial.println("Kekeruhan (NTU)");
        Serial.println("| Rendah | Sedang | Tinggi |");
        Serial.println("|--------|--------|--------|");
        Serial.print ("|   "); Serial.print(tbRendah); Serial.print("    |   ");
        Serial.print(tbSedang); Serial.print("|   ");
        Serial.print(tbTinggi); Serial.println("    |");

        

}
// =========================== TAHAP INFERENCE =========================
void inference(){
        // SUHU: DINGIN
        a_pred1  = pb(shDingin, phAsam,  tbRendah);  // buruk
        a_pred2  = pb(shDingin, phAsam,  tbSedang);  // buruk
        a_pred3  = pb(shDingin, phAsam,  tbTinggi);  // buruk
        a_pred4  = pb(shDingin, phNormal,tbRendah);  // layak
        a_pred5  = pb(shDingin, phNormal,tbSedang);  // sedang
        a_pred6  = pb(shDingin, phNormal,tbTinggi);  // buruk
        a_pred7  = pb(shDingin, phBasa,  tbRendah);  // buruk
        a_pred8  = pb(shDingin, phBasa,  tbSedang);  // buruk
        a_pred9  = pb(shDingin, phBasa,  tbTinggi);  // buruk

        // SUHU: SEDANG
        a_pred10 = pb(shSedang, phAsam,  tbRendah);  // sedang
        a_pred11 = pb(shSedang, phAsam,  tbSedang);  // buruk
        a_pred12 = pb(shSedang, phAsam,  tbTinggi);  // buruk
        a_pred13 = pb(shSedang, phNormal,tbRendah);  // layak
        a_pred14 = pb(shSedang, phNormal,tbSedang);  // layak
        a_pred15 = pb(shSedang, phNormal,tbTinggi);  // buruk
        a_pred16 = pb(shSedang, phBasa,  tbRendah);  // sedang
        a_pred17 = pb(shSedang, phBasa,  tbSedang);  // buruk
        a_pred18 = pb(shSedang, phBasa,  tbTinggi);  // buruk

        // SUHU: PANAS
        a_pred19 = pb(shPanas,  phAsam,  tbRendah);  // buruk
        a_pred20 = pb(shPanas,  phAsam,  tbSedang);  // buruk
        a_pred21 = pb(shPanas,  phAsam,  tbTinggi);  // buruk
        a_pred22 = pb(shPanas,  phNormal,tbRendah);  // layak
        a_pred23 = pb(shPanas,  phNormal,tbSedang);  // sedang
        a_pred24 = pb(shPanas,  phNormal,tbTinggi);  // buruk
        a_pred25 = pb(shPanas,  phBasa,  tbRendah);  // buruk
        a_pred26 = pb(shPanas,  phBasa,  tbSedang);  // buruk
        a_pred27 = pb(shPanas,  phBasa,  tbTinggi);  // buruk



        z1  = buruk(a_pred1);
        z2  = buruk(a_pred2);
        z3  = buruk(a_pred3);
        z4  = layak(a_pred4);
        z5  = sedang(a_pred5);
        z6  = buruk(a_pred6);
        z7  = buruk(a_pred7);
        z8  = buruk(a_pred8);
        z9  = buruk(a_pred9);
        z10 = sedang(a_pred10);
        z11 = buruk(a_pred11);
        z12 = buruk(a_pred12);
        z13 = layak(a_pred13);
        z14 = layak(a_pred14);
        z15 = buruk(a_pred15);
        z16 = sedang(a_pred16);
        z17 = buruk(a_pred17);
        z18 = buruk(a_pred18);
        z19 = buruk(a_pred19);
        z20 = buruk(a_pred20);
        z21 = buruk(a_pred21);
        z22 = layak(a_pred22);
        z23 = sedang(a_pred23);
        z24 = buruk(a_pred24);
        z25 = buruk(a_pred25);
        z26 = buruk(a_pred26);
        z27 = buruk(a_pred27);



        Serial.println("===============================================================");
        Serial.println("2) Tahap Inferensi (Rule Evaluation)");
        Serial.println("Rule  | IF (Suhu & pH & Kekeruhan)         THEN   | a_predikat | Z");
        Serial.println("---------------------------------------------------------------");

        Serial.println("[R1]  IF Dingin & Asam & Rendah          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred1);  Serial.print("     | "); Serial.println(z1);

        Serial.println("[R2]  IF Dingin & Asam & Sedang          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred2);  Serial.print("     | "); Serial.println(z2);

        Serial.println("[R3]  IF Dingin & Asam & Tinggi          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred3);  Serial.print("     | "); Serial.println(z3);

        Serial.println("[R4]  IF Dingin & Normal & Rendah        THEN LAYAK");  
        Serial.print ("      |                                    "); Serial.print(a_pred4);  Serial.print("     | "); Serial.println(z4);

        Serial.println("[R5]  IF Dingin & Normal & Sedang        THEN SEDANG");  
        Serial.print ("      |                                    "); Serial.print(a_pred5);  Serial.print("     | "); Serial.println(z5);

        Serial.println("[R6]  IF Dingin & Normal & Tinggi        THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred6);  Serial.print("     | "); Serial.println(z6);

        Serial.println("[R7]  IF Dingin & Basa & Rendah          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred7);  Serial.print("     | "); Serial.println(z7);

        Serial.println("[R8]  IF Dingin & Basa & Sedang          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred8);  Serial.print("     | "); Serial.println(z8);

        Serial.println("[R9]  IF Dingin & Basa & Tinggi          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred9);  Serial.print("     | "); Serial.println(z9);

        Serial.println("[R10] IF Sedang & Asam & Rendah          THEN SEDANG");  
        Serial.print ("      |                                    "); Serial.print(a_pred10); Serial.print("     | "); Serial.println(z10);

        Serial.println("[R11] IF Sedang & Asam & Sedang          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred11); Serial.print("     | "); Serial.println(z11);

        Serial.println("[R12] IF Sedang & Asam & Tinggi          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred12); Serial.print("     | "); Serial.println(z12);

        Serial.println("[R13] IF Sedang & Normal & Rendah        THEN LAYAK");  
        Serial.print ("      |                                    "); Serial.print(a_pred13); Serial.print("     | "); Serial.println(z13);

        Serial.println("[R14] IF Sedang & Normal & Sedang        THEN LAYAK");  
        Serial.print ("      |                                    "); Serial.print(a_pred14); Serial.print("     | "); Serial.println(z14);

        Serial.println("[R15] IF Sedang & Normal & Tinggi        THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred15); Serial.print("     | "); Serial.println(z15);

        Serial.println("[R16] IF Sedang & Basa & Rendah          THEN SEDANG");  
        Serial.print ("      |                                    "); Serial.print(a_pred16); Serial.print("     | "); Serial.println(z16);

        Serial.println("[R17] IF Sedang & Basa & Sedang          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred17); Serial.print("     | "); Serial.println(z17);

        Serial.println("[R18] IF Sedang & Basa & Tinggi          THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred18); Serial.print("     | "); Serial.println(z18);

        Serial.println("[R19] IF Panas & Asam & Rendah           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred19); Serial.print("     | "); Serial.println(z19);

        Serial.println("[R20] IF Panas & Asam & Sedang           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred20); Serial.print("     | "); Serial.println(z20);

        Serial.println("[R21] IF Panas & Asam & Tinggi           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred21); Serial.print("     | "); Serial.println(z21);

        Serial.println("[R22] IF Panas & Normal & Rendah         THEN LAYAK");  
        Serial.print ("      |                                    "); Serial.print(a_pred22); Serial.print("     | "); Serial.println(z22);

        Serial.println("[R23] IF Panas & Normal & Sedang         THEN SEDANG");  
        Serial.print ("      |                                    "); Serial.print(a_pred23); Serial.print("     | "); Serial.println(z23);

        Serial.println("[R24] IF Panas & Normal & Tinggi         THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred24); Serial.print("     | "); Serial.println(z24);

        Serial.println("[R25] IF Panas & Basa & Rendah           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred25); Serial.print("     | "); Serial.println(z25);

        Serial.println("[R26] IF Panas & Basa & Sedang           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred26); Serial.print("     | "); Serial.println(z26);

        Serial.println("[R27] IF Panas & Basa & Tinggi           THEN BURUK");  
        Serial.print ("      |                                    "); Serial.print(a_pred27); Serial.print("     | "); Serial.println(z27);

        Serial.println("===============================================================");
}

 // ================ TAHAP DEFFUZIFIKASI =======================
void defuzifikasi() {
   zTerbobot = ((a_pred1*z1)+(a_pred2*z2)+(a_pred3*z3)+(a_pred4*z4)+(a_pred5*z5)
   +(a_pred6*z6)+(a_pred7*z7)+(a_pred8*z8)+(a_pred9*z9)+(a_pred10*z10)+(a_pred11*z11)
   +(a_pred12*z12)+(a_pred13*z13)+(a_pred14*z14)+(a_pred15*z15)+(a_pred16*z16)+(a_pred17*z17)
   +(a_pred18*z18)+(a_pred19*z19)+(a_pred20*z20)+(a_pred21*z21)+(a_pred22*z22)+(a_pred23*z23)
   +(a_pred24*z24)+(a_pred25*z25)+(a_pred26*z26)+(a_pred27*z27))  / 
   (a_pred1+a_pred2+a_pred3+a_pred4+a_pred5+a_pred6+a_pred7+a_pred8+a_pred9+a_pred10+a_pred11
   +a_pred12+a_pred13+a_pred14+a_pred15+a_pred16+a_pred17+a_pred18+a_pred19+a_pred20+a_pred21
   +a_pred22+a_pred23+a_pred24+a_pred25+a_pred26+a_pred27);


  Serial.println("📊 Tahap Defuzzifikasi");
  Serial.print("  Persentase Kualitas Air : ");
  Serial.print(zTerbobot, 1);
  Serial.println(" %");
  lcd.setCursor(0,1);
  lcd.print("Pot:");
  lcd.setCursor(4,1);
  lcd.print(zTerbobot, 1);
  lcd.print("%");
  Blynk.virtualWrite(V4, zTerbobot);

  Serial.print  ("  Kualitas   : ");

  if (zTerbobot >= 0 && zTerbobot <= 30) {
    Serial.println("layak");
    lcd.setCursor(12, 1);
    lcd.print("Good");
    Blynk.virtualWrite(V3, "Layak");
    rgb(0,1,0);//lampu led hijau menyala
  } else if (zTerbobot >= 31 && zTerbobot <= 70) {
    Serial.println("Sedang");
    lcd.setCursor(12, 1);
    lcd.print("Mdrt");
    Blynk.virtualWrite(V3, "Sedang");
    rgb(0,0,1);//lampu led kuning menyala
  } else if (zTerbobot >= 71) {
    Serial.println("buruk");
    lcd.setCursor(12, 1);
    lcd.print("Bad");
    Blynk.virtualWrite(V3, "Kotor");
    rgb(1,0,0);//lampu led merah menyala
  } else {
    Serial.println("Tidak Terbobot");
  }

  Serial.println("====================================\n");
}



void setup(){
  Serial.begin(115200);
  
  // Inisialisasi sensor dan pin
  pinMode(ph_Pin, INPUT);
  pinMode(oneWireBus, INPUT);
  pinMode(turbidityPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  suhu.begin();

  // Inisialisasi LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to:");
  lcd.setCursor(0, 1);
  lcd.print(String("Wifi: ") + ssid);
  Serial.println("Connecting to WiFi...");

  // Hubungkan ke Blynk (fungsi ini akan menangani koneksi WiFi juga)
  // Program akan menunggu di sini sampai terhubung.
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Jika kode sampai di sini, artinya koneksi berhasil
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print("Blynk Ready!");
  Serial.println("WiFi and Blynk Connected!");
  delay(2000); // Tampilkan pesan sukses selama 2 detik
  lcd.clear();

  // Atur timer untuk memeriksa status koneksi secara berkala
  timer.setInterval(2000L, checkBlynkStatus);

  // Buat task untuk logika utama (setelah koneksi berhasil)
  xTaskCreatePinnedToCore(taskTemperature, "SensorTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskFuzzyLogic, "FuzzyTask", 4096, NULL, 1, NULL, 1);
}

void loop(){
}

void taskTemperature(void *pvParameters) {
  while (true) {
    Serial.println("================================");
    temperature();
    pH_Sensor();
    tbdy();
    Serial.println("================================");
    vTaskDelay(2000 / portTICK_PERIOD_MS); // delay 2 detik
  }
}


void taskFuzzyLogic(void *pvParameters) {
  while (true) {
    Blynk.run();
    fuzzyfikasi();
    inference();
    defuzifikasi();
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}

