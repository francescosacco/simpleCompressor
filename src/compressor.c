#include "compressor.h"

compressedSize_t compressor( uint8_t dataIn , uint16_t * dataOut )
{
    if( dataOut == ( uint16_t * ) NULL )
    {
        return( compSize_error ) ;
    }
    
    if( dataIn < 0x0F ) // First 15 bytes, from 0h to Eh. 
    {
        *dataOut = ( uint16_t ) dataIn ;
        return( compSize_halfByte ) ;
    }

    if( dataIn < 0x1E ) // Next 15 bytes, from F0h to FEh.
    {
        *dataOut  = ( uint16_t ) dataIn ;
        *dataOut += 0x00F0 - 0x000F ;
        return( compSize_fullByte ) ;
    }

    // The rest of 226 bytes, from FF00h to FFE1h.
    *dataOut  = ( uint16_t ) dataIn ;
    *dataOut += 0xFF00 - 0x001E ;
    return( compSize_doubleByte ) ;
}
