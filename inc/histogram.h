#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct _HISTOGRAM_DATACALC_T_
{
    uint16_t  data ;
    uint32_t frequency ;
} histogram_dataCalc_t ;

typedef enum
{
    eHistDataSize_2bits ,
    eHistDataSize_4bits ,
    eHistDataSize_8bits ,
    eHistDataSize_16bits
} eHistDataSize_t ;

#define HISTOGRAM_2BITS_SIZE                     ( ( size_t ) (     4 * sizeof( histogram_dataCalc_t ) ) )
#define HISTOGRAM_4BITS_SIZE                     ( ( size_t ) (    16 * sizeof( histogram_dataCalc_t ) ) )
#define HISTOGRAM_8BITS_SIZE                     ( ( size_t ) (   256 * sizeof( histogram_dataCalc_t ) ) )
#define HISTOGRAM_16BITS_SIZE                    ( ( size_t ) ( 65536 * sizeof( histogram_dataCalc_t ) ) )

#define TABLE_2BITS_SIZE                         ( ( size_t ) (     4 * sizeof( uint16_t ) ) )
#define TABLE_4BITS_SIZE                         ( ( size_t ) (    16 * sizeof( uint16_t ) ) )
#define TABLE_8BITS_SIZE                         ( ( size_t ) (   256 * sizeof( uint16_t ) ) )
#define TABLE_16BITS_SIZE                        ( ( size_t ) ( 65536 * sizeof( uint16_t ) ) )

void histogram_calculation( uint8_t * bufferIn , size_t bufferSize , eHistDataSize_t eDataSize , histogram_dataCalc_t * histogram_dataCalc ) ;
void histogram_generateTable( histogram_dataCalc_t * histogram_dataCalc , eHistDataSize_t eDataSize , uint16_t * convert , uint16_t * unConvert ) ;

#ifdef __cplusplus
    }
#endif

#endif // _HISTOGRAM_H_
