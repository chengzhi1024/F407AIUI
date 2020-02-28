/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "aiui.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t a[20480];
uint8_t b[20480];


//void rx_it_init() {
//    HAL_UART_Receive_IT(&huart1, a, 1);
//    HAL_UART_Receive_IT(&huart2, b, 1);
//    vTaskDelete(NULL);
//}
//
//void rx_it_deinit() {
//
//    vTaskDelete(NULL);
//}
//
//void set_wifi() {
//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
//    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
//    wifi_init();
//    vTaskDelete(NULL);
//}
//
//void get_rtc() {
//    get_rtc_time();
//    vTaskDelete(NULL);
//}
//
//void aiui_link() {
//    aiui();
//    vTaskDelete(NULL);
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
    setvbuf(stdout, NULL, _IONBF, 0);
    HAL_UART_Receive_IT(&huart1, a, 1);
    HAL_UART_Receive_IT(&huart2, b, 1);

    wifi_init();
//    get_rtc_time();
    aiui();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//    xTaskCreate(rx_it_init, "rx_it_init", 128, NULL, 11, NULL);
//    xTaskCreate(rx_it_deinit, "rx_it_deinit", 128, NULL, 11, NULL);

//    xTaskHandle xHandle_set_wifi;
//    xTaskHandle xHandle_get_rtc;
//    xTaskHandle xHandle_aiui_connection;

//    //配网
//    xTaskCreate(set_wifi, "set_wifi", 128, NULL, 6, &xHandle_set_wifi);
//    //获取实时时间（ntp)
//    xTaskCreate(get_rtc, "get_rtc", 128, NULL, 5, &xHandle_get_rtc);
    //连接语音
//    xTaskCreate(aiui_link, "aiui_link", 128, NULL, 4, &xHandle_aiui_connection);
//
//    vTaskStartScheduler();
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */

//HAL_StatusTypeDef HAL_RTCEx_SetSecond_IT(RTC_HandleTypeDef *hrtc){
//    RTC_TimeTypeDef stTime = {0};
//    RTC_DateTypeDef stDate = {0};
//    HAL_RTC_GetDate(hrtc, &stDate, RTC_FORMAT_BIN);
//    printf("20%02d:%02d:%02d:%02d\t", stDate.Year, stDate.Month, stDate.Date, stDate.WeekDay);
//    HAL_RTC_GetTime(hrtc, &stTime, RTC_FORMAT_BIN);
//    printf("%02d:%02d:%02d\r\n", stTime.Hours, stTime.Minutes, stTime.Seconds);
//
//}

//在串口中断回调函数中操作
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    static int i = 0;
    static int j = 0;
    if (huart->Instance == USART1) {
        if (i < 20480) {
            HAL_UART_Transmit(&huart1, &a[i], 1, 0);
            HAL_UART_Receive_IT(&huart2,&a[i+1], 1);
            if (i == 20479) {
                i = 0;
            } else {
                i++;
            }
        }
    } else if (huart->Instance == USART2) {
        if (j < 20480) {
            HAL_UART_Transmit(&huart1, &b[j], 1, 0);
            HAL_UART_Receive_IT(&huart2, &b[j+1], 1);
            if (j == 20479) {
                j = 0;
            } else {
                j++;
            }
        }
    }
}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    printf("HELLO\r\n");
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc) {
    RTC_TimeTypeDef stTime = {0};
    RTC_DateTypeDef stDate = {0};
    HAL_RTC_GetTime(hrtc, &stTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &stDate, RTC_FORMAT_BIN);

    HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR2, (uint16_t) stDate.Year);
    HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR3, (uint16_t) stDate.Month);
    HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR4, (uint16_t) stDate.Date);
    HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR5, (uint16_t) stDate.WeekDay);


    printf("20%02d:%02d:%02d:%02d\t", stDate.Year, stDate.Month, stDate.Date, stDate.WeekDay);
    printf("%02d:%02d:%02d\r\n", stTime.Hours, stTime.Minutes, stTime.Seconds);
    printf("\r\n");


}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //判断按键是否按下
    if (GPIO_Pin == GPIO_PIN_2) {
        //延时 20ms 消除抖动
//        my_delay_us(20000);
        //再次判断按键是否按下
        if (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)) {
            //�?????????????????????????????????????????�????????????????????????????????????????? LED
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            //等待按键松变为为低电�????????????????????????????????????????? 则跳出循�?????????????????????????????????????????
            while (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2));
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
