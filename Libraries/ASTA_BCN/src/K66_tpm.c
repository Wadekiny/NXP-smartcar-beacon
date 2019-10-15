#include "K66_tpm.h"

void TPM_PulseInit()
{   
    //使能时钟
    SIM->SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1);
    
    SIM->SCGC2 |= SIM_SCGC2_TPM1_MASK;
    SIM->SCGC2 |= SIM_SCGC2_TPM2_MASK;
    //clksel
    
    SIM->SOPT9 &= ~SIM_SOPT9_TPM1CLKSEL_MASK; //TPM1 CLK0
    SIM->SOPT9 |= SIM_SOPT9_TPM2CLKSEL_MASK; //TPM2 CLK1

    
    port_init(TPM_CLKIN0,PULLUP|ALT7); //CLKIN0
    port_init(TPM_CLKIN1,PULLUP|ALT7); //CLKIN1
    
     
    TPM1->SC |=  TPM_SC_CMOD(2); 
    TPM1->MOD = TPM_MOD_MOD_MASK;
    TPM1->CNT = 0;  
    
    TPM2->SC |=  TPM_SC_CMOD(2); 
    TPM2->MOD = TPM_MOD_MOD_MASK;
    TPM2->CNT = 0;   
}

int16 TPM_Read(TPMn tpmn)
{
  if (tpmn == tpm1)
    return TPM1->CNT;
  else
    return TPM2->CNT;
}
void TPM_Clear(TPMn tpmn)
{
  if (tpmn == tpm1)
    TPM1->CNT = 0;
  else
    TPM2->CNT = 0;
}