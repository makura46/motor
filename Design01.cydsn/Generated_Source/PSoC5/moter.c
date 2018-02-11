/*******************************************************************************
* File Name: moter.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "moter.h"

/* Error message for removed <resource> through optimization */
#ifdef moter_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* moter_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 moter_initVar = 0u;


/*******************************************************************************
* Function Name: moter_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  moter_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void moter_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(moter_initVar == 0u)
    {
        moter_Init();
        moter_initVar = 1u;
    }
    moter_Enable();

}


/*******************************************************************************
* Function Name: moter_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  moter_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void moter_Init(void) 
{
    #if (moter_UsingFixedFunction || moter_UseControl)
        uint8 ctrl;
    #endif /* (moter_UsingFixedFunction || moter_UseControl) */

    #if(!moter_UsingFixedFunction)
        #if(moter_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 moter_interruptState;
        #endif /* (moter_UseStatus) */
    #endif /* (!moter_UsingFixedFunction) */

    #if (moter_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        moter_CONTROL |= moter_CFG0_MODE;
        #if (moter_DeadBand2_4)
            moter_CONTROL |= moter_CFG0_DB;
        #endif /* (moter_DeadBand2_4) */

        ctrl = moter_CONTROL3 & ((uint8 )(~moter_CTRL_CMPMODE1_MASK));
        moter_CONTROL3 = ctrl | moter_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        moter_RT1 &= ((uint8)(~moter_RT1_MASK));
        moter_RT1 |= moter_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        moter_RT1 &= ((uint8)(~moter_SYNCDSI_MASK));
        moter_RT1 |= moter_SYNCDSI_EN;

    #elif (moter_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = moter_CONTROL & ((uint8)(~moter_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~moter_CTRL_CMPMODE1_MASK));
        moter_CONTROL = ctrl | moter_DEFAULT_COMPARE2_MODE |
                                   moter_DEFAULT_COMPARE1_MODE;
    #endif /* (moter_UsingFixedFunction) */

    #if (!moter_UsingFixedFunction)
        #if (moter_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            moter_AUX_CONTROLDP0 |= (moter_AUX_CTRL_FIFO0_CLR);
        #else /* (moter_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            moter_AUX_CONTROLDP0 |= (moter_AUX_CTRL_FIFO0_CLR);
            moter_AUX_CONTROLDP1 |= (moter_AUX_CTRL_FIFO0_CLR);
        #endif /* (moter_Resolution == 8) */

        moter_WriteCounter(moter_INIT_PERIOD_VALUE);
    #endif /* (!moter_UsingFixedFunction) */

    moter_WritePeriod(moter_INIT_PERIOD_VALUE);

        #if (moter_UseOneCompareMode)
            moter_WriteCompare(moter_INIT_COMPARE_VALUE1);
        #else
            moter_WriteCompare1(moter_INIT_COMPARE_VALUE1);
            moter_WriteCompare2(moter_INIT_COMPARE_VALUE2);
        #endif /* (moter_UseOneCompareMode) */

        #if (moter_KillModeMinTime)
            moter_WriteKillTime(moter_MinimumKillTime);
        #endif /* (moter_KillModeMinTime) */

        #if (moter_DeadBandUsed)
            moter_WriteDeadTime(moter_INIT_DEAD_TIME);
        #endif /* (moter_DeadBandUsed) */

    #if (moter_UseStatus || moter_UsingFixedFunction)
        moter_SetInterruptMode(moter_INIT_INTERRUPTS_MODE);
    #endif /* (moter_UseStatus || moter_UsingFixedFunction) */

    #if (moter_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        moter_GLOBAL_ENABLE |= moter_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        moter_CONTROL2 |= moter_CTRL2_IRQ_SEL;
    #else
        #if(moter_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            moter_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            moter_STATUS_AUX_CTRL |= moter_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(moter_interruptState);

            /* Clear the FIFO to enable the moter_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            moter_ClearFIFO();
        #endif /* (moter_UseStatus) */
    #endif /* (moter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: moter_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void moter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (moter_UsingFixedFunction)
        moter_GLOBAL_ENABLE |= moter_BLOCK_EN_MASK;
        moter_GLOBAL_STBY_ENABLE |= moter_BLOCK_STBY_EN_MASK;
    #endif /* (moter_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (moter_UseControl || moter_UsingFixedFunction)
        moter_CONTROL |= moter_CTRL_ENABLE;
    #endif /* (moter_UseControl || moter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: moter_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void moter_Stop(void) 
{
    #if (moter_UseControl || moter_UsingFixedFunction)
        moter_CONTROL &= ((uint8)(~moter_CTRL_ENABLE));
    #endif /* (moter_UseControl || moter_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (moter_UsingFixedFunction)
        moter_GLOBAL_ENABLE &= ((uint8)(~moter_BLOCK_EN_MASK));
        moter_GLOBAL_STBY_ENABLE &= ((uint8)(~moter_BLOCK_STBY_EN_MASK));
    #endif /* (moter_UsingFixedFunction) */
}

