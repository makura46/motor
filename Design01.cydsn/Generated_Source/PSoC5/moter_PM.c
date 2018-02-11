/*******************************************************************************
* File Name: moter_PM.c
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

#include "moter.h"

static moter_backupStruct moter_backup;


/*******************************************************************************
* Function Name: moter_SaveConfig
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
*  moter_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void moter_SaveConfig(void) 
{

    #if(!moter_UsingFixedFunction)
        #if(!moter_PWMModeIsCenterAligned)
            moter_backup.PWMPeriod = moter_ReadPeriod();
        #endif /* (!moter_PWMModeIsCenterAligned) */
        moter_backup.PWMUdb = moter_ReadCounter();
        #if (moter_UseStatus)
            moter_backup.InterruptMaskValue = moter_STATUS_MASK;
        #endif /* (moter_UseStatus) */

        #if(moter_DeadBandMode == moter__B_PWM__DBM_256_CLOCKS || \
            moter_DeadBandMode == moter__B_PWM__DBM_2_4_CLOCKS)
            moter_backup.PWMdeadBandValue = moter_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(moter_KillModeMinTime)
             moter_backup.PWMKillCounterPeriod = moter_ReadKillTime();
        #endif /* (moter_KillModeMinTime) */

        #if(moter_UseControl)
            moter_backup.PWMControlRegister = moter_ReadControlRegister();
        #endif /* (moter_UseControl) */
    #endif  /* (!moter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: moter_RestoreConfig
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
*  moter_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void moter_RestoreConfig(void) 
{
        #if(!moter_UsingFixedFunction)
            #if(!moter_PWMModeIsCenterAligned)
                moter_WritePeriod(moter_backup.PWMPeriod);
            #endif /* (!moter_PWMModeIsCenterAligned) */

            moter_WriteCounter(moter_backup.PWMUdb);

            #if (moter_UseStatus)
                moter_STATUS_MASK = moter_backup.InterruptMaskValue;
            #endif /* (moter_UseStatus) */

            #if(moter_DeadBandMode == moter__B_PWM__DBM_256_CLOCKS || \
                moter_DeadBandMode == moter__B_PWM__DBM_2_4_CLOCKS)
                moter_WriteDeadTime(moter_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(moter_KillModeMinTime)
                moter_WriteKillTime(moter_backup.PWMKillCounterPeriod);
            #endif /* (moter_KillModeMinTime) */

            #if(moter_UseControl)
                moter_WriteControlRegister(moter_backup.PWMControlRegister);
            #endif /* (moter_UseControl) */
        #endif  /* (!moter_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: moter_Sleep
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
*  moter_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void moter_Sleep(void) 
{
    #if(moter_UseControl)
        if(moter_CTRL_ENABLE == (moter_CONTROL & moter_CTRL_ENABLE))
        {
            /*Component is enabled */
            moter_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            moter_backup.PWMEnableState = 0u;
        }
    #endif /* (moter_UseControl) */

    /* Stop component */
    moter_Stop();

    /* Save registers configuration */
    moter_SaveConfig();
}


/*******************************************************************************
* Function Name: moter_Wakeup
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
*  moter_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void moter_Wakeup(void) 
{
     /* Restore registers values */
    moter_RestoreConfig();

    if(moter_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        moter_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
