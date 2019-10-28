#include <Arduino.h>
#define MAGNET_AWAY 1
#define MOVING_AVG 1000

class MagnetSensor {
    int currentStateIndex;
    bool states[MOVING_AVG];
    bool state;
    int8_t pin;
    char name[16];
    int8_t magnetAway;
  public:
    MagnetSensor(int, int8_t, char[]);
    int getState();
    void setup(); // arduino setup
  private:
    bool calcAvg();
    int read();
};

MagnetSensor::MagnetSensor(int somePin, int8_t pinMagnetAway = MAGNET_AWAY, char givenName[] = "Sensor"){
  pin = somePin;
  strcpy(name, givenName);
  magnetAway = pinMagnetAway;
}

void MagnetSensor::setup(){
  pinMode(pin, INPUT_PULLUP);
}

int MagnetSensor::read(){
  return digitalRead(pin);
}

// like elections, if 50% of booleans are one direction, that's the average
bool MagnetSensor::calcAvg(){
  int sum = 0;
  for(int i=0; i<MOVING_AVG; i++){
    //Serial.print(arr[i]);
    //Serial.print(" ");
    sum += states[i];
  }
  //Serial.println("");
  return (sum > MOVING_AVG / 2);
}

int MagnetSensor::getState() {
  states[currentStateIndex] = read();
  currentStateIndex++;
  if(currentStateIndex >= MOVING_AVG){
    currentStateIndex = 0;
  }
  int newState = calcAvg();
  if(newState != state){
    Serial.println("Change detected!!!");
    Serial.print(name);
    Serial.print(" state: ");
    if(newState == magnetAway){
      Serial.println("Away");
    } else {
      Serial.println("On sensor");
    }
    state = newState;
  }
  return state;
}

MagnetSensor mushroom(21, 1, "Mushroom");
MagnetSensor cards(5, 0, "Playing Cards");
MagnetSensor cup(6, 0, "Cup");

void setup() {
  Serial.begin(9600);
  Serial.println("Starting!");
  mushroom.setup();
  cards.setup();
  cup.setup();
}


void loop() {
  mushroom.getState();
  cards.getState();
  cup.getState();
}