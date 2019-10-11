//#include <A2Ylib.h>
//┌────────────────────┐
//│         高通科技版权所有  │
//│      GENITOP RESEARCH CO.,LTD.         │
//│        created on 2015.7.27            │
//└────────────────────┘

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flash.h"

typedef  unsigned char  u8;
//typedef  unsigned long  u32;
typedef  unsigned short  u16;
typedef  unsigned long   ulong;
typedef  unsigned char   uchar;
typedef  unsigned char   BYTE;
typedef  unsigned short  WORD;
typedef  unsigned long   DWORD;

static DWORD hfmbdd;

extern void r_dat_bat(unsigned long TAB_addr,unsigned int Num,unsigned char *p_arr);



#define ASCII_5X7              1      //ASCII编码5X7点阵
#define ASCII_7X8              2      //ASCII编码7X8点阵
#define ASCII_6X12             3      //ASCII编码6X12点阵
#define ASCII_12_B_A           4      //ASCII编码12x12点阵不等宽Arial风格字符（具体字形看规格书）
#define ASCII_12_B_T           5      //ASCII编码12x12点阵不等宽Time news Roman风格字符（具体字形看规格书）
#define ASCII_8X16             6      //ASCII编码8X16点阵
#define ASCII_16_A             7      //ASCII编码16X16点阵不等宽Arial风格字符（具体字形看规格书）
#define ASCII_16_T             8      //ASCII编码16X16点阵不等宽Time news Roman风格字符（具体字形看规格书）
#define ASCII_12X24            9      //ASCII编码12X24点阵打印体风格字符（具体字形看规格书）
#define ASCII_24_B             10     //ASCII编码24x24点阵不等宽字符
#define ASCII_16X32            11     //ASCII编码16X32点阵Arial风格字符（具体字形看规格书）
#define ASCII_32_B             12     //ASCII编码32X32点阵Arial风格字符（具体字形看规格书）

#define B_11X16_A              13     //自制字符11X16 Arial风格字体
#define B_18X24_A              14     //自制字符18X24 Arial风格字体
#define B_22X32_A              15     //自制字符22X32 Arial风格字体
#define B_34X48_A              16     //自制字符34X48 Arial风格字体
#define B_40X64_A              17     //自制字符40X64 Arial风格字体
#define B_11X16_T              18     //自制字符11X16 Time news Roman风格字体
#define B_18X24_T              19     //自制字符18X24 Time news Roman风格字体
#define B_22X32_T              20     //自制字符22X32 Time news Roman风格字体
#define B_34X48_T              21     //自制字符34X48 Time news Roman风格字体
#define B_40X64_T              22     //自制字符40X64 Time news Roman风格字体
#define T_FONT_20X24           23     //自制字符20X24 时钟体风格字体
#define T_FONT_24X32           24     //自制字符24X32 时钟体风格字体
#define T_FONT_34X48           25     //自制字符34X48 时钟体风格字体
#define T_FONT_48X64           26     //自制字符48X64 时钟体风格字体
#define F_FONT_816             27     //自制字符8X16 时钟体风格字体
#define F_FONT_1624            28     //自制字符16X24 时钟体风格字体
#define F_FONT_1632            29     //自制字符16X32 时钟体风格字体
#define F_FONT_2448            30     //自制字符24X48 时钟体风格字体
#define F_FONT_3264            31     //自制字符32X64 时钟体风格字体
#define KCD_UI_32              32     //自制字符32X32 时钟体风格字体

#define SEL_GB                 33     //在hzbmp16函数中用于charset参数，SEL_GB表示简体中文
#define SEL_JIS                34     //在hzbmp16函数中用于charset参数，SEL_JIS表示日文
#define SEL_KSC                35     //在hzbmp16函数中用于charset参数，SEL_KSC表示韩文

/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
    ASCII_GetData(0x41,ASCII_5X7,DZ_Data);      //读取5X7点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为8 BYTE
	  ASCII_GetData(0x41,ASCII_7X8,DZ_Data);      //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为8 BYTE
	  ASCII_GetData(0x41,ASCII_6X12,DZ_Data);     //读取6X12点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为12 BYTE
	  ASCII_GetData(0x41,ASCII_12_B_A,DZ_Data);   //读取12X12点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为26 BYTE
	  ASCII_GetData(0x41,ASCII_12_B_T,DZ_Data);   //读取12X12点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为26 BYTE
	  ASCII_GetData(0x41,ASCII_8X16,DZ_Data);     //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为16 BYTE
	  ASCII_GetData(0x41,ASCII_16_A,DZ_Data);     //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
	  ASCII_GetData(0x41,ASCII_16_T,DZ_Data);     //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
	  ASCII_GetData(0x41,ASCII_12X24,DZ_Data);    //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为48 BYTE
	  ASCII_GetData(0x41,ASCII_24_B,DZ_Data);     //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为74 BYTE
	  ASCII_GetData(0x41,ASCII_16X32,DZ_Data);    //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为64 BYTE
	  ASCII_GetData(0x41,ASCII_32_B,DZ_Data);     //读取7X8点阵 ASCII 编码A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为130 BYTE
