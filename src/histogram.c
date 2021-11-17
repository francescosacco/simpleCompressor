#include "histogram.h"

typedef struct _HISTOGRAM_DATACALC_T_
{
    uint8_t  data ;
    uint32_t frequency ;
} histogram_dataCalc_t ;

void histogram_calculation( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc )
{
    if( bufferIn == ( uint8_t * ) NULL )
    {
        return ;
    }
    
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        histogram_dataCalc[ i ].data = ( uint8_t ) i ;
        histogram_dataCalc[ i ].frequency = 0 ;
    }

    for( size_t count = 0 ; count < bufferSize ; count++ )
    {
        histogram_dataCalc[ bufferIn[ count ] ].frequency++ ;
    }
}

void histogram_generateTable( histogram_dataCalc_t * histogram_dataCalc , uint8_t * tableOut )
{
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        for( uint16_t j = 0 ; j < 256 ; j++ )
        {
            if( histogram_dataCalc[ i ].frequency > histogram_dataCalc[ j ].frequency )
            {
                histogram_dataCalc_t histSwap ;

                histSwap.data      = histogram_dataCalc[ i ].data ;
                histSwap.frequency = histogram_dataCalc[ i ].frequency ;

                histogram_dataCalc[ i ].data      = histogram_dataCalc[ j ].data ;
                histogram_dataCalc[ i ].frequency = histogram_dataCalc[ j ].frequency ;

                histogram_dataCalc[ i ].data      = histSwap.data ;
                histogram_dataCalc[ i ].frequency = histSwap.frequency ;
            }
        }
    }
}
