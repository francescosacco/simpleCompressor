#include "compressor.h"
#include <string.h> // To use memset().

uint8_t compressor_8to4( uint8_t dataIn , uint16_t * dataOut )
{
    if( dataOut == ( uint16_t * ) NULL )
    {
        return( 0 ) ;
    }
    
    if( dataIn < 0x0F ) // First 15 bytes, from 0h to Eh. 
    {
        *dataOut = ( uint16_t ) dataIn ;
        return( 4 ) ;
    }

    if( dataIn < 0x1E ) // Next 15 bytes, from F0h to FEh.
    {
        *dataOut  = ( uint16_t ) dataIn ;
        *dataOut += 0x00F0 - 0x000F ;
        return( 8 ) ;
    }

    // The rest of 226 bytes, from FF00h to FFE1h.
    *dataOut  = ( uint16_t ) dataIn ;
    *dataOut += 0xFF00 - 0x001E ;
    return( 16 ) ;
}

uint8_t compressor_4to1( uint8_t dataIn , uint16_t * dataOut )
{
    uint16_t compressBlock[] = { 
    0x0000 , // 0 
    0x0002 , // 10
    0x0006 , // 110
    0x000E , // 1110
    0x001E , // 11110
    0x003E , // 111110
    0x007E , // 1111110
    0x00FE , // 11111110
    0x01FE , // 111111110
    0x03FE , // 1111111110
    0x07FE , // 11111111110
    0x0FFE , // 111111111110
    0x1FFE , // 1111111111110
    0x3FFE , // 11111111111110
    0x7FFE , // 111111111111110
    0xFFFE   // 1111111111111110
    } ;

    if( ( uint16_t * ) NULL == dataOut )
    {
        return( 0 ) ;
    }

    *dataOut = compressBlock[ dataIn & 0x0F ] ;
    return( ( dataIn & 0x0F ) + 1 ) ;
}

uint8_t compressor_4to3( uint8_t dataIn , uint16_t * dataOut )
{
    uint16_t compressBlock[] = { 
    0x0000 , // 000 
    0x0001 , // 001
    0x0002 , // 010
    0x0003 , // 011
    0x0004 , // 100
    0x0005 , // 101
    0x0006 , // 110
    0x001C , // 111.00
    0x001D , // 111.01
    0x001E , // 111.10
    0x00F8 , // 111.11.000
    0x00F9 , // 111.11.001
    0x00FA , // 111.11.010
    0x00FB , // 111.11.011
    0x00FC , // 111.11.100
    0x00FD   // 111.11.101
    } ;

    uint8_t compressSize[] = { 3 , 3 , 3 , 3 , 3 , 3 , 3 , 5 , 5 , 5 , 8 , 8 , 8 , 8 , 8 , 8 } ;

    if( ( uint16_t * ) NULL == dataOut )
    {
        return( 0 ) ;
    }

    *dataOut = compressBlock[ dataIn & 0x0F ] ;
    return( compressSize[ dataIn & 0x0F ] ) ;
}

void writeCompressed_init( writeCompressed_handle_t * pWriteHandle , writeFunction_t pWriteFuncIn )
{
    if( ( writeCompressed_handle_t * ) NULL == pWriteHandle )
    {
        return ;
    }

    memset( ( void * ) pWriteHandle , 0x00 , sizeof( writeCompressed_handle_t ) ) ;
    pWriteHandle->pWriteFunc = pWriteFuncIn ;
}

void writeCompressed_data( uint16_t dataIn , uint8_t dataSize , writeCompressed_handle_t * pWriteHandle )
{
    uint16_t dataInWrite ;
    uint8_t  dataInWriteSize ;
    
    if( ( writeCompressed_handle_t * ) NULL == pWriteHandle )
    {
        return ;
    }

    if( 0 == dataSize )
    {
        return ;
    }

    // Check the consistency of the data.
    if( pWriteHandle->sizeLoaded > 8 )
    {
        pWriteHandle->sizeLoaded = 8 ;
    }
    dataInWriteSize = ( dataSize > 16 ) ? ( 16 ) : ( dataSize ) ; 
    dataInWrite     = dataIn << ( 16 - dataInWriteSize ) ;

    // Are there data enough to write?
    do
    {
        uint8_t bitsToLoad ;

        if( pWriteHandle->sizeLoaded >= 8 )
        {
            if( pWriteHandle->pWriteFunc )
            {
                pWriteHandle->pWriteFunc( pWriteHandle->data ) ;
            }
            pWriteHandle->sizeLoaded = 0 ;
            pWriteHandle->data = 0x00 ;
            pWriteHandle->finalSize++ ;
        }
        
        // How many bits are free?
        bitsToLoad = 8 - pWriteHandle->sizeLoaded ;
        // Are there space to load?
        if( bitsToLoad )
        {
            // Yes.
            
            // Do I have more space than bits to load?
            if( bitsToLoad > dataInWriteSize )
            {
                // Yes.
                // So, load everything.
                pWriteHandle->data |= ( uint8_t ) ( dataInWrite >> ( 8 + pWriteHandle->sizeLoaded ) ) ;
                pWriteHandle->sizeLoaded += dataInWriteSize ;
                bitsToLoad -= dataInWriteSize ;
                dataInWriteSize = 0 ;
            }
            else
            {
                // No.
                // So, move just the possible amount.
                pWriteHandle->data |= ( uint8_t ) ( dataInWrite >> ( 8 + pWriteHandle->sizeLoaded ) ) ;
                pWriteHandle->sizeLoaded += bitsToLoad ;
                dataInWriteSize -= bitsToLoad ;
                bitsToLoad = 0 ;
            }
        }
    } while( ( pWriteHandle->sizeLoaded + dataInWriteSize ) >= 8 ) ;
}

uint32_t writeCompressed_end( writeCompressed_handle_t * pWriteHandle )
{
    if( ( writeCompressed_handle_t * ) NULL == pWriteHandle )
    {
        return( 0 ) ;
    }

    writeCompressed_data( COMPRESS_END , 16 , pWriteHandle ) ;

    if( pWriteHandle->sizeLoaded != 0 )
    {
        if( pWriteHandle->pWriteFunc )
        {
            pWriteHandle->pWriteFunc( pWriteHandle->data ) ;
        }
        pWriteHandle->sizeLoaded = 0 ;
        pWriteHandle->data = 0x00 ;
        pWriteHandle->finalSize++ ;
    }

    return( pWriteHandle->finalSize ) ;
}
