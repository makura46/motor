/*******************************************************************************
* File Name: motor3_PM.c
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

#include "motor3.h"

static motor3_backupStruct motor3_backup;


/*******************************************************************************
* Function Name: motor3_SaveConfig
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
*  motor3_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motor3_SaveConfig(void) 
{

    #if(!motor3_UsingFixedFunction)
        #if(!motor3_PWMModeIsCenterAligned)
            motor3_backup.PWMPeriod = motor3_ReadPeriod();
        #endif /* (!motor3_PWMModeIsCenterAligned) */
        motor3_backup.PWMUdb = motor3_ReadCounter();
        #if (motor3_UseStatus)
            motor3_backup.InterruptMaskValue = motor3_STATUS_MASK;
        #endif /* (motor3_UseStatus) */

        #if(motor3_DeadBandMode == motor3__B_PWM__DBM_256_CLOCKS || \
            motor3_DeadBandMode == motor3__B_PWM__DBM_2_4_CLOCKS)
            motor3_backup.PWMdeadBandValue = motor3_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motor3_KillModeMinTime)
             motor3_backup.PWMKillCounterPeriod = motor3_ReadKillTime();
        #endif /* (motor3_KillModeMinTime) */

        #if(motor3_UseControl)
            motor3_backup.PWMControlRegister = motor3_ReadControlRegister();
        #endif /* (motor3_UseControl) */
    #endif  /* (!motor3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor3_RestoreConfig
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
*  motor3_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motor3_RestoreConfig(void) 
{
        #if(!motor3_UsingFixedFunction)
            #if(!motor3_PWMModeIsCenterAligned)
                motor3_WritePeriod(motor3_backup.PWMPeriod);
            #endif /* (!motor3_PWMModeIsCenterAligned) */

            motor3_WriteCounter(motor3_backup.PWMUdb);

            #if (motor3_UseStatus)
                motor3_STATUS_MASK = motor3_backup.InterruptMaskValue;
            #endif /* (motor3_UseStatus) */

            #if(motor3_DeadBandMode == motor3__B_PWM__DBM_256_CLOCKS || \
                motor3_DeadBandMode == motor3__B_PWM__DBM_2_4_CLOCKS)
                motor3_WriteDeadTime(motor3_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motor3_KillModeMinTime)
                motor3_WriteKillTime(motor3_backup.PWMKillCounterPeriod);
            #endif /* (motor3_KillModeMinTime) */

            #if(motor3_UseControl)
                motor3_WriteControlRegister(motor3_backup.PWMControlRegister);
            #endif /* (motor3_UseControl) */
        #endif  /* (!motor3_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motor3_Sleep
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
*  motor3_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motor3_Sleep(void) 
{
    #if(motor3_UseControl)
        if(motor3_CTRL_ENABLE == (motor3_CONTROL & motor3_CTRL_ENABLE))
        {
            /*Component is enabled */
            motor3_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motor3_backup.PWMEnableState = 0u;
        }
    #endif /* (motor3_UseControl) */

    /* Stop component */
    motor3_Stop();

    /* Save registers configuration */
    motor3_SaveConfig();
}


/*******************************************************************************
* Function Name: motor3_Wakeup
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
*  motor3_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motor3_Wakeup(void) 
{
     /* Restore registers values */
    motor3_RestoreConfig();

    if(motor3_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motor3_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
