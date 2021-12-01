#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc() and free().
#include <stdbool.h>
#include <string.h> // To use strcmp().

#include "histogram.h"
#include "compressor.h"

uint32_t fileSize( FILE * fileIn ) ;
void writeFunction( uint8_t dataIn ) ;

void verbose_histogram_8bits( bool verbose , histogram_dataCalc_t * pHistogram_dataCalc ) ;
void verbose_convert_8bits( bool verbose , uint8_t * tableIn ) ;

int main( int argc , char * argv[] )
{
    bool verbose = false ;
    
    FILE * fileIn ;
    uint32_t fileInSize ;

    uint8_t * dataIn = NULL ;
    
    histogram_dataCalc_t * pHistogramDataCalc ;
    uint8_t * pConvTable ;
    
    uint8_t * dataOut = NULL ;
    uint32_t dataOutSize ;
    uint32_t dataOutMaxSize ;

    printf( "Simple Compressor - github.com/francescosacco\n" ) ;
    
    // Check if there are a file input.
    if( argc < 2 )
    {
        printf( "\tUse: SimpleCompressor <file> [--verbose]\n\n" ) ;
        return( 0 ) ;
    }

    fileIn = fopen( argv[ 1 ] , "rb" ) ;
    if( ( FILE * ) NULL == fileIn )
    {
        printf( "\tError to open \"%s\"!\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }

    if( ( argc > 2 ) && ( strcmp( argv[ 2 ] , "--verbose" ) == 0 ) )
    {
        verbose = true ;
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

    histogram_calculation_8bits( dataIn , ( size_t ) fileInSize , pHistogramDataCalc ) ;

    verbose_histogram_8bits( verbose , pHistogramDataCalc ) ;

    printf( "\tGenerating conversion table...\n" ) ;

    pConvTable = ( uint8_t * ) malloc( 256 ) ;
    if( ( uint8_t * ) NULL == pConvTable )
    {
        free( ( void * ) pHistogramDataCalc ) ;
        free( ( void * ) dataIn ) ;
        printf( "\tError allocating Convertion Table!\n" ) ;
        return( -1 ) ;
    }

    histogram_generateTable_8bits( pHistogramDataCalc , pConvTable , NULL ) ;

    verbose_convert_8bits( verbose , pConvTable ) ;

    printf( "\tCompressing data...\n" ) ;

    dataOutMaxSize = fileInSize * 2 ;
    dataOut = ( uint8_t * ) malloc( ( size_t ) dataOutMaxSize ) ;
    if( ( uint8_t * ) NULL == pConvTable )
    {
        free( ( void * ) pConvTable ) ;
        free( ( void * ) pHistogramDataCalc ) ;
        free( ( void * ) dataIn ) ;
        printf( "\tError allocating Compressed Data Out!\n" ) ;
        return( -1 ) ;
    }

    writeCompressed_handle_t writeComphandle ;
    writeCompressed_init( &writeComphandle , writeFunction ) ;

    for( uint32_t i = 0 ; i < fileInSize ; i++ )
    {
        uint8_t compSize ;
        uint16_t dataOutComp ;
        compSize = compressor_8to4( pConvTable[ dataIn[ i ] ] , &dataOutComp ) ;
        
        writeCompressed_data( dataOutComp , compSize , &writeComphandle ) ;
    }
    dataOutSize = writeCompressed_end( &writeComphandle ) ;

    printf( "\t\tCompressed data is %8u bytes.\n" , dataOutSize ) ; 
    printf( "\t\tOriginal file is   %8u bytes.\n" , fileInSize ) ;
    printf( "\t\tCompression rate is %.03f%%.\n" , ( float ) dataOutSize * 100.0 / ( float ) fileInSize ) ;
    
    free( ( void * ) dataOut ) ;
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

void writeFunction( uint8_t dataIn )
{
    ( void ) dataIn ;
}

void verbose_histogram_8bits( bool verbose , histogram_dataCalc_t * pHistogram_dataCalc )
{
    if( false == verbose )
    {
        return ;
    }

    printf( "\t\tData -    Bytes | Data -    Bytes | Data -    Bytes | Data -    Bytes\n" ) ;
    for( uint16_t i = 0 ; i < 64 ; i++ )
    {
        printf( "\t\t%02Xh  - %8u | " , ( pHistogram_dataCalc + i +   0 )->data , ( pHistogram_dataCalc + i +   0 )->frequency ) ;
        printf(     "%02Xh  - %8u | " , ( pHistogram_dataCalc + i +  64 )->data , ( pHistogram_dataCalc + i +  64 )->frequency ) ;
        printf(     "%02Xh  - %8u | " , ( pHistogram_dataCalc + i + 128 )->data , ( pHistogram_dataCalc + i + 128 )->frequency ) ;
        printf(     "%02Xh  - %8u\n"  , ( pHistogram_dataCalc + i + 192 )->data , ( pHistogram_dataCalc + i + 192 )->frequency ) ;
    }
}

void verbose_convert_8bits( bool verbose , uint8_t * tableIn )
{
    if( false == verbose )
    {
        return ;
    }

    printf( "\t\tFrom - To | From - To | From - To | From - To\n" ) ;
    for( uint16_t i = 0 ; i < 64 ; i++ )
    {
        printf( "\t\t%02Xh - %02Xh | " , ( uint8_t ) ( i +   0 ) , tableIn[ i +   0 ] ) ;
        printf(     "%02Xh - %02Xh | " , ( uint8_t ) ( i +  64 ) , tableIn[ i +  64 ] ) ;
        printf(     "%02Xh - %02Xh | " , ( uint8_t ) ( i + 128 ) , tableIn[ i + 128 ] ) ;
        printf(     "%02Xh - %02Xh\n"  , ( uint8_t ) ( i + 192 ) , tableIn[ i + 192 ] ) ;
    }
}
