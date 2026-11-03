/* 
Replace this file with your code. Put your source files in this directory and any libraries in the lib folder. 
If your main program should be assembly-language replace this file with main.S instead.

Libraries (other than vendor SDK and gcc libraries) must have .h-files in /lib/[library name]/include/ and .c-files in /lib/[library name]/src/ to be included automatically.
*/

#include "gd32vf103.h"
#define BITMASK 0xFFFFFFF8

int main(){
	uint32_t button;
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);

	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_5);

while(1){
	button = gpio_input_bit_get(GPIOA, GPIO_PIN_5);
	if(button == 0){
		gpio_bit_set(GPIOB, GPIO_PIN_0);
	}
	else {
		gpio_bit_reset(GPIOB, GPIO_PIN_0);
	}
}

}
