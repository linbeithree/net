/***********************************************************************************
成都浩然电子有限公司
电话：028-86127089，400-998-5300
传真：028-86127039
网址：http://www.hschip.com
日期：2014-2-25
***********************************************************************************/
#include<C8051F340.h>

#include"Device.h"
#include"W5500.h"

/*********************** Delay Subroutine ***********************/
void Delay(unsigned int data d)
{
	unsigned int data i,j;

	i=timer0;
	do
	{
		if(i<=timer0)
			j=timer0-i;
		else
			j=65536-i+timer0;	
	}while(j<d);
}

/***************************************************************************************
                                    C8051F340 initialization
It initializes IO port, Timer, UART, A/D system and I2C port
***************************************************************************************/
void C8051F340_Initialization(void)
{
	PCA0MD&=~0x40; 		/* Disable WDT */                   
	/*************** System clock *************/
	OSCICN=0x83;		/* SYSCLK derived from Internal H-F Oscillator divided by 1 */
	while((OSCICN & 0x40)==0);	/* Wait for High Frequency Clock ready */

	CLKMUL=0x80;		/* Clock Multiplier enabled */
	CLKMUL=0xc0;		/* Clock Multiplier enabled and Initialize it */
	while((CLKMUL & 0x20)==0);	/* Wait for CLOCK Multiplier Ready */

	CLKSEL=0x03;		/* Select 48MHz for SYSTEM CLOCK */

	/*************** IO port ***************/
	XBR0=0x03;			/* Enable UART0 Routed to P04 and P05, SPI routed to P0.0, P0.1, P0.2 */
	XBR1=0xc0;			/* Weak Pull Up and Enable Crossbar */
	XBR2=0x00;
	
	/* P0 */
	P0=0xff;
	P0MDOUT=0xd5;

	P1MDOUT=0x21;		/* P10 and P15 are push and pull output */
	P1=0xff;
	/*************** Timer ***************/
	TCON=0x01;			/* INT0 is edge triggered */
	TMOD=0x21;			/* Timer1 is 8-bit timer with auto reload, timer0 is 16-bit timet */
	CKCON=0x02;			/* Timer0 and Timer1 use CLOCK defined by Prescaler, 1/48 system CLOCK */
	
	TH0=0xfc;
	TL0=0x18;
	TR0=1;				/* Start Timer0 */
	ET0=1;				/* Enable Tomer0 interrupt */

	TMR2RLL=0;
	TMR2RLH=1;
	TR2=1;
	/*************** UART ***************/
//	SCON0=0x10;			/* UART0 Receive enable*/
//	TH1=204;			/* Timer1 as BAUD RATE Generator, 9600bps */
//	TL1=204;
//	TR1=1;				/* Start Timer1 */
//	ES0=1;				/* Enable UART0 interrupt */

//	IT01CF=0x03;		/* Set P0.3 as INT0 */
//	EX0=1;				/* Enable external 0 interrupt */

	/* SPI接口初始化 */
	SPI0CFG=0x40;		/* Master mode, SCK line low in idle state */
	SPI0CKR=1;			/* SCK frequency, 12MHz */
	SPI0CN=0x01;		/* 3-wire master mode, SPI Enable */

	RST_W=0;	     	/* Hardware Reset W5200 */

	EA=1;				/* Enable System interrupt */
}

/* Config W5500 */
void W5500_Config(void)
{
	unsigned char data dat[6];

	Write_1_Byte(MR,RST);
	Delay(100);

	/* Set Dateway IP as 192.168.0.1 */
	dat[0]=192;
	dat[1]=168;
	dat[2]=0;
	dat[3]=1;
	Write_Bytes(GAR,dat,4);

	/* Set SUBNET MASK as 255.255.255.0 */
	dat[0]=255;
	dat[1]=255;
	dat[2]=255;
	dat[3]=0;
	Write_Bytes(SUBR,dat,4);

	/* Set MAC Address as: 0x48,0x53,0x00,0x57,0x55,0x00 */
	dat[0]=0x48;
	dat[1]=0x53;
	dat[2]=0x00;
	dat[3]=0x57;
	dat[4]=0x55;
	dat[5]=0x00;
	Write_Bytes(SHAR, dat, 6);

	/* Set W5500 IP as 192.168.0.20 */
	dat[0]=192;
	dat[1]=168;
	dat[2]=0;
	dat[3]=20;
	Write_Bytes(SIPR,dat,4);
}

