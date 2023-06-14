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
#include "bme280.h"
#include "bma456.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define BUFFER_SIZE 128
//#define receive_size 1
#define BITES_TO_SEND 8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
//char ATcommand[64];
uint16_t ble_msg_len;
uint16_t i = 0;
//char buffer[BUFFER_SIZE];
//int bufferIndex = 0;
//uint8_t received_byte[10];
char Temp[10];
char Hum[10];
uint16_t adc_buffer[32];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SENSOR_BUS hspi1;
struct bme280_dev dev;
struct bme280_data comp_data;
void BMA456_SPI_Read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len_addr, uint32_t len_data);
void BMA456_SPI_Write(uint8_t reg_addr, uint8_t reg_data);

void Tensometr();
void Sleep();
void BME_PRINT();
void BMA_PRINT();
uint8_t BMA456_Check_Connection();
void BMA456_Get_Sensor_Data(uint8_t *data_read);
uint8_t data_read[BITES_TO_SEND];
int16_t data_read_readable[3];
char axes[]={'x', 'y','z'};
//void SendMSG(char* cmd){
//	  ble_msg_len= strlen(cmd);
//
//	  strcpy(ATcommand,cmd);
//	  ATcommand[ble_msg_len++]='\r';
////
//
////      received_byte[9] = '\0'; // Null-terminate the buffer
////	  memset( received_byte, 0, 10);
//	  HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,ble_msg_len,HAL_MAX_DELAY);
//	  HAL_Delay(1000);
//	  for(uint8_t u; u<64; u++){
//		  ATcommand[u]=0;
//	  }
//
//
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
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
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(800);
  HAL_ADC_Start_DMA(&hadc, adc_buffer,32);
  //HAL_UART_Receive_IT(&huart1,  received_byte,2);
  HAL_Delay(800);
  int8_t rslt_bme = bme280_init(&dev);
  if (rslt_bme != BME280_OK)
  {
	  while(1);
  }
  HAL_Delay(100);
  uint8_t rslt_bma = BMA456_Check_Connection();
  if(rslt_bma != 1)
  {
	  while(1);
  }
  HAL_Delay(100);
  stream_sensor_data_normal_mode(&dev);
  BMA456_Init();
//  HAL_TIM_Base_Start_IT(&htim6);
  //uint8_t rslt_bme = BME280_Check_Connection(&hspi1);
//  HAL_Delay(1000);
//  HAL_UART_Transmit(&huart1,"^", 1,250);
//  HAL_Delay(250);
//  HAL_UART_Transmit(&huart1,"^", 1,250);
//  HAL_Delay(250);
//  HAL_UART_Transmit(&huart1,"^", 1,250);
//  HAL_Delay(250);
//  HAL_UART_Transmit(&huart1,"^", 1,250);
//  HAL_Delay(1000);
//  SendMSG("AT");
//
//
//  HAL_Delay(1000);
//  SendMSG("AT&F");
//
//
//  HAL_Delay(1000);
//  SendMSG("AT%S 0=\"SweetDreams\""); // ustawienie device name
//
//
//  HAL_Delay(1000);
//  SendMSG("AT%S 1=\"2154657374776961646F6D6F73636921\""); //UUID
//
//
//  HAL_Delay(1000);
//  SendMSG("ATS 100=1"); // VSP And start advertow
//
//
//  HAL_Delay(1000);
//  SendMSG("ATS 100=?"); // VSP And start advertow
//
//
//  HAL_Delay(1000);
//
//  SendMSG("ATS 101=4"); // 4dBm
//
//  HAL_Delay(800);
//  SendMSG("ATS 103=0"); // nazwe ustawia na ta wczesniej wpisana
//
//
//  HAL_Delay(1000);
//  SendMSG("ATS 106=100"); // ustawia 100 sekund na skanowanie
////  HAL_Delay(800);
////  SendMSG("ATS 108=3"); // ustawia tryb advertow dla nonvsp
//
//
//
//
//  HAL_Delay(1000);
//  SendMSG("ATS 109=0");
//  HAL_Delay(1000);
//  SendMSG("ATS 109?");
//  //adc_buffer;
//  HAL_Delay(1000);
// // received_byte[0];
//  SendMSG("ATS 201=200");  // interwal advertow vsp
//
//  HAL_Delay(1000);
//
//  SendMSG("ATS 204=256");  // tx bufor vsp
//
//  HAL_Delay(1000);
//
//  SendMSG("ATS 205=256");  // rx bufor vsp
//
//  HAL_Delay(1000);
//
//  SendMSG("ATS 301=7500");  //vsp connection interval
//
//  HAL_Delay(1000);
//
//
//  SendMSG("AT&W"); // zapis rejestru S
//  HAL_Delay(1000);
//
//
//  SendMSG("ATZ"); //restart
//  HAL_Delay(1000);

//
//  printf("Kuniec \r\n");
  //SendMSG("AT+LSCN, 500"); //start scanowania do vsp z limitem 500 sekund

  HAL_Delay(800);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  uint8_t Sum[2];
