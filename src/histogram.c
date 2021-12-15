#include "histogram.h"

void histogram_calculation( uint8_t * bufferIn , size_t bufferSize , eHistDataSize_t eDataSize , histogram_dataCalc_t * histogram_dataCalc )
{
    uint32_t table_dataSize[] = { 4 , 16 , 256 , 65536 } ;
    
    // Check null pointer.
    if( bufferIn == ( uint8_t * ) NULL )
    {
        return ;
    }
    
    // Check null pointer.
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
    // Check zero size.
    if( bufferSize == 0 )
    {
        return ;
    }

    // Initialize output buffer.
    for( uint32_t i = 0 ; i < table_dataSize[ eDataSize ] ; i++ )
    {
        histogram_dataCalc[ i ].data = ( uint16_t ) i ;
        histogram_dataCalc[ i ].frequency = 0 ;
    }

    // Calculate the frequency of each data.
    switch( eDataSize )
    {
    case eHistDataSize_2bits :
        for( size_t count = 0 ; count < bufferSize ; count++ )
        {
            histogram_dataCalc[ ( bufferIn[ count ] >> 0 ) & 0x03 ].frequency++ ;
            histogram_dataCalc[ ( bufferIn[ count ] >> 2 ) & 0x03 ].frequency++ ;
            histogram_dataCalc[ ( bufferIn[ count ] >> 4 ) & 0x03 ].frequency++ ;
            histogram_dataCalc[ ( bufferIn[ count ] >> 6 ) & 0x03 ].frequency++ ;
        }
        break ;
    case eHistDataSize_4bits :
        for( size_t count = 0 ; count < bufferSize ; count++ )
        {
            histogram_dataCalc[ ( bufferIn[ count ] >> 0 ) & 0x0F ].frequency++ ;
            histogram_dataCalc[ ( bufferIn[ count ] >> 4 ) & 0x0F ].frequency++ ;
        }
        break ;
    case eHistDataSize_8bits :
        for( size_t count = 0 ; count < bufferSize ; count++ )
        {
            histogram_dataCalc[ bufferIn[ count ] ].frequency++ ;
        }
        break ;
    case eHistDataSize_16bits :
        for( size_t count = 0 ; count < bufferSize ; count += 2 )
        {
            uint16_t data ;
            
            data   = ( uint16_t ) bufferIn[ count ] ;
            data <<= 8 ;
            data  |= ( uint16_t ) bufferIn[ count + 1 ] ;
            
            histogram_dataCalc[ data ].frequency++ ;
        }
        break ;
    }
}

void histogram_generateTable( histogram_dataCalc_t * histogram_dataCalc , eHistDataSize_t eDataSize , uint16_t * convert , uint16_t * unConvert )
{
    uint32_t table_dataSize[] = { 4 , 16 , 256 , 65536 } ;

    // Check null pointer.
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
    for( uint32_t i = 0 ; i < table_dataSize[ eDataSize ] ; i++ )
    {
        for( uint32_t j = 0 ; j < i ; j++ )
        {
            // If the data is bigger, swap the data.
            if( histogram_dataCalc[ i ].frequency > histogram_dataCalc[ j ].frequency )
            {
                histogram_dataCalc_t histSwap ;

                histSwap.data      = histogram_dataCalc[ i ].data ;
                histSwap.frequency = histogram_dataCalc[ i ].frequency ;

                histogram_dataCalc[ i ].data      = histogram_dataCalc[ j ].data ;
                histogram_dataCalc[ i ].frequency = histogram_dataCalc[ j ].frequency ;

                histogram_dataCalc[ j ].data      = histSwap.data ;
                histogram_dataCalc[ j ].frequency = histSwap.frequency ;
            }
        }
    }
    
    // Move just the organized data list to the output.
    for( uint32_t i = 0 ; i < table_dataSize[ eDataSize ] ; i++ )
    {
        if( convert != ( uint16_t * ) NULL )
        {
            convert[ histogram_dataCalc[ i ].data ] = ( uint16_t ) i ;
        }

        if( unConvert != ( uint16_t * ) NULL )
        {
            unConvert[ i ] = histogram_dataCalc[ i ].data ;
        }
    }
}
