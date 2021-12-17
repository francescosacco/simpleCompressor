#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // To use malloc() and free().
#include <stdbool.h>
#include <string.h>

#include "histogram.h"
#include "FileManager.h"

char * Arguments_findData( int argc , char * argv[] , const char * strIn ) ;
char * Arguments_findArg( int argc , char * argv[] , const char * strIn ) ;
void setFileName( char * strIn , char * strExt , char * strOut , size_t strOutSize ) ;
void printHistogram( histogram_dataCalc_t * pHist , eHistDataSize_t eHistSize ) ;
void printHistogramFile( histogram_dataCalc_t * pHist , eHistDataSize_t eHistSize , char * fileName ) ;

int main( int argc , char * argv[] )
{
    uint8_t * pData ;
    size_t dataSize ;

    bool cmd_sorted = false ;
    char * cmd_sortedTxt ;

    bool cmd_outFile = false ;
    char * cmd_outFileName ;
    char outFileName[ 64 ] ;

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
        printf( "\tUse: SimpleHistogram <file name> [--output=name] [--datasize=2,4,8,16] [--sorted]\n" ) ;
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

    cmd_sortedTxt = Arguments_findArg( argc , argv , "--sorted" ) ;
    cmd_sorted = ( ( char * ) NULL != cmd_sortedTxt ) ? ( true ) : ( false ) ;
    if( cmd_sorted )
    {
        printf( "\tSort enabled.\n" ) ;
    }

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

    if( cmd_sorted )
    {
        printf( "\tGenerating conversion tables.\n" ) ;
        histogram_generateTable( pHistCalc , eHistSize , pConvTable , pUncConvTable ) ;

        if( false == cmd_outFile )
        {
            printHistogram( pHistCalc , eHistSize ) ;
        }
    }
    else
    {
        if( false == cmd_outFile )
        {
            printHistogram( pHistCalc , eHistSize ) ;
        }

        printf( "\tGenerating conversion tables.\n" ) ;
        histogram_generateTable( pHistCalc , eHistSize , pConvTable , pUncConvTable ) ;
    }

    if( cmd_outFile )
    {
        // Writting convertion table.
        setFileName( cmd_outFileName , ".cta" , outFileName , sizeof( outFileName ) ) ;
        printf( "\tWritting convertion table at \"%s\" file.\n" , outFileName ) ;
        FileManager_writeBlock( outFileName , ( uint8_t * ) pConvTable , tableSize ) ;

        // Writting unconvertion table.
        setFileName( cmd_outFileName , ".uta" , outFileName , sizeof( outFileName ) ) ;
        printf( "\tWritting unconvertion table at \"%s\" file.\n" , outFileName ) ;
        FileManager_writeBlock( outFileName , ( uint8_t * ) pUncConvTable , tableSize ) ;

        // Writting histogram table.
        setFileName( cmd_outFileName , ".hta" , outFileName , sizeof( outFileName ) ) ;
        printf( "\tWritting histogram table at \"%s\" file.\n" , outFileName ) ;
        FileManager_writeBlock( outFileName , ( uint8_t * ) pHistCalc , histSize ) ;
        
        setFileName( cmd_outFileName , ".his" , outFileName , sizeof( outFileName ) ) ;
        printf( "\tWritting histogram output at \"%s\" file.\n" , outFileName ) ;
        printHistogramFile( pHistCalc , eHistSize , outFileName ) ;
    }

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

char * Arguments_findArg( int argc , char * argv[] , const char * strIn )
{
    char * dataFound = ( char * ) NULL ;

    for( int i = 0 ; i < argc ; i++ )
    {
        dataFound = strstr( argv[ i ] , strIn ) ;
        
        if( ( char * ) NULL != dataFound )
        {
            break ;
        }
    }
    
    return( dataFound ) ;
}

void setFileName( char * strIn , char * strExt , char * strOut , size_t strOutSize )
{
    size_t outSize ;
    size_t extSize ;
    
    outSize = strlen( strIn ) ;
    extSize = strlen( strExt ) ;
    
    if( extSize >= outSize )
    {
        strOut[ 0 ] = '\0' ;
        return ;
    }

    if( outSize > ( strOutSize - extSize - 1 ) )
    {
        outSize = ( strOutSize - extSize - 1 ) ;
    }

    memset( strOut , '\0' , strOutSize ) ;
    strncpy( strOut , strIn , outSize ) ;
    strcpy( strOut + outSize , strExt ) ;
}

void printHistogram( histogram_dataCalc_t * pHist , eHistDataSize_t eHistSize )
{
    
    
    uint32_t tableSize[] = { 4 , 16 , 256 , 65536 } ;

    printf( "\n" ) ;

    switch( eHistSize )
    {
    case eHistDataSize_2bits :
    case eHistDataSize_4bits :
        printf( "  Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < tableSize[ eHistSize ] ; i++ )
        {
            printf( "    %1Xh  %10u\n" , pHist[ i ].data , pHist[ i ].frequency ) ;
        }
        break ;
    case eHistDataSize_8bits :
        printf( "  Data   Frequency | Data   Frequency | Data   Frequency | Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < ( tableSize[ eHistSize ] / 4 ) ; i++ )
        {
            printf( "   %02Xh  %10u |" , pHist[ i       ].data , pHist[ i       ].frequency ) ;
            printf(  "  %02Xh  %10u |" , pHist[ i +  64 ].data , pHist[ i +  64 ].frequency ) ;
            printf(  "  %02Xh  %10u |" , pHist[ i + 128 ].data , pHist[ i + 128 ].frequency ) ;
            printf(  "  %02Xh  %10u\n" , pHist[ i + 192 ].data , pHist[ i + 192 ].frequency ) ;
        }
        break ;
    case eHistDataSize_16bits :
        printf( "  Data   Frequency | Data   Frequency | Data   Frequency | Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < ( tableSize[ eHistSize ] / 4 ) ; i++ )
        {
            printf( "  %04Xh %10u |" , pHist[ i         ].data , pHist[ i         ].frequency ) ;
            printf(  " %04Xh %10u |" , pHist[ i + 16384 ].data , pHist[ i + 16384 ].frequency ) ;
            printf(  " %04Xh %10u |" , pHist[ i + 32768 ].data , pHist[ i + 32768 ].frequency ) ;
            printf(  " %04Xh %10u\n" , pHist[ i + 49152 ].data , pHist[ i + 49152 ].frequency ) ;
        }
        break ;
    }

    printf( "\n" ) ;
}

void printHistogramFile( histogram_dataCalc_t * pHist , eHistDataSize_t eHistSize , char * fileName )
{
    uint32_t tableSize[] = { 4 , 16 , 256 , 65536 } ;

    FILE * pFile = fopen( fileName , "w" ) ;
    if( ( FILE * ) NULL == pFile )
    {
        return ;
    }

    switch( eHistSize )
    {
    case eHistDataSize_2bits :
    case eHistDataSize_4bits :
        fprintf( pFile , "  Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < tableSize[ eHistSize ] ; i++ )
        {
            fprintf( pFile , "    %1Xh  %10u\n" , pHist[ i ].data , pHist[ i ].frequency ) ;
        }
        break ;
    case eHistDataSize_8bits :
        fprintf( pFile , "  Data   Frequency | Data   Frequency | Data   Frequency | Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < ( tableSize[ eHistSize ] / 4 ) ; i++ )
        {
            fprintf( pFile , "   %02Xh  %10u |" , pHist[ i       ].data , pHist[ i       ].frequency ) ;
            fprintf( pFile ,  "  %02Xh  %10u |" , pHist[ i +  64 ].data , pHist[ i +  64 ].frequency ) ;
            fprintf( pFile ,  "  %02Xh  %10u |" , pHist[ i + 128 ].data , pHist[ i + 128 ].frequency ) ;
            fprintf( pFile ,  "  %02Xh  %10u\n" , pHist[ i + 192 ].data , pHist[ i + 192 ].frequency ) ;
        }
        break ;
    case eHistDataSize_16bits :
        fprintf( pFile , "  Data   Frequency | Data   Frequency | Data   Frequency | Data   Frequency\n" ) ;
        for( uint32_t i = 0 ; i < ( tableSize[ eHistSize ] / 4 ) ; i++ )
        {
            fprintf( pFile , "  %04Xh %10u |" , pHist[ i         ].data , pHist[ i         ].frequency ) ;
            fprintf( pFile ,  " %04Xh %10u |" , pHist[ i + 16384 ].data , pHist[ i + 16384 ].frequency ) ;
            fprintf( pFile ,  " %04Xh %10u |" , pHist[ i + 32768 ].data , pHist[ i + 32768 ].frequency ) ;
            fprintf( pFile ,  " %04Xh %10u\n" , pHist[ i + 49152 ].data , pHist[ i + 49152 ].frequency ) ;
        }
        break ;
    }

    fclose( pFile ) ;
}
