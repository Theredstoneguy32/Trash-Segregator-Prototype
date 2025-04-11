#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <Servo.h>

// pin setup
#define ULTRASONIC_TRIG_PIN 9
#define ULTRASONIC_ECHO_PIN 8
#define SOIL_MOISTURE_ANALOG_PIN A0
#define INFRARED_PIN 10
#define SERVO_PIN 11

LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, 200);
Servo myServo;

// servo angles
int leftAngle = 45;   // dry trash goes left
int rightAngle = 135; // wet trash goes right

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(SOIL_MOISTURE_ANALOG_PIN, INPUT);
  pinMode(INFRARED_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90); // center by default
}

void loop() {
  // check soil moisture
  int soilMoistureAnalogValue = analogRead(SOIL_MOISTURE_ANALOG_PIN);
  bool isDry = soilMoistureAnalogValue > 600; // dry if high

  // get distance from ultrasonic
  long duration = sonar.ping();
  int distance = duration / 58.2;

  // check if object is in front
  bool isObjectDetected = (digitalRead(INFRARED_PIN) == HIGH);// && (distance < 11);

  // lcd display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TRASH STATUS:");

  // logic
  if (isObjectDetected) {
    if (isDry) {
      myServo.write(leftAngle); // dry -> left
      lcd.setCursor(0, 1);
      lcd.print("DRY");
    } else {
      myServo.write(rightAngle); // wet -> right
      lcd.setCursor(0, 1);
      lcd.print("WET");
    }
  } else {
    myServo.write(90); // no trash -> center
    lcd.setCursor(0, 1);
    lcd.print("NONE");
  }

  // serial monitor output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Infrared: ");
  Serial.print(digitalRead(INFRARED_PIN));
  Serial.print(" | Moisture: ");
  Serial.println(isDry ? "Dry" : "Wet");

  delay(1500);
}