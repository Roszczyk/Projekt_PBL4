/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
//#include<stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUFFER_SIZE 128
#define receive_size 1

#define RxBuf_SIZE 10
#define MainBuf_SIZE 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char ATcommand[64];
uint16_t ble_msg_len;
uint16_t i = 0;
char buffer[BUFFER_SIZE];
int bufferIndex = 0;
uint8_t received_byte[10];
uint8_t received_mess[100];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uint8_t RxBuf[RxBuf_SIZE];
//uint8_t MainBuf[MainBuf_SIZE];
//HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart, pUART_RxEventCallbackTypeDef pCallback)
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}
void SendMSG(char* cmd){
	  ble_msg_len= strlen(cmd);

	  strcpy(ATcommand,cmd);
	  ATcommand[ble_msg_len++]='\r';
//

//      received_byte[9] = '\0'; // Null-terminate the buffer
//	  memset( received_byte, 0, 10);
	  HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,ble_msg_len,1000);
	  HAL_Delay(1000);
	  for(uint8_t u; u<64; u++){
		  ATcommand[u]=0;
	  }


}


void ReadBuffer(void);

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuf , RxBuf_SIZE);
  //HAL_UART_Receive_DMA(&huart1, (uint8_t *) dataOut, 64);
  HAL_Delay(800);
  HAL_UART_Receive_IT(&huart1,  received_byte,2);
  //HAL_UART_IRQHandler(&huart1);
  HAL_Delay(800);
  printf("Test UART\r\n");
  uint8_t status;
  SendMSG("AT");

  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("AT%S 0=\"Cuk\""); // ustawienie device name

  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("AT%S 1=\"2154657374776961646F6D6F73636921\""); //UUID

  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("ATS 100=1"); // VSP And start advertow

  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("ATS 100?"); // VSP And start advertow

  ReadBuffer();
  HAL_Delay(1000);
  ReadBuffer();
  SendMSG("ATS 101=4"); // 4dBm

  ReadBuffer();
  HAL_Delay(800);
  SendMSG("ATS 103=0"); // nazwe ustawia na ta wczesniej wpisana

  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("ATS 106=100"); // ustawia 100 sekund na skanowanie
//  HAL_Delay(800);
//  SendMSG("ATS 108=3"); // ustawia tryb advertow dla nonvsp



  ReadBuffer();
  HAL_Delay(1000);
  SendMSG("ATS 109=1");

  HAL_Delay(1000);
  ReadBuffer();
  SendMSG("ATS 201=200");  // interwal advertow vsp

  HAL_Delay(1000);
  ReadBuffer();
  SendMSG("ATS 204=256");  // tx bufor vsp

  HAL_Delay(1000);
  ReadBuffer();
  SendMSG("ATS 205=256");  // rx bufor vsp

  HAL_Delay(1000);
  ReadBuffer();
  SendMSG("ATS 301=7500");  //vsp connection interval

  HAL_Delay(1000);

  ReadBuffer();
  SendMSG("AT&W"); // zapis rejestru S
  HAL_Delay(1000);


  ReadBuffer();

  SendMSG("ATZ"); //restart
  HAL_Delay(1000);
  ReadBuffer();

  printf("Kuniec \r\n");
  //SendMSG("AT+LSCN, 500"); //start scanowania do vsp z limitem 500 sekund

  HAL_Delay(800);

  ReadBuffer();
 // SendMSG("AT+LADV 3,500");
  //status = HAL_UART_Transmit(&huart2, pBuff, Size, 0xFFFF);
  //send_string("AT\r");

 // sprintf(ATcommand,"");
 // status = HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,strlen(ATcommand),1000);
 //printf("STATUS : %d", status);
////  while(all_rx[rx_cnt] != 0x0D){
//	  HAL_UART_Receive_IT(&huart1,  &received_byte, 1);
//	  //strcat(all_rx, &received_byte);
//	  all_rx[rx_cnt] = received_byte;
//	  printf("Plz to wynik %s\r\n",all_rx);
//	  rx_cnt++;
 // }



//  HAL_Delay(1000);
//  InitBLEAddrTable();
//  HAL_Delay(100);
//  Configure_BLE();
//  HAL_Delay(100);
//  StartAdverts();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t Test[10] = "testowy";
  HAL_UART_Transmit(&huart1,(uint8_t *)Test,10,1000);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  printf("%s", buffer);


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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	HAL_UART_Receive_IT(&huart1,received_byte,2);

	buffer[bufferIndex] = received_byte[0];
	bufferIndex++;
	buffer[bufferIndex] = received_byte[1];
	bufferIndex++;
}
//	while(received_byte[i] != '\r'){
//		buffer[bufferIndex] = received_byte[i];
//		bufferIndex++;
//		i++;
//	}
//	if(received_byte[i] == '\r'){
//		printf("Otrzymany bajt: %s",buffer);
//		buffer[bufferIndex] = '\0'; // Null-terminate the buffer
//        memset(buffer, 0, BUFFER_SIZE);
//        received_byte[i] = '\0'; // Null-terminate the buffer
//    	memset(received_byte, 0, receive_size);
//    	i=0;
//	}
////
//	   if (received_byte[1] == '\r' )
//	    {
//	      // End of message reached
//		  printf("Otrzymany bajt: %s",buffer);
//	    //  buffer[bufferIndex] = '\0'; // Null-terminate the buffer
////	      memset(buffer, 0, BUFFER_SIZE);
////	      bufferIndex = 0;
//
//	    }
//	    else
//	    {
//	      // Add the character to the buffer
//	      if (bufferIndex < BUFFER_SIZE - 1)
//	      {
//	        buffer[bufferIndex] = received_byte[0];
//	        bufferIndex++;
//	        buffer[bufferIndex] = received_byte[1];
//	        bufferIndex++;
//
//
//
//	      }
//	    }
int o = 1;
void ReadBuffer(void){
	//printf("%d.", o);
	printf(" %s", buffer);
	memset(buffer, 0, 127);
	bufferIndex = 0;
	//o++;
}





/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
