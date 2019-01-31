#define TRUE	0xff
#define FALSE	0x00

typedef unsigned char SOCKET;

sbit RST_W=		P1^0;
sbit D_OUTPUT1=	P1^1;
sbit D_OUTPUT2=	P1^2;

sbit L_LINK=	P1^3;
sbit D_INPUT1=	P1^4;
sbit SCS=		P1^5;

unsigned int data timer0;
unsigned char xdata S_Buffer[1470];

extern unsigned char Read_1_Byte(unsigned int data reg);
extern void Write_1_Byte(unsigned int data reg, unsigned char data dat);
extern void Write_Bytes(unsigned int data reg, unsigned char data *dat_ptr, unsigned int data size);

extern unsigned int Read_SOCK_Data_Buffer(SOCKET data s, unsigned char xdata *dat_ptr);
extern unsigned char Read_SOCK_1_Byte(SOCKET data s, unsigned int data reg);
extern void Write_SOCK_Data_Buffer(SOCKET data s, unsigned char xdata *dat_ptr, unsigned int data size);
extern void Write_SOCK_1_Byte(SOCKET data s, unsigned int data reg, unsigned char data dat);
extern void Write_SOCK_2_Byte(SOCKET data s, unsigned int data reg, unsigned int data dat);
extern void Write_SOCK_4_Byte(SOCKET data s, unsigned int data reg, unsigned char data *dat_ptr);

extern unsigned char Socket_Listen(SOCKET data s);
