//#include <TimerThree.h>


#include <PID.h>
#include <Outputs.h>
#include <Inputs.h>
#include <Measure.h>
#include "pinouts.h"
#include "AirFeed.h"
#include "IOController.h"
#include "Height.h"
#include "CanSerial.h"
#include <DTC.h>
#include <Arduino.h>
#ifdef DEBUG_CAN_SERIAL
#include "CanSerial.h"
#endif


// PIDS (datastores) in this ECU
USE_leftFill_PID;
USE_rightFill_PID;
USE_leftDump_PID;
USE_rightDump_PID;
USE_leftSuck_PID;
USE_rightSuck_PID;
USE_airdryer_PID;
USE_compressorFan_PID;
USE_compressorRelay_PID;
USE_compressorCurrent_PID;
USE_leftHeight_PID;
USE_rightHeight_PID;
USE_leftPressure_PID;
USE_rightPressure_PID;
USE_systemPressure_PID;
USE_compressorTemp_PID;
USE_suspensionSpareTemp1_PID
USE_suspensionSpareAnalog1_PID
USE_airFeedState_PID;
USE_heightState_PID;
USE_loadWeight_PID;

// outputs
ITS5215L leftFillDrv(leftFill_outPin, leftFill_diagPin, &leftFill_PID),
       rightFillDrv(rightFill_outPin, rightFill_diagPin, &rightFill_PID),
       leftDumpDrv(leftDump_outPin, leftDump_diagPin, &leftDump_PID),
       rightDumpDrv(rightDump_outPin, rightDump_diagPin, &rightDump_PID),
       leftSuckDrv(leftSuck_outPin, leftSuck_diagPin, &leftSuck_PID),
       rightSuckDrv(rightDump_outPin, rightDump_diagPin, &rightSuck_PID),
       airdryerDrv(airdryer_outPin, airdryer_diagPin, &airdryer_PID),
	   compressorRelayDrv(spare1_outPin, spare1_diagPin, &compressorRelay_PID);

BTS6133D compressorFanDrv(compressor_outPin, compressor_diagPin, compressor_csPin,
                       &compressorFan_PID, &compressorCurrent_PID,
                       measure.sensorADC0(), 0);

// inputs
AnalogIn leftHeightDrv(leftHeightAD_pin, 100, 4096 - 100, 25, &leftHeight_PID),
       rightHeightDrv(rightHeightAD_pin, 100, 4096 - 100, 25, &rightHeight_PID),
       leftPressureDrv(leftPressureAD_pin, 800, 4070, 8, &leftPressure_PID),
       rightPressureDrv(rightPressureAD_pin, 800, 4070, 8, &rightPressure_PID),
       systemPressureDrv(systemPressureAD_pin, 800, 4070, 4, &systemPressure_PID),
       compressorTempDrv(compressorTempAD_pin, 300, 4096 -300, 1, &compressorTemp_PID);

//Ntc compressorNtc(2000, 3960, 5000, 25, 4096); // sensor AVX  NI24MA0502H-- 12bits AD precision
//TempIn compressorTemp(compressorTempAD_pin, 300, 4096 - 300, 1, &compressorNtc, PIDs::compressorTemp);

// AirFeed, compressor and airdryer statemachine
AirFeed airFeedStateMachine(&airFeedState_PID, &compressorFanDrv, &airdryerDrv,
                            &compressorRelayDrv, &systemPressure_PID, &compressorTemp_PID);


// the suspension height controller
HeightController heightStateMachine(&leftFillDrv,  &leftDumpDrv,  &leftSuckDrv,
                                    &rightFillDrv, &rightDumpDrv, &rightSuckDrv,
                                    &leftPressureDrv,   &rightPressureDrv,
                                    &systemPressureDrv, &leftHeightDrv,
                                    &rightHeightDrv,    &heightState_PID,
                                    &loadWeight_PID);


// communication controller
CAN::IOController canIO;

#ifdef DEBUG_CAN_SERIAL
CAN::Serial1 CanSerial(&canIO, C_suspensionDiagSerial);
#endif



//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(115200);

  measure.initADC();

  // build PID table
  PIDs::collection.addItem(&leftFill_PID);
  PIDs::collection.addItem(&rightFill_PID);
  PIDs::collection.addItem(&leftDump_PID);
  PIDs::collection.addItem(&rightDump_PID);
  PIDs::collection.addItem(&leftSuck_PID);
  PIDs::collection.addItem(&rightSuck_PID);
  PIDs::collection.addItem(&airdryer_PID);
  PIDs::collection.addItem(&compressorFan_PID);
  PIDs::collection.addItem(&compressorRelay_PID);
