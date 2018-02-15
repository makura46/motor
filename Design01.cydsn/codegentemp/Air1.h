/*******************************************************************************
* File Name: Air1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Air1_H) /* Pins Air1_H */
#define CY_PINS_Air1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Air1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Air1__PORT == 15 && ((Air1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Air1_Write(uint8 value);
void    Air1_SetDriveMode(uint8 mode);
uint8   Air1_ReadDataReg(void);
uint8   Air1_Read(void);
void    Air1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Air1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Air1_SetDriveMode() function.
     *  @{
     */
        #define Air1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Air1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Air1_DM_RES_UP          PIN_DM_RES_UP
        #define Air1_DM_RES_DWN         PIN_DM_RES_DWN
        #define Air1_DM_OD_LO           PIN_DM_OD_LO
        #define Air1_DM_OD_HI           PIN_DM_OD_HI
        #define Air1_DM_STRONG          PIN_DM_STRONG
        #define Air1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Air1_MASK               Air1__MASK
#define Air1_SHIFT              Air1__SHIFT
#define Air1_WIDTH              1u

/* Interrupt constants */
#if defined(Air1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Air1_SetInterruptMode() function.
     *  @{
     */
        #define Air1_INTR_NONE      (uint16)(0x0000u)
        #define Air1_INTR_RISING    (uint16)(0x0001u)
        #define Air1_INTR_FALLING   (uint16)(0x0002u)
        #define Air1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Air1_INTR_MASK      (0x01u) 
#endif /* (Air1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Air1_PS                     (* (reg8 *) Air1__PS)
/* Data Register */
#define Air1_DR                     (* (reg8 *) Air1__DR)
/* Port Number */
#define Air1_PRT_NUM                (* (reg8 *) Air1__PRT) 
/* Connect to Analog Globals */                                                  
#define Air1_AG                     (* (reg8 *) Air1__AG)                       
/* Analog MUX bux enable */
#define Air1_AMUX                   (* (reg8 *) Air1__AMUX) 
/* Bidirectional Enable */                                                        
#define Air1_BIE                    (* (reg8 *) Air1__BIE)
/* Bit-mask for Aliased Register Access */
#define Air1_BIT_MASK               (* (reg8 *) Air1__BIT_MASK)
/* Bypass Enable */
#define Air1_BYP                    (* (reg8 *) Air1__BYP)
/* Port wide control signals */                                                   
#define Air1_CTL                    (* (reg8 *) Air1__CTL)
/* Drive Modes */
#define Air1_DM0                    (* (reg8 *) Air1__DM0) 
#define Air1_DM1                    (* (reg8 *) Air1__DM1)
#define Air1_DM2                    (* (reg8 *) Air1__DM2) 
/* Input Buffer Disable Override */
#define Air1_INP_DIS                (* (reg8 *) Air1__INP_DIS)
/* LCD Common or Segment Drive */
#define Air1_LCD_COM_SEG            (* (reg8 *) Air1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Air1_LCD_EN                 (* (reg8 *) Air1__LCD_EN)
/* Slew Rate Control */
#define Air1_SLW                    (* (reg8 *) Air1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Air1_PRTDSI__CAPS_SEL       (* (reg8 *) Air1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Air1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Air1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Air1_PRTDSI__OE_SEL0        (* (reg8 *) Air1__PRTDSI__OE_SEL0) 
#define Air1_PRTDSI__OE_SEL1        (* (reg8 *) Air1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Air1_PRTDSI__OUT_SEL0       (* (reg8 *) Air1__PRTDSI__OUT_SEL0) 
#define Air1_PRTDSI__OUT_SEL1       (* (reg8 *) Air1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Air1_PRTDSI__SYNC_OUT       (* (reg8 *) Air1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Air1__SIO_CFG)
    #define Air1_SIO_HYST_EN        (* (reg8 *) Air1__SIO_HYST_EN)
    #define Air1_SIO_REG_HIFREQ     (* (reg8 *) Air1__SIO_REG_HIFREQ)
    #define Air1_SIO_CFG            (* (reg8 *) Air1__SIO_CFG)
    #define Air1_SIO_DIFF           (* (reg8 *) Air1__SIO_DIFF)
#endif /* (Air1__SIO_CFG) */

/* Interrupt Registers */
#if defined(Air1__INTSTAT)
    #define Air1_INTSTAT            (* (reg8 *) Air1__INTSTAT)
    #define Air1_SNAP               (* (reg8 *) Air1__SNAP)
    
	#define Air1_0_INTTYPE_REG 		(* (reg8 *) Air1__0__INTTYPE)
#endif /* (Air1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Air1_H */


/* [] END OF FILE */
