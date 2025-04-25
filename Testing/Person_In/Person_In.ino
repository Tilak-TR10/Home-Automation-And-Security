#define IR1 A3  // Entrance 1
#define IR2 A2  // Entrance 2
#define IR3 A1  // Exit 1
#define IR4 A0  // Exit 2

int peopleCount = 0;
int sequence[4] = {0, 0, 0, 0}; // To track sensor activations

void setup() {
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  
  Serial.println("Visitor Counter Started");
}

void loop() {
  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3);
  int s4 = digitalRead(IR4);

  // Update sequence tracking
  if (s1 == HIGH && sequence[0] == 0) {
  sequence[0] = 1;
  Serial.println(sequence[0]);
  }
  
  if (s2 == HIGH && sequence[0] == 1 && sequence[1] == 0){
  sequence[1] = 2;
  Serial.println(sequence[1]);
  }
  
  if (s3 == HIGH && sequence[1] == 2 && sequence[2] == 0){
  sequence[2] = 3;
  Serial.println(sequence[2]);
  }
  
  if (s4 == HIGH && sequence[2] == 3 && sequence[3] == 0){
    sequence[3] = 4;
    Serial.println(sequence[3]);
    peopleCount++; // Person entered
    Serial.println("Person Entered - Count: " + String(peopleCount));
    resetSequence(); // Reset tracking
    //delay(2000);
  }
/*
 if(peopleCount > 0){
  // Reverse order detection (Exit case)
  if (s4 == HIGH && sequence[3] == 0){
    sequence[3] = 4;
    Serial.println(sequence[3]);
  }
  if (s3 == HIGH && sequence[3] == 4 && sequence[2] == 0){
    sequence[2] = 3;
    Serial.println(sequence[2]);
  }
  if (s2 == HIGH && sequence[2] == 3 && sequence[1] == 0){
    sequence[1] = 2;
    Serial.println(sequence[1]);
  }
  if (s1 == HIGH && sequence[1] == 2 && sequence[0] == 0){
    sequence[0] = 1; 
    Serial.println(sequence[0]);
    peopleCount--;
    if (peopleCount < 0) peopleCount = 0;  // Prevent negative count
    Serial.println("Person Exited - Count: " + String(peopleCount));
    resetSequence();
    delay(2000);
  }
 }  
 */
}

// Function to reset sequence tracking
void resetSequence() {
  for (int i = 0; i < 4; i++) {
    sequence[i] = 0;
  }
  delay(1000);  // Avoid multiple detections
}