*************************************************************/
unsigned char  ASCII_GetData(unsigned char ASCIICode,unsigned long ascii_kind,unsigned char *DZ_Data);

/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
    LATIN_GetData(0xa5,DZ_Data); //读取8X16点阵 拉丁文编码0xa5的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为16 BYTE
*************************************************************/
unsigned long LATIN_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
    CYRILLIC_GetData(0x405,DZ_Data); //读取8X16点阵西里尔文编码0x405的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为16 BYTE
*************************************************************/
unsigned long  CYRILLIC_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
	  GREECE_GetData(0x375,DZ_Data);  //读取8X16点阵希腊文编码0x375的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为16 BYTE
*************************************************************/
unsigned long GREECE_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
	  HEBREW_GetData(0x595,DZ_Data);  //读取8X16点阵希伯来文编码0x375的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为16 BYTE
*************************************************************/
unsigned long HEBREW_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
	  LATIN_B_GetData(0xaA,DZ_Data);  //读取16X16点阵不等宽拉丁文编码0xaA的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
*************************************************************/
unsigned long LATIN_B_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
		CYRILLIC_B_GetData(0x40A,DZ_Data);  //读取16X16点阵不等宽西里尔文编码0x40A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
*************************************************************/
unsigned long CYRILLIC_B_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
	  GREECE_B_GetData(0x37A,DZ_Data); //读取16X16点阵不等宽希腊文编码0x37A的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
