/*******************************************************************************
* File Name: motorC_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "motorC.h"

static motorC_backupStruct motorC_backup;


/*******************************************************************************
* Function Name: motorC_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  motorC_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorC_SaveConfig(void) 
{

    #if(!motorC_UsingFixedFunction)
        #if(!motorC_PWMModeIsCenterAligned)
            motorC_backup.PWMPeriod = motorC_ReadPeriod();
        #endif /* (!motorC_PWMModeIsCenterAligned) */
        motorC_backup.PWMUdb = motorC_ReadCounter();
        #if (motorC_UseStatus)
            motorC_backup.InterruptMaskValue = motorC_STATUS_MASK;
        #endif /* (motorC_UseStatus) */

        #if(motorC_DeadBandMode == motorC__B_PWM__DBM_256_CLOCKS || \
            motorC_DeadBandMode == motorC__B_PWM__DBM_2_4_CLOCKS)
            motorC_backup.PWMdeadBandValue = motorC_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorC_KillModeMinTime)
             motorC_backup.PWMKillCounterPeriod = motorC_ReadKillTime();
        #endif /* (motorC_KillModeMinTime) */

        #if(motorC_UseControl)
            motorC_backup.PWMControlRegister = motorC_ReadControlRegister();
        #endif /* (motorC_UseControl) */
    #endif  /* (!motorC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  motorC_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorC_RestoreConfig(void) 
{
        #if(!motorC_UsingFixedFunction)
            #if(!motorC_PWMModeIsCenterAligned)
                motorC_WritePeriod(motorC_backup.PWMPeriod);
            #endif /* (!motorC_PWMModeIsCenterAligned) */

            motorC_WriteCounter(motorC_backup.PWMUdb);

            #if (motorC_UseStatus)
                motorC_STATUS_MASK = motorC_backup.InterruptMaskValue;
            #endif /* (motorC_UseStatus) */

            #if(motorC_DeadBandMode == motorC__B_PWM__DBM_256_CLOCKS || \
                motorC_DeadBandMode == motorC__B_PWM__DBM_2_4_CLOCKS)
                motorC_WriteDeadTime(motorC_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorC_KillModeMinTime)
                motorC_WriteKillTime(motorC_backup.PWMKillCounterPeriod);
            #endif /* (motorC_KillModeMinTime) */

            #if(motorC_UseControl)
                motorC_WriteControlRegister(motorC_backup.PWMControlRegister);
            #endif /* (motorC_UseControl) */
        #endif  /* (!motorC_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorC_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  motorC_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorC_Sleep(void) 
{
    #if(motorC_UseControl)
        if(motorC_CTRL_ENABLE == (motorC_CONTROL & motorC_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorC_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorC_backup.PWMEnableState = 0u;
        }
    #endif /* (motorC_UseControl) */

    /* Stop component */
    motorC_Stop();

    /* Save registers configuration */
    motorC_SaveConfig();
}


/*******************************************************************************
* Function Name: motorC_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  motorC_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorC_Wakeup(void) 
{
     /* Restore registers values */
    motorC_RestoreConfig();

    if(motorC_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorC_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
