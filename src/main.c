

/* Includes */
#include "stm32f4xx.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */


int main(void)
{
	// Inicializando clock GPIO C
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

		// Saidas
		GPIOC->MODER &= ~(GPIO_MODER_MODER0 |
						GPIO_MODER_MODER1);
		GPIOC->MODER |= (GPIO_MODER_MODER0_0 |
						GPIO_MODER_MODER1_0);

		// Teoricamente 256 e 1999 fazem 60Hz
		TIM10->PSC = 265;
		TIM10->ARR = 1999;
		TIM10->CR1 = TIM_CR1_CEN;

		// Teoricamente 499 e 1999 fazem 16Hz
		TIM11->PSC = 499;
		TIM11->ARR = 1999;
		TIM11->CR1 = TIM_CR1_CEN;


  while (1)
  {

  }
}