*************************************************************/
unsigned long GREECE_B_GetData(unsigned int FontCode,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
	  ALB_B_GetData(0x632,DZ_Data); //读取16X16点阵不等宽希腊文编码0x632的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为34 BYTE
*************************************************************/
unsigned long ALB_B_GetData(unsigned int unicode_alb,unsigned char *DZ_Data);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
    THAILAND_GetData(0xe05,DZ_Data);  //读取16X24点阵不等宽希腊文编码0x632的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为50 BYTE
*************************************************************/
unsigned long THAILAND_GetData(unsigned int FontCode,unsigned char *DZ_Data);
//Unicode杞珿B18030杞崲绠楁硶
/*************************************************************
函数用法：
    unsigned int GB_CODE;
		GB_CODE=U2G(0x554a);//将“啊”字的UNICODE编码转换成GB国标编码，存在GB_CODE中.如例GB_CODE=0xb0a1
*************************************************************/
unsigned int U2G(unsigned int UN_CODE);
/*************************************************************
函数用法：
    unsigned int BIG_CODE;
    BIG5_CODE=BIG52GBK(0xb0,0xda);//将“啊”字的BIG5编码转换成GB国标编码，存在BIG_CODE中.如例BIG_CODE=0xb0a1
*************************************************************/
unsigned int BIG52GBK( unsigned char h,unsigned char l);
/*************************************************************
函数用法：
    unsigned int JIS_CODE;
    JIS_CODE=U2J(0x6F2B); //十进制4401//将0x6F2B的UNICODE编码转换成jis0208编码，存在JIS_CODE中.如例JIS_CODE=0x2C01
    其中区码等于0X2C,十进制为44，位码为0X01,十进制为1.
*************************************************************/
unsigned int U2J(WORD Unicode);
/*************************************************************
函数用法：
    unsigned int KSC_CODE;
    KSC_CODE=U2K(0xB27C);//b4bc//将韩文字符unicode编码B27C的BIG5编码转换成GB国标编码，存在BIG_CODE中.如例BIG_CODE=0xb4bc
*************************************************************/
unsigned int U2K(WORD Unicode);
/*************************************************************
函数用法：
    unsigned int JIS_CODE;
    JIS_CODE=SJIS2JIS(0x88ae); //十进制1616//将0x88AE的SHIFT-JIS编码转换成jis0208编码，存在JIS_CODE中.如例JIS_CODE=0x1010
*************************************************************/
unsigned int SJIS2JIS(WORD code);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
		zz_zf(1,B_11X16_A,DZ_Data);  //读取顺序为1的不等宽11X16点阵自制字符Arial字形的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为24 BYTE
		zz_zf(1,B_18X24_A,DZ_Data);  //读取顺序为1的不等宽18X24点阵自制字符Arial字形的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为56 BYTE
		zz_zf(1,B_22X32_A,DZ_Data);  //读取顺序为1的不等宽22X32点阵自制字符Arial字形的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为90 BYTE
		zz_zf(1,B_34X48_A,DZ_Data);  //读取顺序为1的不等宽34X48点阵自制字符Arial字形的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为206 BYTE
		zz_zf(1,B_40X64_A,DZ_Data);  //读取顺序为1的不等宽40X64点阵自制字符Arial字形的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为322 BYTE
		zz_zf(1,B_11X16_T,DZ_Data);  //读取顺序为1的不等宽11X26点阵自制字符Time news Roman的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为24 BYTE
		zz_zf(1,B_18X24_T,DZ_Data);  //读取顺序为1的不等宽18X24点阵自制字符Time news Roman的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为56 BYTE
		zz_zf(1,B_22X32_T,DZ_Data);  //读取顺序为1的不等宽22X32点阵自制字符Time news Roman的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为90 BYTE
		zz_zf(1,B_34X48_T,DZ_Data);  //读取顺序为1的不等宽34X48点阵自制字符Time news Roman的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为206 BYTE
		zz_zf(1,B_40X64_T,DZ_Data);  //读取顺序为1的不等宽40X64点阵自制字符Time news Roman的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为322 BYTE
		zz_zf(1,T_FONT_20X24,DZ_Data);  //读取顺序为1的不等宽20X24点阵自制字符时钟体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为62 BYTE
		zz_zf(1,T_FONT_24X32,DZ_Data);  //读取顺序为1的不等宽24X32点阵自制字符时钟体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为98 BYTE
		zz_zf(1,T_FONT_34X48,DZ_Data);  //读取顺序为1的不等宽34X48点阵自制字符时钟体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为206 BYTE
		zz_zf(1,T_FONT_48X64,DZ_Data);  //读取顺序为1的不等宽48X64点阵自制字符时钟体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为386 BYTE
	  zz_zf(1,F_FONT_816,DZ_Data);    //读取顺序为1的不等宽8X16点阵自制字符方块体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为18 BYTE
		zz_zf(1,F_FONT_1624,DZ_Data);   //读取顺序为1的不等宽16X24点阵自制字符方块体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为50 BYTE
		zz_zf(1,F_FONT_1632,DZ_Data);   //读取顺序为1的不等宽16X32点阵自制字符方块体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为66 BYTE
		zz_zf(1,F_FONT_2448,DZ_Data);   //读取顺序为1的不等宽24X48点阵自制字符方块体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为146 BYTE
	  zz_zf(1,F_FONT_3264,DZ_Data);   //读取顺序为1的不等宽32X64点阵自制字符方块体的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为258 BYTE
	  zz_zf(1,KCD_UI_32,DZ_Data);     //读取顺序为1的不等宽32X32点阵可穿戴UI的点阵数据，并将点阵数据存在DZ_Data数组中；数据长度为130 BYTE
*************************************************************/
void zz_zf(unsigned char Sequence,unsigned char kind,unsigned char *DZ_Data);

#define ntohs(s)
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
		hzbmp16(SEL_GB, 0xb0a1, 0, 16,DZ_Data);   //读取GB18030编码为0xb0a1 16X16汉字的点阵数据，并将点阵数据存在DZ_Data数组中，数据长度为32 BYTE
		hzbmp16(SEL_JIS, 0x0401, 0, 16,DZ_Data);  //读取JIS0208编码为0x0401 16X16汉字的点阵数据，并将点阵数据存在DZ_Data数组中，数据长度为32 BYTE
		hzbmp16(SEL_KSC, 0xa1D9, 0, 16,DZ_Data);  //读取KSC5601编码为0xA1D9 16X16汉字的点阵数据，并将点阵数据存在DZ_Data数组中，数据长度为32 BYTE
*************************************************************/           
void hzbmp16(unsigned char charset, unsigned int code, unsigned int codeex, unsigned char size,unsigned char *buf);
/*************************************************************
函数用法：
    unsigned char DZ_Data[数组长度客户自定义];
    hzbmp24(0,0xb0a1,0,24,DZ_Data);   //读取GB2312编码为0xb0a1  24X24汉字的点阵数据，并将点阵数据存在DZ_Data数组中，数据长度为72 BYTE
*************************************************************/       
void hzbmp24(unsigned char charset, unsigned int code, unsigned int codeex, unsigned char size,unsigned char *buf);

// 附：	r_dat_bat 从字库中读数据函数 该函数需客户自己实现，如下是实现该函数的参考程序。
/****************************************************
u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
说明：
Address  ： 表示字符点阵在芯片中的字节地址。
byte_long： 是读点阵数据字节数。
*p_arr   ： 是保存读出的点阵数据的数组。
*****************************************************/
u8 r_dat_bat(u32 address,u8 byte_long,u8 *p_arr)
{
	u32 j=0;
	CS_L;
	SendByte(address);	 //发送命令及地址
	for(j=0;j<byte_long;j++)
	{
	 p_arr[j]=ReadByte();//取点阵数据
	}
	CS_H;
	return p_arr[0];	
}

void SendByte(u32 cmd)
{
	u8 i;
	cmd=cmd|0x03000000;
	for(i=0;i<32;i++)
	{
		CLK_L;
		if(cmd&0x80000000)
			SI_H;
		else 
			SI_L;
		CLK_H;
		cmd=cmd<<1;
	}					
}

u8 ReadByte(void)
{
	u8 i;
	u8 dat=0;
	CLK_H;
	for(i=0;i<8;i++)
	{
		CLK_L;
		dat=dat<<1;
		if(SO)
			dat=dat|0x01;
		else 
			dat&=0xfe;
		CLK_H;		
	}	
	return dat;
}


		

