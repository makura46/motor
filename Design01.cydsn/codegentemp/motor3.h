/*******************************************************************************
* File Name: motor3.h
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

#if !defined(CY_PWM_motor3_H)
#define CY_PWM_motor3_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motor3_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motor3_Resolution                     (8u)
#define motor3_UsingFixedFunction             (0u)
#define motor3_DeadBandMode                   (2u)
#define motor3_KillModeMinTime                (0u)
#define motor3_KillMode                       (0u)
#define motor3_PWMMode                        (0u)
#define motor3_PWMModeIsCenterAligned         (0u)
#define motor3_DeadBandUsed                   (1u)
#define motor3_DeadBand2_4                    (0u)

#if !defined(motor3_PWMUDB_genblk8_stsreg__REMOVED)
    #define motor3_UseStatus                  (1u)
#else
    #define motor3_UseStatus                  (0u)
#endif /* !defined(motor3_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motor3_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motor3_UseControl                 (1u)
#else
    #define motor3_UseControl                 (0u)
#endif /* !defined(motor3_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motor3_UseOneCompareMode              (1u)
#define motor3_MinimumKillTime                (1u)
#define motor3_EnableMode                     (0u)

#define motor3_CompareMode1SW                 (0u)
#define motor3_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motor3__B_PWM__DISABLED 0
#define motor3__B_PWM__ASYNCHRONOUS 1
#define motor3__B_PWM__SINGLECYCLE 2
#define motor3__B_PWM__LATCHED 3
#define motor3__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motor3__B_PWM__DBMDISABLED 0
#define motor3__B_PWM__DBM_2_4_CLOCKS 1
#define motor3__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motor3__B_PWM__ONE_OUTPUT 0
#define motor3__B_PWM__TWO_OUTPUTS 1
#define motor3__B_PWM__DUAL_EDGE 2
#define motor3__B_PWM__CENTER_ALIGN 3
#define motor3__B_PWM__DITHER 5
#define motor3__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motor3__B_PWM__LESS_THAN 1
#define motor3__B_PWM__LESS_THAN_OR_EQUAL 2
#define motor3__B_PWM__GREATER_THAN 3
#define motor3__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motor3__B_PWM__EQUAL 0
#define motor3__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motor3_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motor3_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motor3_PWMModeIsCenterAligned) */
        #if (motor3_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motor3_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motor3_DeadBandMode == motor3__B_PWM__DBM_256_CLOCKS || \
            motor3_DeadBandMode == motor3__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motor3_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motor3_KillModeMinTime) */

        /* Backup control register */
        #if(motor3_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motor3_UseControl) */

    #endif /* (!motor3_UsingFixedFunction) */

}motor3_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motor3_Start(void) ;
void    motor3_Stop(void) ;

#if (motor3_UseStatus || motor3_UsingFixedFunction)
    void  motor3_SetInterruptMode(uint8 interruptMode) ;
    uint8 motor3_ReadStatusRegister(void) ;
#endif /* (motor3_UseStatus || motor3_UsingFixedFunction) */

#define motor3_GetInterruptSource() motor3_ReadStatusRegister()

#if (motor3_UseControl)
    uint8 motor3_ReadControlRegister(void) ;
    void  motor3_WriteControlRegister(uint8 control)
          ;
#endif /* (motor3_UseControl) */

#if (motor3_UseOneCompareMode)
   #if (motor3_CompareMode1SW)
       void    motor3_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motor3_CompareMode1SW) */
#else
    #if (motor3_CompareMode1SW)
        void    motor3_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motor3_CompareMode1SW) */
    #if (motor3_CompareMode2SW)
        void    motor3_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motor3_CompareMode2SW) */
#endif /* (motor3_UseOneCompareMode) */

#if (!motor3_UsingFixedFunction)
    uint8   motor3_ReadCounter(void) ;
    uint8 motor3_ReadCapture(void) ;

    #if (motor3_UseStatus)
            void motor3_ClearFIFO(void) ;
    #endif /* (motor3_UseStatus) */

    void    motor3_WriteCounter(uint8 counter)
            ;
#endif /* (!motor3_UsingFixedFunction) */

void    motor3_WritePeriod(uint8 period)
        ;
uint8 motor3_ReadPeriod(void) ;

#if (motor3_UseOneCompareMode)
    void    motor3_WriteCompare(uint8 compare)
            ;
    uint8 motor3_ReadCompare(void) ;
