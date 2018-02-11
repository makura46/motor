/*******************************************************************************
* File Name: houkou_2_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "houkou_2.h"

/* Check for removal by optimization */
#if !defined(houkou_2_Sync_ctrl_reg__REMOVED)

static houkou_2_BACKUP_STRUCT  houkou_2_backup = {0u};

    
/*******************************************************************************
* Function Name: houkou_2_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void houkou_2_SaveConfig(void) 
{
    houkou_2_backup.controlState = houkou_2_Control;
}


/*******************************************************************************
* Function Name: houkou_2_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void houkou_2_RestoreConfig(void) 
{
     houkou_2_Control = houkou_2_backup.controlState;
}


/*******************************************************************************
* Function Name: houkou_2_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void houkou_2_Sleep(void) 
{
    houkou_2_SaveConfig();
}


/*******************************************************************************
* Function Name: houkou_2_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void houkou_2_Wakeup(void)  
{
    houkou_2_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
