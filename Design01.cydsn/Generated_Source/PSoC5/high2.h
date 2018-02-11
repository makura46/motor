/*******************************************************************************
* File Name: high2.h  
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

#if !defined(CY_PINS_high2_H) /* Pins high2_H */
#define CY_PINS_high2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "high2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 high2__PORT == 15 && ((high2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    high2_Write(uint8 value);
void    high2_SetDriveMode(uint8 mode);
uint8   high2_ReadDataReg(void);
uint8   high2_Read(void);
void    high2_SetInterruptMode(uint16 position, uint16 mode);
uint8   high2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the high2_SetDriveMode() function.
     *  @{
     */
        #define high2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define high2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define high2_DM_RES_UP          PIN_DM_RES_UP
        #define high2_DM_RES_DWN         PIN_DM_RES_DWN
        #define high2_DM_OD_LO           PIN_DM_OD_LO
        #define high2_DM_OD_HI           PIN_DM_OD_HI
        #define high2_DM_STRONG          PIN_DM_STRONG
        #define high2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define high2_MASK               high2__MASK
#define high2_SHIFT              high2__SHIFT
#define high2_WIDTH              1u

/* Interrupt constants */
#if defined(high2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in high2_SetInterruptMode() function.
     *  @{
     */
        #define high2_INTR_NONE      (uint16)(0x0000u)
        #define high2_INTR_RISING    (uint16)(0x0001u)
        #define high2_INTR_FALLING   (uint16)(0x0002u)
        #define high2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define high2_INTR_MASK      (0x01u) 
#endif /* (high2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define high2_PS                     (* (reg8 *) high2__PS)
/* Data Register */
#define high2_DR                     (* (reg8 *) high2__DR)
/* Port Number */
#define high2_PRT_NUM                (* (reg8 *) high2__PRT) 
/* Connect to Analog Globals */                                                  
#define high2_AG                     (* (reg8 *) high2__AG)                       
/* Analog MUX bux enable */
#define high2_AMUX                   (* (reg8 *) high2__AMUX) 
/* Bidirectional Enable */                                                        
#define high2_BIE                    (* (reg8 *) high2__BIE)
/* Bit-mask for Aliased Register Access */
#define high2_BIT_MASK               (* (reg8 *) high2__BIT_MASK)
/* Bypass Enable */
#define high2_BYP                    (* (reg8 *) high2__BYP)
/* Port wide control signals */                                                   
#define high2_CTL                    (* (reg8 *) high2__CTL)
/* Drive Modes */
#define high2_DM0                    (* (reg8 *) high2__DM0) 
#define high2_DM1                    (* (reg8 *) high2__DM1)
#define high2_DM2                    (* (reg8 *) high2__DM2) 
/* Input Buffer Disable Override */
#define high2_INP_DIS                (* (reg8 *) high2__INP_DIS)
/* LCD Common or Segment Drive */
#define high2_LCD_COM_SEG            (* (reg8 *) high2__LCD_COM_SEG)
/* Enable Segment LCD */
#define high2_LCD_EN                 (* (reg8 *) high2__LCD_EN)
/* Slew Rate Control */
#define high2_SLW                    (* (reg8 *) high2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define high2_PRTDSI__CAPS_SEL       (* (reg8 *) high2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define high2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) high2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define high2_PRTDSI__OE_SEL0        (* (reg8 *) high2__PRTDSI__OE_SEL0) 
#define high2_PRTDSI__OE_SEL1        (* (reg8 *) high2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define high2_PRTDSI__OUT_SEL0       (* (reg8 *) high2__PRTDSI__OUT_SEL0) 
#define high2_PRTDSI__OUT_SEL1       (* (reg8 *) high2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define high2_PRTDSI__SYNC_OUT       (* (reg8 *) high2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(high2__SIO_CFG)
    #define high2_SIO_HYST_EN        (* (reg8 *) high2__SIO_HYST_EN)
    #define high2_SIO_REG_HIFREQ     (* (reg8 *) high2__SIO_REG_HIFREQ)
    #define high2_SIO_CFG            (* (reg8 *) high2__SIO_CFG)
    #define high2_SIO_DIFF           (* (reg8 *) high2__SIO_DIFF)
#endif /* (high2__SIO_CFG) */

/* Interrupt Registers */
#if defined(high2__INTSTAT)
    #define high2_INTSTAT            (* (reg8 *) high2__INTSTAT)
    #define high2_SNAP               (* (reg8 *) high2__SNAP)
    
	#define high2_0_INTTYPE_REG 		(* (reg8 *) high2__0__INTTYPE)
#endif /* (high2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_high2_H */


/* [] END OF FILE */
