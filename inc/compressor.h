#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

#define COMPRESS_END 0xFFFF

typedef void ( * writeFunction_t )( uint8_t ) ;

typedef struct _WRITECOMPRESSED_HANDLE_T_
{
    uint8_t data ;
    uint8_t sizeLoaded ;
    writeFunction_t pWriteFunc ;
    uint32_t finalSize ;
} writeCompressed_handle_t ;

uint8_t compressor_8to4( uint8_t dataIn , uint16_t * dataOut ) ;
uint8_t compressor_4to1( uint8_t dataIn , uint16_t * dataOut ) ;

void writeCompressed_init( writeCompressed_handle_t * pWriteHandle , writeFunction_t pWriteFuncIn ) ;
void writeCompressed_data( uint16_t dataIn , uint8_t dataSize , writeCompressed_handle_t * pWriteHandle ) ;
uint32_t writeCompressed_end( writeCompressed_handle_t * pWriteHandle ) ;

#ifdef __cplusplus
    }
#endif

#endif // _COMPRESSOR_H_
