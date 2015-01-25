
#ifndef __XGATE_H_
#define __XGATE_H_

/* XGATE vector table */
typedef void (*_NEAR XGATE_Function)(int);
typedef struct {
  XGATE_Function pc;        /* pointer to the handler */
  int dataptr;              /* pointer to the data of the handler */
} XGATE_TableEntry;
extern XGATE_TableEntry XGATE_VectorTable[0x80];


/* used with XGATE_CODE section (see prm file) */
extern char __SEG_START_XGATE_CODE[];
#define XGATE_ROM_CODE_START ((void*)__SEG_START_XGATE_CODE)

extern char __SEG_SIZE_XGATE_CODE[];
#define XGATE_CODE_SIZE ((int)__SEG_SIZE_XGATE_CODE)

extern char __SEG_RELOCATE_TO_XGATE_CODE[];
#define XGATE_RAM_CODE_START ((void*)__SEG_RELOCATE_TO_XGATE_CODE)

#endif /* __XGATE_H_ */
