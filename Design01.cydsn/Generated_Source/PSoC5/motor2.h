/*******************************************************************************
* File Name: motor2.h
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

#if !defined(CY_PWM_motor2_H)
#define CY_PWM_motor2_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motor2_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motor2_Resolution                     (8u)
#define motor2_UsingFixedFunction             (0u)
#define motor2_DeadBandMode                   (2u)
#define motor2_KillModeMinTime                (0u)
#define motor2_KillMode                       (0u)
#define motor2_PWMMode                        (0u)
#define motor2_PWMModeIsCenterAligned         (0u)
#define motor2_DeadBandUsed                   (1u)
#define motor2_DeadBand2_4                    (0u)

#if !defined(motor2_PWMUDB_genblk8_stsreg__REMOVED)
    #define motor2_UseStatus                  (1u)
#else
    #define motor2_UseStatus                  (0u)
#endif /* !defined(motor2_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motor2_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motor2_UseControl                 (1u)
#else
    #define motor2_UseControl                 (0u)
#endif /* !defined(motor2_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motor2_UseOneCompareMode              (1u)
#define motor2_MinimumKillTime                (1u)
#define motor2_EnableMode                     (0u)

#define motor2_CompareMode1SW                 (0u)
#define motor2_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motor2__B_PWM__DISABLED 0
#define motor2__B_PWM__ASYNCHRONOUS 1
#define motor2__B_PWM__SINGLECYCLE 2
#define motor2__B_PWM__LATCHED 3
#define motor2__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motor2__B_PWM__DBMDISABLED 0
#define motor2__B_PWM__DBM_2_4_CLOCKS 1
#define motor2__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motor2__B_PWM__ONE_OUTPUT 0
#define motor2__B_PWM__TWO_OUTPUTS 1
#define motor2__B_PWM__DUAL_EDGE 2
#define motor2__B_PWM__CENTER_ALIGN 3
#define motor2__B_PWM__DITHER 5
#define motor2__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motor2__B_PWM__LESS_THAN 1
#define motor2__B_PWM__LESS_THAN_OR_EQUAL 2
#define motor2__B_PWM__GREATER_THAN 3
#define motor2__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motor2__B_PWM__EQUAL 0
#define motor2__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motor2_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!motor2_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!motor2_PWMModeIsCenterAligned) */
        #if (motor2_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motor2_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motor2_DeadBandMode == motor2__B_PWM__DBM_256_CLOCKS || \
            motor2_DeadBandMode == motor2__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motor2_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motor2_KillModeMinTime) */

        /* Backup control register */
        #if(motor2_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motor2_UseControl) */

    #endif /* (!motor2_UsingFixedFunction) */

}motor2_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motor2_Start(void) ;
void    motor2_Stop(void) ;

#if (motor2_UseStatus || motor2_UsingFixedFunction)
    void  motor2_SetInterruptMode(uint8 interruptMode) ;
    uint8 motor2_ReadStatusRegister(void) ;
#endif /* (motor2_UseStatus || motor2_UsingFixedFunction) */

#define motor2_GetInterruptSource() motor2_ReadStatusRegister()

#if (motor2_UseControl)
    uint8 motor2_ReadControlRegister(void) ;
    void  motor2_WriteControlRegister(uint8 control)
          ;
#endif /* (motor2_UseControl) */

#if (motor2_UseOneCompareMode)
   #if (motor2_CompareMode1SW)
       void    motor2_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motor2_CompareMode1SW) */
#else
    #if (motor2_CompareMode1SW)
        void    motor2_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motor2_CompareMode1SW) */
    #if (motor2_CompareMode2SW)
        void    motor2_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motor2_CompareMode2SW) */
#endif /* (motor2_UseOneCompareMode) */

#if (!motor2_UsingFixedFunction)
    uint8   motor2_ReadCounter(void) ;
    uint8 motor2_ReadCapture(void) ;

    #if (motor2_UseStatus)
            void motor2_ClearFIFO(void) ;
    #endif /* (motor2_UseStatus) */

    void    motor2_WriteCounter(uint8 counter)
            ;
#endif /* (!motor2_UsingFixedFunction) */

void    motor2_WritePeriod(uint8 period)
        ;
uint8 motor2_ReadPeriod(void) ;

#if (motor2_UseOneCompareMode)
    void    motor2_WriteCompare(uint8 compare)
            ;
    uint8 motor2_ReadCompare(void) ;
