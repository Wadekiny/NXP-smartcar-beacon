#ifndef _K66_TPM_H_
#define _K66_TPM_H_

#include "common.h"

#define TPM_CLKIN0  C12         //PTA18 、PTB16  、PTC12
#define TPM_CLKIN1  C13         //PTA19  、PTB17  、PTC13
typedef enum
{
    tpm1,
    tpm2,

    tpm_max,
} TPMn;

void TPM_PulseInit();
int16 TPM_Read(TPMn tpmn);
void TPM_Clear(TPMn tpmn);


#endif 