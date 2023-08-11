

/* Includes */
#include "stm32f4xx.h"

/* Private macro */
#define PINO0 GPIO_ODR_ODR_0
#define PINO1 GPIO_ODR_ODR_1
#define TEMPO_ESTADO 6
/* Private variables */
uint8_t contador = 0;
uint8_t segundos = 0;
uint8_t impares = 1;
uint8_t pares = 1;
uint8_t estado = 0;
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

		// Teoricamente 132 e 1999 fazem 120Hz Para piscar rapido e fazer mux
		//(16Mhz / 120 hz)/ (1999 + 1) = 132 + 1
		TIM10->PSC = 132;
		TIM10->ARR = 1999;
		TIM10->CR1 = TIM_CR1_CEN;

		// Teoricamente 499 e 1999 fazem 16Hz
		//(16Mhz / 16 hz)/ (1999 + 1) = 499 + 1
		TIM11->PSC = 499;
		TIM11->ARR = 1999;
		TIM11->CR1 = TIM_CR1_CEN;


  while (1)
  {
	  if(TIM10->SR & TIM_SR_UIF){// confere se a flag do timer é '1', "terminou o periodo"
		  TIM10->SR &= ~TIM_SR_UIF;// "zera" a flag permitindo que o timer conte novamente
		  if(pares == 1){
			  if (!((GPIOC -> ODR & PINO1) && ((GPIOC -> ODR & PINO0) == 0))){//se o pino1 está ligado e o pino0 desligado, n entra no if
				  if(GPIOC -> ODR & PINO0 ){
					  GPIOC -> ODR &= ~PINO0;
				  }else{
					  GPIOC -> ODR |= PINO0;
				  }
				}
		  }else{
			  GPIOC -> ODR &= ~PINO0;//garante que o pino esteja desligado para a troca de estados
		  }
		  if(impares == 1){
			  if (!((GPIOC -> ODR & PINO0) && ((GPIOC -> ODR & PINO1) == 0))){//se o pino0 está ligado e o pino1 desligado, n entra no if
				  if(GPIOC -> ODR & PINO1 ){
					  GPIOC -> ODR &= ~PINO1;
				  }else{
					  GPIOC -> ODR |= PINO1;
				  }
			  }
		  }else{
			  GPIOC -> ODR &= ~PINO1;//garante que o pino esteja desligado para a troca de estados
		  }
	  }

	  if(TIM11->SR & TIM_SR_UIF){// confere se a flag do timer é '1', "terminou o periodo"
		  TIM11->SR &= ~TIM_SR_UIF;// "zera" a flag permitindo que o timer conte novamente
		  contador++;
		  if (contador >= 16) {//quantidade = frequencia do timer11 para dar 1 segundo
			  contador = 0;
			  segundos++;
		  }
		  switch(estado){

			  case 0:
			  default:
				  pares = 1;
			  	  impares= 1;
			  	  if(segundos >= TEMPO_ESTADO){
			  		  estado = 1;//garantir que sempre vai voltar para o primeiro estado
			  		  segundos = 0;
			  	  }
				  break;

			  case 1:
				  impares = 0;
				  if (contador % 2){
					  if(pares == 1){
						  pares = 0;
					  }else{
						  pares = 1;
					  }
				  }
				  if(segundos >= TEMPO_ESTADO){
					  estado ++;
					  segundos = 0;
				  }
				  break;

			  case 2:
				  pares = 0;
				  if (contador % 2){
					  if(impares == 1){
						  impares = 0;
					  }else{
						  impares = 1;
					  }
				  }
				  if(segundos >= TEMPO_ESTADO){
					  estado ++;
					  segundos = 0;
				  }
				  break;

			  case 3:
				  if (contador % 2){
					  if(impares == 1){
						  impares = 0;
					  }else{
						  impares = 1;
					  }
					  pares = impares;
				  }
				  if(segundos >= TEMPO_ESTADO){
					  estado ++;
					  segundos = 0;
				  }
				  break;


		  }


	  }
  }
}
