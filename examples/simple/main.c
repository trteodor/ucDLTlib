

#define DLT_LOG_CONTEX           "MAIN"
#define DLT_LOG_APPID            "NUM1"

#include "main.h"
#include "DLTuc.h"

#define UseSysTick

UART_DMA_Handle_Td *Uart2HandlerPointer;  


/*CallBacks used by ucDltLibrary section start..*/
void UART2_TransmitDMAEndCallBack()
{
	DLTuc_MessageTransmitDone(); /*Inform ucDLTlib about message transmission end*/
		/*This way to make this example simple to read...*/
}


/*This CallBack was registered in main function using function: DLTuc_RegisterTransmitSerialDataCallback*/
void UART2_LowLevelDataTransmit(uint8_t *DltLogData, uint8_t Size)
{
	/*This function is called by ucDLTlib when is something to send*/

	TUART_DMA_Trasmit(Uart2HandlerPointer,DltLogData,Size); /*Call of this function may block contex!!! */
		/*This way to make this example simple to read...*/
}
/*CallBacks used by ucDltLibrary section end..*/


int Main(void) 
{
	/**********************************************************/
	/*Microcontoler Initialization start...*/
	LowLevelSystemInit(SleepMode);
	ConfigSysTick1ms();
	// ConfigRTC(40000, 1); /*1 Seconds*/
	GPIO_Pin_Cfg(GPIOA,Px5,gpio_mode_output_PP_2MHz);
	GPIO_PinSet(GPIOA, Psm5); /*Led is very exicited! :) */
	Uart2HandlerPointer = UART2_Init115200(UART2_TransmitDMAEndCallBack);
	/*Init UART with baud 115200 and pass pointer called after transmission end (transmit complet DMA)*/
	/*Microcontoler Initialization end...*/
	/**********************************************************/


	/*Register Low Level Transmit function for DLTuc Library*/
	DLTuc_RegisterTransmitSerialDataCallback(UART2_LowLevelDataTransmit);
	DLTuc_RegisterGetTimeStampMsCallback(GetSysTime); 	/*Register GetSysTime function*/
	/*The function "GetSysTime" must return the time in ms*/

	/*Now ucDLTlib is ready to work!*/
	LOGL(DL_INFO, "DLT TESTS START!!!");

	for(int i=0; i<50; i++)
	{
		LOGL(DL_DEBUG, "Hello DLT with period 5ms");
		DelayMs(5);
	}
	DelayMs(100);
	/*LOG DROP TEST*/
	for(int i=0; i<20; i++)
	{
		LOG("Log Drop testing0 :)  %d" , 5);
		LOG("Log Drop testing1 :)  %d" , 5);
		LOG("Log Drop testing2:)  %d" , 5);
		LOG("Log Drop testing3 :)  %d" , 5);
		LOG("Log Drop testing4 :)  %d" , 5);
	}
		LOGL(DL_INFO, "Dropped log...  %d" , 5);
		LOGL(DL_FATAL, "Dropped log...   %d" , 5);
	DelayMs(100);

	while(1)
	{
		/*Send example Logs in loop...*/
		LOGL(DL_ERROR, "Hello DLT Again Arg1 %d Arg2 :%d" , 2565, 56);
		LOGFF(DL_FATAL, "GENERALLY DLT Again1");
		LOGF(DL_DEBUG, "AnotherTest DLT Again");
		LOG("AnotherTest2 DLT Again");
		DelayMs(1000);
		LOGL(DL_WARN, "Orange is sweet fruit");
		DelayMs(1000);
		tooglePIN(GPIOA, Psm5);
	}
}


