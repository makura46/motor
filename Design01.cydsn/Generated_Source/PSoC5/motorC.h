/*******************************************************************************
* File Name: motorC.h
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

#if !defined(CY_PWM_motorC_H)
#define CY_PWM_motorC_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motorC_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motorC_Resolution                     (8u)
#define motorC_UsingFixedFunction             (0u)
#define motorC_DeadBandMode                   (2u)
#define motorC_KillModeMinTime                (0u)
#define motorC_KillMode                       (0u)
#define motorC_PWMMode                        (0u)
#define motorC_PWMModeIsCenterAligned         (0u)
#define motorC_DeadBandUsed                   (1u)
#define motorC_DeadBand2_4                    (0u)

#if !defined(motorC_PWMUDB_genblk8_stsreg__REMOVED)
    #define motorC_UseStatus                  (1u)
#else
    #define motorC_UseStatus                  (0u)
#endif /* !defined(motorC_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motorC_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motorC_UseControl                 (1u)
#else
    #define motorC_UseControl                 (0u)
#endif /* !defined(motorC_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motorC_UseOneCompareMode              (1u)
#define motorC_MinimumKillTime                (1u)
#define motorC_EnableMode                     (0u)

#define motorC_CompareMode1SW                 (0u)
#define motorC_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motorC__B_PWM__DISABLED 0
#define motorC__B_PWM__ASYNCHRONOUS 1
#define motorC__B_PWM__SINGLECYCLE 2
#define motorC__B_PWM__LATCHED 3
#define motorC__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motorC__B_PWM__DBMDISABLED 0
#define motorC__B_PWM__DBM_2_4_CLOCKS 1
#define motorC__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motorC__B_PWM__ONE_OUTPUT 0
#define motorC__B_PWM__TWO_OUTPUTS 1
#define motorC__B_PWM__DUAL_EDGE 2
#define motorC__B_PWM__CENTER_ALIGN 3
#define motorC__B_PWM__DITHER 5
#define motorC__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motorC__B_PWM__LESS_THAN 1
#define motorC__B_PWM__LESS_THAN_OR_EQUAL 2
#define motorC__B_PWM__GREATER_THAN 3
#define motorC__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motorC__B_PWM__EQUAL 0
#define motorC__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motorC_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motorC_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motorC_PWMModeIsCenterAligned) */
        #if (motorC_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motorC_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motorC_DeadBandMode == motorC__B_PWM__DBM_256_CLOCKS || \
            motorC_DeadBandMode == motorC__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motorC_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motorC_KillModeMinTime) */

        /* Backup control register */
        #if(motorC_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motorC_UseControl) */

    #endif /* (!motorC_UsingFixedFunction) */

}motorC_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motorC_Start(void) ;
void    motorC_Stop(void) ;

#if (motorC_UseStatus || motorC_UsingFixedFunction)
    void  motorC_SetInterruptMode(uint8 interruptMode) ;
    uint8 motorC_ReadStatusRegister(void) ;
#endif /* (motorC_UseStatus || motorC_UsingFixedFunction) */

#define motorC_GetInterruptSource() motorC_ReadStatusRegister()

#if (motorC_UseControl)
    uint8 motorC_ReadControlRegister(void) ;
    void  motorC_WriteControlRegister(uint8 control)
          ;
#endif /* (motorC_UseControl) */

#if (motorC_UseOneCompareMode)
   #if (motorC_CompareMode1SW)
       void    motorC_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motorC_CompareMode1SW) */
#else
    #if (motorC_CompareMode1SW)
        void    motorC_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motorC_CompareMode1SW) */
    #if (motorC_CompareMode2SW)
        void    motorC_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motorC_CompareMode2SW) */
#endif /* (motorC_UseOneCompareMode) */

#if (!motorC_UsingFixedFunction)
    uint8   motorC_ReadCounter(void) ;
    uint8 motorC_ReadCapture(void) ;

    #if (motorC_UseStatus)
            void motorC_ClearFIFO(void) ;
    #endif /* (motorC_UseStatus) */

    void    motorC_WriteCounter(uint8 counter)
            ;
#endif /* (!motorC_UsingFixedFunction) */

void    motorC_WritePeriod(uint8 period)
        ;
uint8 motorC_ReadPeriod(void) ;

#if (motorC_UseOneCompareMode)
    void    motorC_WriteCompare(uint8 compare)
            ;
    uint8 motorC_ReadCompare(void) ;
