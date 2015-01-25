#include "per_XDx512_L15Y.h"
#include "xgCAN_drv.h"
#include "xgate_vectors.h"

void XGATE_Init(void);
void SetIntPrio(char channel, char prio);
char GetIntPrio(char channel);
void Int_Init(void);