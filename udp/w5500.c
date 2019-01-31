/***********************************************************************************
成都浩然电子有限公司
电话：028-86127089，400-998-5300
传真：028-86127039
网址：http://www.hschip.com
时间：2014-2-25
***********************************************************************************/
#include <c8051f340.h>              /* C8051F340库定义 */
#include"W5500.h"

typedef  unsigned char SOCKET;

#define TRUE	0xff
#define FALSE	0x00

#define S_RX_SIZE	2048
#define S_TX_SIZE	2048

sbit W5500_SCS=	P1^5;

extern void Delay(unsigned int d);		/* Delay d*1ms */

/******************************* W5500 Write Operation *******************************/
/* Write W5500 Common Register a byte */
void Write_1_Byte(unsigned int data reg, unsigned char data dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
 	SPIF=0;
	SPI0DAT=reg/256;					/* Send Register Address */
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM1|RWB_WRITE|COMMON_R);
	while(SPIF==0);

	/* Write 1 byte */
	SPIF=0;
	SPI0DAT=dat;
	while(SPIF==0);

	/* Set W5500 SCS High */
	W5500_SCS=1;
}

/* Write W5500 Common Register 2 bytes */
void Write_2_Byte(unsigned int data reg, unsigned char data *dat_ptr)
{
	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM2|RWB_WRITE|COMMON_R);
	while(SPIF==0);

	/* Write 2 bytes */
	SPIF=0;
	SPI0DAT=*dat_ptr++;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=*dat_ptr;
	while(SPIF==0);

	/* Set W5500 SCS High */
	W5500_SCS=1;;
}

/* Write W5500 Common Register n bytes */
void Write_Bytes(unsigned int data reg, unsigned char data *dat_ptr, unsigned int data size)
{
	unsigned int i;

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(VDM|RWB_WRITE|COMMON_R);
	while(SPIF==0);

	/* Write n bytes */
	for(i=0;i<size;i++)
	{
		SPIF=0;
		SPI0DAT=*dat_ptr++;
		while(SPIF==0);
	}

	/* Set W5500 SCS High */
	W5500_SCS=1;
}

/* Write W5500 Socket Register 1 byte */
void Write_SOCK_1_Byte(SOCKET data s, unsigned int data reg, unsigned char data dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM1|RWB_WRITE|(s*0x20+0x08));
	while(SPIF==0);

	/* Write 1 byte */
	SPIF=0;
	SPI0DAT=dat;
	while(SPIF==0);

	/* Set W5500 SCS High */
	W5500_SCS=1;
}

/* Write W5500 Socket Register 2 byte */
void Write_SOCK_2_Byte(SOCKET data s, unsigned int data reg, unsigned int data dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM2|RWB_WRITE|(s*0x20+0x08));
	while(SPIF==0);

	/* Write 2 bytes */
	SPIF=0;
	SPI0DAT=dat/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=dat;
	while(SPIF==0);

	/* Set W5500 SCS High */
	W5500_SCS=1;
}

/* Write W5500 Socket Register 2 byte */
void Write_SOCK_4_Byte(SOCKET data s, unsigned int data reg, unsigned char data *dat_ptr)
{
	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM4|RWB_WRITE|(s*0x20+0x08));
	while(SPIF==0);

	/* Write 4 bytes */
	SPIF=0;
	SPI0DAT=*dat_ptr++;
	while(SPIF==0);

	SPIF=0;
	SPI0DAT=*dat_ptr++;
	while(SPIF==0);

	SPIF=0;
	SPI0DAT=*dat_ptr++;
	while(SPIF==0);

	SPIF=0;
	SPI0DAT=*dat_ptr;
	while(SPIF==0);

	/* Set W5500 SCS High */
	W5500_SCS=1;
}

/******************************* W5500 Read Operation *******************************/
/* Read W5500 Common register 1 Byte */
unsigned char Read_1_Byte(unsigned int data reg)
{
	unsigned char data i;

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM1|RWB_READ|COMMON_R);
	while(SPIF==0);

