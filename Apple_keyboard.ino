//#include 

int outputMap[7] = {2, 3, 4, 5, 6, 7, 8};
int strobePin = 9;
int strobeDuration = 5;
int resetPin = 10;
int delayBetweenKeys = 2;

void setup() {
    Serial.begin(9600);
    for(int i=0; i<7; i++) {
        pinMode(outputMap[i], OUTPUT);
    }
    pinMode(strobePin, OUTPUT);
    digitalWrite(strobePin, LOW);
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
}

void loop() {
    unsigned int inByte;
    if(Serial.available() > 0) {
        inByte = Serial.read();
        if( inByte=='\x7f' )
          inByte='\b';
        if( inByte>='a' && inByte<='z' ) 
          inByte-='a'-'A';
        
        if( inByte==28 ) {
          doReset();
          return;
        }
        for(int i=0; i<7; i++) {
            int state = LOW;
            int bitmask = 1 << i;
            if(inByte & bitmask) state = HIGH;
            digitalWrite(outputMap[i], state);
        }
        digitalWrite(strobePin, HIGH);
        if(inByte=='\r')
          Serial.write('\n');
        Serial.write(inByte);
        delay(strobeDuration);
        digitalWrite(strobePin, LOW);
    }
    delay(delayBetweenKeys);
}

void doReset() {
  digitalWrite(resetPin, LOW);
  delay(strobeDuration);
  digitalWrite(resetPin, HIGH);
}