#else
    void    motor2_WriteCompare1(uint8 compare)
            ;
    uint8 motor2_ReadCompare1(void) ;
    void    motor2_WriteCompare2(uint8 compare)
            ;
    uint8 motor2_ReadCompare2(void) ;
#endif /* (motor2_UseOneCompareMode) */


#if (motor2_DeadBandUsed)
    void    motor2_WriteDeadTime(uint8 deadtime) ;
    uint8   motor2_ReadDeadTime(void) ;
#endif /* (motor2_DeadBandUsed) */

#if ( motor2_KillModeMinTime)
    void motor2_WriteKillTime(uint8 killtime) ;
    uint8 motor2_ReadKillTime(void) ;
#endif /* ( motor2_KillModeMinTime) */

void motor2_Init(void) ;
void motor2_Enable(void) ;
void motor2_Sleep(void) ;
void motor2_Wakeup(void) ;
void motor2_SaveConfig(void) ;
void motor2_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motor2_INIT_PERIOD_VALUE          (255u)
#define motor2_INIT_COMPARE_VALUE1        (50u)
#define motor2_INIT_COMPARE_VALUE2        (63u)
#define motor2_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motor2_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor2_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor2_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor2_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motor2_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motor2_CTRL_CMPMODE2_SHIFT)
#define motor2_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motor2_CTRL_CMPMODE1_SHIFT)
#define motor2_INIT_DEAD_TIME             (4u)


/********************************
*         Registers
******************************** */

#if (motor2_UsingFixedFunction)
   #define motor2_PERIOD_LSB              (*(reg16 *) motor2_PWMHW__PER0)
   #define motor2_PERIOD_LSB_PTR          ( (reg16 *) motor2_PWMHW__PER0)
   #define motor2_COMPARE1_LSB            (*(reg16 *) motor2_PWMHW__CNT_CMP0)
   #define motor2_COMPARE1_LSB_PTR        ( (reg16 *) motor2_PWMHW__CNT_CMP0)
   #define motor2_COMPARE2_LSB            (0x00u)
   #define motor2_COMPARE2_LSB_PTR        (0x00u)
   #define motor2_COUNTER_LSB             (*(reg16 *) motor2_PWMHW__CNT_CMP0)
   #define motor2_COUNTER_LSB_PTR         ( (reg16 *) motor2_PWMHW__CNT_CMP0)
   #define motor2_CAPTURE_LSB             (*(reg16 *) motor2_PWMHW__CAP0)
   #define motor2_CAPTURE_LSB_PTR         ( (reg16 *) motor2_PWMHW__CAP0)
   #define motor2_RT1                     (*(reg8 *)  motor2_PWMHW__RT1)
   #define motor2_RT1_PTR                 ( (reg8 *)  motor2_PWMHW__RT1)

#else
   #if (motor2_Resolution == 8u) /* 8bit - PWM */

       #if(motor2_PWMModeIsCenterAligned)
           #define motor2_PERIOD_LSB      (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define motor2_PERIOD_LSB_PTR  ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define motor2_PERIOD_LSB      (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define motor2_PERIOD_LSB_PTR  ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (motor2_PWMModeIsCenterAligned) */

       #define motor2_COMPARE1_LSB        (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motor2_COMPARE1_LSB_PTR    ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define motor2_COMPARE2_LSB        (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motor2_COMPARE2_LSB_PTR    ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define motor2_COUNTERCAP_LSB      (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motor2_COUNTERCAP_LSB_PTR  ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define motor2_COUNTER_LSB         (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motor2_COUNTER_LSB_PTR     ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define motor2_CAPTURE_LSB         (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define motor2_CAPTURE_LSB_PTR     ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motor2_PWMModeIsCenterAligned)
               #define motor2_PERIOD_LSB      (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define motor2_PERIOD_LSB_PTR  ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define motor2_PERIOD_LSB      (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define motor2_PERIOD_LSB_PTR  ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (motor2_PWMModeIsCenterAligned) */

            #define motor2_COMPARE1_LSB       (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motor2_COMPARE1_LSB_PTR   ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define motor2_COMPARE2_LSB       (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motor2_COMPARE2_LSB_PTR   ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define motor2_COUNTERCAP_LSB     (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motor2_COUNTERCAP_LSB_PTR ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define motor2_COUNTER_LSB        (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motor2_COUNTER_LSB_PTR    ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define motor2_CAPTURE_LSB        (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define motor2_CAPTURE_LSB_PTR    ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(motor2_PWMModeIsCenterAligned)
               #define motor2_PERIOD_LSB      (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define motor2_PERIOD_LSB_PTR  ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motor2_PERIOD_LSB      (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define motor2_PERIOD_LSB_PTR  ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motor2_PWMModeIsCenterAligned) */

            #define motor2_COMPARE1_LSB       (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motor2_COMPARE1_LSB_PTR   ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define motor2_COMPARE2_LSB       (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motor2_COMPARE2_LSB_PTR   ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define motor2_COUNTERCAP_LSB     (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motor2_COUNTERCAP_LSB_PTR ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define motor2_COUNTER_LSB        (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motor2_COUNTER_LSB_PTR    ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define motor2_CAPTURE_LSB        (*(reg16 *) motor2_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define motor2_CAPTURE_LSB_PTR    ((reg16 *)  motor2_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motor2_AUX_CONTROLDP1          (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define motor2_AUX_CONTROLDP1_PTR      ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motor2_Resolution == 8) */

   #define motor2_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define motor2_AUX_CONTROLDP0          (*(reg8 *)  motor2_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define motor2_AUX_CONTROLDP0_PTR      ((reg8 *)   motor2_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motor2_UsingFixedFunction) */

