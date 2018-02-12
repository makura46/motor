/*******************************************************************************
* File Name: motorD.h
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

#if !defined(CY_PWM_motorD_H)
#define CY_PWM_motorD_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motorD_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motorD_Resolution                     (8u)
#define motorD_UsingFixedFunction             (0u)
#define motorD_DeadBandMode                   (2u)
#define motorD_KillModeMinTime                (0u)
#define motorD_KillMode                       (0u)
#define motorD_PWMMode                        (0u)
#define motorD_PWMModeIsCenterAligned         (0u)
#define motorD_DeadBandUsed                   (1u)
#define motorD_DeadBand2_4                    (0u)

#if !defined(motorD_PWMUDB_genblk8_stsreg__REMOVED)
    #define motorD_UseStatus                  (1u)
#else
    #define motorD_UseStatus                  (0u)
#endif /* !defined(motorD_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motorD_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motorD_UseControl                 (1u)
#else
    #define motorD_UseControl                 (0u)
#endif /* !defined(motorD_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motorD_UseOneCompareMode              (1u)
#define motorD_MinimumKillTime                (1u)
#define motorD_EnableMode                     (0u)

#define motorD_CompareMode1SW                 (0u)
#define motorD_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motorD__B_PWM__DISABLED 0
#define motorD__B_PWM__ASYNCHRONOUS 1
#define motorD__B_PWM__SINGLECYCLE 2
#define motorD__B_PWM__LATCHED 3
#define motorD__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motorD__B_PWM__DBMDISABLED 0
#define motorD__B_PWM__DBM_2_4_CLOCKS 1
#define motorD__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motorD__B_PWM__ONE_OUTPUT 0
#define motorD__B_PWM__TWO_OUTPUTS 1
#define motorD__B_PWM__DUAL_EDGE 2
#define motorD__B_PWM__CENTER_ALIGN 3
#define motorD__B_PWM__DITHER 5
#define motorD__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motorD__B_PWM__LESS_THAN 1
#define motorD__B_PWM__LESS_THAN_OR_EQUAL 2
#define motorD__B_PWM__GREATER_THAN 3
#define motorD__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motorD__B_PWM__EQUAL 0
#define motorD__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motorD_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motorD_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motorD_PWMModeIsCenterAligned) */
        #if (motorD_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motorD_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motorD_DeadBandMode == motorD__B_PWM__DBM_256_CLOCKS || \
            motorD_DeadBandMode == motorD__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motorD_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motorD_KillModeMinTime) */

        /* Backup control register */
        #if(motorD_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motorD_UseControl) */

    #endif /* (!motorD_UsingFixedFunction) */

}motorD_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motorD_Start(void) ;
void    motorD_Stop(void) ;

#if (motorD_UseStatus || motorD_UsingFixedFunction)
    void  motorD_SetInterruptMode(uint8 interruptMode) ;
    uint8 motorD_ReadStatusRegister(void) ;
#endif /* (motorD_UseStatus || motorD_UsingFixedFunction) */

#define motorD_GetInterruptSource() motorD_ReadStatusRegister()

#if (motorD_UseControl)
    uint8 motorD_ReadControlRegister(void) ;
    void  motorD_WriteControlRegister(uint8 control)
          ;
#endif /* (motorD_UseControl) */

#if (motorD_UseOneCompareMode)
   #if (motorD_CompareMode1SW)
       void    motorD_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motorD_CompareMode1SW) */
#else
    #if (motorD_CompareMode1SW)
        void    motorD_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motorD_CompareMode1SW) */
    #if (motorD_CompareMode2SW)
        void    motorD_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motorD_CompareMode2SW) */
#endif /* (motorD_UseOneCompareMode) */

#if (!motorD_UsingFixedFunction)
    uint8   motorD_ReadCounter(void) ;
    uint8 motorD_ReadCapture(void) ;

    #if (motorD_UseStatus)
            void motorD_ClearFIFO(void) ;
    #endif /* (motorD_UseStatus) */

    void    motorD_WriteCounter(uint8 counter)
            ;
#endif /* (!motorD_UsingFixedFunction) */

void    motorD_WritePeriod(uint8 period)
        ;
uint8 motorD_ReadPeriod(void) ;

#if (motorD_UseOneCompareMode)
    void    motorD_WriteCompare(uint8 compare)
            ;
    uint8 motorD_ReadCompare(void) ;