#else
    void    motorC_WriteCompare1(uint8 compare)
            ;
    uint8 motorC_ReadCompare1(void) ;
    void    motorC_WriteCompare2(uint8 compare)
            ;
    uint8 motorC_ReadCompare2(void) ;
#endif /* (motorC_UseOneCompareMode) */


#if (motorC_DeadBandUsed)
    void    motorC_WriteDeadTime(uint8 deadtime) ;
    uint8   motorC_ReadDeadTime(void) ;
#endif /* (motorC_DeadBandUsed) */

#if ( motorC_KillModeMinTime)
    void motorC_WriteKillTime(uint8 killtime) ;
    uint8 motorC_ReadKillTime(void) ;
#endif /* ( motorC_KillModeMinTime) */

void motorC_Init(void) ;
void motorC_Enable(void) ;
void motorC_Sleep(void) ;
void motorC_Wakeup(void) ;
void motorC_SaveConfig(void) ;
void motorC_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motorC_INIT_PERIOD_VALUE          (255u)
#define motorC_INIT_COMPARE_VALUE1        (50u)
#define motorC_INIT_COMPARE_VALUE2        (63u)
#define motorC_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motorC_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorC_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorC_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorC_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motorC_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motorC_CTRL_CMPMODE2_SHIFT)
#define motorC_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motorC_CTRL_CMPMODE1_SHIFT)
#define motorC_INIT_DEAD_TIME             (4u)


/********************************
*         Registers
******************************** */

#if (motorC_UsingFixedFunction)
   #define motorC_PERIOD_LSB              (*(reg16 *) motorC_PWMHW__PER0)
   #define motorC_PERIOD_LSB_PTR          ( (reg16 *) motorC_PWMHW__PER0)
   #define motorC_COMPARE1_LSB            (*(reg16 *) motorC_PWMHW__CNT_CMP0)
   #define motorC_COMPARE1_LSB_PTR        ( (reg16 *) motorC_PWMHW__CNT_CMP0)
   #define motorC_COMPARE2_LSB            (0x00u)
   #define motorC_COMPARE2_LSB_PTR        (0x00u)
   #define motorC_COUNTER_LSB             (*(reg16 *) motorC_PWMHW__CNT_CMP0)
   #define motorC_COUNTER_LSB_PTR         ( (reg16 *) motorC_PWMHW__CNT_CMP0)
   #define motorC_CAPTURE_LSB             (*(reg16 *) motorC_PWMHW__CAP0)
   #define motorC_CAPTURE_LSB_PTR         ( (reg16 *) motorC_PWMHW__CAP0)
   #define motorC_RT1                     (*(reg8 *)  motorC_PWMHW__RT1)
   #define motorC_RT1_PTR                 ( (reg8 *)  motorC_PWMHW__RT1)

#else
   #if (motorC_Resolution == 8u) /* 8bit - PWM */

       #if(motorC_PWMModeIsCenterAligned)
           #define motorC_PERIOD_LSB      (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motorC_PERIOD_LSB_PTR  ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motorC_PERIOD_LSB      (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motorC_PERIOD_LSB_PTR  ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motorC_PWMModeIsCenterAligned) */

       #define motorC_COMPARE1_LSB        (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorC_COMPARE1_LSB_PTR    ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorC_COMPARE2_LSB        (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorC_COMPARE2_LSB_PTR    ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorC_COUNTERCAP_LSB      (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorC_COUNTERCAP_LSB_PTR  ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorC_COUNTER_LSB         (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorC_COUNTER_LSB_PTR     ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorC_CAPTURE_LSB         (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motorC_CAPTURE_LSB_PTR     ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motorC_PWMModeIsCenterAligned)
               #define motorC_PERIOD_LSB      (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motorC_PERIOD_LSB_PTR  ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motorC_PERIOD_LSB      (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motorC_PERIOD_LSB_PTR  ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motorC_PWMModeIsCenterAligned) */

            #define motorC_COMPARE1_LSB       (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorC_COMPARE1_LSB_PTR   ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorC_COMPARE2_LSB       (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorC_COMPARE2_LSB_PTR   ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorC_COUNTERCAP_LSB     (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorC_COUNTERCAP_LSB_PTR ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorC_COUNTER_LSB        (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorC_COUNTER_LSB_PTR    ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorC_CAPTURE_LSB        (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motorC_CAPTURE_LSB_PTR    ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motorC_PWMModeIsCenterAligned)
               #define motorC_PERIOD_LSB      (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motorC_PERIOD_LSB_PTR  ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motorC_PERIOD_LSB      (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motorC_PERIOD_LSB_PTR  ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motorC_PWMModeIsCenterAligned) */

            #define motorC_COMPARE1_LSB       (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorC_COMPARE1_LSB_PTR   ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorC_COMPARE2_LSB       (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorC_COMPARE2_LSB_PTR   ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorC_COUNTERCAP_LSB     (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorC_COUNTERCAP_LSB_PTR ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorC_COUNTER_LSB        (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorC_COUNTER_LSB_PTR    ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorC_CAPTURE_LSB        (*(reg16 *) motorC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motorC_CAPTURE_LSB_PTR    ((reg16 *)  motorC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motorC_AUX_CONTROLDP1          (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motorC_AUX_CONTROLDP1_PTR      ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motorC_Resolution == 8) */

   #define motorC_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motorC_AUX_CONTROLDP0          (*(reg8 *)  motorC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motorC_AUX_CONTROLDP0_PTR      ((reg8 *)   motorC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motorC_UsingFixedFunction) */

