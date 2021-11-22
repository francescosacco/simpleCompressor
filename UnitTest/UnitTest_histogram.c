#include "histogram.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc().
#include <string.h> // To use memset().

#define DATA_SIGNATURE                           0x5A

/**********
 * Support functions.
 **********/

uint8_t isDataSignatureOk( uint8_t * pData , size_t size ) ;

/**********
 * Unit Test for histogram_calculation(), NULL pointer.
 **********/

void histogram_calculation_SET_checkNullPointer( void ) ;
void histogram_calculation_RUN_checkNullPointer( void ) ;
void histogram_calculation_CLEAN_checkNullPointer( void ) ;

/**********
 * Unit Test for histogram_calculation(), Check data calculation.
 **********/

void histogram_calculation_SET_checkDataCalc( void ) ;
void histogram_calculation_RUN_checkDataCalc( void ) ;
void histogram_calculation_CLEAN_checkDataCalc( void ) ;

/**********
 * Unit Test for histogram_generateTable(), Check data calculation.
 **********/

void histogram_generateTable_SET_checkDataCalc( void ) ;
void histogram_generateTable_RUN_checkDataCalc( void ) ;
void histogram_generateTable_CLEAN_checkDataCalc( void ) ;

int main( void )
{
    printf( "Unit Test for Histogram\n" ) ;

    /**********
     * Unit Test for histogram_calculation() for NULL pointer.
     **********/

    histogram_calculation_SET_checkNullPointer() ;
    histogram_calculation_RUN_checkNullPointer() ;
    histogram_calculation_CLEAN_checkNullPointer() ;

    /**********
     * Unit Test for histogram_calculation(), Check data calculation.
     **********/

    histogram_calculation_SET_checkDataCalc() ;
    histogram_calculation_RUN_checkDataCalc() ;
    histogram_calculation_CLEAN_checkDataCalc() ;

    /**********
     * Unit Test for histogram_generateTable(), Check data calculation.
     **********/

    histogram_generateTable_SET_checkDataCalc() ;
    histogram_generateTable_RUN_checkDataCalc() ;
    histogram_generateTable_CLEAN_checkDataCalc() ;

    return( 0 ) ;
}

/**********
 * Unit Test for histogram_calculation() for NULL pointer.
 **********/

histogram_dataCalc_t * phistogram_dataCalc ;
uint8_t * pDataIn ;
size_t histogramSize ;

void histogram_calculation_SET_checkNullPointer( void )
{
    printf( "SET   - histogram_calculation() - Check Null Pointer\n" ) ;

    histogramSize = 256 * sizeof( histogram_dataCalc_t ) ;

    phistogram_dataCalc = ( histogram_dataCalc_t * ) malloc( histogramSize ) ;
    if( ( histogram_dataCalc_t * ) NULL == phistogram_dataCalc )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }
    
    ( void ) memset( ( void * ) phistogram_dataCalc , DATA_SIGNATURE , histogramSize ) ;

    pDataIn = ( uint8_t * ) malloc( 256 ) ;
    if( ( uint8_t * ) NULL == pDataIn )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        free( ( void * ) phistogram_dataCalc ) ;
        exit( -1 ) ;
    }
    
    ( void ) memset( ( void * ) pDataIn , DATA_SIGNATURE , 256 ) ;
}

void histogram_calculation_RUN_checkNullPointer( void )
{
    printf( "RUN   - histogram_calculation() - Check Null Pointer\n" ) ;

    histogram_calculation( ( uint8_t * ) NULL , 256 , phistogram_dataCalc ) ;
    
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

void histogram_calculation_CLEAN_checkNullPointer( void )
{
    printf( "CLEAN - histogram_calculation() - Check Null Pointer\n" ) ;

    free( ( void * ) pDataIn ) ;
    free( ( void * ) phistogram_dataCalc ) ;
}

/**********
 * Unit Test for histogram_calculation(), Check data calculation.
 **********/

histogram_dataCalc_t * pHist_dataCalcChk ;
uint8_t * pDataChk ;
size_t histSizeChk ;

void histogram_calculation_SET_checkDataCalc( void )
{
    printf( "SET   - histogram_calculation() - Check Data Calculation\n" ) ;
    
    histSizeChk = 256 * sizeof( histogram_dataCalc_t ) ;

    pHist_dataCalcChk = ( histogram_dataCalc_t * ) malloc( histSizeChk ) ;
    if( ( histogram_dataCalc_t * ) NULL == pHist_dataCalcChk )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }
    
    ( void ) memset( ( void * ) pHist_dataCalcChk , 0x00 , histSizeChk ) ;

    pDataChk = ( uint8_t * ) malloc( 256 ) ;
    if( ( uint8_t * ) NULL == pDataChk )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        free( ( void * ) pHist_dataCalcChk ) ;
        exit( -1 ) ;
    }

    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        pDataChk[ i ] = ( uint8_t ) i ;
    }
}

