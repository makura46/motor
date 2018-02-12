/*******************************************************************************
* File Name: motorC.c
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

#include "motorC.h"

/* Error message for removed <resource> through optimization */
#ifdef motorC_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* motorC_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 motorC_initVar = 0u;


/*******************************************************************************
* Function Name: motorC_Start
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
*  motorC_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void motorC_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(motorC_initVar == 0u)
    {
        motorC_Init();
        motorC_initVar = 1u;
    }
    motorC_Enable();

}


/*******************************************************************************
* Function Name: motorC_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  motorC_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void motorC_Init(void) 
{
    #if (motorC_UsingFixedFunction || motorC_UseControl)
        uint8 ctrl;
    #endif /* (motorC_UsingFixedFunction || motorC_UseControl) */

    #if(!motorC_UsingFixedFunction)
        #if(motorC_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 motorC_interruptState;
        #endif /* (motorC_UseStatus) */
    #endif /* (!motorC_UsingFixedFunction) */

    #if (motorC_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        motorC_CONTROL |= motorC_CFG0_MODE;
        #if (motorC_DeadBand2_4)
            motorC_CONTROL |= motorC_CFG0_DB;
        #endif /* (motorC_DeadBand2_4) */

        ctrl = motorC_CONTROL3 & ((uint8 )(~motorC_CTRL_CMPMODE1_MASK));
        motorC_CONTROL3 = ctrl | motorC_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        motorC_RT1 &= ((uint8)(~motorC_RT1_MASK));
        motorC_RT1 |= motorC_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        motorC_RT1 &= ((uint8)(~motorC_SYNCDSI_MASK));
        motorC_RT1 |= motorC_SYNCDSI_EN;

    #elif (motorC_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = motorC_CONTROL & ((uint8)(~motorC_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~motorC_CTRL_CMPMODE1_MASK));
        motorC_CONTROL = ctrl | motorC_DEFAULT_COMPARE2_MODE |
                                   motorC_DEFAULT_COMPARE1_MODE;
    #endif /* (motorC_UsingFixedFunction) */

    #if (!motorC_UsingFixedFunction)
        #if (motorC_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            motorC_AUX_CONTROLDP0 |= (motorC_AUX_CTRL_FIFO0_CLR);
        #else /* (motorC_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            motorC_AUX_CONTROLDP0 |= (motorC_AUX_CTRL_FIFO0_CLR);
            motorC_AUX_CONTROLDP1 |= (motorC_AUX_CTRL_FIFO0_CLR);
        #endif /* (motorC_Resolution == 8) */

        motorC_WriteCounter(motorC_INIT_PERIOD_VALUE);
    #endif /* (!motorC_UsingFixedFunction) */

    motorC_WritePeriod(motorC_INIT_PERIOD_VALUE);

        #if (motorC_UseOneCompareMode)
            motorC_WriteCompare(motorC_INIT_COMPARE_VALUE1);
        #else
            motorC_WriteCompare1(motorC_INIT_COMPARE_VALUE1);
            motorC_WriteCompare2(motorC_INIT_COMPARE_VALUE2);
        #endif /* (motorC_UseOneCompareMode) */

        #if (motorC_KillModeMinTime)
            motorC_WriteKillTime(motorC_MinimumKillTime);
        #endif /* (motorC_KillModeMinTime) */

        #if (motorC_DeadBandUsed)
            motorC_WriteDeadTime(motorC_INIT_DEAD_TIME);
        #endif /* (motorC_DeadBandUsed) */

    #if (motorC_UseStatus || motorC_UsingFixedFunction)
        motorC_SetInterruptMode(motorC_INIT_INTERRUPTS_MODE);
    #endif /* (motorC_UseStatus || motorC_UsingFixedFunction) */

    #if (motorC_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        motorC_GLOBAL_ENABLE |= motorC_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        motorC_CONTROL2 |= motorC_CTRL2_IRQ_SEL;
    #else
        #if(motorC_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            motorC_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            motorC_STATUS_AUX_CTRL |= motorC_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(motorC_interruptState);

            /* Clear the FIFO to enable the motorC_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            motorC_ClearFIFO();
        #endif /* (motorC_UseStatus) */
    #endif /* (motorC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorC_Enable
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
void motorC_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (motorC_UsingFixedFunction)
        motorC_GLOBAL_ENABLE |= motorC_BLOCK_EN_MASK;
        motorC_GLOBAL_STBY_ENABLE |= motorC_BLOCK_STBY_EN_MASK;
    #endif /* (motorC_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (motorC_UseControl || motorC_UsingFixedFunction)
        motorC_CONTROL |= motorC_CTRL_ENABLE;
    #endif /* (motorC_UseControl || motorC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motorC_Stop
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
void motorC_Stop(void) 
{
    #if (motorC_UseControl || motorC_UsingFixedFunction)
        motorC_CONTROL &= ((uint8)(~motorC_CTRL_ENABLE));
    #endif /* (motorC_UseControl || motorC_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (motorC_UsingFixedFunction)
        motorC_GLOBAL_ENABLE &= ((uint8)(~motorC_BLOCK_EN_MASK));
        motorC_GLOBAL_STBY_ENABLE &= ((uint8)(~motorC_BLOCK_STBY_EN_MASK));
    #endif /* (motorC_UsingFixedFunction) */
}

