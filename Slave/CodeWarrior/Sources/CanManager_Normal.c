#include "type.h"

void LoadForTransmit(uint8* start, uint8 length) {
  uint8 i = 0;
  for (; i < length; ++i)  *(&CAN0TXDSR0 + i) = start[i];
}

uint8 ExistEmptyTransmitBuffer() {
  return CAN0TFLG;
}

void LoadTxParams(uint8 slaveId, uint8 msgId, uint8 length, uint8 priority) {
  uint16 id = MAKE_CAN_ID(slaveId, msgId);
  CAN0TXIDR0 = (uint8)(id >> 3);  // Load Id to IDR Registers. Id is on 11 bits,
  CAN0TXIDR1 = (uint8)(id << 5);  // and must be left justified.
  CAN0TXDLR = length;  // Set data length code
  CAN0TXTBPR = priority;  // Set priority. As compared to other buffered TX messages
}

uint8 SelectTxBuffer() {
  CAN0TBSEL = CAN0TFLG;  // Select lowest empty buffer
  return CAN0TBSEL;      // Backup selected buffer
}

void StartTx(uint8 bufferId) {
  CAN0TFLG = bufferId;
}