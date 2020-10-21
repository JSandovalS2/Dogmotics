#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHTTYPE DHT11
#include <Servo.h>
Servo servo;
BlynkTimer timer;

int aire = 14;
int rgb = 12;
int On = 13;

int pirdentro = 16;
int pirfuera = 5;
int luz = 4;
int airepw = 0;

#define DHTPIN 2
int encender = 0;

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "Zjn25YfNeWlPEUXwG6MejaFMQjY0-SZ7";
char ssid[] = "Meganet_Hi-Tech3D";
char pass[] = "hitech3D85";

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Hay un error en la lectura del sensor");
    return;
  }
  if (t >= 30 && encender == 0) {
    encender = 1;
  }
  if (encender == 1) {
    digitalWrite(On, LOW);
    delay(200);
    digitalWrite(On, HIGH);
    Serial.println(t);
    encender = 2;
  }
  if (t < 30 && encender == 2) {
    encender = 3;
  }
  if (t < 30 && encender == 3) {
    digitalWrite(On, LOW);
    delay(200);
    digitalWrite(On, HIGH);
    encender = 0;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}


bool door = false;
int i = 0;
int automatico = 0;
bool au = false;
int pir = 0;

BLYNK_WRITE(V0)
{
  i = param.asInt();
  if (i >= 1 && door == false) {
    servo.write(0);
    delay(5500);
    servo.write(90);
    door = true;
    i = 0;
  }
  if (i >= 1 && door == true) {
    servo.write(180);
    delay(4000);
    servo.write(90);
    door = false;
    i = 0;
  }

}

BLYNK_WRITE(V1)
{
  automatico = param.asInt();
  if (automatico <= 0) {
    au = false;
  }
  if (automatico >= 1 ) {
  au = true;
  }

}

void setup()
{

  Serial.begin(9600);
  pinMode(aire, OUTPUT);
  pinMode(rgb, OUTPUT);
  pinMode(On, OUTPUT);
  pinMode(airepw, OUTPUT);
  pinMode(luz, OUTPUT);
  pinMode(pirdentro, INPUT);
  pinMode(pirfuera, INPUT);
  digitalWrite(aire, HIGH);
  digitalWrite(rgb, HIGH);
  digitalWrite(airepw, LOW);
  digitalWrite(On, HIGH);
  digitalWrite(luz, HIGH);
  delay(500);
  servo.attach(15);
  servo.write(90);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);

}

void loop()
{
  Blynk.run();
  timer.run();
  pir = digitalRead(pirfuera);
  Serial.print("PIR = ");
  Serial.println(pir);
    if (pir == HIGH && au == true) {
      servo.write(0);
      delay(5500);
      servo.write(90);
      delay(5000);
      servo.write(180);
      delay(4000);
      servo.write(90);
      delay(1000);
      pir = 0;
    }
}