#if (motorC_UseOneCompareMode)
    #if (motorC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motorC_SetCompareMode
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
        void motorC_SetCompareMode(uint8 comparemode) 
        {
            #if(motorC_UsingFixedFunction)

                #if(0 != motorC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorC_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != motorC_CTRL_CMPMODE1_SHIFT) */

                motorC_CONTROL3 &= ((uint8)(~motorC_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                motorC_CONTROL3 |= comparemodemasked;

            #elif (motorC_UseControl)

                #if(0 != motorC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << motorC_CTRL_CMPMODE1_SHIFT)) &
                                                motorC_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & motorC_CTRL_CMPMODE1_MASK;
                #endif /* (0 != motorC_CTRL_CMPMODE1_SHIFT) */

                #if(0 != motorC_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << motorC_CTRL_CMPMODE2_SHIFT)) &
                                               motorC_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & motorC_CTRL_CMPMODE2_MASK;
                #endif /* (0 != motorC_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                motorC_CONTROL &= ((uint8)(~(motorC_CTRL_CMPMODE1_MASK |
                                            motorC_CTRL_CMPMODE2_MASK)));
                motorC_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (motorC_UsingFixedFunction) */
        }
    #endif /* motorC_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (motorC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motorC_SetCompareMode1
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
        void motorC_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != motorC_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorC_CTRL_CMPMODE1_SHIFT)) &
                                           motorC_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & motorC_CTRL_CMPMODE1_MASK;
            #endif /* (0 != motorC_CTRL_CMPMODE1_SHIFT) */

            #if (motorC_UseControl)
                motorC_CONTROL &= ((uint8)(~motorC_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                motorC_CONTROL |= comparemodemasked;
            #endif /* (motorC_UseControl) */
        }
    #endif /* motorC_CompareMode1SW */

#if (motorC_CompareMode2SW)


    /*******************************************************************************
    * Function Name: motorC_SetCompareMode2
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
    void motorC_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != motorC_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << motorC_CTRL_CMPMODE2_SHIFT)) &
                                                 motorC_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & motorC_CTRL_CMPMODE2_MASK;
        #endif /* (0 != motorC_CTRL_CMPMODE2_SHIFT) */

        #if (motorC_UseControl)
            motorC_CONTROL &= ((uint8)(~motorC_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            motorC_CONTROL |= comparemodemasked;
        #endif /* (motorC_UseControl) */
    }
    #endif /*motorC_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!motorC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorC_WriteCounter
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
    void motorC_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(motorC_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: motorC_ReadCounter
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
    uint8 motorC_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(motorC_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(motorC_CAPTURE_LSB_PTR));
    }

    #if (motorC_UseStatus)


        /*******************************************************************************
        * Function Name: motorC_ClearFIFO
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
        void motorC_ClearFIFO(void) 
        {
            while(0u != (motorC_ReadStatusRegister() & motorC_STATUS_FIFONEMPTY))
            {
                (void)motorC_ReadCapture();
            }
        }

    #endif /* motorC_UseStatus */

#endif /* !motorC_UsingFixedFunction */


/*******************************************************************************
* Function Name: motorC_WritePeriod
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
void motorC_WritePeriod(uint8 period) 
{
    #if(motorC_UsingFixedFunction)
        CY_SET_REG16(motorC_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(motorC_PERIOD_LSB_PTR, period);
    #endif /* (motorC_UsingFixedFunction) */
}

#if (motorC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motorC_WriteCompare
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
    void motorC_WriteCompare(uint8 compare) \
                                       
    {
        #if(motorC_UsingFixedFunction)
            CY_SET_REG16(motorC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motorC_COMPARE1_LSB_PTR, compare);
        #endif /* (motorC_UsingFixedFunction) */

        #if (motorC_PWMMode == motorC__B_PWM__DITHER)
            #if(motorC_UsingFixedFunction)
                CY_SET_REG16(motorC_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(motorC_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (motorC_UsingFixedFunction) */
        #endif /* (motorC_PWMMode == motorC__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: motorC_WriteCompare1
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
    void motorC_WriteCompare1(uint8 compare) \
                                        
    {
        #if(motorC_UsingFixedFunction)
            CY_SET_REG16(motorC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(motorC_COMPARE1_LSB_PTR, compare);
        #endif /* (motorC_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: motorC_WriteCompare2
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
    void motorC_WriteCompare2(uint8 compare) \
                                        
    {
        #if(motorC_UsingFixedFunction)
            CY_SET_REG16(motorC_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(motorC_COMPARE2_LSB_PTR, compare);
        #endif /* (motorC_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (motorC_DeadBandUsed)


    /*******************************************************************************
    * Function Name: motorC_WriteDeadTime
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
    void motorC_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!motorC_DeadBand2_4)
            CY_SET_REG8(motorC_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            motorC_DEADBAND_COUNT &= ((uint8)(~motorC_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(motorC_DEADBAND_COUNT_SHIFT)
                motorC_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << motorC_DEADBAND_COUNT_SHIFT)) &
                                                    motorC_DEADBAND_COUNT_MASK;
            #else
                motorC_DEADBAND_COUNT |= deadtime & motorC_DEADBAND_COUNT_MASK;
            #endif /* (motorC_DEADBAND_COUNT_SHIFT) */

        #endif /* (!motorC_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: motorC_ReadDeadTime
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
    uint8 motorC_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!motorC_DeadBand2_4)
            return (CY_GET_REG8(motorC_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(motorC_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(motorC_DEADBAND_COUNT & motorC_DEADBAND_COUNT_MASK)) >>
                                                                           motorC_DEADBAND_COUNT_SHIFT));
            #else
                return (motorC_DEADBAND_COUNT & motorC_DEADBAND_COUNT_MASK);
            #endif /* (motorC_DEADBAND_COUNT_SHIFT) */
        #endif /* (!motorC_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (motorC_UseStatus || motorC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorC_SetInterruptMode
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
    void motorC_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(motorC_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: motorC_ReadStatusRegister
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
    uint8 motorC_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(motorC_STATUS_PTR));
    }

#endif /* (motorC_UseStatus || motorC_UsingFixedFunction) */


#if (motorC_UseControl)


    /*******************************************************************************
    * Function Name: motorC_ReadControlRegister
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
    uint8 motorC_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(motorC_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: motorC_WriteControlRegister
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
    void motorC_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(motorC_CONTROL_PTR, control);
    }

#endif /* (motorC_UseControl) */


#if (!motorC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motorC_ReadCapture
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
    uint8 motorC_ReadCapture(void) 
    {
        return (CY_GET_REG8(motorC_CAPTURE_LSB_PTR));
    }

#endif /* (!motorC_UsingFixedFunction) */


#if (motorC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motorC_ReadCompare
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
    uint8 motorC_ReadCompare(void) 
    {
        #if(motorC_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(motorC_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(motorC_COMPARE1_LSB_PTR));
        #endif /* (motorC_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: motorC_ReadCompare1
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
    uint8 motorC_ReadCompare1(void) 
    {
        return (CY_GET_REG8(motorC_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: motorC_ReadCompare2
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
    uint8 motorC_ReadCompare2(void) 
    {
        return (CY_GET_REG8(motorC_COMPARE2_LSB_PTR));
    }

#endif /* (motorC_UseOneCompareMode) */


/*******************************************************************************
* Function Name: motorC_ReadPeriod
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
uint8 motorC_ReadPeriod(void) 
{
    #if(motorC_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(motorC_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(motorC_PERIOD_LSB_PTR));
    #endif /* (motorC_UsingFixedFunction) */
}

#if ( motorC_KillModeMinTime)


    /*******************************************************************************
    * Function Name: motorC_WriteKillTime
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
    void motorC_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(motorC_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: motorC_ReadKillTime
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
    uint8 motorC_ReadKillTime(void) 
    {
        return (CY_GET_REG8(motorC_KILLMODEMINTIME_PTR));
    }

#endif /* ( motorC_KillModeMinTime) */

/* [] END OF FILE */