//	i=SPI0DAT;
	/* Write a dummy byte */
	SPIF=0;
	SPI0DAT=0x00;
	while(SPIF==0);
	/* Read 1 byte */
	i=SPI0DAT;

	/* Set W5500 SCS High*/
	W5500_SCS=1;
	return i;
}

/* Read W5500 Socket register 1 Byte */
unsigned char Read_SOCK_1_Byte(SOCKET data s, unsigned int data reg)
{
	unsigned char data i;

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM1|RWB_READ|(s*0x20+0x08));
	while(SPIF==0);

//	i=SPI0DAT;
	/* Write a dummy byte */
	SPIF=0;
	SPI0DAT=0x00;
	while(SPIF==0);
	/* Read 1 byte */
	i=SPI0DAT;

	/* Set W5500 SCS High*/
	W5500_SCS=1;
	return i;
}

/* Read W5500 Socket register 2 Bytes */
unsigned int Read_SOCK_2_Byte(SOCKET data s, unsigned int data reg)
{
	unsigned int data i;

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=reg/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=reg;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(FDM2|RWB_READ|(s*0x20+0x08));
	while(SPIF==0);

//	i=SPI0DAT;
	/* Write a dummy byte */
	SPIF=0;
	SPI0DAT=0x00;
	while(SPIF==0);
	i=SPI0DAT;
	i*=256;

	SPIF=0;
	SPI0DAT=0x00;
	while(SPIF==0);
	i+=SPI0DAT;

	/* Set W5500 SCS High*/
	W5500_SCS=1;
	return i;
}

/******************** Read data from W5500 Socket data RX Buffer *******************/
unsigned int Read_SOCK_Data_Buffer(SOCKET data s, unsigned char xdata *dat_ptr)
{
	unsigned int data rx_size;
	unsigned int data offset, offset1;
	unsigned int data i;

	rx_size=Read_SOCK_2_Byte(s,Sn_RX_RSR);
	if(rx_size==0)		/* if no data received, return */
		return 0;
	if(rx_size>1460)
		rx_size=1460;

	offset=Read_SOCK_2_Byte(s,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	/* Write Address */
	SPIF=0;
	SPI0DAT=offset/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=offset;
	while(SPIF==0);

	/* Write Control Byte */
	SPIF=0;
	SPI0DAT=(VDM|RWB_READ|(s*0x20+0x18));		/* Read variable size */
	while(SPIF==0);
//	i=SPI0DAT;
	
	if((offset+rx_size)<S_RX_SIZE)
	{
		/* Read Data */
		for(i=0;i<rx_size;i++)
		{
			SPIF=0;
			SPI0DAT=0x00;		/* Send a dummy byte */
			while(SPIF==0);
			*dat_ptr++=SPI0DAT;
		}
	}
	else
	{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			SPIF=0;
			SPI0DAT=0x00; 		/* Send a dummy byte */
			while(SPIF==0);
			*dat_ptr++=SPI0DAT;
		}
		/* Set W5500 SCS High*/
		W5500_SCS=1;

		/* Set W5500 SCS Low */
		W5500_SCS=0;
		/* Write Address */
		SPIF=0;
		SPI0DAT=0x00;
		while(SPIF==0);
		SPIF=0;
		SPI0DAT=0x00;
		while(SPIF==0);

		/* Write Control Byte */
		SPIF=0;
		SPI0DAT=(VDM|RWB_READ|(s*0x20+0x18));		/* Read variable size */
		while(SPIF==0);
//		j=SPI0DAT;
		/* Read Data */
		for(;i<rx_size;i++)
		{
			SPIF=0;
			SPI0DAT=0x00;		/* Send a dummy byte */
			while(SPIF==0);
			*dat_ptr++=SPI0DAT;
		}
	}
	/* Set W5500 SCS High*/
	W5500_SCS=1;

	/* Update offset*/
	offset1+=rx_size;
	Write_SOCK_2_Byte(s, Sn_RX_RD, offset1);
	Write_SOCK_1_Byte(s, Sn_CR, RECV);					/* Write RECV Command */
	return rx_size;
}

