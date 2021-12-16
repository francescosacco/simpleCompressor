#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

#define FILEMGR_NULL                             ( ( uint8_t * ) NULL )

uint8_t * FileManager_allocFile( const char * fileName , size_t * pFileSize ) ;
void FileManager_freeFile( uint8_t * filePtr ) ;

void FileManager( char * fileName , uint8_t * pData , size_t dataSize ) ;

#ifdef __cplusplus
    }
#endif

#endif // _FILEMANAGER_H_
