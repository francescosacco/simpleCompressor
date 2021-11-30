#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc() and free().

#include "histogram.h"

uint32_t fileSize( FILE * fileIn ) ;

int main( int argc , char * argv[] )
{
    FILE * fileIn ;
    uint32_t fileInSize ;

    uint8_t * dataIn = NULL ;
    
    histogram_dataCalc_t * pHistogramDataCalc ;
    uint8_t * pConvTable ;

    printf( "Simple Compressor - %s - github.com/francescosacco\n" , __DATE__ ) ;
    
    // Check if there are a file input.
    if( argc != 2 )
    {
        printf( "\tUse: SimpleCompressor <file>\n\n" ) ;
        return( 0 ) ;
    }
    
    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( ( FILE * ) NULL == fileIn )
    {
        printf( "\tError to open \"%s\"!\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    
    fileInSize = fileSize( fileIn ) ;
    if( 0 == fileInSize )
    {
        fclose( fileIn ) ;
        printf( "\tFile \"%s\" has size zero!\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    
    dataIn = ( uint8_t * ) malloc( ( size_t ) fileInSize ) ;
    if( ( uint8_t * ) NULL == dataIn )
    {
        fclose( fileIn ) ;
        printf( "\tError to alloc %u size!\n" , fileInSize ) ;
        return( -1 ) ;
    }
    
    printf( "\tLoading \"%s\"... " , argv[ 1 ] ) ;
    for( uint32_t i = 0 ; i < fileInSize ; i++ )
    {
        dataIn[ i ] = ( uint8_t ) getc( fileIn ) ;
    }
    printf( "%u Bytes.\n" , fileInSize ) ;

    fclose( fileIn ) ;
    fileIn = ( FILE * ) NULL ;

    printf( "\tCalculating histogram...\n" ) ;
    pHistogramDataCalc = ( histogram_dataCalc_t * ) malloc( 256 * sizeof( histogram_dataCalc_t ) ) ;
    if( ( histogram_dataCalc_t * ) NULL == pHistogramDataCalc )
    {
        free( ( void * ) dataIn ) ;
        printf( "\tError allocating histogram!\n" ) ;
        return( -1 ) ;
    }

    histogram_calculation( dataIn , ( size_t ) fileInSize , pHistogramDataCalc ) ;

    printf( "\t\tData -    Bytes | Data -    Bytes | Data -    Bytes | Data -    Bytes\n" ) ;
    for( uint16_t i = 0 ; i < 256 ; i += 4 )
    {
        printf( "\t\t%02Xh  - %8u | " , ( pHistogramDataCalc + i + 0 )->data , ( pHistogramDataCalc + i + 0 )->frequency ) ;
        printf(     "%02Xh  - %8u | " , ( pHistogramDataCalc + i + 1 )->data , ( pHistogramDataCalc + i + 1 )->frequency ) ;
        printf(     "%02Xh  - %8u | " , ( pHistogramDataCalc + i + 2 )->data , ( pHistogramDataCalc + i + 2 )->frequency ) ;
        printf(     "%02Xh  - %8u\n"  , ( pHistogramDataCalc + i + 3 )->data , ( pHistogramDataCalc + i + 3 )->frequency ) ;
    }

    printf( "\tGenerating conversion table...\n" ) ;

    pConvTable = ( uint8_t * ) malloc( 256 ) ;
    if( ( uint8_t * ) NULL == pConvTable )
    {
        free( ( void * ) pHistogramDataCalc ) ;
        free( ( void * ) dataIn ) ;
        printf( "\tError allocating Convertion Table!\n" ) ;
        return( -1 ) ;
    }

    histogram_generateTable( pHistogramDataCalc , pConvTable ) ;

    printf( "\t\tFrom - To | From - To | From - To | From - To\n" ) ;
    for( uint16_t i = 0 ; i < 256 ; i += 4 )
    {
        printf( "\t\t%02Xh - %02Xh | " , pConvTable[ i + 0 ] , ( uint8_t ) ( i + 0 ) ) ;
        printf(     "%02Xh - %02Xh | " , pConvTable[ i + 1 ] , ( uint8_t ) ( i + 1 ) ) ;
        printf(     "%02Xh - %02Xh | " , pConvTable[ i + 2 ] , ( uint8_t ) ( i + 2 ) ) ;
        printf(     "%02Xh - %02Xh\n"  , pConvTable[ i + 3 ] , ( uint8_t ) ( i + 3 ) ) ;
    }


    
    
    free( ( void * ) pConvTable ) ;
    free( ( void * ) pHistogramDataCalc ) ;
    free( ( void * ) dataIn ) ;
    
    return( 0 ) ;
}

uint32_t fileSize( FILE * fileIn )
{
    uint32_t ret = 0 ;
    fpos_t pos ;
    int fPosRet ;
    
    fPosRet = fgetpos( fileIn , &pos ) ;
    if( fPosRet )
    {
        return( ret ) ;
    }
    
    fPosRet = fseek( fileIn , 0 , SEEK_END ) ;
    if( fPosRet )
    {
        return( ret ) ;
    }
    
    ret = ( uint32_t ) ftell( fileIn ) ;
    if( ret == 0xFFFFFFFF )
    {
        ret = 0 ;
        return( ret ) ;
    }

    ( void ) fsetpos( fileIn , &pos ) ;
    
    return( ret ) ;
}