#if(motorC_KillModeMinTime )
    #define motorC_KILLMODEMINTIME        (*(reg8 *)  motorC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motorC_KILLMODEMINTIME_PTR    ((reg8 *)   motorC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motorC_KillModeMinTime ) */

#if(motorC_DeadBandMode == motorC__B_PWM__DBM_256_CLOCKS)
    #define motorC_DEADBAND_COUNT         (*(reg8 *)  motorC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorC_DEADBAND_COUNT_PTR     ((reg8 *)   motorC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorC_DEADBAND_LSB_PTR       ((reg8 *)   motorC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motorC_DEADBAND_LSB           (*(reg8 *)  motorC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motorC_DeadBandMode == motorC__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motorC_UsingFixedFunction)
        #define motorC_DEADBAND_COUNT         (*(reg8 *)  motorC_PWMHW__CFG0)
        #define motorC_DEADBAND_COUNT_PTR     ((reg8 *)   motorC_PWMHW__CFG0)
        #define motorC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorC_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motorC_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motorC_DEADBAND_COUNT         (*(reg8 *)  motorC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorC_DEADBAND_COUNT_PTR     ((reg8 *)   motorC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorC_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motorC_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motorC_UsingFixedFunction) */
#endif /* (motorC_DeadBandMode == motorC__B_PWM__DBM_256_CLOCKS) */



#if (motorC_UsingFixedFunction)
    #define motorC_STATUS                 (*(reg8 *) motorC_PWMHW__SR0)
    #define motorC_STATUS_PTR             ((reg8 *) motorC_PWMHW__SR0)
    #define motorC_STATUS_MASK            (*(reg8 *) motorC_PWMHW__SR0)
    #define motorC_STATUS_MASK_PTR        ((reg8 *) motorC_PWMHW__SR0)
    #define motorC_CONTROL                (*(reg8 *) motorC_PWMHW__CFG0)
    #define motorC_CONTROL_PTR            ((reg8 *) motorC_PWMHW__CFG0)
    #define motorC_CONTROL2               (*(reg8 *) motorC_PWMHW__CFG1)
    #define motorC_CONTROL3               (*(reg8 *) motorC_PWMHW__CFG2)
    #define motorC_GLOBAL_ENABLE          (*(reg8 *) motorC_PWMHW__PM_ACT_CFG)
    #define motorC_GLOBAL_ENABLE_PTR      ( (reg8 *) motorC_PWMHW__PM_ACT_CFG)
    #define motorC_GLOBAL_STBY_ENABLE     (*(reg8 *) motorC_PWMHW__PM_STBY_CFG)
    #define motorC_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motorC_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motorC_BLOCK_EN_MASK          (motorC_PWMHW__PM_ACT_MSK)
    #define motorC_BLOCK_STBY_EN_MASK     (motorC_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motorC_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motorC_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motorC_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motorC_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motorC_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motorC_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motorC_CTRL_ENABLE            (uint8)((uint8)0x01u << motorC_CTRL_ENABLE_SHIFT)
    #define motorC_CTRL_RESET             (uint8)((uint8)0x01u << motorC_CTRL_RESET_SHIFT)
    #define motorC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorC_CTRL_CMPMODE2_SHIFT)
    #define motorC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorC_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motorC_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motorC_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motorC_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motorC_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motorC_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motorC_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motorC_STATUS_TC_INT_EN_MASK_SHIFT            (motorC_STATUS_TC_SHIFT - 4u)
    #define motorC_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motorC_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorC_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motorC_STATUS_TC              (uint8)((uint8)0x01u << motorC_STATUS_TC_SHIFT)
    #define motorC_STATUS_CMP1            (uint8)((uint8)0x01u << motorC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motorC_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motorC_STATUS_TC >> 4u)
    #define motorC_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motorC_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motorC_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motorC_RT1_MASK              (uint8)((uint8)0x03u << motorC_RT1_SHIFT)
    #define motorC_SYNC                  (uint8)((uint8)0x03u << motorC_RT1_SHIFT)
    #define motorC_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motorC_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motorC_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motorC_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motorC_SYNCDSI_SHIFT)