/******************** Write data to W5500 Socket data TX Buffer *******************/
void Write_SOCK_Data_Buffer(SOCKET data s, unsigned char xdata *dat_ptr, unsigned int data size)
{
	unsigned int data offset,offset1;
	unsigned int data i;

	offset=Read_SOCK_2_Byte(s,Sn_TX_WR);
	offset1=offset;
	offset&=(S_TX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_SCS=0;

	SPIF=0;
	/* Write Address */
	SPI0DAT=offset/256;
	while(SPIF==0);
	SPIF=0;
	SPI0DAT=offset;
	while(SPIF==0);

	SPIF=0;
	/* Write Control Byte */
	SPI0DAT=(VDM|RWB_WRITE|(s*0x20+0x10));		/* Read variable size */
	while(SPIF==0);

	if((offset+size)<S_TX_SIZE)
	{
		/* Write Data */
		for(i=0;i<size;i++)
		{
			SPIF=0;
			SPI0DAT=*dat_ptr++;		/* Send a byte*/
			while(SPIF==0);
		}
	}
	else
	{
		offset=S_TX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			SPIF=0;
			SPI0DAT=*dat_ptr++; 		/* Send a byte */
			while(SPIF==0);
		}
		/* Set W5500 SCS High*/
		W5500_SCS=1;

		/* Set W5500 SCS Low */
		W5500_SCS=0;
		/* Write Address */
		SPIF=0;
		SPI0DAT=0x00;
		while(SPIF==0);
		SPIF=0;
		SPI0DAT=0x00;
		while(SPIF==0);
		SPIF=0;
		/* Write Control Byte */
		SPI0DAT=(VDM|RWB_WRITE|(s*0x20+0x10));		/* Read variable size */
		while(SPIF==0);
		/* Read Data */
		for(;i<size;i++)
		{
			SPIF=0;
			SPI0DAT=*dat_ptr++;		/* Send a byte */
			while(SPIF==0);
		}
	}
	/* Set W5500 SCS High*/
	W5500_SCS=1;

	/* Updata offset */
	offset1+=size;
	Write_SOCK_2_Byte(s, Sn_TX_WR, offset1);
	Write_SOCK_1_Byte(s, Sn_CR, SEND);					/* Write SEND Command */
}

/*********************** Set Socket n in TCP Client mode ************************/
unsigned char Socket_Connect(SOCKET data s)
{
	/* Set Socket n in TCP mode */
	Write_SOCK_1_Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	Write_SOCK_1_Byte(s,Sn_CR,OPEN);

	Delay(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_INIT)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	/* make connect */
	Write_SOCK_1_Byte(s,Sn_CR,CONNECT);
	return TRUE;
}

/*********************** Set Socket n in TCP Server mode ************************/
unsigned char Socket_Listen(SOCKET data s)
{
	/* Set Socket n in TCP mode */
	Write_SOCK_1_Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	Write_SOCK_1_Byte(s,Sn_CR,OPEN);

	Delay(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_INIT)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	/* Set Socket n in Server mode */
	Write_SOCK_1_Byte(s,Sn_CR,LISTEN);
	Delay(5);
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_LISTEN)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	return TRUE;
}

/*********************** Set Socket n in UDP mode ************************/
unsigned char Socket_UDP(SOCKET data s)
{
	/* Set Socket n in UDP mode */
	Write_SOCK_1_Byte(s,Sn_MR,MR_UDP);
	/* Open Socket n */
	Write_SOCK_1_Byte(s,Sn_CR,OPEN);

	Delay(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_UDP)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	return TRUE;
}