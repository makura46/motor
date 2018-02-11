/*******************************************************************************
* File Name: motor1.c
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

#include "motor1.h"

/* Error message for removed <resource> through optimization */
#ifdef motor1_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* motor1_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 motor1_initVar = 0u;


/*******************************************************************************
* Function Name: motor1_Start
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
*  motor1_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void motor1_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(motor1_initVar == 0u)
    {
        motor1_Init();
        motor1_initVar = 1u;
    }
    motor1_Enable();

}


/*******************************************************************************
* Function Name: motor1_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  motor1_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void motor1_Init(void) 
{
    #if (motor1_UsingFixedFunction || motor1_UseControl)
        uint8 ctrl;
    #endif /* (motor1_UsingFixedFunction || motor1_UseControl) */

    #if(!motor1_UsingFixedFunction)
        #if(motor1_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 motor1_interruptState;
        #endif /* (motor1_UseStatus) */
    #endif /* (!motor1_UsingFixedFunction) */

    #if (motor1_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        motor1_CONTROL |= motor1_CFG0_MODE;
        #if (motor1_DeadBand2_4)
            motor1_CONTROL |= motor1_CFG0_DB;
        #endif /* (motor1_DeadBand2_4) */

        ctrl = motor1_CONTROL3 & ((uint8 )(~motor1_CTRL_CMPMODE1_MASK));
        motor1_CONTROL3 = ctrl | motor1_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        motor1_RT1 &= ((uint8)(~motor1_RT1_MASK));
        motor1_RT1 |= motor1_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        motor1_RT1 &= ((uint8)(~motor1_SYNCDSI_MASK));
        motor1_RT1 |= motor1_SYNCDSI_EN;

    #elif (motor1_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = motor1_CONTROL & ((uint8)(~motor1_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~motor1_CTRL_CMPMODE1_MASK));
        motor1_CONTROL = ctrl | motor1_DEFAULT_COMPARE2_MODE |
                                   motor1_DEFAULT_COMPARE1_MODE;
    #endif /* (motor1_UsingFixedFunction) */

    #if (!motor1_UsingFixedFunction)
        #if (motor1_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            motor1_AUX_CONTROLDP0 |= (motor1_AUX_CTRL_FIFO0_CLR);
        #else /* (motor1_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            motor1_AUX_CONTROLDP0 |= (motor1_AUX_CTRL_FIFO0_CLR);
            motor1_AUX_CONTROLDP1 |= (motor1_AUX_CTRL_FIFO0_CLR);
        #endif /* (motor1_Resolution == 8) */

        motor1_WriteCounter(motor1_INIT_PERIOD_VALUE);
    #endif /* (!motor1_UsingFixedFunction) */

    motor1_WritePeriod(motor1_INIT_PERIOD_VALUE);

        #if (motor1_UseOneCompareMode)
            motor1_WriteCompare(motor1_INIT_COMPARE_VALUE1);
        #else
            motor1_WriteCompare1(motor1_INIT_COMPARE_VALUE1);
            motor1_WriteCompare2(motor1_INIT_COMPARE_VALUE2);
        #endif /* (motor1_UseOneCompareMode) */

        #if (motor1_KillModeMinTime)
            motor1_WriteKillTime(motor1_MinimumKillTime);
        #endif /* (motor1_KillModeMinTime) */

        #if (motor1_DeadBandUsed)
            motor1_WriteDeadTime(motor1_INIT_DEAD_TIME);
        #endif /* (motor1_DeadBandUsed) */

    #if (motor1_UseStatus || motor1_UsingFixedFunction)
        motor1_SetInterruptMode(motor1_INIT_INTERRUPTS_MODE);
    #endif /* (motor1_UseStatus || motor1_UsingFixedFunction) */

    #if (motor1_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        motor1_GLOBAL_ENABLE |= motor1_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        motor1_CONTROL2 |= motor1_CTRL2_IRQ_SEL;
    #else
        #if(motor1_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            motor1_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            motor1_STATUS_AUX_CTRL |= motor1_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(motor1_interruptState);

            /* Clear the FIFO to enable the motor1_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            motor1_ClearFIFO();
        #endif /* (motor1_UseStatus) */
    #endif /* (motor1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor1_Enable
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
void motor1_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (motor1_UsingFixedFunction)
        motor1_GLOBAL_ENABLE |= motor1_BLOCK_EN_MASK;
        motor1_GLOBAL_STBY_ENABLE |= motor1_BLOCK_STBY_EN_MASK;
    #endif /* (motor1_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (motor1_UseControl || motor1_UsingFixedFunction)
        motor1_CONTROL |= motor1_CTRL_ENABLE;
    #endif /* (motor1_UseControl || motor1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor1_Stop
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
void motor1_Stop(void) 
{
    #if (motor1_UseControl || motor1_UsingFixedFunction)
        motor1_CONTROL &= ((uint8)(~motor1_CTRL_ENABLE));
    #endif /* (motor1_UseControl || motor1_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (motor1_UsingFixedFunction)
        motor1_GLOBAL_ENABLE &= ((uint8)(~motor1_BLOCK_EN_MASK));
        motor1_GLOBAL_STBY_ENABLE &= ((uint8)(~motor1_BLOCK_STBY_EN_MASK));
    #endif /* (motor1_UsingFixedFunction) */
}

