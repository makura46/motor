/*******************************************************************************
* File Name: houkou_PM.c
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

#include "houkou.h"

/* Check for removal by optimization */
#if !defined(houkou_Sync_ctrl_reg__REMOVED)

static houkou_BACKUP_STRUCT  houkou_backup = {0u};

    
/*******************************************************************************
* Function Name: houkou_SaveConfig
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
void houkou_SaveConfig(void) 
{
    houkou_backup.controlState = houkou_Control;
}


/*******************************************************************************
* Function Name: houkou_RestoreConfig
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
void houkou_RestoreConfig(void) 
{
     houkou_Control = houkou_backup.controlState;
}


/*******************************************************************************
* Function Name: houkou_Sleep
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
void houkou_Sleep(void) 
{
    houkou_SaveConfig();
}


/*******************************************************************************
* Function Name: houkou_Wakeup
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
void houkou_Wakeup(void)  
{
    houkou_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