#if(motor2_KillModeMinTime )
    #define motor2_KILLMODEMINTIME        (*(reg8 *)  motor2_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motor2_KILLMODEMINTIME_PTR    ((reg8 *)   motor2_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motor2_KillModeMinTime ) */

#if(motor2_DeadBandMode == motor2__B_PWM__DBM_256_CLOCKS)
    #define motor2_DEADBAND_COUNT         (*(reg8 *)  motor2_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor2_DEADBAND_COUNT_PTR     ((reg8 *)   motor2_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor2_DEADBAND_LSB_PTR       ((reg8 *)   motor2_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motor2_DEADBAND_LSB           (*(reg8 *)  motor2_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motor2_DeadBandMode == motor2__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motor2_UsingFixedFunction)
        #define motor2_DEADBAND_COUNT         (*(reg8 *)  motor2_PWMHW__CFG0)
        #define motor2_DEADBAND_COUNT_PTR     ((reg8 *)   motor2_PWMHW__CFG0)
        #define motor2_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor2_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motor2_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motor2_DEADBAND_COUNT         (*(reg8 *)  motor2_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor2_DEADBAND_COUNT_PTR     ((reg8 *)   motor2_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor2_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor2_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motor2_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motor2_UsingFixedFunction) */
#endif /* (motor2_DeadBandMode == motor2__B_PWM__DBM_256_CLOCKS) */



#if (motor2_UsingFixedFunction)
    #define motor2_STATUS                 (*(reg8 *) motor2_PWMHW__SR0)
    #define motor2_STATUS_PTR             ((reg8 *) motor2_PWMHW__SR0)
    #define motor2_STATUS_MASK            (*(reg8 *) motor2_PWMHW__SR0)
    #define motor2_STATUS_MASK_PTR        ((reg8 *) motor2_PWMHW__SR0)
    #define motor2_CONTROL                (*(reg8 *) motor2_PWMHW__CFG0)
    #define motor2_CONTROL_PTR            ((reg8 *) motor2_PWMHW__CFG0)
    #define motor2_CONTROL2               (*(reg8 *) motor2_PWMHW__CFG1)
    #define motor2_CONTROL3               (*(reg8 *) motor2_PWMHW__CFG2)
    #define motor2_GLOBAL_ENABLE          (*(reg8 *) motor2_PWMHW__PM_ACT_CFG)
    #define motor2_GLOBAL_ENABLE_PTR      ( (reg8 *) motor2_PWMHW__PM_ACT_CFG)
    #define motor2_GLOBAL_STBY_ENABLE     (*(reg8 *) motor2_PWMHW__PM_STBY_CFG)
    #define motor2_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motor2_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motor2_BLOCK_EN_MASK          (motor2_PWMHW__PM_ACT_MSK)
    #define motor2_BLOCK_STBY_EN_MASK     (motor2_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motor2_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motor2_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motor2_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motor2_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motor2_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motor2_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motor2_CTRL_ENABLE            (uint8)((uint8)0x01u << motor2_CTRL_ENABLE_SHIFT)
    #define motor2_CTRL_RESET             (uint8)((uint8)0x01u << motor2_CTRL_RESET_SHIFT)
    #define motor2_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor2_CTRL_CMPMODE2_SHIFT)
    #define motor2_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor2_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motor2_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motor2_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motor2_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motor2_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motor2_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motor2_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motor2_STATUS_TC_INT_EN_MASK_SHIFT            (motor2_STATUS_TC_SHIFT - 4u)
    #define motor2_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motor2_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor2_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motor2_STATUS_TC              (uint8)((uint8)0x01u << motor2_STATUS_TC_SHIFT)
    #define motor2_STATUS_CMP1            (uint8)((uint8)0x01u << motor2_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motor2_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motor2_STATUS_TC >> 4u)
    #define motor2_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motor2_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motor2_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motor2_RT1_MASK              (uint8)((uint8)0x03u << motor2_RT1_SHIFT)
    #define motor2_SYNC                  (uint8)((uint8)0x03u << motor2_RT1_SHIFT)
    #define motor2_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motor2_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motor2_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motor2_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motor2_SYNCDSI_SHIFT)