#else
    void    motor3_WriteCompare1(uint8 compare)
            ;
    uint8 motor3_ReadCompare1(void) ;
    void    motor3_WriteCompare2(uint8 compare)
            ;
    uint8 motor3_ReadCompare2(void) ;
#endif /* (motor3_UseOneCompareMode) */


#if (motor3_DeadBandUsed)
    void    motor3_WriteDeadTime(uint8 deadtime) ;
    uint8   motor3_ReadDeadTime(void) ;
#endif /* (motor3_DeadBandUsed) */

#if ( motor3_KillModeMinTime)
    void motor3_WriteKillTime(uint8 killtime) ;
    uint8 motor3_ReadKillTime(void) ;
#endif /* ( motor3_KillModeMinTime) */

void motor3_Init(void) ;
void motor3_Enable(void) ;
void motor3_Sleep(void) ;
void motor3_Wakeup(void) ;
void motor3_SaveConfig(void) ;
void motor3_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motor3_INIT_PERIOD_VALUE          (255u)
#define motor3_INIT_COMPARE_VALUE1        (50u)
#define motor3_INIT_COMPARE_VALUE2        (63u)
#define motor3_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motor3_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor3_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor3_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor3_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motor3_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motor3_CTRL_CMPMODE2_SHIFT)
#define motor3_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motor3_CTRL_CMPMODE1_SHIFT)
#define motor3_INIT_DEAD_TIME             (4u)


/********************************
*         Registers
******************************** */

#if (motor3_UsingFixedFunction)
   #define motor3_PERIOD_LSB              (*(reg16 *) motor3_PWMHW__PER0)
   #define motor3_PERIOD_LSB_PTR          ( (reg16 *) motor3_PWMHW__PER0)
   #define motor3_COMPARE1_LSB            (*(reg16 *) motor3_PWMHW__CNT_CMP0)
   #define motor3_COMPARE1_LSB_PTR        ( (reg16 *) motor3_PWMHW__CNT_CMP0)
   #define motor3_COMPARE2_LSB            (0x00u)
   #define motor3_COMPARE2_LSB_PTR        (0x00u)
   #define motor3_COUNTER_LSB             (*(reg16 *) motor3_PWMHW__CNT_CMP0)
   #define motor3_COUNTER_LSB_PTR         ( (reg16 *) motor3_PWMHW__CNT_CMP0)
   #define motor3_CAPTURE_LSB             (*(reg16 *) motor3_PWMHW__CAP0)
   #define motor3_CAPTURE_LSB_PTR         ( (reg16 *) motor3_PWMHW__CAP0)
   #define motor3_RT1                     (*(reg8 *)  motor3_PWMHW__RT1)
   #define motor3_RT1_PTR                 ( (reg8 *)  motor3_PWMHW__RT1)

#else
   #if (motor3_Resolution == 8u) /* 8bit - PWM */

       #if(motor3_PWMModeIsCenterAligned)
           #define motor3_PERIOD_LSB      (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motor3_PERIOD_LSB_PTR  ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motor3_PERIOD_LSB      (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motor3_PERIOD_LSB_PTR  ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motor3_PWMModeIsCenterAligned) */

       #define motor3_COMPARE1_LSB        (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motor3_COMPARE1_LSB_PTR    ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motor3_COMPARE2_LSB        (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motor3_COMPARE2_LSB_PTR    ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motor3_COUNTERCAP_LSB      (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motor3_COUNTERCAP_LSB_PTR  ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motor3_COUNTER_LSB         (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motor3_COUNTER_LSB_PTR     ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motor3_CAPTURE_LSB         (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motor3_CAPTURE_LSB_PTR     ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motor3_PWMModeIsCenterAligned)
               #define motor3_PERIOD_LSB      (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motor3_PERIOD_LSB_PTR  ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motor3_PERIOD_LSB      (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motor3_PERIOD_LSB_PTR  ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motor3_PWMModeIsCenterAligned) */

            #define motor3_COMPARE1_LSB       (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motor3_COMPARE1_LSB_PTR   ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motor3_COMPARE2_LSB       (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motor3_COMPARE2_LSB_PTR   ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motor3_COUNTERCAP_LSB     (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motor3_COUNTERCAP_LSB_PTR ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motor3_COUNTER_LSB        (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motor3_COUNTER_LSB_PTR    ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motor3_CAPTURE_LSB        (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motor3_CAPTURE_LSB_PTR    ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motor3_PWMModeIsCenterAligned)
               #define motor3_PERIOD_LSB      (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motor3_PERIOD_LSB_PTR  ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motor3_PERIOD_LSB      (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motor3_PERIOD_LSB_PTR  ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motor3_PWMModeIsCenterAligned) */

            #define motor3_COMPARE1_LSB       (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motor3_COMPARE1_LSB_PTR   ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motor3_COMPARE2_LSB       (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motor3_COMPARE2_LSB_PTR   ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motor3_COUNTERCAP_LSB     (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motor3_COUNTERCAP_LSB_PTR ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motor3_COUNTER_LSB        (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motor3_COUNTER_LSB_PTR    ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motor3_CAPTURE_LSB        (*(reg16 *) motor3_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motor3_CAPTURE_LSB_PTR    ((reg16 *)  motor3_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motor3_AUX_CONTROLDP1          (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motor3_AUX_CONTROLDP1_PTR      ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motor3_Resolution == 8) */

   #define motor3_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motor3_AUX_CONTROLDP0          (*(reg8 *)  motor3_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motor3_AUX_CONTROLDP0_PTR      ((reg8 *)   motor3_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motor3_UsingFixedFunction) */