#if (motor1_UseOneCompareMode)
    #if (motor1_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motor1_SetCompareMode
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
        void motor1_SetCompareMode(uint8 comparemode) 
        {
            #if(motor1_UsingFixedFunction)

                #if(0 != motor1_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor1_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != motor1_CTRL_CMPMODE1_SHIFT) */

                motor1_CONTROL3 &= ((uint8)(~motor1_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                motor1_CONTROL3 |= comparemodemasked;

            #elif (motor1_UseControl)

                #if(0 != motor1_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << motor1_CTRL_CMPMODE1_SHIFT)) &
                                                motor1_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & motor1_CTRL_CMPMODE1_MASK;
                #endif /* (0 != motor1_CTRL_CMPMODE1_SHIFT) */

                #if(0 != motor1_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << motor1_CTRL_CMPMODE2_SHIFT)) &
                                               motor1_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & motor1_CTRL_CMPMODE2_MASK;
                #endif /* (0 != motor1_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                motor1_CONTROL &= ((uint8)(~(motor1_CTRL_CMPMODE1_MASK |
                                            motor1_CTRL_CMPMODE2_MASK)));
                motor1_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (motor1_UsingFixedFunction) */
        }
    #endif /* motor1_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (motor1_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motor1_SetCompareMode1
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
        void motor1_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != motor1_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor1_CTRL_CMPMODE1_SHIFT)) &
                                           motor1_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & motor1_CTRL_CMPMODE1_MASK;
            #endif /* (0 != motor1_CTRL_CMPMODE1_SHIFT) */

            #if (motor1_UseControl)
                motor1_CONTROL &= ((uint8)(~motor1_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                motor1_CONTROL |= comparemodemasked;
            #endif /* (motor1_UseControl) */
        }
    #endif /* motor1_CompareMode1SW */

#if (motor1_CompareMode2SW)


    /*******************************************************************************
    * Function Name: motor1_SetCompareMode2
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
    void motor1_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != motor1_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor1_CTRL_CMPMODE2_SHIFT)) &
                                                 motor1_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & motor1_CTRL_CMPMODE2_MASK;
        #endif /* (0 != motor1_CTRL_CMPMODE2_SHIFT) */

        #if (motor1_UseControl)
            motor1_CONTROL &= ((uint8)(~motor1_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            motor1_CONTROL |= comparemodemasked;
        #endif /* (motor1_UseControl) */
    }
    #endif /*motor1_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!motor1_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor1_WriteCounter
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
    void motor1_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(motor1_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: motor1_ReadCounter
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
    uint8 motor1_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(motor1_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(motor1_CAPTURE_LSB_PTR));
    }

    #if (motor1_UseStatus)


        /*******************************************************************************
        * Function Name: motor1_ClearFIFO
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
        void motor1_ClearFIFO(void) 
        {
            while(0u != (motor1_ReadStatusRegister() & motor1_STATUS_FIFONEMPTY))
            {
                (void)motor1_ReadCapture();
            }
        }

    #endif /* motor1_UseStatus */

#endif /* !motor1_UsingFixedFunction */


/*******************************************************************************
* Function Name: motor1_WritePeriod
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
void motor1_WritePeriod(uint8 period) 
{
    #if(motor1_UsingFixedFunction)
        CY_SET_REG16(motor1_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(motor1_PERIOD_LSB_PTR, period);
    #endif /* (motor1_UsingFixedFunction) */
}

#if (motor1_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motor1_WriteCompare
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
    void motor1_WriteCompare(uint8 compare) \
                                       
    {
        #if(motor1_UsingFixedFunction)
            CY_SET_REG16(motor1_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motor1_COMPARE1_LSB_PTR, compare);
        #endif /* (motor1_UsingFixedFunction) */

        #if (motor1_PWMMode == motor1__B_PWM__DITHER)
            #if(motor1_UsingFixedFunction)
                CY_SET_REG16(motor1_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(motor1_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (motor1_UsingFixedFunction) */
        #endif /* (motor1_PWMMode == motor1__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: motor1_WriteCompare1
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
    void motor1_WriteCompare1(uint8 compare) \
                                        
    {
        #if(motor1_UsingFixedFunction)
            CY_SET_REG16(motor1_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motor1_COMPARE1_LSB_PTR, compare);
        #endif /* (motor1_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: motor1_WriteCompare2
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
    void motor1_WriteCompare2(uint8 compare) \
                                        
    {
        #if(motor1_UsingFixedFunction)
            CY_SET_REG16(motor1_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(motor1_COMPARE2_LSB_PTR, compare);
        #endif /* (motor1_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (motor1_DeadBandUsed)


    /*******************************************************************************
    * Function Name: motor1_WriteDeadTime
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
    void motor1_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!motor1_DeadBand2_4)
            CY_SET_REG8(motor1_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            motor1_DEADBAND_COUNT &= ((uint8)(~motor1_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(motor1_DEADBAND_COUNT_SHIFT)
                motor1_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << motor1_DEADBAND_COUNT_SHIFT)) &
                                                    motor1_DEADBAND_COUNT_MASK;
            #else
                motor1_DEADBAND_COUNT |= deadtime & motor1_DEADBAND_COUNT_MASK;
            #endif /* (motor1_DEADBAND_COUNT_SHIFT) */

        #endif /* (!motor1_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: motor1_ReadDeadTime
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
    uint8 motor1_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!motor1_DeadBand2_4)
            return (CY_GET_REG8(motor1_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(motor1_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(motor1_DEADBAND_COUNT & motor1_DEADBAND_COUNT_MASK)) >>
                                                                           motor1_DEADBAND_COUNT_SHIFT));
            #else
                return (motor1_DEADBAND_COUNT & motor1_DEADBAND_COUNT_MASK);
            #endif /* (motor1_DEADBAND_COUNT_SHIFT) */
        #endif /* (!motor1_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (motor1_UseStatus || motor1_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor1_SetInterruptMode
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
    void motor1_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(motor1_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: motor1_ReadStatusRegister
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
    uint8 motor1_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(motor1_STATUS_PTR));
    }

#endif /* (motor1_UseStatus || motor1_UsingFixedFunction) */


#if (motor1_UseControl)


    /*******************************************************************************
    * Function Name: motor1_ReadControlRegister
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
    uint8 motor1_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(motor1_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: motor1_WriteControlRegister
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
    void motor1_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(motor1_CONTROL_PTR, control);
    }

#endif /* (motor1_UseControl) */


#if (!motor1_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor1_ReadCapture
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
    uint8 motor1_ReadCapture(void) 
    {
        return (CY_GET_REG8(motor1_CAPTURE_LSB_PTR));
    }

#endif /* (!motor1_UsingFixedFunction) */


#if (motor1_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motor1_ReadCompare
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
    uint8 motor1_ReadCompare(void) 
    {
        #if(motor1_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(motor1_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(motor1_COMPARE1_LSB_PTR));
        #endif /* (motor1_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: motor1_ReadCompare1
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
    uint8 motor1_ReadCompare1(void) 
    {
        return (CY_GET_REG8(motor1_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: motor1_ReadCompare2
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
    uint8 motor1_ReadCompare2(void) 
    {
        return (CY_GET_REG8(motor1_COMPARE2_LSB_PTR));
    }

#endif /* (motor1_UseOneCompareMode) */


/*******************************************************************************
* Function Name: motor1_ReadPeriod
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
uint8 motor1_ReadPeriod(void) 
{
    #if(motor1_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(motor1_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(motor1_PERIOD_LSB_PTR));
    #endif /* (motor1_UsingFixedFunction) */
}

#if ( motor1_KillModeMinTime)


    /*******************************************************************************
    * Function Name: motor1_WriteKillTime
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
    void motor1_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(motor1_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: motor1_ReadKillTime
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
    uint8 motor1_ReadKillTime(void) 
    {
        return (CY_GET_REG8(motor1_KILLMODEMINTIME_PTR));
    }

#endif /* ( motor1_KillModeMinTime) */

/* [] END OF FILE */
