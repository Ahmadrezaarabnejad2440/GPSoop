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

class GPS {
public:
  void begin(uint32_t baudRate = 9600) {
    gpsSerial.begin(baudRate);
  }

  bool readData() {
    while (gpsSerial.available() > 0) {
      if (gps.encode(gpsSerial.read())) {
        return true;
      }
    }
    return false;
  }

  String generateMapLink() {
    double latitude = gps.location.lat();   // Latitude
    double longitude = gps.location.lng();  // Longitude
    String link = "https://www.openstreetmap.org/?mlat=";
    link += String(latitude, 6);  // Convert Latitude to a string with 6 decimal places
    link += "&mlon=";
    link += String(longitude, 6);  // Convert Longitude to a string with 6 decimal places
    link += "&zoom=15";

    return link;
  }

  String height() {
    double altitude = gps.altitude.meters();
    return String(altitude, 1);
  }

  String speed() {
    int speedKnot = gps.speed.value();  // Knots
    int speedKH = speedKnot * 1.852;    // Kilometers per hour
    return String(speedKH);
  }

  String direction() {  // Movement direction
    int direction = gps.course.value();
    return String(direction);
  }

  String satellites() {  // Number of visible satellites
    int satellites = gps.satellites.value();
    return String(satellites);
  }

  String hdop() {  // Horizontal Dilution of Precision (HDOP)
    int hdop = gps.hdop.value();
    return String(hdop);
  }

  String date() {
    String date = String(gps.date.month());
    date += "/";
    date += String(gps.date.day());
    date += "/";
    date += String(gps.date.year());

    if (gps.date.isValid()) {
      return date;
    } else {
      return "Not Available Date";
    }
  }

  String time() {
    String timeStr = "";

    if (gps.time.hour() < 10) timeStr += "0";
    timeStr += String(gps.time.hour()) + ":";

    if (gps.time.minute() < 10) timeStr += "0";
    timeStr += String(gps.time.minute()) + ":";

    if (gps.time.second() < 10) timeStr += "0";
    timeStr += String(gps.time.second());

    if (gps.time.isValid()) {
      return timeStr;
    } else {
      return "Not Available Time";
    }
  }
};

GPS serialgps;

void setup() {
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(9600);
}

void loop() {
  if (serialgps.readData()) {
    Serial.print("Link Location: ");
    Serial.println(serialgps.generateMapLink());
    Serial.print("Height above sea: ");
    Serial.println(serialgps.height());
    Serial.print("Speed: ");
    Serial.println(serialgps.speed());
    Serial.print("Movement direction: ");
    Serial.println(serialgps.direction());
    Serial.print("Satellites: ");
    Serial.println(serialgps.satellites());
    Serial.print("Horizontal accuracy: ");
    Serial.println(serialgps.hdop());
    Serial.println();
    Serial.println();
    delay(2000);
  } else {
    Serial.println("GPS Not Available");
  }
  Serial.println(serialgps.date());
  Serial.println(serialgps.time());

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true)
      ;
  }
}
