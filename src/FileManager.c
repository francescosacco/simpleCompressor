#include "FileManager.h"

#include <stdlib.h> // To use malloc() and free().

size_t FileManager_fileSize( FILE * fileIn ) ;

uint8_t * FileManager_allocFile( const char * fileName , size_t * pFileSize )
{
    FILE * pFile ;
    uint8_t * pData = FILEMGR_NULL ;
    
    *pFileSize = 0 ;
    
    pFile = fopen( fileName , "rb" ) ;
    if( ( FILE * ) NULL == pFile )
    {
        goto FileManager_allocFile_return ;
    }
    
    *pFileSize = FileManager_fileSize( pFile ) ;
    
    pData = ( uint8_t * ) malloc( *pFileSize ) ;
    if( FILEMGR_NULL == pData )
    {
        goto FileManager_allocFile_closeAndReturn ;
    }
    
    for( size_t i = 0 ; i < *pFileSize ; i++ )
    {
        pData[ i ] = ( uint8_t ) getc( pFile ) ; 
    }

    FileManager_allocFile_closeAndReturn:
    fclose( pFile ) ;
    
    FileManager_allocFile_return:
    return( pData ) ;
}

void FileManager_freeFile( uint8_t * filePtr )
{
    free( filePtr ) ;
}

size_t FileManager_fileSize( FILE * fileIn )
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

void FileManager( char * fileName , uint8_t * pData , size_t dataSize )
{
    FILE * filePtr ;
    
    filePtr = fopen( fileName , "wb" ) ;
    if( ( FILE * ) NULL == filePtr )
    {
        return ;
    }

    for( size_t i = 0 ; i < dataSize ; i++ )
    {
        ( void ) putc( ( int ) pData[ i ] , filePtr ) ;
    }

    fclose( filePtr ) ;
}
