#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc() and free().
#include <stdbool.h>
#include <string.h> // To use strcmp().

#include "histogram.h"
#include "compressor.h"

size_t dataInSize = 0 ;
uint8_t * dataIn = NULL ;
histogram_dataCalc_t * pHistCalc = NULL ;
uint8_t * pConvTable = NULL ;

size_t fileSize( FILE * fileIn ) ;

bool allocateMemory_load( const char * fileName ) ;
void freeMemory( void ) ;

void writeFunction( uint8_t dataIn ) ;

void verbose_allocAndLoad( bool verbose , const char * fileName , size_t fileSize ) ;
void verbose_histogram_8bits( bool verbose , histogram_dataCalc_t * pHistogram_dataCalc ) ;
void verbose_convert_8bits( bool verbose , uint8_t * tableIn ) ;
void verbose_histogram_4bits( bool verbose , histogram_dataCalc_t * pHistogram_dataCalc ) ;
void verbose_convert_4bits( bool verbose , uint8_t * tableIn ) ;

int main( int argc , char * argv[] )
{
    bool verbose = false ;
    bool allocLoad = false ;
    
    writeCompressed_handle_t writeComphandle ;
    uint32_t dataOutSize ;

    printf( "Simple Compressor - github.com/francescosacco\n" ) ;
    
    // Check if there are a file input.
    if( argc < 2 )
    {
        printf( "\tUse: SimpleCompressor <file> [--verbose]\n\n" ) ;
        return( 0 ) ;
    }

    if( ( argc > 2 ) && ( strcmp( argv[ 2 ] , "--verbose" ) == 0 ) )
    {
        verbose = true ;
    }

    printf( "\tLoading memory...\n" ) ;
    allocLoad = allocateMemory_load( argv[ 1 ] ) ;
    if( false == allocLoad )
    {
        return( -1 ) ;
    }

    /****************************
     * 8 TO 4
     ****************************/

    verbose_allocAndLoad( verbose , argv[ 1 ] , dataInSize ) ;

    printf( "\tCalculating histogram 4bits...\n" ) ;
    histogram_calculation_4bits( dataIn , dataInSize , pHistCalc ) ;
    verbose_histogram_4bits( verbose , pHistCalc ) ;

    printf( "\tGenerating conversion table 4bits...\n" ) ;
    histogram_generateTable_4bits( pHistCalc , pConvTable , NULL ) ;
    verbose_convert_4bits( verbose , pConvTable ) ;

    printf( "\tCompressing data 4to1...\n" ) ;
    if( verbose )
    {
        printf( "\t\twriteCompressed_init()\n" ) ;
    }
    writeCompressed_init( &writeComphandle , writeFunction ) ;

    if( verbose )
    {
        printf( "\t\twriteCompressed_data()\n" ) ;
    }

    for( size_t i = 0 ; i < dataInSize ; i++ )
    {
        static float verboseProgress = 10.0 ;

        uint8_t compSize ;
        uint16_t dataOutComp ;

        compSize = compressor_4to1( pConvTable[ dataIn[ i ] >> 4 ] , &dataOutComp ) ;

        if( verbose )
        {
            printf( "\t\t%d = compressor_4to1( %02Xh -> %02Xh ,OUT -> %04Xh )\n" , compSize , dataIn[ i ] >> 4 , pConvTable[ dataIn[ i ] >> 4 ] , dataOutComp ) ;
            printf( "\t\twriteCompressed_data( %04Xh , %02Xh , func() )\n" , dataOutComp , compSize ) ;
        }

        writeCompressed_data( dataOutComp , compSize , &writeComphandle ) ;

        compSize = compressor_4to1( pConvTable[ dataIn[ i ] & 0x0F ] , &dataOutComp ) ;

        if( verbose )
        {
            printf( "\t\t%d = compressor_4to1( %02Xh -> %02Xh ,OUT -> %04Xh )\n" , compSize , dataIn[ i ] & 0x0F , pConvTable[ dataIn[ i ] & 0x0F ] , dataOutComp ) ;
            printf( "\t\twriteCompressed_data( %04Xh , %02Xh , func() )\n" , dataOutComp , compSize ) ;
        }

        writeCompressed_data( dataOutComp , compSize , &writeComphandle ) ;
    }
    if( verbose )
    {
        printf( "\t\twriteCompressed_end()\n" ) ;
    }
    dataOutSize = writeCompressed_end( &writeComphandle ) ;

    printf( "\t\tCompressed data is %8u bytes.\n" , dataOutSize ) ; 
    printf( "\t\tOriginal file is   %8u bytes.\n" , ( uint32_t ) dataInSize ) ;
    printf( "\t\tCompression rate is %.03f%%.\n" , ( float ) dataOutSize * 100.0 / ( float ) dataInSize ) ;

    freeMemory() ;
    
    return( 0 ) ;
}

