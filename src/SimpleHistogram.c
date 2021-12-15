#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc() and free().
#include <stdbool.h>
#include <string.h>

#include "histogram.h"
#include "FileManager.h"

char * Arguments_findData( int argc , char * argv[] , const char * strIn ) ;

int main( int argc , char * argv[] )
{
    uint8_t * pData ;
    size_t dataSize ;

    bool cmd_outFile = false ;
    char * cmd_outFileName ;

    bool cmd_dataSize = false ;
    char * cmd_dataSizeStr ;
    int cmd_dataSizeValue ;

    size_t tableSize ;
    size_t histSize ;
    eHistDataSize_t eHistSize ;
    uint16_t * pConvTable ;
    uint16_t * pUncConvTable ;
    histogram_dataCalc_t * pHistCalc ;

    printf( "Simple Histogram - github.com/francescosacco\n" ) ;

    if( argc < 2 )
    {
        printf( "\tUse: SimpleHistogram <file name> [--output=file] [--datasize=2,4,8,16]\n" ) ;
        return( 0 ) ;
    }

    pData = FileManager_allocFile( argv[ 1 ] , &dataSize ) ;
    if( FILEMGR_NULL == pData )
    {
        printf( "\tError - Problems oppening %s file.\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    if( 0 == dataSize )
    {
        printf( "\tError - File \"%s\" is zero size.\n" , argv[ 1 ] ) ;
        return( -1 ) ;
    }
    printf( "\tFile %s loaded, %u bytes.\n" , argv[ 1 ] , ( uint32_t ) dataSize ) ;

    cmd_outFileName = Arguments_findData( argc , argv , "--output=" ) ;
    cmd_outFile = ( ( char * ) NULL != cmd_outFileName ) ? ( true ) : ( false ) ;
    if( cmd_outFile )
    {
        printf( "\tOutput file defined as \"%s\".\n" , cmd_outFileName ) ;
    }

    cmd_dataSizeStr = Arguments_findData( argc , argv , "--datasize=" ) ;
    cmd_dataSize = ( ( char * ) NULL != cmd_dataSizeStr ) ? ( true ) : ( false ) ;
    if( cmd_dataSize )
    {
        cmd_dataSizeValue = atoi( cmd_dataSizeStr ) ;
        if( ( 2 != cmd_dataSizeValue ) && ( 4 != cmd_dataSizeValue ) && ( 8 != cmd_dataSizeValue ) && ( 16 != cmd_dataSizeValue ) )
        {
            printf( "\tError - Data Size can not be %d.\n" , cmd_dataSizeValue ) ;
            return( -1 ) ;
        }
    }
    else
    {
        // Default value.
        cmd_dataSizeValue = 8 ;
    }
    printf( "\tData size defined as %d bits.\n" , cmd_dataSizeValue ) ;

    switch( cmd_dataSizeValue )
    {
    case 2 :
        tableSize = TABLE_2BITS_SIZE ;
        histSize  = HISTOGRAM_2BITS_SIZE ;
        eHistSize = eHistDataSize_2bits ;
        break ;
    case 4 :
        tableSize = TABLE_4BITS_SIZE ;
        histSize  = HISTOGRAM_4BITS_SIZE ;
        eHistSize = eHistDataSize_4bits ;
        break ;
    case 16 :
        tableSize = TABLE_16BITS_SIZE ;
        histSize  = HISTOGRAM_16BITS_SIZE ;
        eHistSize = eHistDataSize_16bits ;
        break ;
    case 8 :
    default:
        tableSize = TABLE_8BITS_SIZE ;
        histSize  = HISTOGRAM_8BITS_SIZE ;
        eHistSize = eHistDataSize_8bits ;
        break ;
    }

    pConvTable    = ( uint16_t * ) malloc( tableSize ) ;
    pUncConvTable = ( uint16_t * ) malloc( tableSize ) ;
    pHistCalc     = ( histogram_dataCalc_t * ) malloc( histSize ) ;

    if( ( ( uint16_t * ) NULL == pConvTable ) || ( ( uint16_t * ) NULL == pUncConvTable ) || ( ( histogram_dataCalc_t * ) NULL == pHistCalc ) )
    {
        printf( "\tError - Not possible alloc memory.\n" ) ;
        return( -1 ) ;
    }

    // Calculating Histogram.
    printf( "\tCalculating Histogram.\n" ) ;
    histogram_calculation( pData , dataSize , eHistSize , pHistCalc ) ;
    printf( "\tGenerating conversion tables.\n" ) ;
    histogram_generateTable( pHistCalc , eHistSize , pConvTable , pUncConvTable ) ;

    free( pConvTable    ) ;
    free( pUncConvTable ) ;
    free( pHistCalc     ) ;

    FileManager_freeFile( pData ) ;
    return( 0 ) ;
}

char * Arguments_findData( int argc , char * argv[] , const char * strIn )
{
    char * dataFound = ( char * ) NULL ;

    for( int i = 0 ; i < argc ; i++ )
    {
        dataFound = strstr( argv[ i ] , strIn ) ;
        
        if( ( char * ) NULL != dataFound )
        {
            // Are there no data?
            if( strlen( dataFound ) == strlen( strIn ) )
            {
                // Yes, there's no data.
                dataFound = ( char * ) NULL ;
            }
            else
            {
                dataFound += strlen( strIn ) ;
            }
            break ;
        }
    }
    
    return( dataFound ) ;
}
