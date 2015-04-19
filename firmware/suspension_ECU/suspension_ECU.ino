
#include <PID.h>
#include <Outputs.h>
#include <Inputs.h>
#include <Measure.h>
#include "pinouts.h"
#include "AirFeed.h"
#include "IOController.h"
#include "Height.h"
#include <DTC.h>
#include <Arduino.h>


// PIDS (datastores) in this ECU
USE_leftFill_PID;
USE_rightFill_PID;
USE_leftDump_PID;
USE_rightDump_PID;
USE_leftSuck_PID;
USE_rightSuck_PID;
USE_airdryer_PID;
USE_compressor_PID;
USE_compressorCurrent_PID;
USE_leftHeight_PID;
USE_rightHeight_PID;
USE_leftPressure_PID;
USE_rightPressure_PID;
USE_systemPressure_PID;
USE_compressorTemp_PID;
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
       airdryerDrv(airdryer_outPin, airdryer_diagPin, &airdryer_PID);

BTS6133D compressorDrv(compressor_outPin, compressor_diagPin, compressor_csPin,
                       &compressor_PID, &compressorCurrent_PID,
                       measure.compressorADC());

// inputs
AnalogIn leftHeightDrv(leftHeightAD_pin, 100, 4096 - 100, 25, &leftHeight_PID),
       rightHeightDrv(rightHeightAD_pin, 100, 4096 - 100, 25, &rightHeight_PID),
       leftPressureDrv(leftPressureAD_pin, 800, 4070, 8, &leftPressure_PID),
       rightPressureDrv(rightPressureAD_pin, 800, 4070, 8, &rightPressure_PID),
       systemPressureDrv(systemPressureAD_pin, 800, 4070, 4, &systemPressure_PID),
       compressorTempDrv(compressorTempAD_pin, 300, 4096 -300, 1, &compressorTemp_PID);
/*
Ntc compressorNtc(2000, 3960, 5000, 25, 4096); // sensor AVX  NI24MA0502H-- 12bits AD precision
TempIn compressorTemp(compressorTempAD_pin, 300, 4096 - 300, 1, &compressorNtc, PIDs::compressorTemp);
*/
// AirFeed, compressor and airdryer statemachine
AirFeed airFeedStateMachine(&airFeedState_PID, &compressorDrv, &airdryerDrv,
                            &systemPressure_PID, &compressorTemp_PID);


// the suspension height controller
HeightController heightStateMachine(&leftFillDrv,  &leftDumpDrv,  &leftSuckDrv,
                                    &rightFillDrv, &rightDumpDrv, &rightSuckDrv,
                                    &leftPressureDrv,   &rightPressureDrv,
                                    &systemPressureDrv, &leftHeightDrv,
                                    &rightHeightDrv,    &heightState_PID,
                                    &loadWeight_PID);


// communication controller
CAN::IOController canIO;

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(115200);

  measure.initForCompressor();

  // build PID table
  PIDs::collection.addItem(&leftFill_PID);
  PIDs::collection.addItem(&rightFill_PID);
  PIDs::collection.addItem(&leftDump_PID);
  PIDs::collection.addItem(&rightDump_PID);
  PIDs::collection.addItem(&leftSuck_PID);
  PIDs::collection.addItem(&rightSuck_PID);
  PIDs::collection.addItem(&airdryer_PID);
  PIDs::collection.addItem(&compressor_PID);
  PIDs::collection.addItem(&compressorCurrent_PID);
  PIDs::collection.addItem(&leftHeight_PID);
  PIDs::collection.addItem(&rightHeight_PID);
  PIDs::collection.addItem(&leftPressure_PID);
  PIDs::collection.addItem(&rightPressure_PID);
  PIDs::collection.addItem(&systemPressure_PID);
  PIDs::collection.addItem(&compressorTemp_PID);
  PIDs::collection.addItem(&airFeedState_PID);
  PIDs::collection.addItem(&heightState_PID);

  // init output drivers
  leftFillDrv.init();
  rightFillDrv.init();
  leftDumpDrv.init();
  rightDumpDrv.init();
  leftSuckDrv.init();
  rightSuckDrv.init();
  airdryerDrv.init();
  compressorDrv.init();
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

}

// The loop function is called in an endless loop
void loop()
{
  Serial.print("loop:");Serial.println(millis(),DEC);

  static uint32_t lastErrCheck = millis();
  if (lastErrCheck + 1000 < millis()){
    // check each second
    lastErrCheck = millis();

    DTC *dtc = nullptr;

    // error check output drivers
    for (IOutput *outDrv = OutputsController.first();
         outDrv != nullptr;
         outDrv = OutputsController.next())
    {
       errorTypes err = outDrv->error();
       if (err != errorTypes::NoError) {
           dtc = DTCs.setDTC(outDrv->pid(), err);
           canIO.sendDTC(dtc);

           // then try to reset error if it was a glitch
           outDrv->resetError();
       }
    }

    // error check input drivers
    for (IInput *inDrv = InputsController.first();
         inDrv != nullptr;
         inDrv = InputsController.next())
    {
      errorTypes err = inDrv->error();
      if (err != errorTypes::NoError) {
          dtc = DTCs.setDTC(inDrv->pid(), err);
          canIO.sendDTC(dtc);

          // then try to reset error if it was a glitch
          inDrv->resetError();
      }
    }

    // error check system pressure controller
    errorTypes err = airFeedStateMachine.error();
    if (err != errorTypes::NoError) {
        dtc = DTCs.setDTC(airFeedStateMachine.pid(), err);
        canIO.sendDTC(dtc);

      // then try to reset if it was a glitch
      airFeedStateMachine.resetError();
    }
  }
  // call eventloop on statemachines
  airFeedStateMachine.loop();
  heightStateMachine.loop();

  canIO.loop();

}