#if(motor3_KillModeMinTime )
    #define motor3_KILLMODEMINTIME        (*(reg8 *)  motor3_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motor3_KILLMODEMINTIME_PTR    ((reg8 *)   motor3_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motor3_KillModeMinTime ) */

#if(motor3_DeadBandMode == motor3__B_PWM__DBM_256_CLOCKS)
    #define motor3_DEADBAND_COUNT         (*(reg8 *)  motor3_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor3_DEADBAND_COUNT_PTR     ((reg8 *)   motor3_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor3_DEADBAND_LSB_PTR       ((reg8 *)   motor3_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motor3_DEADBAND_LSB           (*(reg8 *)  motor3_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motor3_DeadBandMode == motor3__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motor3_UsingFixedFunction)
        #define motor3_DEADBAND_COUNT         (*(reg8 *)  motor3_PWMHW__CFG0)
        #define motor3_DEADBAND_COUNT_PTR     ((reg8 *)   motor3_PWMHW__CFG0)
        #define motor3_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor3_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motor3_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motor3_DEADBAND_COUNT         (*(reg8 *)  motor3_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor3_DEADBAND_COUNT_PTR     ((reg8 *)   motor3_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor3_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor3_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motor3_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motor3_UsingFixedFunction) */
#endif /* (motor3_DeadBandMode == motor3__B_PWM__DBM_256_CLOCKS) */



#if (motor3_UsingFixedFunction)
    #define motor3_STATUS                 (*(reg8 *) motor3_PWMHW__SR0)
    #define motor3_STATUS_PTR             ((reg8 *) motor3_PWMHW__SR0)
    #define motor3_STATUS_MASK            (*(reg8 *) motor3_PWMHW__SR0)
    #define motor3_STATUS_MASK_PTR        ((reg8 *) motor3_PWMHW__SR0)
    #define motor3_CONTROL                (*(reg8 *) motor3_PWMHW__CFG0)
    #define motor3_CONTROL_PTR            ((reg8 *) motor3_PWMHW__CFG0)
    #define motor3_CONTROL2               (*(reg8 *) motor3_PWMHW__CFG1)
    #define motor3_CONTROL3               (*(reg8 *) motor3_PWMHW__CFG2)
    #define motor3_GLOBAL_ENABLE          (*(reg8 *) motor3_PWMHW__PM_ACT_CFG)
    #define motor3_GLOBAL_ENABLE_PTR      ( (reg8 *) motor3_PWMHW__PM_ACT_CFG)
    #define motor3_GLOBAL_STBY_ENABLE     (*(reg8 *) motor3_PWMHW__PM_STBY_CFG)
    #define motor3_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motor3_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motor3_BLOCK_EN_MASK          (motor3_PWMHW__PM_ACT_MSK)
    #define motor3_BLOCK_STBY_EN_MASK     (motor3_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motor3_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motor3_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motor3_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motor3_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motor3_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motor3_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motor3_CTRL_ENABLE            (uint8)((uint8)0x01u << motor3_CTRL_ENABLE_SHIFT)
    #define motor3_CTRL_RESET             (uint8)((uint8)0x01u << motor3_CTRL_RESET_SHIFT)
    #define motor3_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor3_CTRL_CMPMODE2_SHIFT)
    #define motor3_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor3_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motor3_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motor3_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motor3_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motor3_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motor3_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motor3_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motor3_STATUS_TC_INT_EN_MASK_SHIFT            (motor3_STATUS_TC_SHIFT - 4u)
    #define motor3_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motor3_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor3_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motor3_STATUS_TC              (uint8)((uint8)0x01u << motor3_STATUS_TC_SHIFT)
    #define motor3_STATUS_CMP1            (uint8)((uint8)0x01u << motor3_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motor3_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motor3_STATUS_TC >> 4u)
    #define motor3_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motor3_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motor3_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motor3_RT1_MASK              (uint8)((uint8)0x03u << motor3_RT1_SHIFT)
    #define motor3_SYNC                  (uint8)((uint8)0x03u << motor3_RT1_SHIFT)
    #define motor3_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motor3_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motor3_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motor3_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motor3_SYNCDSI_SHIFT)


