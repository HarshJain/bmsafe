#include <stdio.h>
#include "..\..\Slave\CodeWarrior\Sources\CANSlave.h"
#include "..\..\Slave\CodeWarrior\Sources\Defines.h"

void PrintMsg(Message* msg);
void SendTempMsg(Can* can);
void SendVoltMsg(Can* can);
void SendFirmMsg(Can* can);
void SendInitMsg(Can* can);
void SendEquiMsg(Can* can);

int main(void) {
  
  Can can;
  uint8 slaveId = 1;

  InitCan(slaveId, &can);

  SendTempMsg(&can);
  SendVoltMsg(&can);
  SendFirmMsg(&can);
  SendInitMsg(&can);
  SendEquiMsg(&can);

  return 0;
}

void SendTempMsg(Can* can) {
  Message msg;
  uint16 temps[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};

  InitMsg(&msg, CAN_TEMP_ID, temps);
  can->Send(can->SlaveID, &msg);
}

void SendVoltMsg(Can* can) {
  Message msg;
  uint16 volts[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};

  InitMsg(&msg, CAN_VOLTAGES_ID, volts);
  can->Send(can->SlaveID, &msg);
}

void SendFirmMsg(Can* can) {
  Message msg;
  InitMsg(&msg, CAN_FIRMWARE_REVISION_ID, 0);
  can->Send(can->SlaveID, &msg);
}

void SendInitMsg(Can* can) {
  Message msg;
  InitMsg(&msg, CAN_INIT_REPORT_ID, 0);
  can->Send(can->SlaveID, &msg);
}

void SendEquiMsg(Can* can) {
  Message msg;
  uint16 equi[10] = {0x01,0x02,0xFFFF};

  InitMsg(&msg, CAN_EQUI_REPORT_ID, equi);
  can->Send(can->SlaveID, &msg);
}

void PrintMsg(Message* msg) {
  printf("Msg = {'type' : %d, 'Priority' : %d, 'Length' : %d}\n",
    msg->Type,
    msg->Priority,
    msg->Length);  
}