/* Detect Gateway */
unsigned char Detect_Gateway(void)
{
	unsigned char data dat[4];

	/* Set a different subnet destination IP */
	dat[0]=192+1;
	dat[1]=168+1;
	dat[2]=0+1;
	dat[3]=20+1;
	Write_SOCK_4_Byte(0,Sn_DIPR,dat);

	/* Set Socket n in TCP mode */
	Write_SOCK_1_Byte(0,Sn_MR,MR_TCP);
	/* Open Socket n */
	Write_SOCK_1_Byte(0,Sn_CR,OPEN);

	Delay(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(0,Sn_SR)!=SOCK_INIT)
	{
		Write_SOCK_1_Byte(0,Sn_CR,CLOSE);		/* Close Socket 0 */
		return FALSE;
	}

	/* Make connect */
	Write_SOCK_1_Byte(0,Sn_CR,CONNECT);
	
	do
	{
		if(Read_SOCK_1_Byte(0,Sn_IR)&IR_TIMEOUT) 	/* No Gateway Detected */
		{
			Write_SOCK_1_Byte(0,Sn_CR,CLOSE);		/* Close Socket 0 */
			return FALSE;
		}
		else if(Read_SOCK_1_Byte(0,Sn_DHAR)!=0xff)	/* Detect Gateway */
		{
			Write_SOCK_1_Byte(0,Sn_CR,CLOSE);		/* Close Socket 0 */
			break;
		}
	}while(1);
	return TRUE;
}

/* Config Scoket */
void Socket0_Config(void)
{
	unsigned char data dat[4];

	/* Set Socket 0 Port Number as 5000 */
	Write_SOCK_2_Byte(0,Sn_PORT,5000);		

	/* Set Socket0 Destination IP as 192.168.0.40*/
	dat[0]=192;
	dat[1]=168;
	dat[2]=0;
	dat[3]=30;
	Write_SOCK_4_Byte(0,Sn_DIPR,dat);

	/* Set Socket0 Destination Port Number as 5000 */
	Write_SOCK_2_Byte(0,Sn_DPORTR,5000);

	/* Set Maximum Segment Size as 1470 */
	Write_SOCK_2_Byte(0, Sn_MSSR, 1470);

	/* Set RX Buffer Size as 2K */
	Write_SOCK_1_Byte(0,Sn_RXBUF_SIZE, 0x02);
	/* Set TX Buffer Size as 2K */
	Write_SOCK_1_Byte(0,Sn_TXBUF_SIZE, 0x02);
}

/*Read interrup flag */
unsigned char Read_IR(SOCKET data s)
{
	unsigned char data i;

	i=Read_SOCK_1_Byte(s,Sn_IR);

	if(i!=0)
		Write_SOCK_1_Byte(s,Sn_IR,i);

	return i;
}

/* Process Received data */
void Loop_Back(void)
{
	unsigned int data i;
	unsigned char data dat[4];

	i=Read_SOCK_Data_Buffer(0, S_Buffer);      /* Read W5200 Data to Rx_Buffer */
	if(i==0)
		return;
	
	/* Get Destination IP */
	dat[0]=S_Buffer[0];
	dat[1]=S_Buffer[1];
	dat[2]=S_Buffer[2];
	dat[3]=S_Buffer[3];
	Write_SOCK_4_Byte(0,Sn_DIPR,dat);
	/* Get Destination Port Number */
	Write_SOCK_2_Byte(0,Sn_DPORTR,(S_Buffer[4]*0x100+S_Buffer[5]));

	i=S_Buffer[6]*0x100+S_Buffer[7];
	Write_SOCK_Data_Buffer(0, S_Buffer+8,i);
   	do
	{
		i=Read_SOCK_1_Byte(0,Sn_IR);
	}while(((i&IR_TIMEOUT)==0)&&((i&IR_SEND_OK)==0));
}

/***************************************************************************************
                                    Main program
***************************************************************************************/
main(void)
{
	unsigned char data i;

   	C8051F340_Initialization();
	
	Delay(100);
	RST_W=1;					/* Restore W5200 */
	
	/* Detect Ethernet Link */
	do
	{
		Delay(100);					/* wait 100ms */
		i=Read_1_Byte(PHYCFGR);
	}while((i&LINK)==0);

	W5500_Config();				/* Initialize W5200 */
	if(Detect_Gateway()==TRUE) 		/* Detect Gateway */
		D_OUTPUT1=0;

	while(1)
	{
		if((Read_1_Byte(PHYCFGR)&LINK)==0)	/* Detect Ethernet Link */
		{
			D_OUTPUT2=1;
			Write_SOCK_1_Byte(0,Sn_CR,CLOSE);
			while((Read_1_Byte(PHYCFGR)&LINK)==0);		/* Wait Ethernet Link */
		}

		i=Read_IR(0);	/* Process IR */
		if(i&IR_TIMEOUT)
		{
			D_OUTPUT2=1;
			Write_SOCK_1_Byte(0,Sn_CR,CLOSE);
		}		

		i=Read_SOCK_1_Byte(0,Sn_SR);
		if(i==0)
		{
			Socket0_Config();	
			while(Socket_UDP(0)==FALSE)
				Delay(1000);

			D_OUTPUT2=0;
		}
		else if(i==SOCK_UDP)
			Loop_Back();
	}
}

/**************************************************************************************
Timer0 overflow interrupt handler
**************************************************************************************/
void Timer0_isr(void) interrupt 1
{
	TH0=0xfc;
	TL0=0x18;

	timer0++;
}

/**************************************************************************************
UART0 RX/TX interrupt handler
*************************************************************************************
void USRT0_isr(void) interrupt 4
{
	if(RI0)
	{
		RI0=0;
	}
	
	if(TI0)
	{
		TI0=0;
	}
}
*/