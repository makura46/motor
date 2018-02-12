/*******************************************************************************
* File Name: motorB.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_motorB_H)
#define CY_PWM_motorB_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motorB_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motorB_Resolution                     (8u)
#define motorB_UsingFixedFunction             (0u)
#define motorB_DeadBandMode                   (2u)
#define motorB_KillModeMinTime                (0u)
#define motorB_KillMode                       (0u)
#define motorB_PWMMode                        (0u)
#define motorB_PWMModeIsCenterAligned         (0u)
#define motorB_DeadBandUsed                   (1u)
#define motorB_DeadBand2_4                    (0u)

#if !defined(motorB_PWMUDB_genblk8_stsreg__REMOVED)
    #define motorB_UseStatus                  (1u)
#else
    #define motorB_UseStatus                  (0u)
#endif /* !defined(motorB_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motorB_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motorB_UseControl                 (1u)
#else
    #define motorB_UseControl                 (0u)
#endif /* !defined(motorB_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motorB_UseOneCompareMode              (1u)
#define motorB_MinimumKillTime                (1u)
#define motorB_EnableMode                     (0u)

#define motorB_CompareMode1SW                 (0u)
#define motorB_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motorB__B_PWM__DISABLED 0
#define motorB__B_PWM__ASYNCHRONOUS 1
#define motorB__B_PWM__SINGLECYCLE 2
#define motorB__B_PWM__LATCHED 3
#define motorB__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motorB__B_PWM__DBMDISABLED 0
#define motorB__B_PWM__DBM_2_4_CLOCKS 1
#define motorB__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motorB__B_PWM__ONE_OUTPUT 0
#define motorB__B_PWM__TWO_OUTPUTS 1
#define motorB__B_PWM__DUAL_EDGE 2
#define motorB__B_PWM__CENTER_ALIGN 3
#define motorB__B_PWM__DITHER 5
#define motorB__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motorB__B_PWM__LESS_THAN 1
#define motorB__B_PWM__LESS_THAN_OR_EQUAL 2
#define motorB__B_PWM__GREATER_THAN 3
#define motorB__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motorB__B_PWM__EQUAL 0
#define motorB__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motorB_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motorB_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motorB_PWMModeIsCenterAligned) */
        #if (motorB_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motorB_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motorB_DeadBandMode == motorB__B_PWM__DBM_256_CLOCKS || \
            motorB_DeadBandMode == motorB__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motorB_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motorB_KillModeMinTime) */

        /* Backup control register */
        #if(motorB_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motorB_UseControl) */

    #endif /* (!motorB_UsingFixedFunction) */

}motorB_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motorB_Start(void) ;
void    motorB_Stop(void) ;

#if (motorB_UseStatus || motorB_UsingFixedFunction)
    void  motorB_SetInterruptMode(uint8 interruptMode) ;
    uint8 motorB_ReadStatusRegister(void) ;
#endif /* (motorB_UseStatus || motorB_UsingFixedFunction) */

#define motorB_GetInterruptSource() motorB_ReadStatusRegister()

#if (motorB_UseControl)
    uint8 motorB_ReadControlRegister(void) ;
    void  motorB_WriteControlRegister(uint8 control)
          ;
#endif /* (motorB_UseControl) */

#if (motorB_UseOneCompareMode)
   #if (motorB_CompareMode1SW)
       void    motorB_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motorB_CompareMode1SW) */
#else
    #if (motorB_CompareMode1SW)
        void    motorB_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motorB_CompareMode1SW) */
    #if (motorB_CompareMode2SW)
        void    motorB_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motorB_CompareMode2SW) */
#endif /* (motorB_UseOneCompareMode) */

#if (!motorB_UsingFixedFunction)
    uint8   motorB_ReadCounter(void) ;
    uint8 motorB_ReadCapture(void) ;

    #if (motorB_UseStatus)
            void motorB_ClearFIFO(void) ;
    #endif /* (motorB_UseStatus) */

    void    motorB_WriteCounter(uint8 counter)
            ;
#endif /* (!motorB_UsingFixedFunction) */

void    motorB_WritePeriod(uint8 period)
        ;
uint8 motorB_ReadPeriod(void) ;

#if (motorB_UseOneCompareMode)
    void    motorB_WriteCompare(uint8 compare)
            ;
    uint8 motorB_ReadCompare(void) ;
