
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc;
Servo servo;
const int servoPin = 9;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Waktu pemberian makan ikan
const int feedingHour = 6; // Jam 06:00
const int feedingMinute = 10; // Menit 10
bool hasFedToday = false;

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("RTC tidak terdeteksi");
    while (1); // Jika RTC tidak terdeteksi, hentikan program
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC belum diatur, mengatur waktu sekarang...");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }

  servo.attach(servoPin);
  servo.write(0); // Posisi awal servo (tutup)

  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Menyalakan backlight LCD
  lcd.setCursor(0,0);
  lcd.print("Fish Feeder");
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  delay(2000); // Tampilkan selama 2 detik
  lcd.clear();
}

void loop() {
 DateTime now = rtc.now(); // Dapatkan waktu sekarang

 lcd.setCursor(0,0);
 lcd.print("Time:");
 if (now.hour() < 10) lcd.print("0");
 lcd.print(now.hour());
 lcd.print(":");
 if (now.minute() < 10) lcd.print("0");
 lcd.print(now.minute());

 // Cek apakah saat ini sudah waktu pemberian makan ikan
 if (now.hour() == feedingHour && now.minute() == feedingMinute && !hasFedToday) 
 {
  feedFish(now); // Berikan makanan ikan dan kirim waktu saat ini
  hasFedToday = true; // Tandai bahwa ikan sudah diberi makan
 }

 // Reset status pemberian makan di tengah malam (00:00)
 if (now.hour() == 0 && now.minute() == 0) {
  hasFedToday = false;
 }

 // Tampilkan status pemberian makan di LCD
 lcd.setCursor(0,1);
 if (hasFedToday) {
  lcd.print("Fed Today"); // Menampilkan bahwa ikan sudah diberi makan
 } else {
  lcd.print("Not Fed Today"); // Menampilkan bahwa ikan belum diberi makan
 }
 
}
void feedFish(DateTime now) {
  Serial.println("Feeding the fish!");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Feeding the fish");
  lcd.setCursor(0,1);
  lcd.print("Please wait...");

  // Gerakan servo untuk membuka tutup tempat makan
  servo.write(90); // Posisi buka
  delay(5000); // Biarkan terbuka selama 5 detik

  // Tutup kembali penutup makanan
  servo.write(0); // Posisi tutup

  // Tampilkan pesan selesai memberi makan
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fed at");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  delay(2000);
  lcd.clear();
}
