/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This is a lab file with 5 tasks:
  * Task1 – Print integer and float
  * Task2 – UART string input/output
  * Task3 – Matrix multiplication
  * Task4 – Armstrong number check
  * Task5 – Simple Caesar cipher encryption
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN 0 */
void myPrintf(const char *fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len > 0) {
        if (len > sizeof(buffer)) len = sizeof(buffer);
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, HAL_MAX_DELAY);
    }
}

void task1(void)
{
    int x = 42;
    float y = 3.14f;
    int y_int  = (int)y;
    int y_frac = (int)((y - (float)y_int) * 100.0f + 0.5f);

    myPrintf("Task1 -> x = %d, y = %d.%02d\r\n", x, y_int, y_frac);
}

void task2(void)
{
    const char msg[] = "Task2 -> Enter a string: ";
    char rxBuf[32] = {0};
    int idx = 0;
    char ch;

    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    while (1) {
        HAL_UART_Receive(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

        if (ch == '\r' || ch == '\n') {
            rxBuf[idx] = '\0';
            break;
        } else if (idx < sizeof(rxBuf)-1) {
            rxBuf[idx++] = ch;
            HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY); // echo
        }
    }

    myPrintf("\r\nYou typed: %s\r\n", rxBuf);
}

void task3(void)
{
    int a[2][2] = {{1, 2}, {3, 4}};
    int b[2][2] = {{5, 6}, {7, 8}};
    int c[2][2] = {0};

    for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
            for(int k=0; k<2; k++)
                c[i][j] += a[i][k] * b[k][j];

    myPrintf("Task3 -> Matrix multiplication result:\r\n");
    for(int i=0; i<2; i++) {
        myPrintf("[%d %d]\r\n", c[i][0], c[i][1]);
    }
}

void task4(void)
{
    char msg[] = "Task4 -> Enter a number: ";
    char rxBuf[16] = {0};
    int num, original, remainder, result = 0;

    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    int idx = 0;
    char ch;
    while (1) {
        HAL_UART_Receive(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
        if (ch == '\r' || ch == '\n') {
            rxBuf[idx] = '\0';
            break;
        } else if (idx < sizeof(rxBuf)-1) {
            rxBuf[idx++] = ch;
            HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
        }
    }

    num = atoi(rxBuf);
    original = num;

    while (original != 0) {
        remainder = original % 10;
        result += remainder * remainder * remainder;
        original /= 10;
    }

    if (result == num)
        myPrintf("\r\n%d is an Armstrong number\r\n", num);
    else
        myPrintf("\r\n%d is NOT an Armstrong number\r\n", num);
}

void task5(void)
{
    char text[] = "HELLO STM32";
    int shift = 3;
    char enc[32];

    for (int i=0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (ch >= 'A' && ch <= 'Z')
            enc[i] = (ch - 'A' + shift) % 26 + 'A';
        else if (ch >= 'a' && ch <= 'z')
            enc[i] = (ch - 'a' + shift) % 26 + 'a';
        else
            enc[i] = ch;
    }
    enc[strlen(text)] = '\0';

    myPrintf("Task5 -> Original: %s\r\n", text);
    myPrintf("Task5 -> Encrypted: %s\r\n", enc);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  while (1)
  {
    // Uncomment ONE task at a time
    //task1();
    //task2();
    //task3();
    //task4();
    task5();

    HAL_Delay(1000);
  }
}

/**
  * @brief System Clock Configuration (72 MHz from HSI)
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Configure HSI and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 8 MHz × 9 = 72 MHz
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* Configure bus clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // max 36 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
