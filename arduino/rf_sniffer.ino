#define ledPin 7            // Onboard LED on shield
#define buttonPin 6            // Onboard button attached on the shield
#define RxAdataPin A0          //RF Receiver data pin = Analog pin 0

int rxAdata = 0;
const int dataSize = 200;  //Arduino memory is limited (max=1700)
byte storedData[dataSize];  //Create an array to store the data
unsigned long storedTime[dataSize];  //Create an array to store the data
int maxSignalLength = 255;   //Set the maximum length of the signal
int lowCnt = 0;
int highCnt = 0;
unsigned long startTime = 0; //Variable to record the start time
unsigned long endTime = 0;  //Variable to record the end time
unsigned long smplTime = 0;
unsigned long signalDuration = 0; //Variable to record signal reading time
const unsigned int highThreshold = 100;  //upper threshold value
const unsigned int lowThreshold = 80;  //lower threshold value

// put your setup code here, to run once:
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

// put your main code here, to run repeatedly:
void loop() {
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Listening for Signal");
    for (int i = 0; i < 3; i = i + 1) {
      InitVariables();
      SampleSignal();
    }
  }
  delay(20);
}

void InitVariables() {
  for (int i = 0; i < dataSize; i++) {
    storedData[i] = 0;
  }
  startTime = micros();
  lowCnt = 0;
  highCnt = 0;
}

void SampleSignal() {

  do {
    rxAdata = analogRead(RxAdataPin);
    if (rxAdata<lowThreshold) {
      lowCnt++;
    } else {
      lowCnt = 0;
    }
    //Wait here until a LOW signal is received
    Serial.println(rxAdata);
    //Serial.print(", ");
    //Serial.println(micros());
  } while((rxAdata<lowThreshold) || (lowCnt < 2));
  
  startTime=micros();  //Update start time with every cycle.
  smplTime=startTime;
  digitalWrite(ledPin, HIGH);  //Turn LED ON

  //Read and store the rest of the signal into the storedData array
  for (int i = 0; i < dataSize; i = i + 1) {
    storedData[i] = analogRead(RxAdataPin);
    storedTime[i] = micros() - smplTime;
    smplTime = micros();
  }

  endTime = micros(); //Record the end time of the read period.
  signalDuration = endTime - startTime;

  digitalWrite(ledPin, LOW);//Turn LED OFF

  //Send report to the Serial Monitor
  Serial.println("=====================");
  Serial.print("Read duration: ");
  Serial.print(signalDuration);
  Serial.println(" microseconds");
  Serial.println("=====================");
  Serial.println("Analog data");
  delay(20);
  for (int i = 0; i < dataSize; i = i + 1) {
    Serial.print(storedTime[i]);
    Serial.print(", ");
    Serial.println(storedData[i]);
  }
}
