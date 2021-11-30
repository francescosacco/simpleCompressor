#include "compressor.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc().
#include <string.h> // To use memset().

/**********
 * Unit Test for histogram_calculation(), NULL pointer.
 **********/

void compressor_SET_checkData( void ) ;
void compressor_RUN_checkData( void ) ;
void compressor_CLEAN_checkData( void ) ;

int main( void )
{
    printf( "Unit Test for Compressor\n" ) ;

    /**********
     * Unit Test for histogram_calculation() for NULL pointer.
     **********/

    compressor_SET_checkData() ;
    compressor_RUN_checkData() ;
    compressor_CLEAN_checkData() ;

    return( 0 ) ;
}

/**********
 * Unit Test for histogram_calculation() for NULL pointer.
 **********/

uint8_t * dataIn ;
uint16_t * dataOutExpected ;
uint16_t * dataOutCalculated ;
compressedSize_t * dataSizeOut ;

size_t dataInSize = 0 ;

void compressor_SET_checkData( void )
{
    printf( "SET   - compressor() - Check Data\n" ) ;

    dataInSize = 1024 ;

    dataIn = ( uint8_t * ) malloc( dataInSize * sizeof( uint8_t ) ) ;
    if( ( uint8_t * ) NULL == dataIn )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }

    dataOutExpected = ( uint8_t * ) malloc( dataInSize * sizeof( uint16_t ) ) ;
    if( ( uint8_t * ) NULL == dataOutExpected )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }

    dataOutCalculated = ( uint8_t * ) malloc( dataInSize * sizeof( uint16_t ) ) ;
    if( ( uint8_t * ) NULL == dataOutCalculated )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }

    dataSizeOut = ( compressedSize_t * ) malloc( dataInSize * sizeof( compressedSize_t ) ) ;
    if( ( uint8_t * ) NULL == dataSizeOut )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }

    memset( ( void * ) dataOutCalculated , 0x00 , dataInSize ) ;

    for( size_t i = 0 ; i < dataInSize ; i++ )
    {
        dataIn[ i ] = ( uint8_t ) i ;
        
        if( i < 0x0F )
        {
            dataSizeOut[ i ] = compSize_halfByte ;
            dataOut[ i ] = ( uint16_t ) i ;
        }
        else if( i < 0x1E )
        {
            dataSizeOut[ i ] = compSize_fullByte ;
            dataOut[ i ] = ( uint16_t ) ( i - 0x000F ) + 0x00F0 ;
        }
        else
        {
            dataSizeOut[ i ] = compSize_fullByte ;
            dataOut[ i ] = ( uint16_t ) ( i - 0x001E ) + 0xFF00 ;
        }
    }
}

void compressor_RUN_checkData( void )
{
    printf( "RUN   - compressor() - Check Data\n" ) ;

    compressor
    
    if( isDataSignatureOk( ( void * ) phistogram_dataCalc , histogramSize ) == 0 )
    {
        printf( "\t[ ERROR ] The data structure was corrupted after NULL pointer [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        free( ( void * ) pDataIn ) ;
        free( phistogram_dataCalc ) ;
        exit( -1 ) ;
    }
    else
    {
        printf( "\t[  OK   ] Data not corrupted using NULL pointer.\n" ) ;
    }

    histogram_calculation( ( uint8_t * ) pDataIn , 0 , phistogram_dataCalc ) ;
    
    if( isDataSignatureOk( ( void * ) phistogram_dataCalc , histogramSize ) == 0 )
    {
        printf( "\t[ ERROR ] The data structure was corrupted after NULL pointer [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        free( ( void * ) pDataIn ) ;
        free( phistogram_dataCalc ) ;
        exit( -1 ) ;
    }
    else
    {
        printf( "\t[  OK   ] Data not corrupted using 0 size.\n" ) ;
    }
}

void compressor_CLEAN_checkData( void )
{
    printf( "CLEAN - compressor() - Check Data\n" ) ;

    free( ( void * ) dataIn  ) ;
    free( ( void * ) dataOutExpected ) ;
    free( ( void * ) dataOutCalculated ) ;
    free( ( void * ) dataSizeOut ) ;

    dataInSize = 0 ;
}
