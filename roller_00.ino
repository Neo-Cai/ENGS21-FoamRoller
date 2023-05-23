#define LED_PIN_1 11
#define LED_PIN_2 10
#define LED_PIN_3 9
#define LED_PIN_4 8
void setup()
{
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
}

void loop()
{
  digitalWrite(LED_PIN_1, HIGH);
  delay(5000);
  digitalWrite(LED_PIN_2, HIGH);
  delay(5000);
  digitalWrite(LED_PIN_3, HIGH);
  delay(5000);
  digitalWrite(LED_PIN_4, HIGH);
  delay(5000);
  digitalWrite(LED_PIN_4, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_1, LOW);
  delay(5000);
}