//  Sum[0] = rslt_a;
//  Sum[1] = rslt_e;
 // char Sum_s[2][4];



//  itoa(rslt_bma, Sum_s[0], 10);
//  itoa(rslt_bme, Sum_s[1], 10);
//
//  for(int i=strlen(Sum_s[0]); i<=3; i++){
//	  Sum_s[0][i]=' ';
//  }
//
//  for(int i=strlen(Sum_s[1]); i<=3; i++){
//	  Sum_s[1][i]=' ';
//  }


  while (1)
  {
	  	HAL_Delay(100);
	  	Tensometr(adc_buffer);
	  	HAL_Delay(100);
	  	BME_PRINT();
	 	HAL_Delay(200);
	 	BMA_PRINT();
	 	HAL_Delay(200);
		Sleep();
		HAL_Delay(100);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x5A56, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
  huart1.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Tensometr(){
	float suma = 0;
	short wynik;
	short counter = 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(1000);
	for(uint8_t i; i<sizeof(adc_buffer); i++){
		suma += adc_buffer[i];
		if(adc_buffer[i]!=0){
			counter++;
		}
	}
	suma=suma/counter;
	if(suma < 24000){
		wynik = 0;
	}
	else{
		wynik = 1;
	}
	if(wynik){
	  HAL_UART_Transmit(&huart1,"Lezy\r\n", 6,HAL_MAX_DELAY);
	}
	else{
	  HAL_UART_Transmit(&huart1,"Nie lezy\r\n", 10,HAL_MAX_DELAY);
	}
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

}
void Sleep(){
	HAL_UART_Transmit(&huart1,"Ide spac\r\n", 10,HAL_MAX_DELAY);
	HAL_SuspendTick();
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x5A56, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	SystemClock_Config();
    HAL_ResumeTick();
    HAL_UART_Transmit(&huart1,"Wstaje\r\n", 8,HAL_MAX_DELAY);
}
void BME_PRINT(){
	char TempCala[20] = "Temperatura: ";
	char HumCala[20] = "Wilgotnosc: ";

	bme280_get_sensor_data(BME280_HUM, &comp_data,&dev);
	sprintf(Temp, "%f",comp_data.temperature);
	sprintf(Hum, "%f",comp_data.humidity);
	strncat(TempCala,Temp,4);
	strncat(HumCala,Hum,4);
	strcat(TempCala,"\r\n");
	strcat(HumCala,"\r\n");
	HAL_UART_Transmit(&huart1,TempCala,19,HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1,HumCala,18,HAL_MAX_DELAY);

}
char x[10];
char y[10];
char z[10];
void measureAcc(int16_t *data_read_readable, uint8_t *data_read)
{
	BMA456_Get_Sensor_Data(data_read);
	data_read_readable[0] = (int16_t)data_read[1+2]*256 + data_read[0+2];
	data_read_readable[1] = (int16_t)data_read[3+2]*256 + data_read[2+2];
	data_read_readable[2] = (int16_t)data_read[5+2]*256 + data_read[4+2];
	HAL_Delay(100);
}
void BMA_PRINT(){
	char Grawitacja[20] = "Grawitacja: ";
//	char yCala[20] = "y: ";
//	char zCala[20] = "z: ";
//	for (int i=0; i<7;i++){
//		data_read[i]=0xff;
//	}
//	BMA456_Get_Sensor_Data(data_read);
//	int16_t data_read_readable[3];
//	data_read_readable[0] = (int16_t)data_read[1+2]*256 + data_read[0+2];
//	data_read_readable[1] = (int16_t)data_read[3+2]*256 + data_read[2+2];
//	data_read_readable[2] = (int16_t)data_read[5+2]*256 + data_read[4+2];
//	sprintf(x, "%d",data_read_readable[0]);
//	sprintf(y, "%d",data_read_readable[1]);
//	sprintf(z, "%d",data_read_readable[2]);
//	strncat(xCala,x,5);
//	strncat(yCala,y,5);
//	strncat(zCala,z,5);
//	strcat(xCala,"\r\n");
//	strcat(yCala,"\r\n");
//	strcat(zCala,"\r\n");
//	HAL_UART_Transmit(&huart1,xCala,10,HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1,yCala,10,HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1,zCala,10,HAL_MAX_DELAY);
	measureAcc(data_read_readable, data_read);
	  for (int i=0; i<3;i++){
		  if(data_read_readable[i]>1900 && data_read_readable[i]<2500 && data_read_readable[i]>data_read_readable[(i+1)%3] && data_read_readable[i]>data_read_readable[(i+2)%3]){
		  //printf("Grawitacja: %c\r\n", axes[i]);
		 // strncat(Grawitacja,axes[i],1);
	      Grawitacja[12] = axes[i];
		  strcat(Grawitacja,"\r\n");
		  HAL_UART_Transmit(&huart1,Grawitacja,15,HAL_MAX_DELAY);
	  }
		  if(data_read_readable[i]<-1900 && data_read_readable[i]>-2500 && data_read_readable[i]<data_read_readable[(i+1)%3] && data_read_readable[i]<data_read_readable[(i+2)%3]){
		  		 // printf("Grawitacja: - %c\r\n", axes[i]);
			  strcat(Grawitacja," - ");
			  Grawitacja[15] = axes[i];
			  strcat(Grawitacja,"\r\n");
			  HAL_UART_Transmit(&huart1,Grawitacja,18,HAL_MAX_DELAY);
		  	  }
	  }
}
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){


}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){


}
void BME280_SPI_Read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len)
{
    reg_addr = reg_addr | 0x80;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 1000);
	HAL_SPI_Receive(&hspi1, reg_data, len, 1000);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void BME280_SPI_Write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len)
{
    reg_addr = reg_addr & 0x7F;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 1000);
    HAL_SPI_Transmit(&hspi1, reg_data, len, 1000);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
	//int8_t rslt;
	uint8_t settings_sel;
	//struct bme280_data comp_data;

	/* Recommended mode of operation: Indoor navigation */
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_NO_OVERSAMPLING; // BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	//dev->settings.filter = BME280_FILTER_COEFF_16;
	//dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	//settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel = BME280_OSR_TEMP_SEL; // |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	bme280_set_sensor_settings(settings_sel, dev);
	bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

}

