
//21 -  SDA
//22 - SCL
//3v3 - vcc
//gnd - gnd
#define BLYNK_TEMPLATE_ID "TMPL356P7nd-D"
#define BLYNK_TEMPLATE_NAME "fall"
#define BLYNK_AUTH_TOKEN "rV-StfZuSNTYHQMHzVF1bQc1XEO6zYTt"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Blynk auth token (replace with your own token from the Blynk app)
char auth[] = "rV-StfZuSNTYHQMHzVF1bQc1XEO6zYTt";

// WiFi credentials (replace with your own Wi-Fi SSID and Password)
char ssid[] = "praharsha";
char pass[] = "harsha2005";


// ADXL345 sensor
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
int buzzer=5 ;
void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2"); 
  Serial.println("------------------------------------");
  Serial.println("");
}

void displayDataRate(void) {
  Serial.print  ("Data Rate:    "); 
  switch(accel.getDataRate()) {
    case ADXL345_DATARATE_3200_HZ: Serial.print("3200 "); break;
    case ADXL345_DATARATE_1600_HZ: Serial.print("1600 "); break;
    case ADXL345_DATARATE_800_HZ: Serial.print("800 "); break;
    case ADXL345_DATARATE_400_HZ: Serial.print("400 "); break;
    case ADXL345_DATARATE_200_HZ: Serial.print("200 "); break;
    case ADXL345_DATARATE_100_HZ: Serial.print("100 "); break;
    case ADXL345_DATARATE_50_HZ: Serial.print("50 "); break;
    case ADXL345_DATARATE_25_HZ: Serial.print("25 "); break;
    case ADXL345_DATARATE_12_5_HZ: Serial.print("12.5 "); break;
    case ADXL345_DATARATE_6_25HZ: Serial.print("6.25 "); break;
    case ADXL345_DATARATE_3_13_HZ: Serial.print("3.13 "); break;
    case ADXL345_DATARATE_1_56_HZ: Serial.print("1.56 "); break;
    case ADXL345_DATARATE_0_78_HZ: Serial.print("0.78 "); break;
    case ADXL345_DATARATE_0_39_HZ: Serial.print("0.39 "); break;
    case ADXL345_DATARATE_0_20_HZ: Serial.print("0.20 "); break;
    case ADXL345_DATARATE_0_10_HZ: Serial.print("0.10 "); break;
    default: Serial.print("???? "); break;
  }
  Serial.println(" Hz");
}

void displayRange(void) {
  Serial.print ("Range:         +/- ");
  switch(accel.getRange()) {
    case ADXL345_RANGE_16_G: Serial.print("16 "); break;
    case ADXL345_RANGE_8_G: Serial.print("8 "); break;
    case ADXL345_RANGE_4_G: Serial.print("4 "); break;
    case ADXL345_RANGE_2_G: Serial.print("2 "); break;
    default: Serial.print("?? "); break;
  }
  Serial.println(" g");
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzer,OUTPUT);
  Serial.println("Accelerometer Test");

  // Connect to Wi-Fi
  Blynk.begin(auth, ssid, pass);

  // Initialize the ADXL345
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G); // Set desired range

  displaySensorDetails();
  displayDataRate();
  displayRange();
}

void loop() {
  // Handle Blynk communication
  Blynk.run();

  // Get a new sensor event
  sensors_event_t event;
  accel.getEvent(&event);
                                                                                      
  // Display the results (acceleration is measured in m/s^2)
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");

  // Send data to Blynk app (Virtual Pin V1 for X, V2 for Y, V3 for Z)
  Blynk.virtualWrite(V0, event.acceleration.x);
  Blynk.virtualWrite(V1, event.acceleration.y);
  Blynk.virtualWrite(V2, event.acceleration.z);
  if(event.acceleration.z < 7){
    Blynk.logEvent("fall_alert","The person is fallen");
    digitalWrite(buzzer,1);
  }

  // Add a delay between measurements
  delay(500);
}