#else
    #define motor3_STATUS                (*(reg8 *)   motor3_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor3_STATUS_PTR            ((reg8 *)    motor3_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor3_STATUS_MASK           (*(reg8 *)   motor3_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor3_STATUS_MASK_PTR       ((reg8 *)    motor3_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor3_STATUS_AUX_CTRL       (*(reg8 *)   motor3_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motor3_CONTROL               (*(reg8 *)   motor3_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motor3_CONTROL_PTR           ((reg8 *)    motor3_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motor3_CTRL_ENABLE_SHIFT      (0x07u)
    #define motor3_CTRL_RESET_SHIFT       (0x06u)
    #define motor3_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motor3_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motor3_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motor3_CTRL_ENABLE            (uint8)((uint8)0x01u << motor3_CTRL_ENABLE_SHIFT)
    #define motor3_CTRL_RESET             (uint8)((uint8)0x01u << motor3_CTRL_RESET_SHIFT)
    #define motor3_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor3_CTRL_CMPMODE2_SHIFT)
    #define motor3_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor3_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motor3_STATUS_KILL_SHIFT          (0x05u)
    #define motor3_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motor3_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motor3_STATUS_TC_SHIFT            (0x02u)
    #define motor3_STATUS_CMP2_SHIFT          (0x01u)
    #define motor3_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motor3_STATUS_KILL_INT_EN_MASK_SHIFT          (motor3_STATUS_KILL_SHIFT)
    #define motor3_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motor3_STATUS_FIFONEMPTY_SHIFT)
    #define motor3_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motor3_STATUS_FIFOFULL_SHIFT)
    #define motor3_STATUS_TC_INT_EN_MASK_SHIFT            (motor3_STATUS_TC_SHIFT)
    #define motor3_STATUS_CMP2_INT_EN_MASK_SHIFT          (motor3_STATUS_CMP2_SHIFT)
    #define motor3_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor3_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motor3_STATUS_KILL            (uint8)((uint8)0x00u << motor3_STATUS_KILL_SHIFT )
    #define motor3_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motor3_STATUS_FIFOFULL_SHIFT)
    #define motor3_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motor3_STATUS_FIFONEMPTY_SHIFT)
    #define motor3_STATUS_TC              (uint8)((uint8)0x01u << motor3_STATUS_TC_SHIFT)
    #define motor3_STATUS_CMP2            (uint8)((uint8)0x01u << motor3_STATUS_CMP2_SHIFT)
    #define motor3_STATUS_CMP1            (uint8)((uint8)0x01u << motor3_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motor3_STATUS_KILL_INT_EN_MASK            (motor3_STATUS_KILL)
    #define motor3_STATUS_FIFOFULL_INT_EN_MASK        (motor3_STATUS_FIFOFULL)
    #define motor3_STATUS_FIFONEMPTY_INT_EN_MASK      (motor3_STATUS_FIFONEMPTY)
    #define motor3_STATUS_TC_INT_EN_MASK              (motor3_STATUS_TC)
    #define motor3_STATUS_CMP2_INT_EN_MASK            (motor3_STATUS_CMP2)
    #define motor3_STATUS_CMP1_INT_EN_MASK            (motor3_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motor3_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motor3_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motor3_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motor3_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motor3_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motor3_UsingFixedFunction */

#endif  /* CY_PWM_motor3_H */


/* [] END OF FILE */
