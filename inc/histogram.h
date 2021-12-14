#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct _HISTOGRAM_DATACALC_T_
{
    uint8_t  data ;
    uint32_t frequency ;
} histogram_dataCalc_t ;

void histogram_calculation_8bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc ) ;
void histogram_generateTable_8bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert ) ;

void histogram_calculation_4bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc ) ;
void histogram_generateTable_4bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert ) ;

void histogram_calculation_2bits( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc ) ;
void histogram_generateTable_2bits( histogram_dataCalc_t * histogram_dataCalc , uint8_t * convert , uint8_t * unConvert ) ;

#ifdef __cplusplus
    }
#endif

#endif // _HISTOGRAM_H_
