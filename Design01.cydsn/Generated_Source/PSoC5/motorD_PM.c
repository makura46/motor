/*******************************************************************************
* File Name: motorD_PM.c
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

#include "motorD.h"

static motorD_backupStruct motorD_backup;


/*******************************************************************************
* Function Name: motorD_SaveConfig
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
*  motorD_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorD_SaveConfig(void) 
{

    #if(!motorD_UsingFixedFunction)
        #if(!motorD_PWMModeIsCenterAligned)
            motorD_backup.PWMPeriod = motorD_ReadPeriod();
        #endif /* (!motorD_PWMModeIsCenterAligned) */
        motorD_backup.PWMUdb = motorD_ReadCounter();
        #if (motorD_UseStatus)
            motorD_backup.InterruptMaskValue = motorD_STATUS_MASK;
        #endif /* (motorD_UseStatus) */

        #if(motorD_DeadBandMode == motorD__B_PWM__DBM_256_CLOCKS || \
            motorD_DeadBandMode == motorD__B_PWM__DBM_2_4_CLOCKS)
            motorD_backup.PWMdeadBandValue = motorD_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorD_KillModeMinTime)
             motorD_backup.PWMKillCounterPeriod = motorD_ReadKillTime();
        #endif /* (motorD_KillModeMinTime) */

        #if(motorD_UseControl)
            motorD_backup.PWMControlRegister = motorD_ReadControlRegister();
        #endif /* (motorD_UseControl) */
    #endif  /* (!motorD_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorD_RestoreConfig
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
*  motorD_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorD_RestoreConfig(void) 
{
        #if(!motorD_UsingFixedFunction)
            #if(!motorD_PWMModeIsCenterAligned)
                motorD_WritePeriod(motorD_backup.PWMPeriod);
            #endif /* (!motorD_PWMModeIsCenterAligned) */

            motorD_WriteCounter(motorD_backup.PWMUdb);

            #if (motorD_UseStatus)
                motorD_STATUS_MASK = motorD_backup.InterruptMaskValue;
            #endif /* (motorD_UseStatus) */

            #if(motorD_DeadBandMode == motorD__B_PWM__DBM_256_CLOCKS || \
                motorD_DeadBandMode == motorD__B_PWM__DBM_2_4_CLOCKS)
                motorD_WriteDeadTime(motorD_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorD_KillModeMinTime)
                motorD_WriteKillTime(motorD_backup.PWMKillCounterPeriod);
            #endif /* (motorD_KillModeMinTime) */

            #if(motorD_UseControl)
                motorD_WriteControlRegister(motorD_backup.PWMControlRegister);
            #endif /* (motorD_UseControl) */
        #endif  /* (!motorD_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorD_Sleep
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
*  motorD_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorD_Sleep(void) 
{
    #if(motorD_UseControl)
        if(motorD_CTRL_ENABLE == (motorD_CONTROL & motorD_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorD_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorD_backup.PWMEnableState = 0u;
        }
    #endif /* (motorD_UseControl) */

    /* Stop component */
    motorD_Stop();

    /* Save registers configuration */
    motorD_SaveConfig();
}


/*******************************************************************************
* Function Name: motorD_Wakeup
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
*  motorD_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorD_Wakeup(void) 
{
     /* Restore registers values */
    motorD_RestoreConfig();

    if(motorD_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorD_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
