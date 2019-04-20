/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2019 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "dwt_stm32_delay.h"
#include "epd1in54.h"
#include "epdif.h"
#include "epdpaint.h"
#include "imagedata.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define COLORED      0
#define UNCOLORED    1
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t sum, RH, TEMP, TEMP1;
uint8_t check = 0;

GPIO_InitTypeDef GPIO_InitStruct;

void set_gpio_output(void) {
	/*Configure GPIO pin output: PA1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void set_gpio_input(void) {
	/*Configure GPIO pin input: PA2 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DHT11_start(void) {
	set_gpio_output(); // set the pin as output
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0); // pull the pin low
	DWT_Delay_us(18000); // wait for 18ms
	set_gpio_input(); // set as input
}

void check_response(void) {
	DWT_Delay_us(40);
	if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))) {
		DWT_Delay_us(80);
		if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
			check = 1;
	}
	while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
		;   // wait for the pin to go low
}

uint8_t read_data(void) {
	uint8_t i, j;
	for (j = 0; j < 8; j++) {
		while (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
			;   // wait for the pin to go high
		DWT_Delay_us(40);   // wait for 40 us
		if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) == 0)   // if the pin is low
				{
			i &= ~(1 << (7 - j));   // write 0
		} else
			i |= (1 << (7 - j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
			;  // wait for the pin to go low
	}
	return i;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 *
 * @retval None
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	unsigned char* frame_buffer = (unsigned char*) malloc(
	EPD_WIDTH * EPD_HEIGHT / 8);
	char time_string[] = { '0', '0', ':', '0', '0', '\0' };
	unsigned long time_start_ms;
	unsigned long time_now_s;
	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

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
	MX_SPI1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	EPD epd;
	if (EPD_Init(&epd, lut_full_update) != 0) {
		printf("e-Paper init failed\n");
		return -1;
	}

	Paint paint;
	Paint_Init(&paint, frame_buffer, epd.width, epd.height);
	Paint_Clear(&paint, UNCOLORED);

	//expandable code begin
	/* For simplicity, the arguments are explicit numerical coordinates */
	/* Write strings to the buffer */
	Paint_DrawFilledRectangle(&paint, 0, 6, 200, 26, COLORED);
	Paint_DrawStringAt(&paint, 28, 10, "Hello world!", &Font16, UNCOLORED);
	Paint_DrawStringAt(&paint, 30, 30, "e-Paper Demo", &Font16, COLORED);

	/* Draw something to the frame buffer */
	Paint_DrawRectangle(&paint, 10, 60, 50, 110, COLORED);
	Paint_DrawLine(&paint, 10, 60, 50, 110, COLORED);
	Paint_DrawLine(&paint, 50, 60, 10, 110, COLORED);
	Paint_DrawCircle(&paint, 120, 80, 30, COLORED);
	Paint_DrawFilledRectangle(&paint, 10, 130, 50, 180, COLORED);
	Paint_DrawFilledCircle(&paint, 120, 150, 30, COLORED);
	//expandable code end

	/* Display the frame_buffer */
	EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint),
			Paint_GetHeight(&paint));
	EPD_DisplayFrame(&epd);
	EPD_DelayMs(&epd, 10);

	if (EPD_Init(&epd, lut_partial_update) != 0) {
		printf("e-Paper init failed\n");
		return -1;
	}
	/**
	 *  there are 2 memory areas embedded in the e-paper display
	 *  and once the display is refreshed, the memory area will be auto-toggled,
	 *  i.e. the next action of SetFrameMemory will set the other memory area
	 *  therefore you have to set the frame memory and refresh the display twice.
	 */
	EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
	EPD_DisplayFrame(&epd);
	EPD_SetFrameMemory(&epd, IMAGE_DATA, 0, 0, epd.width, epd.height);
	EPD_DisplayFrame(&epd);

	DWT_Delay_Init();
	HAL_Delay(1000);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		DHT11_start();
		/*check_response();
		 Rh_byte1 = read_data();
		 Rh_byte2 = read_data();
		 Temp_byte1 = read_data();
		 Temp_byte2 = read_data();
		 sum = read_data();
		 if (sum == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) // if the data is correct
		 {
		 TEMP = Temp_byte1;	// Temperatura
		 RH = Rh_byte1;	// Wilgotnosc
		 HAL_Delay(1000);
		 }*/

		Paint_SetWidth(&paint, 200);
		Paint_SetHeight(&paint, 200);
		Paint_SetRotate(&paint, ROTATE_180);

		Paint_Clear(&paint, UNCOLORED);
		//Temperature
		Paint_DrawFilledCircle(&paint, 39, 39, 34, COLORED);
		Paint_DrawStringAt(&paint, 15, 23, "Temp:", &Font16, UNCOLORED);
		Paint_DrawStringAt(&paint, 27, 43, "32", &Font16, UNCOLORED);
		//Cisnienie
		Paint_DrawFilledCircle(&paint, 84, 100, 34, COLORED);
		Paint_DrawStringAt(&paint, 60, 83, "Pres:", &Font16, UNCOLORED);
		Paint_DrawStringAt(&paint, 62, 103, "1013", &Font16, UNCOLORED);
		//Wilgotnosc
		Paint_DrawFilledCircle(&paint, 39, 161, 34, COLORED);
		Paint_DrawStringAt(&paint, 15, 145, "Damp:", &Font16, UNCOLORED);
		Paint_DrawStringAt(&paint, 23, 165, "500", &Font16, UNCOLORED);
		//GUI
		Paint_DrawLine(&paint, 1, 100, 200, 1, COLORED);
		Paint_DrawLine(&paint, 1, 101, 200, 2, COLORED);
		Paint_DrawLine(&paint, 1, 100, 200, 200, COLORED);
		Paint_DrawLine(&paint, 1, 99, 200, 199, COLORED);
		Paint_DrawRectangle(&paint, 1, 1, 199, 199, COLORED);
		Paint_DrawRectangle(&paint, 2, 2, 198, 198, COLORED);

		EPD_SetFrameMemory(&epd, frame_buffer, 0, 0, Paint_GetWidth(&paint),
				Paint_GetHeight(&paint));
		EPD_DisplayFrame(&epd);

		EPD_DelayMs(&epd, 10);
	}
	/* USER CODE END 3 */

}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE()
	;

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void) {

	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_1LINE;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

/* USART2 init function */
static void MX_USART2_UART_Init(void) {

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
static void MX_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 BUSY_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | BUSY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : DC_Pin */
	GPIO_InitStruct.Pin = DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : RST_Pin */
	GPIO_InitStruct.Pin = RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI_CS_Pin */
	GPIO_InitStruct.Pin = SPI_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
void _Error_Handler(char *file, int line) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
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
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