#else
    void    motorD_WriteCompare1(uint8 compare)
            ;
    uint8 motorD_ReadCompare1(void) ;
    void    motorD_WriteCompare2(uint8 compare)
            ;
    uint8 motorD_ReadCompare2(void) ;
#endif /* (motorD_UseOneCompareMode) */


#if (motorD_DeadBandUsed)
    void    motorD_WriteDeadTime(uint8 deadtime) ;
    uint8   motorD_ReadDeadTime(void) ;
#endif /* (motorD_DeadBandUsed) */

#if ( motorD_KillModeMinTime)
    void motorD_WriteKillTime(uint8 killtime) ;
    uint8 motorD_ReadKillTime(void) ;
#endif /* ( motorD_KillModeMinTime) */

void motorD_Init(void) ;
void motorD_Enable(void) ;
void motorD_Sleep(void) ;
void motorD_Wakeup(void) ;
void motorD_SaveConfig(void) ;
void motorD_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motorD_INIT_PERIOD_VALUE          (255u)
#define motorD_INIT_COMPARE_VALUE1        (50u)
#define motorD_INIT_COMPARE_VALUE2        (63u)
#define motorD_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motorD_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorD_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorD_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motorD_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motorD_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motorD_CTRL_CMPMODE2_SHIFT)
#define motorD_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motorD_CTRL_CMPMODE1_SHIFT)
#define motorD_INIT_DEAD_TIME             (4u)


/********************************
*         Registers
******************************** */

#if (motorD_UsingFixedFunction)
   #define motorD_PERIOD_LSB              (*(reg16 *) motorD_PWMHW__PER0)
   #define motorD_PERIOD_LSB_PTR          ( (reg16 *) motorD_PWMHW__PER0)
   #define motorD_COMPARE1_LSB            (*(reg16 *) motorD_PWMHW__CNT_CMP0)
   #define motorD_COMPARE1_LSB_PTR        ( (reg16 *) motorD_PWMHW__CNT_CMP0)
   #define motorD_COMPARE2_LSB            (0x00u)
   #define motorD_COMPARE2_LSB_PTR        (0x00u)
   #define motorD_COUNTER_LSB             (*(reg16 *) motorD_PWMHW__CNT_CMP0)
   #define motorD_COUNTER_LSB_PTR         ( (reg16 *) motorD_PWMHW__CNT_CMP0)
   #define motorD_CAPTURE_LSB             (*(reg16 *) motorD_PWMHW__CAP0)
   #define motorD_CAPTURE_LSB_PTR         ( (reg16 *) motorD_PWMHW__CAP0)
   #define motorD_RT1                     (*(reg8 *)  motorD_PWMHW__RT1)
   #define motorD_RT1_PTR                 ( (reg8 *)  motorD_PWMHW__RT1)

#else
   #if (motorD_Resolution == 8u) /* 8bit - PWM */

       #if(motorD_PWMModeIsCenterAligned)
           #define motorD_PERIOD_LSB      (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motorD_PERIOD_LSB_PTR  ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motorD_PERIOD_LSB      (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motorD_PERIOD_LSB_PTR  ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motorD_PWMModeIsCenterAligned) */

       #define motorD_COMPARE1_LSB        (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorD_COMPARE1_LSB_PTR    ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motorD_COMPARE2_LSB        (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorD_COMPARE2_LSB_PTR    ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motorD_COUNTERCAP_LSB      (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorD_COUNTERCAP_LSB_PTR  ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motorD_COUNTER_LSB         (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorD_COUNTER_LSB_PTR     ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motorD_CAPTURE_LSB         (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motorD_CAPTURE_LSB_PTR     ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motorD_PWMModeIsCenterAligned)
               #define motorD_PERIOD_LSB      (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motorD_PERIOD_LSB_PTR  ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motorD_PERIOD_LSB      (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motorD_PERIOD_LSB_PTR  ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motorD_PWMModeIsCenterAligned) */

            #define motorD_COMPARE1_LSB       (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorD_COMPARE1_LSB_PTR   ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motorD_COMPARE2_LSB       (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorD_COMPARE2_LSB_PTR   ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motorD_COUNTERCAP_LSB     (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorD_COUNTERCAP_LSB_PTR ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motorD_COUNTER_LSB        (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorD_COUNTER_LSB_PTR    ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motorD_CAPTURE_LSB        (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motorD_CAPTURE_LSB_PTR    ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motorD_PWMModeIsCenterAligned)
               #define motorD_PERIOD_LSB      (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motorD_PERIOD_LSB_PTR  ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motorD_PERIOD_LSB      (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motorD_PERIOD_LSB_PTR  ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motorD_PWMModeIsCenterAligned) */

            #define motorD_COMPARE1_LSB       (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorD_COMPARE1_LSB_PTR   ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motorD_COMPARE2_LSB       (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorD_COMPARE2_LSB_PTR   ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motorD_COUNTERCAP_LSB     (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorD_COUNTERCAP_LSB_PTR ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motorD_COUNTER_LSB        (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorD_COUNTER_LSB_PTR    ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motorD_CAPTURE_LSB        (*(reg16 *) motorD_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motorD_CAPTURE_LSB_PTR    ((reg16 *)  motorD_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motorD_AUX_CONTROLDP1          (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motorD_AUX_CONTROLDP1_PTR      ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motorD_Resolution == 8) */

   #define motorD_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motorD_AUX_CONTROLDP0          (*(reg8 *)  motorD_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motorD_AUX_CONTROLDP0_PTR      ((reg8 *)   motorD_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motorD_UsingFixedFunction) */