#if (moter_UseOneCompareMode)
    #if (moter_CompareMode1SW)


        /*******************************************************************************
        * Function Name: moter_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void moter_SetCompareMode(uint8 comparemode) 
        {
            #if(moter_UsingFixedFunction)

                #if(0 != moter_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << moter_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != moter_CTRL_CMPMODE1_SHIFT) */

                moter_CONTROL3 &= ((uint8)(~moter_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                moter_CONTROL3 |= comparemodemasked;

            #elif (moter_UseControl)

                #if(0 != moter_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << moter_CTRL_CMPMODE1_SHIFT)) &
                                                moter_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & moter_CTRL_CMPMODE1_MASK;
                #endif /* (0 != moter_CTRL_CMPMODE1_SHIFT) */

                #if(0 != moter_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << moter_CTRL_CMPMODE2_SHIFT)) &
                                               moter_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & moter_CTRL_CMPMODE2_MASK;
                #endif /* (0 != moter_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                moter_CONTROL &= ((uint8)(~(moter_CTRL_CMPMODE1_MASK |
                                            moter_CTRL_CMPMODE2_MASK)));
                moter_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (moter_UsingFixedFunction) */
        }
    #endif /* moter_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (moter_CompareMode1SW)


        /*******************************************************************************
        * Function Name: moter_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void moter_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != moter_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << moter_CTRL_CMPMODE1_SHIFT)) &
                                           moter_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & moter_CTRL_CMPMODE1_MASK;
            #endif /* (0 != moter_CTRL_CMPMODE1_SHIFT) */

            #if (moter_UseControl)
                moter_CONTROL &= ((uint8)(~moter_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                moter_CONTROL |= comparemodemasked;
            #endif /* (moter_UseControl) */
        }
    #endif /* moter_CompareMode1SW */

#if (moter_CompareMode2SW)


    /*******************************************************************************
    * Function Name: moter_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != moter_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << moter_CTRL_CMPMODE2_SHIFT)) &
                                                 moter_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & moter_CTRL_CMPMODE2_MASK;
        #endif /* (0 != moter_CTRL_CMPMODE2_SHIFT) */

        #if (moter_UseControl)
            moter_CONTROL &= ((uint8)(~moter_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            moter_CONTROL |= comparemodemasked;
        #endif /* (moter_UseControl) */
    }
    #endif /*moter_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!moter_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: moter_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void moter_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(moter_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: moter_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 moter_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(moter_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(moter_CAPTURE_LSB_PTR));
    }

    #if (moter_UseStatus)


        /*******************************************************************************
        * Function Name: moter_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void moter_ClearFIFO(void) 
        {
            while(0u != (moter_ReadStatusRegister() & moter_STATUS_FIFONEMPTY))
            {
                (void)moter_ReadCapture();
            }
        }

    #endif /* moter_UseStatus */

#endif /* !moter_UsingFixedFunction */


/*******************************************************************************
* Function Name: moter_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void moter_WritePeriod(uint8 period) 
{
    #if(moter_UsingFixedFunction)
        CY_SET_REG16(moter_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(moter_PERIOD_LSB_PTR, period);
    #endif /* (moter_UsingFixedFunction) */
}

