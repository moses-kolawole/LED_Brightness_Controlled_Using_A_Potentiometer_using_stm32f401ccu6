#include "main.h"

int main(void)
{
	// Enabling GPIOA Clock
	RCC->AHB1ENR |= (1 << 0);

	// Configure PA6 as Alternate Function
	GPIOA->MODER &= ~(3 << (6 * 2));
	GPIOA->MODER |=  (2 << (6 * 2));

	// Select Alternate Function AF2 (TIM3)
	GPIOA->AFR[0] &= ~(0xF << (6 * 4));
	GPIOA->AFR[0] |=  (2 << (6 * 4));

	// Configure PA0 as Analog Input
	GPIOA->MODER &= ~(3 << (0 * 2));
	GPIOA->MODER |=  (3 << (0 * 2));

	// Enable TIM3 Clock
	RCC->APB1ENR |= (1 << 1);

	// Configure Timer Speed
	TIM3->PSC = 15;

	// Auto Reload Register
	TIM3->ARR = 999;

	// Initial Duty Cycle
	TIM3->CCR1 = 0;

	// Configure PWM Mode 1
	TIM3->CCMR1 &= ~(7 << 4);
	TIM3->CCMR1 |=  (6 << 4);

	// Enable Output Compare Preload
	TIM3->CCMR1 |= (1 << 3);

	// Enable Channel 1 Output
	TIM3->CCER |= (1 << 0);

	// Enable Auto Reload Preload
	TIM3->CR1 |= (1 << 7);

	// Generate Update Event
	TIM3->EGR |= (1 << 0);

	// Start Timer
	TIM3->CR1 |= (1 << 0);

	// Enable ADC1 Clock
	RCC->APB2ENR |= (1 << 8);

	// Select Channel 0 (PA0)
	ADC1->SQR3 = 0;

	// One Conversion
	ADC1->SQR1 = 0;

	// Enable ADC
	ADC1->CR2 |= (1 << 0);

	// Small Delay for ADC Stabilization
	for(volatile uint32_t i = 0; i < 1000; i++);

	uint16_t adcValue = 0;

	while(1)
	{
		// Start ADC Conversion
		ADC1->CR2 |= (1 << 30);

		// Wait Until Conversion Completes
		while(!(ADC1->SR & (1 << 1)));

		// Read ADC Value
		adcValue = ADC1->DR;

		// Clear End Of Conversion Flag
		ADC1->SR &= ~(1 << 1);

		// Update PWM Duty Cycle
		TIM3->CCR1 = (adcValue * 999) / 4095;
	}

	return 0;
}