#else
    void    motorB_WriteCompare1(uint8 compare)
            ;
    uint8 motorB_ReadCompare1(void) ;
    void    motorB_WriteCompare2(uint8 compare)
            ;
    uint8 motorB_ReadCompare2(void) ;
#endif /* (motorB_UseOneCompareMode) */


#if (motorB_DeadBandUsed)
    void    motorB_WriteDeadTime(uint8 deadtime) ;
    uint8   motorB_ReadDeadTime(void) ;
#endif /* (motorB_DeadBandUsed) */

#if ( motorB_KillModeMinTime)
    void motorB_WriteKillTime(uint8 killtime) ;
    uint8 motorB_ReadKillTime(void) ;
#endif /* ( motorB_KillModeMinTime) */

void motorB_Init(void) ;
void motorB_Enable(void) ;
void motorB_Sleep(void) ;
void motorB_Wakeup(void) ;
void motorB_SaveConfig(void) ;
void motorB_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motorB_INIT_PERIOD_VALUE          (255u)
#define motorB_INIT_COMPARE_VALUE1        (50u)
#define motorB_INIT_COMPARE_VALUE2        (63u)
#define motorB_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motorB_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorB_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorB_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorB_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motorB_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motorB_CTRL_CMPMODE2_SHIFT)
#define motorB_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motorB_CTRL_CMPMODE1_SHIFT)
#define motorB_INIT_DEAD_TIME             (4u)


/********************************
*         Registers
******************************** */

#if (motorB_UsingFixedFunction)
   #define motorB_PERIOD_LSB              (*(reg16 *) motorB_PWMHW__PER0)
   #define motorB_PERIOD_LSB_PTR          ( (reg16 *) motorB_PWMHW__PER0)
   #define motorB_COMPARE1_LSB            (*(reg16 *) motorB_PWMHW__CNT_CMP0)
   #define motorB_COMPARE1_LSB_PTR        ( (reg16 *) motorB_PWMHW__CNT_CMP0)
   #define motorB_COMPARE2_LSB            (0x00u)
   #define motorB_COMPARE2_LSB_PTR        (0x00u)
   #define motorB_COUNTER_LSB             (*(reg16 *) motorB_PWMHW__CNT_CMP0)
   #define motorB_COUNTER_LSB_PTR         ( (reg16 *) motorB_PWMHW__CNT_CMP0)
   #define motorB_CAPTURE_LSB             (*(reg16 *) motorB_PWMHW__CAP0)
   #define motorB_CAPTURE_LSB_PTR         ( (reg16 *) motorB_PWMHW__CAP0)
   #define motorB_RT1                     (*(reg8 *)  motorB_PWMHW__RT1)
   #define motorB_RT1_PTR                 ( (reg8 *)  motorB_PWMHW__RT1)

#else
   #if (motorB_Resolution == 8u) /* 8bit - PWM */

       #if(motorB_PWMModeIsCenterAligned)
           #define motorB_PERIOD_LSB      (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motorB_PERIOD_LSB_PTR  ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motorB_PERIOD_LSB      (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motorB_PERIOD_LSB_PTR  ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motorB_PWMModeIsCenterAligned) */

       #define motorB_COMPARE1_LSB        (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorB_COMPARE1_LSB_PTR    ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorB_COMPARE2_LSB        (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorB_COMPARE2_LSB_PTR    ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorB_COUNTERCAP_LSB      (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorB_COUNTERCAP_LSB_PTR  ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorB_COUNTER_LSB         (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorB_COUNTER_LSB_PTR     ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorB_CAPTURE_LSB         (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motorB_CAPTURE_LSB_PTR     ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motorB_PWMModeIsCenterAligned)
               #define motorB_PERIOD_LSB      (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motorB_PERIOD_LSB_PTR  ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motorB_PERIOD_LSB      (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motorB_PERIOD_LSB_PTR  ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motorB_PWMModeIsCenterAligned) */

            #define motorB_COMPARE1_LSB       (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorB_COMPARE1_LSB_PTR   ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorB_COMPARE2_LSB       (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorB_COMPARE2_LSB_PTR   ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorB_COUNTERCAP_LSB     (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorB_COUNTERCAP_LSB_PTR ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorB_COUNTER_LSB        (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorB_COUNTER_LSB_PTR    ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorB_CAPTURE_LSB        (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motorB_CAPTURE_LSB_PTR    ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motorB_PWMModeIsCenterAligned)
               #define motorB_PERIOD_LSB      (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motorB_PERIOD_LSB_PTR  ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motorB_PERIOD_LSB      (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motorB_PERIOD_LSB_PTR  ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motorB_PWMModeIsCenterAligned) */

            #define motorB_COMPARE1_LSB       (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorB_COMPARE1_LSB_PTR   ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorB_COMPARE2_LSB       (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorB_COMPARE2_LSB_PTR   ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorB_COUNTERCAP_LSB     (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorB_COUNTERCAP_LSB_PTR ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorB_COUNTER_LSB        (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorB_COUNTER_LSB_PTR    ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorB_CAPTURE_LSB        (*(reg16 *) motorB_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motorB_CAPTURE_LSB_PTR    ((reg16 *)  motorB_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motorB_AUX_CONTROLDP1          (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motorB_AUX_CONTROLDP1_PTR      ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motorB_Resolution == 8) */

   #define motorB_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motorB_AUX_CONTROLDP0          (*(reg8 *)  motorB_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motorB_AUX_CONTROLDP0_PTR      ((reg8 *)   motorB_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motorB_UsingFixedFunction) */

