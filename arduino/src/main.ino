#include "Ezo_i2c.h"
#include <Wire.h>

struct SensorData
{
  float pH;
  float EC;
  float temperature;
} sensorData;

Ezo_board PH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board RTD = Ezo_board(102, "RTD");

bool reading_request_phase = true;        //selects our phase

uint32_t next_poll_time = 0;              //holds the next time we receive a response, in milliseconds
const unsigned int response_delay = 1000; //how long we wait to receive a response, in milliseconds
    
void setup() {
  Wire.begin();
  Serial.begin(9600);
}
 
void loop() {
  if (reading_request_phase) {           //if were in the phase where we ask for a reading
    if(Serial.available() > 0){
      Serial.read();
      //send a read command. we use this command instead of PH.send_cmd("R"); 
      //to let the library know to parse the reading
      PH.send_read_cmd();                      
      EC.send_read_cmd();
      RTD.send_read_cmd();
      
      next_poll_time = millis() + response_delay; //set when the response will arrive
      reading_request_phase = false;       //switch to the receiving phase
    }
  }
  else {                               //if were in the receiving phase
    if (millis() >= next_poll_time) {  //and its time to get the response

      sensorData.pH = receive_reading(PH);             //get the reading from the PH circuit
      sensorData.EC = receive_reading(EC);             //get the reading from the EC circuit
      sensorData.temperature = receive_reading(RTD);   //get the reading from the EC circuit

      Serial.write((byte *)&sensorData, sizeof(struct SensorData));

      reading_request_phase = true;            //switch back to asking for readings
    }
  }
}


float receive_reading(Ezo_board &Sensor) {               // function to decode the reading after the read command was issued
  
  Sensor.receive_read_cmd();              //get the response data and put it into the [Sensor].reading variable if successful
                                      
  switch (Sensor.get_error()) {             //switch case based on what the response code is.
    case Ezo_board::SUCCESS:        
      return Sensor.get_last_received_reading();
      break;
    case Ezo_board::FAIL:
    case Ezo_board::NOT_READY:
    case Ezo_board::NO_DATA:
      return 0.0;
      break;
  }
}
