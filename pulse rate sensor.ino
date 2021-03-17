int PulseSensorPurplePin = 34;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0

 

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024

int Threshold = 3450;            // Determine which Signal to “count as a beat”, and which to ingore.
void setup() {
         // pin that will blink to your heartbeat!

Serial.begin(9600);         // Set’s up Serial Communication at certain speed.

}

// The Main Loop Function

void loop() {

Signal = analogRead(PulseSensorPurplePin); 
Serial.println(Signal);                    // Send the Signal value to Serial Plotter.


delay(10);

}
