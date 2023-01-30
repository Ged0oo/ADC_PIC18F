/* 
 * File:   application.c
 * Author: Ahmed.Elghafar
 * https://www.linkedin.com/in/ahmedabdelghafarmohammed/
 */

#include "application.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"
Std_ReturnType ret = E_NOT_OK;

void int0_handeler();
void adc_interrept_handeler();



interrupt_INTx_t int0 =
{
    .EXT_InterruptHandler = int0_handeler,
    .edge = INTERRUPT_FALLING_EDGE, 
    .mcu_pin.port = PORTB_INDEX,
    .mcu_pin.pin = GPIO_PIN0,
    .mcu_pin.direction = GPIO_DIRECTION_INPUT,
    .mcu_pin.logic = GPIO_LOW,
    .source = INTERRUPT_EXTERNAL_INT0 
};

led_t led2 =
{
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN2,
    .led_status = LED_OFF
};
led_t led1 =
{
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN1,
    .led_status = LED_OFF
};

chr_lcd_4bit_t lcd_1 ={
	.lcd_rs.port=PORTC_INDEX,
	.lcd_rs.pin=GPIO_PIN2,
	.lcd_rs.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_rs.logic=GPIO_LOW,
	
    .lcd_en.port=PORTC_INDEX,
	.lcd_en.pin=GPIO_PIN3,
	.lcd_en.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_en.logic=GPIO_LOW,
	
    .lcd_data[0].port=PORTC_INDEX,
	.lcd_data[0].pin=GPIO_PIN4,
	.lcd_data[0].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[0].logic=GPIO_LOW,
	
    .lcd_data[1].port=PORTC_INDEX,
	.lcd_data[1].pin=GPIO_PIN5,
	.lcd_data[1].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[1].logic=GPIO_LOW,
	
    .lcd_data[2].port=PORTC_INDEX,
	.lcd_data[2].pin=GPIO_PIN6,
	.lcd_data[2].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[2].logic=GPIO_LOW,
	
    .lcd_data[3].port=PORTC_INDEX,
	.lcd_data[3].pin=GPIO_PIN7,
	.lcd_data[3].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[3].logic=GPIO_LOW,
	
};


keypad_t keypad_1 ={
	.keypad_row_pins[0].port=PORTB_INDEX,
	.keypad_row_pins[0].pin=GPIO_PIN0,
	.keypad_row_pins[0].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[0].logic=GPIO_LOW,	
	
    .keypad_row_pins[1].port=PORTB_INDEX,
	.keypad_row_pins[1].pin=GPIO_PIN1,
	.keypad_row_pins[1].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[1].logic=GPIO_LOW,
	
    .keypad_row_pins[2].port=PORTB_INDEX,
	.keypad_row_pins[2].pin=GPIO_PIN2,
	.keypad_row_pins[2].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[2].logic=GPIO_LOW,
	
    .keypad_row_pins[3].port=PORTB_INDEX,
	.keypad_row_pins[3].pin=GPIO_PIN3,
	.keypad_row_pins[3].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[3].logic=GPIO_LOW,
	
    .keypad_columns_pins[0].port=PORTB_INDEX,
	.keypad_columns_pins[0].pin=GPIO_PIN4,
	.keypad_columns_pins[0].direction=GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[0].logic=GPIO_LOW,
	
    .keypad_columns_pins[1].port=PORTB_INDEX,
	.keypad_columns_pins[1].pin=GPIO_PIN5,
	.keypad_columns_pins[1].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[1].logic=GPIO_LOW,
	
    .keypad_columns_pins[2].port=PORTB_INDEX,
	.keypad_columns_pins[2].pin=GPIO_PIN6,
	.keypad_columns_pins[2].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[2].logic=GPIO_LOW,
	
    .keypad_columns_pins[3].port=PORTB_INDEX,
	.keypad_columns_pins[3].pin=GPIO_PIN7,
	.keypad_columns_pins[3].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[3].logic=GPIO_LOW,				
};

adc_conf_t adc_1 = 
{
    .ADC_InterruptHandler = adc_interrept_handeler,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED
};


uint16 adc_res_1 = 0, adc_res_2 = 0, adc_res_3 = 0, adc_res_4 = 0;
uint8 adc_res_1_txt[6], adc_res_2_txt[6], adc_res_3_txt[6], adc_res_4_txt[6];
uint8 ADC_Req = 0;




void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bit_intialize(&lcd_1);
    ret = keypad_initialize(&keypad_1);
    ret = Interrupt_INTx_Init(&int0);
    ret = led_initialize(&led2);
    ret = led_initialize(&led1);
    ret = ADC_Init(&adc_1);
}


void int0_handeler()
{

}


int main() { 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();    
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 2,7, "ADC TEST.");
    __delay_ms(2000);
    ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR);
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 1, 1, "POT 1 : ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 1, "POT 2 : ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 3, 1, "POT 3 : ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 4, 1, "POT 4 : ");

    while(1)
    {
        if(0 == ADC_Req){
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN0);
        }
        else if(1 == ADC_Req){
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN1);
        }
        else if(2 == ADC_Req){
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN2);
        }
        else if(3 == ADC_Req){
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN3);
        }
        else { /* Nothing */ }
        
        ret = convert_uint16_to_string(adc_res_1, adc_res_1_txt);
        ret = convert_uint16_to_string(adc_res_2, adc_res_2_txt);
        ret = convert_uint16_to_string(adc_res_3, adc_res_3_txt);
        ret = convert_uint16_to_string(adc_res_4, adc_res_4_txt);
        
        ret = lcd_4bit_send_string_pos(&lcd_1, 1, 7, adc_res_1_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 2, 7, adc_res_2_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 3, 7, adc_res_3_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 4, 7, adc_res_4_txt);
    }
    return (EXIT_SUCCESS);
}


void adc_interrept_handeler()
{
    Std_ReturnType ret = E_NOT_OK;
    if(0 == ADC_Req){
        ret = ADC_GetConversionResult(&adc_1, &adc_res_1);
        ADC_Req = 1;
    }
    else if(1 == ADC_Req){
        ret = ADC_GetConversionResult(&adc_1, &adc_res_2);
        ADC_Req = 2;
    }
    else if(2 == ADC_Req){
        ret = ADC_GetConversionResult(&adc_1, &adc_res_3);
        ADC_Req = 3;
    }
    else if(3 == ADC_Req){
        ret = ADC_GetConversionResult(&adc_1, &adc_res_4);
        ADC_Req = 0;
    }
    else { /* Nothing */ }  
}