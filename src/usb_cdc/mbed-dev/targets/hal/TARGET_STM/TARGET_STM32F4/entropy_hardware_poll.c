/*
 *  Hardware entropy collector for the STM32F4 family
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


#if defined(TARGET_STM32F405xx) || defined(TARGET_STM32F415xx) || defined(TARGET_STM32F407xx) || defined(TARGET_STM32F417xx) ||\
    defined(TARGET_STM32F427xx) || defined(TARGET_STM32F437xx) || defined(TARGET_STM32F429xx) || defined(TARGET_STM32F439xx) ||\
    defined(TARGET_STM32F410Tx) || defined(TARGET_STM32F410Cx) || defined(TARGET_STM32F410Rx) || defined(TARGET_STM32F469xx) ||\
    defined(TARGET_STM32F479xx)
#include <stdlib.h>
#include "cmsis.h"

/* RNG handler declaration */
RNG_HandleTypeDef RngHandle;


/** rng_get_byte
 *  @brief Get one byte of entropy from the RNG, assuming it is up and running.
 *  @param pointer to the hardware generated random byte.
 */
static void rng_get_byte( unsigned char *byte )
{
    *byte = (unsigned char)HAL_RNG_GetRandomNumber(&RngHandle);
}


/** mbedtls_hardware_poll
 *  @brief Get len bytes of entropy from the hardware RNG.
 *  @param data pointer will be NULL
 *  @param output pointer to the random generated bytes buffer
 *  @param len input is the requested length of bytes to be generated
 *  @param olen is the pointer to the length of bytes effectively generated
 *  @returns 0 if the generation went well. -1 in case of error
 */
int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen )
{
    int ret;
    ((void) data);

    /* RNG Peripheral clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* Initialize RNG instance */
    RngHandle.Instance = RNG;
    HAL_RNG_Init(&RngHandle);

    /* Get Random byte */
    for( uint32_t i = 0; i < len; i++ ){
        rng_get_byte( output + i );

    }
    *olen = len;
    /* Just be extra sure that we didn't do it wrong */
    if( ( __HAL_RNG_GET_FLAG(&RngHandle, (RNG_FLAG_CECS|RNG_FLAG_SECS)) ) != 0 ) {
        ret = -1;
    } else {
        ret = 0;
    }
    /*Disable the RNG peripheral */
    HAL_RNG_DeInit(&RngHandle);
    /* RNG Peripheral clock disable - assume we're the only users of RNG  */
    __HAL_RCC_RNG_CLK_DISABLE();


    return( ret );
}
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F427xx || STM32F437xx ||\
          STM32F429xx || STM32F439xx || STM32F410xx || STM32F469xx || STM32F479xx */
