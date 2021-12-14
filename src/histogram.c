#include "histogram.h"

void histogram_calculation_8bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc )
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

void histogram_generateTable_8bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert )
{
    // Check null pointer.
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
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
        if( convert != ( uint8_t * ) NULL )
        {
            convert[ histogram_dataCalc[ i ].data ] = ( uint8_t ) i ;
        }

        if( unConvert != ( uint8_t * ) NULL )
        {
            unConvert[ i ] = histogram_dataCalc[ i ].data ;
        }
    }
}

void histogram_calculation_4bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc )
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
    for( uint8_t i = 0 ; i < 16 ; i++ )
    {
        histogram_dataCalc[ i ].data = i ;
        histogram_dataCalc[ i ].frequency = 0 ;
    }

    // Calculate the frequency of each data.
    for( size_t count = 0 ; count < bufferSize ; count++ )
    {
        uint8_t dataIndex ;

        dataIndex = bufferIn[ count ] ;
        dataIndex >>= 4 ;

        histogram_dataCalc[ dataIndex ].frequency++ ;

        dataIndex = bufferIn[ count ] ;
        dataIndex &= 0x0F ;
        
        histogram_dataCalc[ dataIndex ].frequency++ ;
    }
}

void histogram_generateTable_4bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert )
{
    // Check null pointer.
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
    for( uint16_t i = 0 ; i < 16 ; i++ )
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
    for( uint16_t i = 0 ; i < 16 ; i++ )
    {
        if( convert != ( uint8_t * ) NULL )
        {
            convert[ histogram_dataCalc[ i ].data ] = ( uint8_t ) i ;
        }

        if( unConvert != ( uint8_t * ) NULL )
        {
            unConvert[ i ] = histogram_dataCalc[ i ].data ;
        }
    }
}

void histogram_calculation_2bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc )
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
    for( uint8_t i = 0 ; i < 4 ; i++ )
    {
        histogram_dataCalc[ i ].data = i ;
        histogram_dataCalc[ i ].frequency = 0 ;
    }

    // Calculate the frequency of each data.
    for( size_t count = 0 ; count < bufferSize ; count++ )
    {
        uint8_t dataIndex ;

        dataIndex = bufferIn[ count ] ;
        dataIndex >>= 6 ;
        dataIndex  &= 0x03 ;
        histogram_dataCalc[ dataIndex ].frequency++ ;

        dataIndex = bufferIn[ count ] ;
        dataIndex >>= 4 ;
        dataIndex  &= 0x03 ;
        histogram_dataCalc[ dataIndex ].frequency++ ;

        dataIndex = bufferIn[ count ] ;
        dataIndex >>= 2 ;
        dataIndex  &= 0x03 ;
        histogram_dataCalc[ dataIndex ].frequency++ ;

        dataIndex = bufferIn[ count ] ;
        dataIndex >>= 0 ;
        dataIndex &= 0x03 ;
        histogram_dataCalc[ dataIndex ].frequency++ ;
    }
}

void histogram_generateTable_2bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert )
{
    // Check null pointer.
    if( histogram_dataCalc == ( histogram_dataCalc_t * ) NULL )
    {
        return ;
    }
    
    for( uint16_t i = 0 ; i < 4 ; i++ )
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
    for( uint16_t i = 0 ; i < 4 ; i++ )
    {
        if( convert != ( uint8_t * ) NULL )
        {
            convert[ histogram_dataCalc[ i ].data ] = ( uint8_t ) i ;
        }

        if( unConvert != ( uint8_t * ) NULL )
        {
            unConvert[ i ] = histogram_dataCalc[ i ].data ;
        }
    }
}
