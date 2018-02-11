/*******************************************************************************
* File Name: motor2_PM.c
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

#include "motor2.h"

static motor2_backupStruct motor2_backup;


/*******************************************************************************
* Function Name: motor2_SaveConfig
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
*  motor2_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motor2_SaveConfig(void) 
{

    #if(!motor2_UsingFixedFunction)
        #if(!motor2_PWMModeIsCenterAligned)
            motor2_backup.PWMPeriod = motor2_ReadPeriod();
        #endif /* (!motor2_PWMModeIsCenterAligned) */
        motor2_backup.PWMUdb = motor2_ReadCounter();
        #if (motor2_UseStatus)
            motor2_backup.InterruptMaskValue = motor2_STATUS_MASK;
        #endif /* (motor2_UseStatus) */

        #if(motor2_DeadBandMode == motor2__B_PWM__DBM_256_CLOCKS || \
            motor2_DeadBandMode == motor2__B_PWM__DBM_2_4_CLOCKS)
            motor2_backup.PWMdeadBandValue = motor2_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motor2_KillModeMinTime)
             motor2_backup.PWMKillCounterPeriod = motor2_ReadKillTime();
        #endif /* (motor2_KillModeMinTime) */

        #if(motor2_UseControl)
            motor2_backup.PWMControlRegister = motor2_ReadControlRegister();
        #endif /* (motor2_UseControl) */
    #endif  /* (!motor2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor2_RestoreConfig
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
*  motor2_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motor2_RestoreConfig(void) 
{
        #if(!motor2_UsingFixedFunction)
            #if(!motor2_PWMModeIsCenterAligned)
                motor2_WritePeriod(motor2_backup.PWMPeriod);
            #endif /* (!motor2_PWMModeIsCenterAligned) */

            motor2_WriteCounter(motor2_backup.PWMUdb);

            #if (motor2_UseStatus)
                motor2_STATUS_MASK = motor2_backup.InterruptMaskValue;
            #endif /* (motor2_UseStatus) */

            #if(motor2_DeadBandMode == motor2__B_PWM__DBM_256_CLOCKS || \
                motor2_DeadBandMode == motor2__B_PWM__DBM_2_4_CLOCKS)
                motor2_WriteDeadTime(motor2_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motor2_KillModeMinTime)
                motor2_WriteKillTime(motor2_backup.PWMKillCounterPeriod);
            #endif /* (motor2_KillModeMinTime) */

            #if(motor2_UseControl)
                motor2_WriteControlRegister(motor2_backup.PWMControlRegister);
            #endif /* (motor2_UseControl) */
        #endif  /* (!motor2_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motor2_Sleep
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
*  motor2_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motor2_Sleep(void) 
{
    #if(motor2_UseControl)
        if(motor2_CTRL_ENABLE == (motor2_CONTROL & motor2_CTRL_ENABLE))
        {
            /*Component is enabled */
            motor2_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motor2_backup.PWMEnableState = 0u;
        }
    #endif /* (motor2_UseControl) */

    /* Stop component */
    motor2_Stop();

    /* Save registers configuration */
    motor2_SaveConfig();
}


/*******************************************************************************
* Function Name: motor2_Wakeup
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
*  motor2_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motor2_Wakeup(void) 
{
     /* Restore registers values */
    motor2_RestoreConfig();

    if(motor2_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motor2_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
