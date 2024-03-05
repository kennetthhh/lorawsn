#include "header.h"

/* ------ Instantiate LoRaWirelessSensorNetwork ------- */
LoRaWirelessSensorNetwork lwsn;// Instantiation of LoraWSN

void setup() 
{
  /* ------------------ Main Program -------------------- */
  lwsn.initNode();               // Initiate the peripherals
  /* ---------------------------------------------------- */

  return;
}

void loop() 
{
  /* ------------------ Nothing here ---------------------*/
  lwsn.processCyclic();          // Process the cyclic task

  return;
}