//  PIDs::collection.addItem(&compressorCurrent_PID);
  PIDs::collection.addItem(&leftHeight_PID);
  PIDs::collection.addItem(&rightHeight_PID);
  PIDs::collection.addItem(&leftPressure_PID);
  PIDs::collection.addItem(&rightPressure_PID);
  PIDs::collection.addItem(&systemPressure_PID);
  PIDs::collection.addItem(&compressorTemp_PID);
  PIDs::collection.addItem(&suspensionSpareAnalog1_PID);
  PIDs::collection.addItem(&suspensionSpareTemp1_PID);
  PIDs::collection.addItem(&airFeedState_PID);
  PIDs::collection.addItem(&heightState_PID);
  PIDs::collection.addItem(&loadWeight_PID);

  // init output drivers
  leftFillDrv.init();
  rightFillDrv.init();
  leftDumpDrv.init();
  rightDumpDrv.init();
  leftSuckDrv.init();
  rightSuckDrv.init();
  airdryerDrv.init();
  compressorRelayDrv.init();
  compressorFanDrv.init();
  init_outputs();

  // init input drivers
  leftHeightDrv.init();
  rightHeightDrv.init();
  leftPressureDrv.init();
  rightPressureDrv.init();
  systemPressureDrv.init();
  compressorTempDrv.init();
  init_inputs();

  DTCs.init();

  heightStateMachine.init();

  canIO.init();

#ifdef DEBUG_UART_ON
    Serial.println("boot:");
#endif

}

#ifdef DEBUG_BLINK_ON
void testloop() {
  pinMode(13, OUTPUT);
 
  digitalWrite(13, HIGH);   // set the LED on
  delay(200);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(200);              // wait for a second
}
#endif

// The loop function is called in an endless loop
void loop()
{
  static uint32_t startTime = millis();
  static uint32_t lastErrCheck = millis();

#ifdef DEBUG_UART_ON
  static uint32_t loopCounts = 0;
#endif

#ifdef DEBUG_BLINK_ON
  testloop();
#endif
  
  if (lastErrCheck + 999 < millis()){
	// check each second

#ifdef DEBUG_UART_ON
    Serial.print("loops:");Serial.print(loopCounts, DEC);
    Serial.print(" in ");Serial.print( millis() - lastErrCheck,DEC);
    Serial.println("ms");
    loopCounts = 0;
#endif
#ifdef DEBUG_CAN_SERIAL
    CanSerial.printf("loops:%u in %ums\n", loopCounts, millis() - lastErrCheck);
#endif

    lastErrCheck = millis();

    DTC *dtc = nullptr;

    // error check output drivers
    for (IOutput *outDrv = OutputsController.first();
         outDrv != nullptr;
         outDrv = outDrv->next)
    {
       errorTypes err = outDrv->error();
       if (err != errorTypes::NoError) {
           dtc = DTCs.setDTC(outDrv->pid(), err);
           canIO.sendNewDTC(dtc);

           // then try to reset error if it was a glitch
           outDrv->resetError();
       }
    }
    // error check input drivers
    for (IInput *inDrv = InputsController.first();
         inDrv != nullptr;
         inDrv = inDrv->next)
    {
      errorTypes err = inDrv->error();
      if (err != errorTypes::NoError) {
          dtc = DTCs.setDTC(inDrv->pid(), err);
          canIO.sendNewDTC(dtc);

          // then try to reset error if it was a glitch
          inDrv->resetError();
      }
    }

    // error check system pressure controller
    errorTypes err = airFeedStateMachine.error();
    if (err != errorTypes::NoError) {
        dtc = DTCs.setDTC(airFeedStateMachine.pid(), err);
        canIO.sendNewDTC(dtc);

      // then try to reset if it was a glitch
      airFeedStateMachine.resetError();
    }
  }

  // call eventloop on statemachines
  const uint32_t startupSettleTime = 100;
  if (startTime + startupSettleTime) {
    airFeedStateMachine.loop();
    heightStateMachine.loop();
  }

  canIO.loop();

#ifdef DEBUG_UART_ON
  ++loopCounts;
#endif
}
