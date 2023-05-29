#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
// Set WiFi credentials
#define WIFI_SSID "TheOtherESP"
#define WIFI_PASS "flashmeifyoucan"

// UDP
WiFiUDP UDP;
IPAddress remote_IP(192,168,4,1);
#define UDP_PORT 4210

int INPUT_PIN = 4;

void setup() {
  
  // Setup IO
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);
  
  // Setup serial port
  Serial.begin(115200);
  Serial.println();
 
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);
 
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
   
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);

  //Setup MPU6050 
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  //Calibration
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);  
  cali_x = a.acceleration.x;
  cali_y = a.acceleration.y;
  cali_z = a.acceleration.z;


  int state = LOW;
  Serial.println("");
  delay(100);  
}

void loop() {

  /*
  int state = digitalRead(INPUT_PIN);
  Serial.print("Pin state: ");
  Serial.println(state);
  
  //digitalWrite(LED_BUILTIN, state);
  */

  state = LOW;
  mpu.getEvent(&a, &g, &temp); 
  if (abs(a.acceleration.x - cali.x) > 1.0) || (abs(a.acceleration.y - cali.y) > 1.0) || (abs(a.acceleration.z - cali.z) > 1.0){
    state = HIGH;
  }

  
  // Convert to ASCII text
  char message = '0' + state;
  Serial.println(message);
  // Send Packet
  UDP.beginPacket(remote_IP, UDP_PORT);
  UDP.write(message);
  UDP.endPacket();
  delay(500);
  
}
