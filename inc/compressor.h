#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum _COMPRESSEDSIZE_T_
{
    compSize_halfByte ,
    compSize_fullByte ,
    compSize_doubleByte ,
    compSize_error
} compressedSize_t ;

compressedSize_t compressor( uint8_t dataIn , uint16_t * dataOut ) ;

#ifdef __cplusplus
    }
#endif

#endif // _COMPRESSOR_H_
