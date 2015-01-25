#ifndef __XGATE_H_
#define __XGATE_H_

typedef struct {
  void (*isr)(void);	/* address of the routine itself */
/*	unsigned int *const data; */		/* private data of that routine */	
	void *const data;			        /* private data of that routine */	
} xgate_vector;

#pragma CONST_SEG XGATE_VECTORS  /* assign the vector table in separate segment for dedicated placement in linker parameter file */
extern const xgate_vector XGATEVectorTable[];	/* XGATE vector table */

/* used with XGATE_CODE section (see prm file) */

extern char __SEG_START_XGATE_CODE[];
#define XGATE_ROM_CODE_START ((void*)__SEG_START_XGATE_CODE)

extern char __SEG_SIZE_XGATE_CODE[];
#define XGATE_CODE_SIZE ((int)__SEG_SIZE_XGATE_CODE)

extern char __SEG_RELOCATE_TO_XGATE_CODE[];
#define XGATE_RAM_CODE_START ((void*)__SEG_RELOCATE_TO_XGATE_CODE)

#endif /* __XGATE_H_ */
