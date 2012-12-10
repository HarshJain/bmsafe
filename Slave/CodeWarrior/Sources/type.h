#ifndef TYPE_H
#define TYPE_H

#ifdef DEBUG

#define int8 char
#define int16 short
#define int32 int

#define uint8 unsigned int8
#define uint16 unsigned int16
#define uint32 unsigned int32

#else

#define int8 char
#define int16 int
#define int32 long

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

#endif // DEBUG
#endif // TYPE_H