#if(motorB_KillModeMinTime )
    #define motorB_KILLMODEMINTIME        (*(reg8 *)  motorB_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motorB_KILLMODEMINTIME_PTR    ((reg8 *)   motorB_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motorB_KillModeMinTime ) */

#if(motorB_DeadBandMode == motorB__B_PWM__DBM_256_CLOCKS)
    #define motorB_DEADBAND_COUNT         (*(reg8 *)  motorB_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorB_DEADBAND_COUNT_PTR     ((reg8 *)   motorB_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorB_DEADBAND_LSB_PTR       ((reg8 *)   motorB_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motorB_DEADBAND_LSB           (*(reg8 *)  motorB_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motorB_DeadBandMode == motorB__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motorB_UsingFixedFunction)
        #define motorB_DEADBAND_COUNT         (*(reg8 *)  motorB_PWMHW__CFG0)
        #define motorB_DEADBAND_COUNT_PTR     ((reg8 *)   motorB_PWMHW__CFG0)
        #define motorB_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorB_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motorB_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motorB_DEADBAND_COUNT         (*(reg8 *)  motorB_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorB_DEADBAND_COUNT_PTR     ((reg8 *)   motorB_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorB_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorB_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motorB_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motorB_UsingFixedFunction) */
#endif /* (motorB_DeadBandMode == motorB__B_PWM__DBM_256_CLOCKS) */



#if (motorB_UsingFixedFunction)
    #define motorB_STATUS                 (*(reg8 *) motorB_PWMHW__SR0)
    #define motorB_STATUS_PTR             ((reg8 *) motorB_PWMHW__SR0)
    #define motorB_STATUS_MASK            (*(reg8 *) motorB_PWMHW__SR0)
    #define motorB_STATUS_MASK_PTR        ((reg8 *) motorB_PWMHW__SR0)
    #define motorB_CONTROL                (*(reg8 *) motorB_PWMHW__CFG0)
    #define motorB_CONTROL_PTR            ((reg8 *) motorB_PWMHW__CFG0)
    #define motorB_CONTROL2               (*(reg8 *) motorB_PWMHW__CFG1)
    #define motorB_CONTROL3               (*(reg8 *) motorB_PWMHW__CFG2)
    #define motorB_GLOBAL_ENABLE          (*(reg8 *) motorB_PWMHW__PM_ACT_CFG)
    #define motorB_GLOBAL_ENABLE_PTR      ( (reg8 *) motorB_PWMHW__PM_ACT_CFG)
    #define motorB_GLOBAL_STBY_ENABLE     (*(reg8 *) motorB_PWMHW__PM_STBY_CFG)
    #define motorB_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motorB_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motorB_BLOCK_EN_MASK          (motorB_PWMHW__PM_ACT_MSK)
    #define motorB_BLOCK_STBY_EN_MASK     (motorB_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motorB_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motorB_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motorB_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motorB_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motorB_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motorB_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motorB_CTRL_ENABLE            (uint8)((uint8)0x01u << motorB_CTRL_ENABLE_SHIFT)
    #define motorB_CTRL_RESET             (uint8)((uint8)0x01u << motorB_CTRL_RESET_SHIFT)
    #define motorB_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorB_CTRL_CMPMODE2_SHIFT)
    #define motorB_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorB_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motorB_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motorB_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motorB_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motorB_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motorB_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motorB_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motorB_STATUS_TC_INT_EN_MASK_SHIFT            (motorB_STATUS_TC_SHIFT - 4u)
    #define motorB_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motorB_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorB_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motorB_STATUS_TC              (uint8)((uint8)0x01u << motorB_STATUS_TC_SHIFT)
    #define motorB_STATUS_CMP1            (uint8)((uint8)0x01u << motorB_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motorB_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motorB_STATUS_TC >> 4u)
    #define motorB_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motorB_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motorB_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motorB_RT1_MASK              (uint8)((uint8)0x03u << motorB_RT1_SHIFT)
    #define motorB_SYNC                  (uint8)((uint8)0x03u << motorB_RT1_SHIFT)
    #define motorB_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motorB_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motorB_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motorB_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motorB_SYNCDSI_SHIFT)


