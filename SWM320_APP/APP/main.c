#include "SWM320.h"

void SerialInit(void);

int main(void)
{ 	
	uint32_t i;
	
 	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0);	// 输入，开上拉
	GPIO_Init(GPIOA, PIN5, 1, 0, 0);	// 输出
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, SystemCoreClock*1.5, 1);
	TIMR_Start(TIMR0);
	
	__enable_irq();
	
	while(1==1)
 	{
		printf("Hi from App\r\n");
		
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		
		
		if(GPIO_GetBit(GPIOA, PIN4) == 0)			// 检测到按键按下
		{
			SYS->RSTCR |= (1 << SYS_RSTCR_SYS_Pos);	// 通过复位跳到UserBoot
			while(1) __NOP();
		}
 	}
}


void TIMR0_Handler(void)
{
	TIMR_INTClr(TIMR0);
	
	GPIO_InvBit(GPIOA, PIN5);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN2, FUNMUX0_UART0_RXD, 1);	//GPIOA.2配置为UART0输入引脚
	PORT_Init(PORTA, PIN3, FUNMUX1_UART0_TXD, 0);	//GPIOA.3配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}


/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