#if(motorD_KillModeMinTime )
    #define motorD_KILLMODEMINTIME        (*(reg8 *)  motorD_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motorD_KILLMODEMINTIME_PTR    ((reg8 *)   motorD_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motorD_KillModeMinTime ) */

#if(motorD_DeadBandMode == motorD__B_PWM__DBM_256_CLOCKS)
    #define motorD_DEADBAND_COUNT         (*(reg8 *)  motorD_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorD_DEADBAND_COUNT_PTR     ((reg8 *)   motorD_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motorD_DEADBAND_LSB_PTR       ((reg8 *)   motorD_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motorD_DEADBAND_LSB           (*(reg8 *)  motorD_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motorD_DeadBandMode == motorD__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motorD_UsingFixedFunction)
        #define motorD_DEADBAND_COUNT         (*(reg8 *)  motorD_PWMHW__CFG0)
        #define motorD_DEADBAND_COUNT_PTR     ((reg8 *)   motorD_PWMHW__CFG0)
        #define motorD_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorD_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motorD_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motorD_DEADBAND_COUNT         (*(reg8 *)  motorD_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorD_DEADBAND_COUNT_PTR     ((reg8 *)   motorD_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motorD_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motorD_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motorD_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motorD_UsingFixedFunction) */
#endif /* (motorD_DeadBandMode == motorD__B_PWM__DBM_256_CLOCKS) */



#if (motorD_UsingFixedFunction)
    #define motorD_STATUS                 (*(reg8 *) motorD_PWMHW__SR0)
    #define motorD_STATUS_PTR             ((reg8 *) motorD_PWMHW__SR0)
    #define motorD_STATUS_MASK            (*(reg8 *) motorD_PWMHW__SR0)
    #define motorD_STATUS_MASK_PTR        ((reg8 *) motorD_PWMHW__SR0)
    #define motorD_CONTROL                (*(reg8 *) motorD_PWMHW__CFG0)
    #define motorD_CONTROL_PTR            ((reg8 *) motorD_PWMHW__CFG0)
    #define motorD_CONTROL2               (*(reg8 *) motorD_PWMHW__CFG1)
    #define motorD_CONTROL3               (*(reg8 *) motorD_PWMHW__CFG2)
    #define motorD_GLOBAL_ENABLE          (*(reg8 *) motorD_PWMHW__PM_ACT_CFG)
    #define motorD_GLOBAL_ENABLE_PTR      ( (reg8 *) motorD_PWMHW__PM_ACT_CFG)
    #define motorD_GLOBAL_STBY_ENABLE     (*(reg8 *) motorD_PWMHW__PM_STBY_CFG)
    #define motorD_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motorD_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motorD_BLOCK_EN_MASK          (motorD_PWMHW__PM_ACT_MSK)
    #define motorD_BLOCK_STBY_EN_MASK     (motorD_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motorD_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motorD_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motorD_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motorD_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motorD_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motorD_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motorD_CTRL_ENABLE            (uint8)((uint8)0x01u << motorD_CTRL_ENABLE_SHIFT)
    #define motorD_CTRL_RESET             (uint8)((uint8)0x01u << motorD_CTRL_RESET_SHIFT)
    #define motorD_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorD_CTRL_CMPMODE2_SHIFT)
    #define motorD_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorD_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motorD_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motorD_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motorD_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motorD_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motorD_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motorD_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motorD_STATUS_TC_INT_EN_MASK_SHIFT            (motorD_STATUS_TC_SHIFT - 4u)
    #define motorD_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motorD_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorD_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motorD_STATUS_TC              (uint8)((uint8)0x01u << motorD_STATUS_TC_SHIFT)
    #define motorD_STATUS_CMP1            (uint8)((uint8)0x01u << motorD_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motorD_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motorD_STATUS_TC >> 4u)
    #define motorD_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motorD_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motorD_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motorD_RT1_MASK              (uint8)((uint8)0x03u << motorD_RT1_SHIFT)
    #define motorD_SYNC                  (uint8)((uint8)0x03u << motorD_RT1_SHIFT)
    #define motorD_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motorD_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motorD_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motorD_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motorD_SYNCDSI_SHIFT)