uint8_t BMA456_Check_Connection()
{
	uint8_t reg_addr = 0x00;
	reg_addr |= 0x80;
	uint8_t reg_addr_tab[2];
	reg_addr_tab[0] = reg_addr;
	uint8_t reg_data[2];

	do{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    HAL_SPI_TransmitReceive(&hspi1, reg_addr_tab,reg_data, 2, 1000);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	}while(reg_data[1] != 0x16);

    return 1;

}

void BMA456_Init()
{
//  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

	//BMA456_SPI_Write(BMA456_PWR_CONF, 0x02, &hspi2);
	//HAL_Delay(10);

	//BMA456_SPI_Write(BMA456_PWR_CTRL, 0x00, &hspi2);
	//HAL_Delay(10);

	BMA456_SPI_Write(BMA456_CMD, 0xB6);
	HAL_Delay(10);

	BMA456_SPI_Write(BMA456_PWR_CONF, 0x00);
	HAL_Delay(10);

	BMA456_SPI_Write(BMA456_INIT_CTRL, 0x00);
	HAL_Delay(10);

	BMA456_SPI_Write(0x41, 0x03);
	HAL_Delay(10);

	BMA456_SPI_Write(BMA456_INIT_CTRL, 0x01);
	HAL_Delay(10);

	HAL_Delay (150);

	BMA456_SPI_Write(BMA456_PWR_CTRL, 0x04);
	HAL_Delay(10);

	BMA456_SPI_Write(BMA456_ACC_CONF, 0x17);
	HAL_Delay(10);

	BMA456_SPI_Write(BMA456_PWR_CONF, 0x03);
	HAL_Delay(10);
}

void BMA456_Get_Sensor_Data(uint8_t *data_read)
{
	uint8_t reg_addr = 0x12;
	reg_addr |= 0x80;
	uint8_t data_sent[BITES_TO_SEND];
	data_sent[0] = reg_addr;

	//while(1){
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi1, data_sent, data_read, BITES_TO_SEND, 100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);



	//}
}
void BMA456_SPI_Read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len_addr, uint32_t len_data)
{
	reg_addr |= 0x80;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg_addr, len_addr, 1000);
	HAL_SPI_Receive(&hspi1, reg_data, len_data, 1000);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

}
void BMA456_SPI_Write(uint8_t reg_addr, uint8_t reg_data)
{
	uint8_t reg_data_tab[2];
	reg_data_tab[0] = reg_addr & 0x7F;
	reg_data_tab[1] = reg_data;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi1, reg_data_tab, 2, 1000);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}
//short FunctionChoiceFlag = 0;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
//{
//
//	if (htim == &htim6)
//	  {
//		if(FunctionChoiceFlag == 0){
//			BME_PRINT();
//			FunctionChoiceFlag = 1;
//		}
//		else if(FunctionChoiceFlag == 1){
//			Tensometr(adc_buffer);
//			FunctionChoiceFlag = 2;
//		}
//		else if(FunctionChoiceFlag == 2){
//			Sleep();
//			FunctionChoiceFlag = 0;
//		}
////		BME_PRINT();
////		HAL_Delay(1000);
////		Tensometr(adc_buffer);
////		HAL_Delay(100);
////		Sleep();
////		HAL_Delay(100);
//	  }
//
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//
//	HAL_UART_Receive_IT(&huart1,received_byte,2);
//
//	buffer[bufferIndex] = received_byte[0];
//	bufferIndex++;
//	buffer[bufferIndex] = received_byte[1];
//	bufferIndex++;
//}
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
