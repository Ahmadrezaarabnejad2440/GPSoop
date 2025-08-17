#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Choose two Arduino pins to use for software serial
int RXPin = D7;
int TXPin = D8;

int GPSBaud = 9600;


// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop() {
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {
      Serial.println(generateMapLink());
      Serial.println(height());
      Serial.println(speed());
      Serial.println(Direction());
      Serial.println(Satellites());
      Serial.println(Hdop());
      Serial.println();
      Serial.println();
      delay(2000);
    }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true)
      ;
  }
}

String generateMapLink() {
  double latitude = gps.location.lat();   // عرض جغرافیایی
  double longitude = gps.location.lng();  // طول جغرافیایی
  String link = "https://www.openstreetmap.org/?mlat=";
  link += String(latitude, 6);  // تبدیل عرض جغرافیایی به رشته با دقت 6 رقم اعشاری
  link += "&mlon=";
  link += String(longitude, 6);  // تبدیل طول جغرافیایی به رشته با دقت 6 رقم اعشاری
  link += "&zoom=15";

  return link;
}


String height() {
  double Altitude = gps.altitude.meters();
  return String(Altitude, 1);
}


String speed() {
  int speedKnot = gps.speed.value();  //knot
  int speedKH = speedKnot * 1.852;    // kilometer per hour
  return String(speedKH);
}


String Direction() {  // جهت حرکت
  int direction = gps.course.value();
  return String(direction);
}


String Satellites() {  // تعداد ماهواره های قایل مشاهده
  int satellites = gps.satellites.value();
  return String(satellites);
}


String Hdop() {  // میزان کاهش دقت افقی
  int hdop = gps.hdop.value();
  return String(hdop);
}
