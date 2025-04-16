#include <BluetoothSerial.h>
#include <IRremote.h>
#define BULB1_PIN 21  // Relay for Bulb 1
#define BULB2_PIN 22  // Relay for Bulb 2
#define BULB3_PIN 23  // Relay for Bulb 3
#define IR_RECEIVER_PIN 35 // IR Receiver-*-*
BluetoothSerial SerialBT;
bool bulbState[3] = {false, false, false}; 
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_LightControl"); 
  pinMode(BULB1_PIN, OUTPUT);
  pinMode(BULB2_PIN, OUTPUT);
  pinMode(BULB3_PIN, OUTPUT);

  digitalWrite(BULB1_PIN, HIGH);
  digitalWrite(BULB2_PIN, HIGH);
  digitalWrite(BULB3_PIN, HIGH);

  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("System Ready: Bluetooth & IR Remote Initialized!");
}
void loop() {
  // Bluetooth Control
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("BT Command Received: ");
    Serial.println(command);

    switch (command) {
      case '1': 
        bulbState[0] = !bulbState[0]; 
        digitalWrite(BULB1_PIN, !bulbState[0]);
        SerialBT.println(bulbState[0] ? "Bulb 1 ON" : "Bulb 1 OFF");
        break;
      case '2': 
        bulbState[1] = !bulbState[1]; 
        digitalWrite(BULB2_PIN, !bulbState[1]);
        SerialBT.println(bulbState[1] ? "Bulb 2 ON" : "Bulb 2 OFF");
        break;
      case '3': 
        bulbState[2] = !bulbState[2]; 
        digitalWrite(BULB3_PIN, !bulbState[2]);
        SerialBT.println(bulbState[2] ? "Bulb 3 ON" : "Bulb 3 OFF");
        break;
      case 'A': // Turn ON all bulbs
        bulbState[0] = bulbState[1] = bulbState[2] = true;
        digitalWrite(BULB1_PIN, LOW);
        digitalWrite(BULB2_PIN, LOW);
        digitalWrite(BULB3_PIN, LOW);
        SerialBT.println("All Bulbs ON");
        break;
      case 'B': // Turn OFF all bulbs
        bulbState[0] = bulbState[1] = bulbState[2] = false;
        digitalWrite(BULB1_PIN, HIGH);
        digitalWrite(BULB2_PIN, HIGH);
        digitalWrite(BULB3_PIN, HIGH);
        SerialBT.println("All Bulbs OFF");
        break;
    }
  }
  // IR Remote Control
  if (IrReceiver.decode()) {
    uint32_t receivedCode = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Received IR Code: 0x");
    Serial.println(receivedCode, HEX);

    if (receivedCode == 0x6D92FD01) { 
      bulbState[0] = !bulbState[0];
      digitalWrite(BULB1_PIN, !bulbState[0]);
      Serial.println(bulbState[0] ? "Bulb 1 ON" : "Bulb 1 OFF");
    }
    if (receivedCode == 0x6C93FD01) { 
      bulbState[1] = !bulbState[1];
      digitalWrite(BULB2_PIN, !bulbState[1]);
      Serial.println(bulbState[1] ? "Bulb 2 ON" : "Bulb 2 OFF");
    }
    if (receivedCode == 0x33CCFD01) { 
      bulbState[2] = !bulbState[2];
      digitalWrite(BULB3_PIN, !bulbState[2]);
      Serial.println(bulbState[2] ? "Bulb 3 ON" : "Bulb 3 OFF");
    }
    if (receivedCode == 0x3BC4FD01) { 
      bulbState[0] = bulbState[1] = bulbState[2] = true;
      digitalWrite(BULB1_PIN, LOW);
      digitalWrite(BULB2_PIN, LOW);
      digitalWrite(BULB3_PIN, LOW);
      Serial.println("All Bulbs ON");
    }
    if (receivedCode == 0x23DCFD01) {
      bulbState[0] = bulbState[1] = bulbState[2] = false;
      digitalWrite(BULB1_PIN, HIGH);
      digitalWrite(BULB2_PIN, HIGH);
      digitalWrite(BULB3_PIN, HIGH);
      Serial.println("All Bulbs OFF");
    }
    IrReceiver.resume();
  }
}
