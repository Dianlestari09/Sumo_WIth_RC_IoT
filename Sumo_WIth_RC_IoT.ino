#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// ====== PIN DEFINISI UNTUK BTS7960 ======
#define R_EN 4              // Enable motor kanan
#define L_EN 5              // Enable motor kiri

#define RPWM_KANAN 16
#define LPWM_KANAN 17
#define RPWM_KIRI 18
#define LPWM_KIRI 19

#define tendang 1            // opsional, tidak dipakai saat ini

// ====== VARIABEL ======
String command;
int speedCar = 170;          // kecepatan awal (0–255)
int speed_Coeff = 3;         // pengurang kecepatan saat belok

const char* ssid = "SUMO1                                                   ";
const char* password = "12341234";

WebServer server(80);

// ====== KONFIGURASI LEDC ======
#define LEDC_FREQ       5000
#define LEDC_RESOLUTION 8     // 8-bit (0–255)
#define CH_RPWM_KANAN  0
#define CH_LPWM_KANAN  1
#define CH_RPWM_KIRI   2
#define CH_LPWM_KIRI   3

void setup() {
  Serial.begin(115200);

  // ====== KONFIGURASI PIN ENABLE ======
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  digitalWrite(R_EN, HIGH);   // aktifkan BTS7960
  digitalWrite(L_EN, HIGH);

  // ====== KONFIGURASI PWM ======
  ledcAttachPin(RPWM_KANAN, CH_RPWM_KANAN);
  ledcAttachPin(LPWM_KANAN, CH_LPWM_KANAN);
  ledcAttachPin(RPWM_KIRI,  CH_RPWM_KIRI);
  ledcAttachPin(LPWM_KIRI,  CH_LPWM_KIRI);

  ledcSetup(CH_RPWM_KANAN, LEDC_FREQ, LEDC_RESOLUTION);
  ledcSetup(CH_LPWM_KANAN, LEDC_FREQ, LEDC_RESOLUTION);
  ledcSetup(CH_RPWM_KIRI,  LEDC_FREQ, LEDC_RESOLUTION);
  ledcSetup(CH_LPWM_KIRI,  LEDC_FREQ, LEDC_RESOLUTION);

  // ====== WIFI ACCESS POINT ======
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // ====== WEB SERVER ======
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();

  Serial.println("Server started. Waiting for commands...");
}

// ====== GERAKAN ======
void goAhead() {
  ledcWrite(CH_RPWM_KANAN, speedCar);
  ledcWrite(CH_LPWM_KANAN, 0);
  ledcWrite(CH_RPWM_KIRI, speedCar);
  ledcWrite(CH_LPWM_KIRI, 0);
}

void goBack() {
  ledcWrite(CH_RPWM_KANAN, 0);
  ledcWrite(CH_LPWM_KANAN, speedCar);
  ledcWrite(CH_RPWM_KIRI, 0);
  ledcWrite(CH_LPWM_KIRI, speedCar);
}

void goRight() {  // kanan mundur, kiri maju
  ledcWrite(CH_RPWM_KANAN, 0);
  ledcWrite(CH_LPWM_KANAN, speedCar / speed_Coeff);
  ledcWrite(CH_RPWM_KIRI, speedCar);
  ledcWrite(CH_LPWM_KIRI, 0);
}

void goLeft() {   // kiri mundur, kanan maju
  ledcWrite(CH_RPWM_KANAN, speedCar);
  ledcWrite(CH_LPWM_KANAN, 0);
  ledcWrite(CH_RPWM_KIRI, 0);
  ledcWrite(CH_LPWM_KIRI, speedCar / speed_Coeff);
}

void goAheadRight() {
  ledcWrite(CH_RPWM_KANAN, speedCar / speed_Coeff);
  ledcWrite(CH_LPWM_KANAN, 0);
  ledcWrite(CH_RPWM_KIRI, speedCar);
  ledcWrite(CH_LPWM_KIRI, 0);
}

void goAheadLeft() {
  ledcWrite(CH_RPWM_KANAN, speedCar);
  ledcWrite(CH_LPWM_KANAN, 0);
  ledcWrite(CH_RPWM_KIRI, speedCar / speed_Coeff);
  ledcWrite(CH_LPWM_KIRI, 0);
}

void goBackRight() {
  ledcWrite(CH_RPWM_KANAN, 0);
  ledcWrite(CH_LPWM_KANAN, speedCar / speed_Coeff);
  ledcWrite(CH_RPWM_KIRI, 0);
  ledcWrite(CH_LPWM_KIRI, speedCar);
}

void goBackLeft() {
  ledcWrite(CH_RPWM_KANAN, 0);
  ledcWrite(CH_LPWM_KANAN, speedCar);
  ledcWrite(CH_RPWM_KIRI, 0);
  ledcWrite(CH_LPWM_KIRI, speedCar / speed_Coeff);
}

void stopRobot() {
  ledcWrite(CH_RPWM_KANAN, 0);
  ledcWrite(CH_LPWM_KANAN, 0);
  ledcWrite(CH_RPWM_KIRI, 0);
  ledcWrite(CH_LPWM_KIRI, 0);
}

// ====== LOOP ======
void loop() {
  server.handleClient();
  command = server.arg("State");

  if      (command == "MAJU") goAhead();
  else if (command == "MUNDUR") goBack();
  else if (command == "KIRI") goLeft();
  else if (command == "KANAN") goRight();
  else if (command == "A") goAheadRight();
  else if (command == "B") goAheadLeft();
  else if (command == "C") goBackRight();
  else if (command == "D") goBackLeft();
  else if (command == "STOP") stopRobot();

  else if (command == "0") speedCar = 50;
  else if (command == "1") speedCar = 80;
  else if (command == "2") speedCar = 100;
  else if (command == "3") speedCar = 130;
  else if (command == "4") speedCar = 160;
  else if (command == "5") speedCar = 180;
  else if (command == "6") speedCar = 200;
  else if (command == "7") speedCar = 220;
  else if (command == "8") speedCar = 240;
  else if (command == "9") speedCar = 255;
}

void HTTP_handleRoot(void) {
  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}