#if (moter_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: moter_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void moter_WriteCompare(uint8 compare) \
                                       
    {
        #if(moter_UsingFixedFunction)
            CY_SET_REG16(moter_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(moter_COMPARE1_LSB_PTR, compare);
        #endif /* (moter_UsingFixedFunction) */

        #if (moter_PWMMode == moter__B_PWM__DITHER)
            #if(moter_UsingFixedFunction)
                CY_SET_REG16(moter_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(moter_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (moter_UsingFixedFunction) */
        #endif /* (moter_PWMMode == moter__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: moter_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_WriteCompare1(uint8 compare) \
                                        
    {
        #if(moter_UsingFixedFunction)
            CY_SET_REG16(moter_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(moter_COMPARE1_LSB_PTR, compare);
        #endif /* (moter_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: moter_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_WriteCompare2(uint8 compare) \
                                        
    {
        #if(moter_UsingFixedFunction)
            CY_SET_REG16(moter_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(moter_COMPARE2_LSB_PTR, compare);
        #endif /* (moter_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (moter_DeadBandUsed)


    /*******************************************************************************
    * Function Name: moter_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!moter_DeadBand2_4)
            CY_SET_REG8(moter_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            moter_DEADBAND_COUNT &= ((uint8)(~moter_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(moter_DEADBAND_COUNT_SHIFT)
                moter_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << moter_DEADBAND_COUNT_SHIFT)) &
                                                    moter_DEADBAND_COUNT_MASK;
            #else
                moter_DEADBAND_COUNT |= deadtime & moter_DEADBAND_COUNT_MASK;
            #endif /* (moter_DEADBAND_COUNT_SHIFT) */

        #endif /* (!moter_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: moter_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 moter_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!moter_DeadBand2_4)
            return (CY_GET_REG8(moter_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(moter_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(moter_DEADBAND_COUNT & moter_DEADBAND_COUNT_MASK)) >>
                                                                           moter_DEADBAND_COUNT_SHIFT));
            #else
                return (moter_DEADBAND_COUNT & moter_DEADBAND_COUNT_MASK);
            #endif /* (moter_DEADBAND_COUNT_SHIFT) */
        #endif /* (!moter_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (moter_UseStatus || moter_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: moter_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(moter_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: moter_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 moter_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(moter_STATUS_PTR));
    }

#endif /* (moter_UseStatus || moter_UsingFixedFunction) */


#if (moter_UseControl)


    /*******************************************************************************
    * Function Name: moter_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 moter_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(moter_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: moter_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(moter_CONTROL_PTR, control);
    }

#endif /* (moter_UseControl) */


#if (!moter_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: moter_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 moter_ReadCapture(void) 
    {
        return (CY_GET_REG8(moter_CAPTURE_LSB_PTR));
    }

#endif /* (!moter_UsingFixedFunction) */


#if (moter_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: moter_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 moter_ReadCompare(void) 
    {
        #if(moter_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(moter_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(moter_COMPARE1_LSB_PTR));
        #endif /* (moter_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: moter_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 moter_ReadCompare1(void) 
    {
        return (CY_GET_REG8(moter_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: moter_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 moter_ReadCompare2(void) 
    {
        return (CY_GET_REG8(moter_COMPARE2_LSB_PTR));
    }

#endif /* (moter_UseOneCompareMode) */


/*******************************************************************************
* Function Name: moter_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 moter_ReadPeriod(void) 
{
    #if(moter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(moter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(moter_PERIOD_LSB_PTR));
    #endif /* (moter_UsingFixedFunction) */
}

#if ( moter_KillModeMinTime)


    /*******************************************************************************
    * Function Name: moter_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void moter_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(moter_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: moter_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 moter_ReadKillTime(void) 
    {
        return (CY_GET_REG8(moter_KILLMODEMINTIME_PTR));
    }

#endif /* ( moter_KillModeMinTime) */

/* [] END OF FILE */