void histogram_calculation_RUN_checkDataCalc( void )
{
    printf( "RUN   - histogram_calculation() - Check Data Calculation\n" ) ;

    histogram_calculation( pDataChk , 256 , pHist_dataCalcChk ) ;
    
    for( size_t i = 0 ; i < 256 ; i++ )
    {
        if( ( pHist_dataCalcChk[ i ].data != ( ( uint8_t ) i ) ) || ( 1 != pHist_dataCalcChk[ i ].frequency ) )
        {
            printf( "\t[ ERROR ] Buffer is different than expected [%s,%d]\n" , __FILE__ , __LINE__ ) ;
            free( ( void * ) pDataChk ) ;
            free( ( void * ) pHist_dataCalcChk ) ;
            exit( -1 ) ;
        }
    }
    printf( "\t[  OK   ] Data not corrupted doing calculation.\n" ) ;
}

void histogram_calculation_CLEAN_checkDataCalc( void )
{
    printf( "CLEAN - histogram_calculation() - Check Data Calculation\n" ) ;
    
    free( ( void * ) pDataChk ) ;
    free( ( void * ) pHist_dataCalcChk ) ;
}

/**********
 * Unit Test for histogram_generateTable(), Check data calculation.
 **********/

uint8_t * pTabOut ;
histogram_dataCalc_t * pTabHist ;
size_t pTabHistSize ;

void histogram_generateTable_SET_checkDataCalc( void )
{
    printf( "SET   - histogram_generateTable() - Check Data Calculation\n" ) ;
    
    pTabOut = ( uint8_t * ) malloc( 256 ) ;
    if( ( uint8_t * ) NULL == pTabOut )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        exit( -1 ) ;
    }

    ( void ) memset( ( void * ) pTabOut , DATA_SIGNATURE , 256 ) ;

    pTabHistSize = 256 * sizeof( histogram_dataCalc_t ) ;

    pTabHist = ( histogram_dataCalc_t * ) malloc( pTabHistSize ) ;
    if( ( histogram_dataCalc_t * ) NULL == pTabHist )
    {
        printf( "\t[ ERROR ] Error allocation data [%s,%d]\n" , __FILE__ , __LINE__ ) ;
        free( pTabOut ) ;
        exit( -1 ) ;
    }
    
    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        pTabHist[ i ].data      = ( uint8_t  ) i ;
        pTabHist[ i ].frequency = ( uint32_t ) i ;
    }
}

void histogram_generateTable_RUN_checkDataCalc( void )
{
    printf( "RUN   - histogram_generateTable() - Check Data Calculation\n" ) ;
    
    histogram_generateTable( pTabHist , pTabOut ) ;

    for( uint16_t i = 0 ; i < 256 ; i++ )
    {
        uint8_t expectedData ;
        
        expectedData = ( uint8_t ) ( 0xFF - i ) ;
        
        if( pTabOut[ i ] != expectedData )
        {
            printf( "\t[ ERROR ] Buffer is different than expected [%s,%d]\n" , __FILE__ , __LINE__ ) ;
            free( ( void * ) pTabHist ) ;
            free( ( void * ) pTabOut ) ;
            exit( -1 ) ;
        }
    }

    printf( "\t[  OK   ] Data calculation correct.\n" ) ;
}

void histogram_generateTable_CLEAN_checkDataCalc( void )
{
    printf( "CLEAN - histogram_generateTable() - Check Data Calculation\n" ) ;

    free( ( void * ) pTabOut ) ;
    free( ( void * ) pTabHist ) ;
}

/**********
 * Support functions.
 **********/

uint8_t isDataSignatureOk( uint8_t * pData , size_t size )
{
    for( size_t i = 0 ; i < size ; i++ )
    {
        if( DATA_SIGNATURE != pData[ i ] )
        {
            return( 0 ) ;
        }            
    }
    
    return( 1 ) ;
}
