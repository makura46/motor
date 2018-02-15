/*******************************************************************************
* File Name: Air2.h  
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

#if !defined(CY_PINS_Air2_H) /* Pins Air2_H */
#define CY_PINS_Air2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Air2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Air2__PORT == 15 && ((Air2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Air2_Write(uint8 value);
void    Air2_SetDriveMode(uint8 mode);
uint8   Air2_ReadDataReg(void);
uint8   Air2_Read(void);
void    Air2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Air2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Air2_SetDriveMode() function.
     *  @{
     */
        #define Air2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Air2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Air2_DM_RES_UP          PIN_DM_RES_UP
        #define Air2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Air2_DM_OD_LO           PIN_DM_OD_LO
        #define Air2_DM_OD_HI           PIN_DM_OD_HI
        #define Air2_DM_STRONG          PIN_DM_STRONG
        #define Air2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Air2_MASK               Air2__MASK
#define Air2_SHIFT              Air2__SHIFT
#define Air2_WIDTH              1u

/* Interrupt constants */
#if defined(Air2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Air2_SetInterruptMode() function.
     *  @{
     */
        #define Air2_INTR_NONE      (uint16)(0x0000u)
        #define Air2_INTR_RISING    (uint16)(0x0001u)
        #define Air2_INTR_FALLING   (uint16)(0x0002u)
        #define Air2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Air2_INTR_MASK      (0x01u) 
#endif /* (Air2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Air2_PS                     (* (reg8 *) Air2__PS)
/* Data Register */
#define Air2_DR                     (* (reg8 *) Air2__DR)
/* Port Number */
#define Air2_PRT_NUM                (* (reg8 *) Air2__PRT) 
/* Connect to Analog Globals */                                                  
#define Air2_AG                     (* (reg8 *) Air2__AG)                       
/* Analog MUX bux enable */
#define Air2_AMUX                   (* (reg8 *) Air2__AMUX) 
/* Bidirectional Enable */                                                        
#define Air2_BIE                    (* (reg8 *) Air2__BIE)
/* Bit-mask for Aliased Register Access */
#define Air2_BIT_MASK               (* (reg8 *) Air2__BIT_MASK)
/* Bypass Enable */
#define Air2_BYP                    (* (reg8 *) Air2__BYP)
/* Port wide control signals */                                                   
#define Air2_CTL                    (* (reg8 *) Air2__CTL)
/* Drive Modes */
#define Air2_DM0                    (* (reg8 *) Air2__DM0) 
#define Air2_DM1                    (* (reg8 *) Air2__DM1)
#define Air2_DM2                    (* (reg8 *) Air2__DM2) 
/* Input Buffer Disable Override */
#define Air2_INP_DIS                (* (reg8 *) Air2__INP_DIS)
/* LCD Common or Segment Drive */
#define Air2_LCD_COM_SEG            (* (reg8 *) Air2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Air2_LCD_EN                 (* (reg8 *) Air2__LCD_EN)
/* Slew Rate Control */
#define Air2_SLW                    (* (reg8 *) Air2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Air2_PRTDSI__CAPS_SEL       (* (reg8 *) Air2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Air2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Air2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Air2_PRTDSI__OE_SEL0        (* (reg8 *) Air2__PRTDSI__OE_SEL0) 
#define Air2_PRTDSI__OE_SEL1        (* (reg8 *) Air2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Air2_PRTDSI__OUT_SEL0       (* (reg8 *) Air2__PRTDSI__OUT_SEL0) 
#define Air2_PRTDSI__OUT_SEL1       (* (reg8 *) Air2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Air2_PRTDSI__SYNC_OUT       (* (reg8 *) Air2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Air2__SIO_CFG)
    #define Air2_SIO_HYST_EN        (* (reg8 *) Air2__SIO_HYST_EN)
    #define Air2_SIO_REG_HIFREQ     (* (reg8 *) Air2__SIO_REG_HIFREQ)
    #define Air2_SIO_CFG            (* (reg8 *) Air2__SIO_CFG)
    #define Air2_SIO_DIFF           (* (reg8 *) Air2__SIO_DIFF)
#endif /* (Air2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Air2__INTSTAT)
    #define Air2_INTSTAT            (* (reg8 *) Air2__INTSTAT)
    #define Air2_SNAP               (* (reg8 *) Air2__SNAP)
    
	#define Air2_0_INTTYPE_REG 		(* (reg8 *) Air2__0__INTTYPE)
#endif /* (Air2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Air2_H */


/* [] END OF FILE */