#else
    #define motorD_STATUS                (*(reg8 *)   motorD_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorD_STATUS_PTR            ((reg8 *)    motorD_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motorD_STATUS_MASK           (*(reg8 *)   motorD_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorD_STATUS_MASK_PTR       ((reg8 *)    motorD_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motorD_STATUS_AUX_CTRL       (*(reg8 *)   motorD_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motorD_CONTROL               (*(reg8 *)   motorD_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motorD_CONTROL_PTR           ((reg8 *)    motorD_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motorD_CTRL_ENABLE_SHIFT      (0x07u)
    #define motorD_CTRL_RESET_SHIFT       (0x06u)
    #define motorD_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motorD_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motorD_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motorD_CTRL_ENABLE            (uint8)((uint8)0x01u << motorD_CTRL_ENABLE_SHIFT)
    #define motorD_CTRL_RESET             (uint8)((uint8)0x01u << motorD_CTRL_RESET_SHIFT)
    #define motorD_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motorD_CTRL_CMPMODE2_SHIFT)
    #define motorD_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motorD_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motorD_STATUS_KILL_SHIFT          (0x05u)
    #define motorD_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motorD_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motorD_STATUS_TC_SHIFT            (0x02u)
    #define motorD_STATUS_CMP2_SHIFT          (0x01u)
    #define motorD_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motorD_STATUS_KILL_INT_EN_MASK_SHIFT          (motorD_STATUS_KILL_SHIFT)
    #define motorD_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motorD_STATUS_FIFONEMPTY_SHIFT)
    #define motorD_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motorD_STATUS_FIFOFULL_SHIFT)
    #define motorD_STATUS_TC_INT_EN_MASK_SHIFT            (motorD_STATUS_TC_SHIFT)
    #define motorD_STATUS_CMP2_INT_EN_MASK_SHIFT          (motorD_STATUS_CMP2_SHIFT)
    #define motorD_STATUS_CMP1_INT_EN_MASK_SHIFT          (motorD_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motorD_STATUS_KILL            (uint8)((uint8)0x00u << motorD_STATUS_KILL_SHIFT )
    #define motorD_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motorD_STATUS_FIFOFULL_SHIFT)
    #define motorD_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motorD_STATUS_FIFONEMPTY_SHIFT)
    #define motorD_STATUS_TC              (uint8)((uint8)0x01u << motorD_STATUS_TC_SHIFT)
    #define motorD_STATUS_CMP2            (uint8)((uint8)0x01u << motorD_STATUS_CMP2_SHIFT)
    #define motorD_STATUS_CMP1            (uint8)((uint8)0x01u << motorD_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motorD_STATUS_KILL_INT_EN_MASK            (motorD_STATUS_KILL)
    #define motorD_STATUS_FIFOFULL_INT_EN_MASK        (motorD_STATUS_FIFOFULL)
    #define motorD_STATUS_FIFONEMPTY_INT_EN_MASK      (motorD_STATUS_FIFONEMPTY)
    #define motorD_STATUS_TC_INT_EN_MASK              (motorD_STATUS_TC)
    #define motorD_STATUS_CMP2_INT_EN_MASK            (motorD_STATUS_CMP2)
    #define motorD_STATUS_CMP1_INT_EN_MASK            (motorD_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motorD_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motorD_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motorD_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motorD_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motorD_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motorD_UsingFixedFunction */

#endif  /* CY_PWM_motorD_H */


/* [] END OF FILE */
