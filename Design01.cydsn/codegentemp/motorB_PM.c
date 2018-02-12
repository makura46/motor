/*******************************************************************************
* File Name: motorB_PM.c
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

#include "motorB.h"

static motorB_backupStruct motorB_backup;


/*******************************************************************************
* Function Name: motorB_SaveConfig
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
*  motorB_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motorB_SaveConfig(void) 
{

    #if(!motorB_UsingFixedFunction)
        #if(!motorB_PWMModeIsCenterAligned)
            motorB_backup.PWMPeriod = motorB_ReadPeriod();
        #endif /* (!motorB_PWMModeIsCenterAligned) */
        motorB_backup.PWMUdb = motorB_ReadCounter();
        #if (motorB_UseStatus)
            motorB_backup.InterruptMaskValue = motorB_STATUS_MASK;
        #endif /* (motorB_UseStatus) */

        #if(motorB_DeadBandMode == motorB__B_PWM__DBM_256_CLOCKS || \
            motorB_DeadBandMode == motorB__B_PWM__DBM_2_4_CLOCKS)
            motorB_backup.PWMdeadBandValue = motorB_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motorB_KillModeMinTime)
             motorB_backup.PWMKillCounterPeriod = motorB_ReadKillTime();
        #endif /* (motorB_KillModeMinTime) */

        #if(motorB_UseControl)
            motorB_backup.PWMControlRegister = motorB_ReadControlRegister();
        #endif /* (motorB_UseControl) */
    #endif  /* (!motorB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorB_RestoreConfig
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
*  motorB_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motorB_RestoreConfig(void) 
{
        #if(!motorB_UsingFixedFunction)
            #if(!motorB_PWMModeIsCenterAligned)
                motorB_WritePeriod(motorB_backup.PWMPeriod);
            #endif /* (!motorB_PWMModeIsCenterAligned) */

            motorB_WriteCounter(motorB_backup.PWMUdb);

            #if (motorB_UseStatus)
                motorB_STATUS_MASK = motorB_backup.InterruptMaskValue;
            #endif /* (motorB_UseStatus) */

            #if(motorB_DeadBandMode == motorB__B_PWM__DBM_256_CLOCKS || \
                motorB_DeadBandMode == motorB__B_PWM__DBM_2_4_CLOCKS)
                motorB_WriteDeadTime(motorB_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motorB_KillModeMinTime)
                motorB_WriteKillTime(motorB_backup.PWMKillCounterPeriod);
            #endif /* (motorB_KillModeMinTime) */

            #if(motorB_UseControl)
                motorB_WriteControlRegister(motorB_backup.PWMControlRegister);
            #endif /* (motorB_UseControl) */
        #endif  /* (!motorB_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motorB_Sleep
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
*  motorB_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motorB_Sleep(void) 
{
    #if(motorB_UseControl)
        if(motorB_CTRL_ENABLE == (motorB_CONTROL & motorB_CTRL_ENABLE))
        {
            /*Component is enabled */
            motorB_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motorB_backup.PWMEnableState = 0u;
        }
    #endif /* (motorB_UseControl) */

    /* Stop component */
    motorB_Stop();

    /* Save registers configuration */
    motorB_SaveConfig();
}


/*******************************************************************************
* Function Name: motorB_Wakeup
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
*  motorB_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motorB_Wakeup(void) 
{
     /* Restore registers values */
    motorB_RestoreConfig();

    if(motorB_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motorB_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