#else
    #define motor2_STATUS                (*(reg8 *)   motor2_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor2_STATUS_PTR            ((reg8 *)    motor2_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor2_STATUS_MASK           (*(reg8 *)   motor2_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor2_STATUS_MASK_PTR       ((reg8 *)    motor2_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor2_STATUS_AUX_CTRL       (*(reg8 *)   motor2_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motor2_CONTROL               (*(reg8 *)   motor2_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motor2_CONTROL_PTR           ((reg8 *)    motor2_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motor2_CTRL_ENABLE_SHIFT      (0x07u)
    #define motor2_CTRL_RESET_SHIFT       (0x06u)
    #define motor2_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motor2_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motor2_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motor2_CTRL_ENABLE            (uint8)((uint8)0x01u << motor2_CTRL_ENABLE_SHIFT)
    #define motor2_CTRL_RESET             (uint8)((uint8)0x01u << motor2_CTRL_RESET_SHIFT)
    #define motor2_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor2_CTRL_CMPMODE2_SHIFT)
    #define motor2_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor2_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motor2_STATUS_KILL_SHIFT          (0x05u)
    #define motor2_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motor2_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motor2_STATUS_TC_SHIFT            (0x02u)
    #define motor2_STATUS_CMP2_SHIFT          (0x01u)
    #define motor2_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motor2_STATUS_KILL_INT_EN_MASK_SHIFT          (motor2_STATUS_KILL_SHIFT)
    #define motor2_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motor2_STATUS_FIFONEMPTY_SHIFT)
    #define motor2_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motor2_STATUS_FIFOFULL_SHIFT)
    #define motor2_STATUS_TC_INT_EN_MASK_SHIFT            (motor2_STATUS_TC_SHIFT)
    #define motor2_STATUS_CMP2_INT_EN_MASK_SHIFT          (motor2_STATUS_CMP2_SHIFT)
    #define motor2_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor2_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motor2_STATUS_KILL            (uint8)((uint8)0x00u << motor2_STATUS_KILL_SHIFT )
    #define motor2_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motor2_STATUS_FIFOFULL_SHIFT)
    #define motor2_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motor2_STATUS_FIFONEMPTY_SHIFT)
    #define motor2_STATUS_TC              (uint8)((uint8)0x01u << motor2_STATUS_TC_SHIFT)
    #define motor2_STATUS_CMP2            (uint8)((uint8)0x01u << motor2_STATUS_CMP2_SHIFT)
    #define motor2_STATUS_CMP1            (uint8)((uint8)0x01u << motor2_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motor2_STATUS_KILL_INT_EN_MASK            (motor2_STATUS_KILL)
    #define motor2_STATUS_FIFOFULL_INT_EN_MASK        (motor2_STATUS_FIFOFULL)
    #define motor2_STATUS_FIFONEMPTY_INT_EN_MASK      (motor2_STATUS_FIFONEMPTY)
    #define motor2_STATUS_TC_INT_EN_MASK              (motor2_STATUS_TC)
    #define motor2_STATUS_CMP2_INT_EN_MASK            (motor2_STATUS_CMP2)
    #define motor2_STATUS_CMP1_INT_EN_MASK            (motor2_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motor2_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motor2_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motor2_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motor2_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motor2_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motor2_UsingFixedFunction */

#endif  /* CY_PWM_motor2_H */


/* [] END OF FILE */
