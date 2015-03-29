![http://www.oshwa.org/wp-content/uploads/2014/03/oshw-logo-200-px.png](http://www.oshwa.org/wp-content/uploads/2014/03/oshw-logo-200-px.png)

BMSafe is a multi-chemistry open source battery management system (BMS) designed for safety critical systems like EVs.

It uses a master/slave topology, communicating via a high speed CAN link.

PCBs are designed in [Eagle](Eagle.md), code is written in C, IDE used is CodeWarrior, and ICs are Freescale and Linear.

Documentation is coming soon (in redaction).

Originally developed at the École Polytechnique de Montréal.

[Contact us](mailto:bmsafe.project@gmail.com), if you are interested by the product, have questions or suggestions !


**Features:**

  * 4 to 12 cells/slave module (not tested with less than 12 cells)
  * 12 thermistors/slave (for temperature measurement)
  * Isolated High Speed CAN 2.0 Communication
  * Isolated USB/RS232 (BMS master)
  * Passive balancing
  * Cell voltage range: 0 to 5V
  * CAN termination available on each circuit (unpopulated whe unused)
  * Easy slave connectivity (daisy-chain)
  * Relay (MCR, precharge and charge) control
  * Compatible with Brusa NLG5 chargers (CAN controlled charging)
  * Many security features (software and hardware)
  * Slaves are directly powered by the cells
  * Master can be powered by an external voltage source from 7 to 36V
  * Temperature measure range (-40 to 140˚C)
  * Hall effect current measurement (not implemented yet)
  * State of charge (SOC) calculation (not implemented yet)