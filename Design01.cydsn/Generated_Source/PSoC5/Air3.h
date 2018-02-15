/*******************************************************************************
* File Name: Air3.h  
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

#if !defined(CY_PINS_Air3_H) /* Pins Air3_H */
#define CY_PINS_Air3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Air3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Air3__PORT == 15 && ((Air3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Air3_Write(uint8 value);
void    Air3_SetDriveMode(uint8 mode);
uint8   Air3_ReadDataReg(void);
uint8   Air3_Read(void);
void    Air3_SetInterruptMode(uint16 position, uint16 mode);
uint8   Air3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Air3_SetDriveMode() function.
     *  @{
     */
        #define Air3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Air3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Air3_DM_RES_UP          PIN_DM_RES_UP
        #define Air3_DM_RES_DWN         PIN_DM_RES_DWN
        #define Air3_DM_OD_LO           PIN_DM_OD_LO
        #define Air3_DM_OD_HI           PIN_DM_OD_HI
        #define Air3_DM_STRONG          PIN_DM_STRONG
        #define Air3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Air3_MASK               Air3__MASK
#define Air3_SHIFT              Air3__SHIFT
#define Air3_WIDTH              1u

/* Interrupt constants */
#if defined(Air3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Air3_SetInterruptMode() function.
     *  @{
     */
        #define Air3_INTR_NONE      (uint16)(0x0000u)
        #define Air3_INTR_RISING    (uint16)(0x0001u)
        #define Air3_INTR_FALLING   (uint16)(0x0002u)
        #define Air3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Air3_INTR_MASK      (0x01u) 
#endif /* (Air3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Air3_PS                     (* (reg8 *) Air3__PS)
/* Data Register */
#define Air3_DR                     (* (reg8 *) Air3__DR)
/* Port Number */
#define Air3_PRT_NUM                (* (reg8 *) Air3__PRT) 
/* Connect to Analog Globals */                                                  
#define Air3_AG                     (* (reg8 *) Air3__AG)                       
/* Analog MUX bux enable */
#define Air3_AMUX                   (* (reg8 *) Air3__AMUX) 
/* Bidirectional Enable */                                                        
#define Air3_BIE                    (* (reg8 *) Air3__BIE)
/* Bit-mask for Aliased Register Access */
#define Air3_BIT_MASK               (* (reg8 *) Air3__BIT_MASK)
/* Bypass Enable */
#define Air3_BYP                    (* (reg8 *) Air3__BYP)
/* Port wide control signals */                                                   
#define Air3_CTL                    (* (reg8 *) Air3__CTL)
/* Drive Modes */
#define Air3_DM0                    (* (reg8 *) Air3__DM0) 
#define Air3_DM1                    (* (reg8 *) Air3__DM1)
#define Air3_DM2                    (* (reg8 *) Air3__DM2) 
/* Input Buffer Disable Override */
#define Air3_INP_DIS                (* (reg8 *) Air3__INP_DIS)
/* LCD Common or Segment Drive */
#define Air3_LCD_COM_SEG            (* (reg8 *) Air3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Air3_LCD_EN                 (* (reg8 *) Air3__LCD_EN)
/* Slew Rate Control */
#define Air3_SLW                    (* (reg8 *) Air3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Air3_PRTDSI__CAPS_SEL       (* (reg8 *) Air3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Air3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Air3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Air3_PRTDSI__OE_SEL0        (* (reg8 *) Air3__PRTDSI__OE_SEL0) 
#define Air3_PRTDSI__OE_SEL1        (* (reg8 *) Air3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Air3_PRTDSI__OUT_SEL0       (* (reg8 *) Air3__PRTDSI__OUT_SEL0) 
#define Air3_PRTDSI__OUT_SEL1       (* (reg8 *) Air3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Air3_PRTDSI__SYNC_OUT       (* (reg8 *) Air3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Air3__SIO_CFG)
    #define Air3_SIO_HYST_EN        (* (reg8 *) Air3__SIO_HYST_EN)
    #define Air3_SIO_REG_HIFREQ     (* (reg8 *) Air3__SIO_REG_HIFREQ)
    #define Air3_SIO_CFG            (* (reg8 *) Air3__SIO_CFG)
    #define Air3_SIO_DIFF           (* (reg8 *) Air3__SIO_DIFF)
#endif /* (Air3__SIO_CFG) */

/* Interrupt Registers */
#if defined(Air3__INTSTAT)
    #define Air3_INTSTAT            (* (reg8 *) Air3__INTSTAT)
    #define Air3_SNAP               (* (reg8 *) Air3__SNAP)
    
	#define Air3_0_INTTYPE_REG 		(* (reg8 *) Air3__0__INTTYPE)
#endif /* (Air3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Air3_H */


/* [] END OF FILE */
