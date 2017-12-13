/*
 * csl_tsc1.c
 *
 *  Created on: 2016-6-8
 *      Author: WYK
 */

#include <c6x.h>
#include <ti/csl/csl_types.h>

void CSL_tscEnable(void)
{
	TSCL = 0;
}

CSL_Uint64 CSL_tscRead (void)
{
	return _itoll(TSCH, TSCL);
}

