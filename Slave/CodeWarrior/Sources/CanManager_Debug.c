#include <stdio.h>
#include <string.h>

#include "type.h"

uint8 g_SendBuffer[8], g_SlaveId, g_MsgId, g_Length, g_Priority;

void LoadForTransmit(uint8* start, uint8 length) {
  memcpy(g_SendBuffer, start, length);
}

uint8 ExistEmptyTransmitBuffer() {
  return 1;
}

void LoadTxParams(uint8 slaveId, uint8 msgId, uint8 length, uint8 priority) {
  g_SlaveId = slaveId;
  g_MsgId = msgId;
  g_Length = length;
  g_Priority = priority;
}

uint8 SelectTxBuffer() {
  return 0;
}

void StartTx(uint8 bufferId) {
  printf("CAN<Start Tx> : {\n\t'SlaveId' : %d, 'MsgId' : %d, 'Length' : %d, 'Priority' : %d,\n\t'Buffer' : [",
    g_SlaveId, g_MsgId, g_Length, g_Priority);

  uint8 i = 0;
  for (; i < g_Length; ++i) {
    if (i)  printf(", ");
    printf("%d", g_SendBuffer[i]);
  }
  printf("]\n}\n");
}