/*******************************************************************************
* File Name: isr1.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_isr1_H)
#define CY_ISR_isr1_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr1_Start(void);
void isr1_StartEx(cyisraddress address);
void isr1_Stop(void);

CY_ISR_PROTO(isr1_Interrupt);

void isr1_SetVector(cyisraddress address);
cyisraddress isr1_GetVector(void);

void isr1_SetPriority(uint8 priority);
uint8 isr1_GetPriority(void);

void isr1_Enable(void);
uint8 isr1_GetState(void);
void isr1_Disable(void);

void isr1_SetPending(void);
void isr1_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr1 ISR. */
#define isr1_INTC_VECTOR            ((reg32 *) isr1__INTC_VECT)

/* Address of the isr1 ISR priority. */
#define isr1_INTC_PRIOR             ((reg8 *) isr1__INTC_PRIOR_REG)

/* Priority of the isr1 interrupt. */
#define isr1_INTC_PRIOR_NUMBER      isr1__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr1 interrupt. */
#define isr1_INTC_SET_EN            ((reg32 *) isr1__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr1 interrupt. */
#define isr1_INTC_CLR_EN            ((reg32 *) isr1__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr1 interrupt state to pending. */
#define isr1_INTC_SET_PD            ((reg32 *) isr1__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr1 interrupt. */
#define isr1_INTC_CLR_PD            ((reg32 *) isr1__INTC_CLR_PD_REG)


#endif /* CY_ISR_isr1_H */


/* [] END OF FILE */