size_t fileSize( FILE * fileIn )
{
    size_t ret = 0 ;
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
    
    ret = ( size_t ) ftell( fileIn ) ;

    ( void ) fsetpos( fileIn , &pos ) ;
    
    return( ret ) ;
}

void writeFunction( uint8_t dataIn )
{
    ( void ) dataIn ;
    printf( "\t\twriteFunction(%02Xh)\n" , dataIn ) ;
}

void verbose_allocAndLoad( bool verbose , const char * fileName , size_t fileSize )
{
    if( false == verbose )
    {
        return ;
    }

    printf( "\t\tFile \"%s\" loaded, %u bytes.\n" , fileName , ( uint32_t ) fileSize ) ;
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

void verbose_histogram_4bits( bool verbose , histogram_dataCalc_t * pHistogram_dataCalc )
{
    if( false == verbose )
    {
        return ;
    }

    printf( "\t\tData -   Bytes | Data -   Bytes | Data -   Bytes | Data -   Bytes\n" ) ;
    for( uint16_t i = 0 ; i < 4 ; i++ )
    {
        printf( "\t\t%1Xh  - %8u | " , ( pHistogram_dataCalc + i +  0 )->data , ( pHistogram_dataCalc + i +  0 )->frequency ) ;
        printf(     "%1Xh  - %8u | " , ( pHistogram_dataCalc + i +  4 )->data , ( pHistogram_dataCalc + i +  4 )->frequency ) ;
        printf(     "%1Xh  - %8u | " , ( pHistogram_dataCalc + i +  8 )->data , ( pHistogram_dataCalc + i +  8 )->frequency ) ;
        printf(     "%1Xh  - %8u\n"  , ( pHistogram_dataCalc + i + 12 )->data , ( pHistogram_dataCalc + i + 12 )->frequency ) ;
    }
}

void verbose_convert_4bits( bool verbose , uint8_t * tableIn )
{
    if( false == verbose )
    {
        return ;
    }

    printf( "\t\tFrom To | From To | From To | From To\n" ) ;
    for( uint16_t i = 0 ; i < 4 ; i++ )
    {
        printf( "\t\t%1Xh - %1Xh | " , ( uint8_t ) ( i +  0 ) , tableIn[ i +  0 ] ) ;
        printf(     "%1Xh - %1Xh | " , ( uint8_t ) ( i +  4 ) , tableIn[ i +  4 ] ) ;
        printf(     "%1Xh - %1Xh | " , ( uint8_t ) ( i +  8 ) , tableIn[ i +  8 ] ) ;
        printf(     "%1Xh - %1Xh\n"  , ( uint8_t ) ( i + 12 ) , tableIn[ i + 12 ] ) ;
    }
}

bool allocateMemory_load( const char * fileName )
{
    FILE * fileIn ;
    bool ret = false ;
    
    fileIn = fopen( fileName , "rb" ) ;
    if( ( FILE * ) NULL == fileIn )
    {
        printf( "\tError to open \"%s\"!\n" , fileName ) ;
        return( false ) ;
    }

    dataInSize = fileSize( fileIn ) ;

    dataIn = ( uint8_t * ) malloc( dataInSize ) ;
    if( ( uint8_t * ) NULL == dataIn )
    {
        printf( "\tError to alloc %u size!\n" , ( uint32_t ) dataInSize ) ;
        goto errorLevel_1 ;
    }

    for( size_t i = 0 ; i < dataInSize ; i++ )
    {
        dataIn[ i ] = ( uint8_t ) getc( fileIn ) ;
    }

    pHistCalc = ( histogram_dataCalc_t * ) malloc( 256 * sizeof( histogram_dataCalc_t ) ) ;
    if( ( histogram_dataCalc_t * ) NULL == pHistCalc )
    {
        printf( "\tError allocating histogram!\n" ) ;
        goto errorLevel_2 ;
    }

    pConvTable = ( uint8_t * ) malloc( 256 * sizeof( uint8_t ) ) ;
    if( ( uint8_t * ) NULL == pConvTable )
    {
        printf( "\tError allocating Convertion Table!\n" ) ;
        goto errorLevel_3 ;
    }

    ret = true ;
    goto errorLevel_1 ;

    errorLevel_3:
    free( ( void * ) pHistCalc ) ;

    errorLevel_2:
    free( ( void * ) dataIn ) ;

    errorLevel_1:
    fclose( fileIn ) ;
    
    return( ret ) ;
}

void freeMemory( void )
{
    free( ( void * ) dataIn ) ;
    free( ( void * ) pHistCalc ) ;
    free( ( void * ) pConvTable ) ;
}
