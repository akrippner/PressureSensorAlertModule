//Written by Andrew Krippner
//For Toolbox Medical Innovations
//Dec 6 2018

//Parameters
int sampleRate = 10; //[Hz]
float absoluteThreshold = 15; //[psi]
float rampThreshold = 3; //[psi/s]

//Definitions
int pressureSensor1In = A0; //Analog In Pin Number 0
int pressureSensor2In = A1; //Analog In Pin Number 1
int alertOut = 13; //Digital Out Pin Number 13

//Variables
unsigned long timeNow = 0; //[ms] from start of program. Updated at sample rate.
float p1 = 0; //[psi]
float p2 = 0; //[psi]
float p1Dot = 0; //[psi/s]
float p2Dot = 0; //[psi/s]

//Setup
void setup() {
  Serial.begin(9600);
  pinMode(pressureSensor1In, INPUT);
  pinMode(pressureSensor2In, INPUT);
  pinMode(alertOut, OUTPUT);
  digitalWrite(alertOut,HIGH);
}

//Loop
void loop() {
  if(millis() > timeNow + 1000/sampleRate) { //Continues if period for project sample rate is met
    timeNow = millis();
    updateValues();
    if (hasPressureAlertCondition()) {
      digitalWrite(alertOut,LOW);
      Serial.println("Pressure Alert Sent");
    }
  }
}

//Updates the pressure and pressure ramp values
void updateValues() {
  float p1New = (getPressure(pressureSensor1In) + p1)/2;
  float p2New = (getPressure(pressureSensor2In) + p1)/2;
  p1Dot = (p1New - p1)*sampleRate;
  p2Dot = (p2New - p2)*sampleRate;
  p1 = p1New;
  p2 = p2New;

  Serial.print("p1: ");
  Serial.print(p1);
  Serial.print("\t");
  Serial.print("p2: ");
  Serial.print(p2);
  Serial.print("\t");
  Serial.print("p1Dot: ");
  Serial.print(p1Dot);
  Serial.print("\t");
  Serial.print("p2Dot: ");
  Serial.println(p2Dot);
}

//Returns true if alert condition is met
bool hasPressureAlertCondition() {
  if (timeNow > 2000)
    return (p1 > absoluteThreshold || p2 > absoluteThreshold || p1Dot > rampThreshold || p2Dot > rampThreshold);
  else
    return false;
}

//Returns pressure at given pressure sensor input. This is where the sensor is calibrated.
float getPressure(int pressureSensorIn) {
  int analogIn = analogRead(pressureSensorIn);
  float pressure = 0;
  if (analogIn < 469)
    pressure = 0.021*analogIn-2.6681;
  else
    pressure = 0.5472*analogIn-249.38;
  return(pressure);
}
