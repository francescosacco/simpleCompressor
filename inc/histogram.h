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

void histogram_calculation( uint8_t * bufferIn , size_t bufferSize , histogram_dataCalc_t * histogram_dataCalc ) ;
void histogram_generateTable( histogram_dataCalc_t * histogram_dataCalc , uint8_t * tableOut ) ;

#ifdef __cplusplus
    }
#endif

#endif // _HISTOGRAM_H_
