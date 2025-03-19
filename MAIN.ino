#include <SD.h>


// BMP
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


const int chipSelect = 10;
const char fileName[] = "03192025.txt";

File dataFile;



Adafruit_BMP280 bmp; // I2C


void setup() {
  Serial.begin(9600);

  while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }

  Serial.println("initialization done.");

  dataFile = SD.open(fileName, FILE_WRITE);

  if (dataFile) {
    Serial.println("File successfully opened");
  } else {
    Serial.println("Error opening file.");

    while (1);
  }

  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() { 
  int temp = bmp.readTemperature();
  int pressure = bmp.readPressure();

  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure()/1000);
  Serial.println(" kPa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25) ); /* Adjusted to local forecast! */
  Serial.println(" m");

  Serial.println();


  dataFile.println(temp + ", " + pressure);

  dataFile.flush();

  delay(2000);
}

