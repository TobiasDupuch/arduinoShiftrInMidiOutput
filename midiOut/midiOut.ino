#include <Bridge.h>
#include <BridgeClient.h>
#include <MQTTClient.h>
#include <MIDI.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

BridgeClient net;
MQTTClient client;

unsigned long lastMillis = 0;
// Created and binds the MIDI interface to the default hardware Serial port
//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(SoftwareSerial, mySerial,  MIDI);

int pitch = 0;
int velocity = 0;

void setup()
{
    Serial.begin(9600);
    Bridge.begin();
    client.begin("broker.shiftr.io", net);
    client.onMessage(messageReceived);
    theConnect();
    pinMode(LED_BUILTIN, OUTPUT);
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
}

void theConnect() {
  while (!client.connect("midOut", "44fd6874", "bbbb625576bbaa3f")) {

  }
  client.subscribe("/vibration");
}

void loop()
{
    client.loop();
    Serial.println(pitch);

    if(pitch > 50)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      MIDI.sendNoteOn(42, 127, 1);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    //if(!client.connected()) {
    //  theConnect();
    //}

    // Send note 42 with velocity 127 on channel 1
    //if (millis()%3000==0){
     //MIDI.sendNoteOn(42, 127, 1);
    //}


    // Read incoming messages
    //MIDI.read();
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  int commaIndex = payload.indexOf(',');
  pitch = payload.substring(0, commaIndex).toInt();
  velocity = payload.substring(commaIndex + 1).toInt();
}
