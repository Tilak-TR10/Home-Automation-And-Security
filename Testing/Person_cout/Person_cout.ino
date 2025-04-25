#define IR1 A3  // Entrance 1
#define IR2 A2  // Entrance 2
#define IR3 A1  // Exit 1
#define IR4 A0  // Exit 2

int peopleCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  Serial.println("Visitor Counter Started");
}

void loop() {
  static bool inFlag = false;
  static bool outFlag = false;

  // Detect entrance
  if (digitalRead(IR1) == LOW && digitalRead(IR2) == LOW && !inFlag) {
    delay(200);  // Debounce delay
    if (digitalRead(IR1) == LOW && digitalRead(IR2) == LOW) {
      peopleCount++;
      inFlag = true;
      Serial.println("Person Entered - Count: " + String(peopleCount));
    }
  }
  if (digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH) {
    inFlag = false;
  }

  // Detect exit
  if (digitalRead(IR3) == LOW && digitalRead(IR4) == LOW && !outFlag) {
    delay(200);  // Debounce delay
    if (digitalRead(IR3) == LOW && digitalRead(IR4) == LOW) {
      peopleCount--;
      if (peopleCount < 0) peopleCount = 0;  // Prevent negative count
      outFlag = true;
      Serial.println("Person Exited - Count: " + String(peopleCount));
    }
  }
  if (digitalRead(IR3) == HIGH && digitalRead(IR4) == HIGH) {
    outFlag = false;
  }
}
