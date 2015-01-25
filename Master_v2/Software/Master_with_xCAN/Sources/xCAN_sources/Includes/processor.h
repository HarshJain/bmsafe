/*	PROCESSOR SPECIFIC HEADER
 *	Copyright (c) 2004 by COSMIC Software
 */

#ifndef __PROCESSOR__
#define __PROCESSOR__	1

/*	function declarations
 */
@inline void _sif();
@inline int _par(int);
@inline int _carry(void);
@inline int _bffo(int);
@inline void _csem(int);
@inline int _ssem(int);

#endif
