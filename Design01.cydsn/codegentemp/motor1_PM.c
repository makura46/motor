/*******************************************************************************
* File Name: motor1_PM.c
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

#include "motor1.h"

static motor1_backupStruct motor1_backup;


/*******************************************************************************
* Function Name: motor1_SaveConfig
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
*  motor1_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motor1_SaveConfig(void) 
{

    #if(!motor1_UsingFixedFunction)
        #if(!motor1_PWMModeIsCenterAligned)
            motor1_backup.PWMPeriod = motor1_ReadPeriod();
        #endif /* (!motor1_PWMModeIsCenterAligned) */
        motor1_backup.PWMUdb = motor1_ReadCounter();
        #if (motor1_UseStatus)
            motor1_backup.InterruptMaskValue = motor1_STATUS_MASK;
        #endif /* (motor1_UseStatus) */

        #if(motor1_DeadBandMode == motor1__B_PWM__DBM_256_CLOCKS || \
            motor1_DeadBandMode == motor1__B_PWM__DBM_2_4_CLOCKS)
            motor1_backup.PWMdeadBandValue = motor1_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motor1_KillModeMinTime)
             motor1_backup.PWMKillCounterPeriod = motor1_ReadKillTime();
        #endif /* (motor1_KillModeMinTime) */

        #if(motor1_UseControl)
            motor1_backup.PWMControlRegister = motor1_ReadControlRegister();
        #endif /* (motor1_UseControl) */
    #endif  /* (!motor1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor1_RestoreConfig
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
*  motor1_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motor1_RestoreConfig(void) 
{
        #if(!motor1_UsingFixedFunction)
            #if(!motor1_PWMModeIsCenterAligned)
                motor1_WritePeriod(motor1_backup.PWMPeriod);
            #endif /* (!motor1_PWMModeIsCenterAligned) */

            motor1_WriteCounter(motor1_backup.PWMUdb);

            #if (motor1_UseStatus)
                motor1_STATUS_MASK = motor1_backup.InterruptMaskValue;
            #endif /* (motor1_UseStatus) */

            #if(motor1_DeadBandMode == motor1__B_PWM__DBM_256_CLOCKS || \
                motor1_DeadBandMode == motor1__B_PWM__DBM_2_4_CLOCKS)
                motor1_WriteDeadTime(motor1_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motor1_KillModeMinTime)
                motor1_WriteKillTime(motor1_backup.PWMKillCounterPeriod);
            #endif /* (motor1_KillModeMinTime) */

            #if(motor1_UseControl)
                motor1_WriteControlRegister(motor1_backup.PWMControlRegister);
            #endif /* (motor1_UseControl) */
        #endif  /* (!motor1_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motor1_Sleep
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
*  motor1_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motor1_Sleep(void) 
{
    #if(motor1_UseControl)
        if(motor1_CTRL_ENABLE == (motor1_CONTROL & motor1_CTRL_ENABLE))
        {
            /*Component is enabled */
            motor1_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motor1_backup.PWMEnableState = 0u;
        }
    #endif /* (motor1_UseControl) */

    /* Stop component */
    motor1_Stop();

    /* Save registers configuration */
    motor1_SaveConfig();
}


/*******************************************************************************
* Function Name: motor1_Wakeup
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
*  motor1_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motor1_Wakeup(void) 
{
     /* Restore registers values */
    motor1_RestoreConfig();

    if(motor1_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motor1_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
