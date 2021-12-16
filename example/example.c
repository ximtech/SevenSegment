
#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);


#include "../lib/SevenSegment.h"

#define AT_THIRD_LED_DIGIT 3

volatile uint16_t number = 0;
volatile uint16_t counter = 0;

void TIM2Callback() {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        LL_TIM_ClearFlag_UPDATE(TIM2);

        if (counter < 100) {    // blink with decimal point at third digit
            printfSevenSegment("%04d", number);
        } else {
            printfSevenSegmentWithDP(AT_THIRD_LED_DIGIT, "%04d", number);
        }

        if (counter > 200) {
            counter = 0;
        }
        counter++;
    }
}

void showWelcomeMessage(char *message, uint16_t delay) {
    for (int i = 0; i < delay; ++i) {
        printSevenSegment(message);
        LL_mDelay(1);
    }
}

int main(void) {


    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    SystemClock_Config();
    MX_TIM2_Init();

    // MX_GPIO_Init();


    initSevenSegment(COMMON_CATHODE, FOUR_DIGITS, DIGIT_4_Pin, DIGIT_3_Pin, DIGIT_2_Pin, DIGIT_1_Pin);
    setLeadingZeros(true);

    uint16_t delay = 200;

    showWelcomeMessage("C", delay);
    showWelcomeMessage("CO", delay);
    showWelcomeMessage("COO", delay);
    showWelcomeMessage("COOL", delay);
    showWelcomeMessage("OOL ", delay);
    showWelcomeMessage("OL  ", delay);
    showWelcomeMessage("L   ", delay);
    showWelcomeMessage("    ", delay);

    showWelcomeMessage("L   ", delay);
    showWelcomeMessage("OL  ", delay);
    showWelcomeMessage("OOL ", delay);
    showWelcomeMessage("COOL", delay);
    showWelcomeMessage("COO", delay);
    showWelcomeMessage("CO", delay);
    showWelcomeMessage("C", delay);
    showWelcomeMessage("    ", delay);


    for (int i = 0; i < 1000; ++i) {
        printSevenSegmentWithDP("1234", AT_THIRD_LED_DIGIT);
        LL_mDelay(2);
    }

    for (int i = 0; i < 1000; ++i) {
        printfSevenSegmentWithDP(AT_THIRD_LED_DIGIT, "F%d", 455);
        LL_mDelay(2);
    }

    LL_TIM_EnableIT_UPDATE(TIM2);
    LL_TIM_EnableCounter(TIM2);

    while (1) {

        number++;
        LL_mDelay(100);

        if (number > 10000) {
            number = 0;
        }

    }
}

void SystemClock_Config(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1) {

    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_12, 96, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {

    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {

    }
    LL_Init1msTick(100000000);
    LL_SetSystemCoreClock(100000000);
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

static void MX_TIM2_Init(void) {

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    LL_TIM_InitTypeDef TIM_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    /* TIM2 interrupt Init */
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(TIM2_IRQn);

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    TIM_InitStruct.Prescaler = 4999;
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 49;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM2, &TIM_InitStruct);
    LL_TIM_DisableARRPreload(TIM2);
    LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
    LL_TIM_DisableMasterSlaveMode(TIM2);
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */

}

static void MX_GPIO_Init(void) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    /**/
    LL_GPIO_ResetOutputPin(GPIOA, SEGMENT_A_Pin | SEGMENT_B_Pin | SEGMENT_C_Pin | SEGMENT_D_Pin | SEGMENT_E_Pin | SEGMENT_F_Pin | SEGMENT_G_Pin |
                                  SEGMENT_DP_Pin);

    /**/
    LL_GPIO_ResetOutputPin(GPIOB, DIGIT_1_Pin | DIGIT_2_Pin | DIGIT_3_Pin | DIGIT_4_Pin);

    /**/
    GPIO_InitStruct.Pin = SEGMENT_A_Pin | SEGMENT_B_Pin | SEGMENT_C_Pin | SEGMENT_D_Pin
                          | SEGMENT_E_Pin | SEGMENT_F_Pin | SEGMENT_G_Pin | SEGMENT_DP_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = DIGIT_1_Pin | DIGIT_2_Pin | DIGIT_3_Pin | DIGIT_4_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

