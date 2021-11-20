#include "histogram.h"

void histogram_calculation( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc )
{
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
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        histogram_dataCalc[ i ].data = ( uint8_t ) i ;
        histogram_dataCalc[ i ].frequency = 0 ;
    }

    // Calculate the frequency of each data.
    for( size_t count = 0 ; count < bufferSize ; count++ )
    {
        histogram_dataCalc[ bufferIn[ count ] ].frequency++ ;
    }
}

void histogram_generateTable( histogram_dataCalc_t * histogram_dataCalc , uint8_t * tableOut )
{
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        for( uint16_t j = 0 ; j < i ; j++ )
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
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        tableOut[ i ] = histogram_dataCalc[ i ].data ;
    }
}
