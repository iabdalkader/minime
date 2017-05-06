#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_tim.h"
#include "system_stm32f0xx.h"

extern void SystemClock_Config(void);
static GPIO_InitTypeDef  GPIO_InitStruct;

void Error_Handler()
{
    /* -1- Enable GPIO Clock (to be able to program the configuration registers) */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* -2- Configure IO in output push-pull mode to drive external LEDs */
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    while (1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(150);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(150);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_Delay(600);
    }
}

int main(void)
{
    /* This sample code shows how to use GPIO HAL API to toggle LED3 IO
       in an infinite loop. */

    /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
       can eventually implement his proper time base source (a general purpose 
       timer for example or other time source), keeping in mind that Time base 
       duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
       handled in milliseconds basis.
       - Low Level Initialization
     */
    HAL_Init();

    /* Configure the system clock to 48 MHz */
    SystemClock_Config();

#if 0
    /* -1- Enable GPIO Clock (to be able to program the configuration registers) */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* -2- Configure IO in output push-pull mode to drive external LEDs */
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    while (1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(150);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(150);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_Delay(600);
    }
#endif

    /* Compute the prescaler value to have TIM3 counter clock equal to 16000000 Hz */
    uint32_t uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;

    GPIO_InitTypeDef   GPIO_InitStruct;
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIMx Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Enable GPIO Channels Clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Common configuration for all channels */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
       TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.

       In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),
       since APB1 prescaler is equal to 1.
       TIM3CLK = PCLK1
       PCLK1 = HCLK
       => TIM3CLK = HCLK = SystemCoreClock

       To get TIM3 counter clock at 16 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock) /16 MHz) - 1

       To get TIM3 output clock at 24 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1
       = 665

       TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR + 1)* 100 = 50%
       TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR + 1)* 100 = 37.5%
       TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR + 1)* 100 = 25%

       Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
       1) by calling CMSIS function SystemCoreClockUpdate()
       2) by calling HAL API function HAL_RCC_GetSysClockFreq()
       3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
       ----------------------------------------------------------------------- */

     /* 
       Initialize TIMx peripheral as follows:
       + Prescaler = (SystemCoreClock / 16000000) - 1
       + Period = (666 - 1)
       + ClockDivision = 0
       + Counter direction = Up
     */
    #define  PERIOD_VALUE        (uint32_t)(666 - 1)  /* Period Value  */
    #define  PULSE_VALUE_B       (uint32_t)(10)        /* Capture Compare 1 Value  */
    #define  PULSE_VALUE_G       (uint32_t)(PERIOD_VALUE*37.5/100) /* Capture Compare 2 Value  */
    #define  PULSE_VALUE_R       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 3 Value  */

    TIM_HandleTypeDef    TimHandle;
    TIM_OC_InitTypeDef sConfig;

    TimHandle.Instance = TIM1;
    TimHandle.Init.Prescaler         = uhPrescalerValue;
    TimHandle.Init.Period            = PERIOD_VALUE;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Configure the PWM channels #########################################*/
    /* Common configuration for all channels */
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    /* Set the pulse value for channel 1 */
    sConfig.Pulse = PULSE_VALUE_B;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);

    /* Set the pulse value for channel 2 */
    sConfig.Pulse = PULSE_VALUE_G;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);

    /* Set the pulse value for channel 3 */
    sConfig.Pulse = PULSE_VALUE_R;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);

    ///*##-3- Start PWM signals generation #######################################*/
    ///* Start channel 1 */
    //HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    ///* Start channel 2 */
    //HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
    ///* Start channel 3 */
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);

    while (1) {
        for (int i=640; i>1; i-=3) {
            HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_3);
            sConfig.Pulse = i;
            HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
            HAL_Delay(5);
        }
        for (int i=1; i<640; i+=3) {
            /* Set the pulse value for channel 3 */
            HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_3);
            sConfig.Pulse = i;
            HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
            HAL_Delay(5);
        }
    }
}