#else
    #define motorC_STATUS                (*(reg8 *)   motorC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorC_STATUS_PTR            ((reg8 *)    motorC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorC_STATUS_MASK           (*(reg8 *)   motorC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorC_STATUS_MASK_PTR       ((reg8 *)    motorC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorC_STATUS_AUX_CTRL       (*(reg8 *)   motorC_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motorC_CONTROL               (*(reg8 *)   motorC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motorC_CONTROL_PTR           ((reg8 *)    motorC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motorC_CTRL_ENABLE_SHIFT      (0x07u)
    #define motorC_CTRL_RESET_SHIFT       (0x06u)
    #define motorC_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motorC_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motorC_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motorC_CTRL_ENABLE            (uint8)((uint8)0x01u << motorC_CTRL_ENABLE_SHIFT)
    #define motorC_CTRL_RESET             (uint8)((uint8)0x01u << motorC_CTRL_RESET_SHIFT)
    #define motorC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorC_CTRL_CMPMODE2_SHIFT)
    #define motorC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorC_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motorC_STATUS_KILL_SHIFT          (0x05u)
    #define motorC_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motorC_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motorC_STATUS_TC_SHIFT            (0x02u)
    #define motorC_STATUS_CMP2_SHIFT          (0x01u)
    #define motorC_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motorC_STATUS_KILL_INT_EN_MASK_SHIFT          (motorC_STATUS_KILL_SHIFT)
    #define motorC_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motorC_STATUS_FIFONEMPTY_SHIFT)
    #define motorC_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motorC_STATUS_FIFOFULL_SHIFT)
    #define motorC_STATUS_TC_INT_EN_MASK_SHIFT            (motorC_STATUS_TC_SHIFT)
    #define motorC_STATUS_CMP2_INT_EN_MASK_SHIFT          (motorC_STATUS_CMP2_SHIFT)
    #define motorC_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorC_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motorC_STATUS_KILL            (uint8)((uint8)0x00u << motorC_STATUS_KILL_SHIFT )
    #define motorC_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motorC_STATUS_FIFOFULL_SHIFT)
    #define motorC_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motorC_STATUS_FIFONEMPTY_SHIFT)
    #define motorC_STATUS_TC              (uint8)((uint8)0x01u << motorC_STATUS_TC_SHIFT)
    #define motorC_STATUS_CMP2            (uint8)((uint8)0x01u << motorC_STATUS_CMP2_SHIFT)
    #define motorC_STATUS_CMP1            (uint8)((uint8)0x01u << motorC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motorC_STATUS_KILL_INT_EN_MASK            (motorC_STATUS_KILL)
    #define motorC_STATUS_FIFOFULL_INT_EN_MASK        (motorC_STATUS_FIFOFULL)
    #define motorC_STATUS_FIFONEMPTY_INT_EN_MASK      (motorC_STATUS_FIFONEMPTY)
    #define motorC_STATUS_TC_INT_EN_MASK              (motorC_STATUS_TC)
    #define motorC_STATUS_CMP2_INT_EN_MASK            (motorC_STATUS_CMP2)
    #define motorC_STATUS_CMP1_INT_EN_MASK            (motorC_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motorC_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motorC_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motorC_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motorC_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motorC_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motorC_UsingFixedFunction */

#endif  /* CY_PWM_motorC_H */


/* [] END OF FILE */