#else
    #define motorB_STATUS                (*(reg8 *)   motorB_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorB_STATUS_PTR            ((reg8 *)    motorB_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorB_STATUS_MASK           (*(reg8 *)   motorB_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorB_STATUS_MASK_PTR       ((reg8 *)    motorB_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorB_STATUS_AUX_CTRL       (*(reg8 *)   motorB_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motorB_CONTROL               (*(reg8 *)   motorB_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motorB_CONTROL_PTR           ((reg8 *)    motorB_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motorB_CTRL_ENABLE_SHIFT      (0x07u)
    #define motorB_CTRL_RESET_SHIFT       (0x06u)
    #define motorB_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motorB_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motorB_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motorB_CTRL_ENABLE            (uint8)((uint8)0x01u << motorB_CTRL_ENABLE_SHIFT)
    #define motorB_CTRL_RESET             (uint8)((uint8)0x01u << motorB_CTRL_RESET_SHIFT)
    #define motorB_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorB_CTRL_CMPMODE2_SHIFT)
    #define motorB_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorB_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motorB_STATUS_KILL_SHIFT          (0x05u)
    #define motorB_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motorB_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motorB_STATUS_TC_SHIFT            (0x02u)
    #define motorB_STATUS_CMP2_SHIFT          (0x01u)
    #define motorB_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motorB_STATUS_KILL_INT_EN_MASK_SHIFT          (motorB_STATUS_KILL_SHIFT)
    #define motorB_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motorB_STATUS_FIFONEMPTY_SHIFT)
    #define motorB_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motorB_STATUS_FIFOFULL_SHIFT)
    #define motorB_STATUS_TC_INT_EN_MASK_SHIFT            (motorB_STATUS_TC_SHIFT)
    #define motorB_STATUS_CMP2_INT_EN_MASK_SHIFT          (motorB_STATUS_CMP2_SHIFT)
    #define motorB_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorB_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motorB_STATUS_KILL            (uint8)((uint8)0x00u << motorB_STATUS_KILL_SHIFT )
    #define motorB_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motorB_STATUS_FIFOFULL_SHIFT)
    #define motorB_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motorB_STATUS_FIFONEMPTY_SHIFT)
    #define motorB_STATUS_TC              (uint8)((uint8)0x01u << motorB_STATUS_TC_SHIFT)
    #define motorB_STATUS_CMP2            (uint8)((uint8)0x01u << motorB_STATUS_CMP2_SHIFT)
    #define motorB_STATUS_CMP1            (uint8)((uint8)0x01u << motorB_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motorB_STATUS_KILL_INT_EN_MASK            (motorB_STATUS_KILL)
    #define motorB_STATUS_FIFOFULL_INT_EN_MASK        (motorB_STATUS_FIFOFULL)
    #define motorB_STATUS_FIFONEMPTY_INT_EN_MASK      (motorB_STATUS_FIFONEMPTY)
    #define motorB_STATUS_TC_INT_EN_MASK              (motorB_STATUS_TC)
    #define motorB_STATUS_CMP2_INT_EN_MASK            (motorB_STATUS_CMP2)
    #define motorB_STATUS_CMP1_INT_EN_MASK            (motorB_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motorB_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motorB_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motorB_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motorB_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motorB_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motorB_UsingFixedFunction */

#endif  /* CY_PWM_motorB_H */


/* [] END OF FILE */
