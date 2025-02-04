
#include "ble_wechat_util.h"
data_handler first_handler = {PRODUCT_TYPE_UNKNOWN, NULL, NULL, NULL, NULL};

//function for getting handler by type
//find the type in the list structure of data handler then return the node pointer
data_handler* get_handler_by_type(int type)
{
		data_handler* handler = &first_handler;
		while(handler->next != NULL) {
				handler = handler->next;
				if (handler->m_product_type == type) {
						return handler;
				}
		}
		return NULL;
}
#define BigLittleSwap16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                            (((uint16_t)(A) & 0x00ff) << 8))

 
#define BigLittleSwap32(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                            (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                            (((uint32_t)(A) & 0x0000ff00) << 8) | \
                            (((uint32_t)(A) & 0x000000ff) << 24))

int checkCPUendian()
{
       union{
              unsigned long i;
              uint8_t s[4];
       }c;
 
       c.i = 0x12345678;
       return (0x12 == c.s[0]);
}

unsigned long t_htonl(unsigned long h)
{
       return checkCPUendian() ? h : BigLittleSwap32(h);
}
 
unsigned long t_ntohl(unsigned long n)
{

       return checkCPUendian() ? n : BigLittleSwap32(n);
}

unsigned short wechat_htons(unsigned short h)
{
       return checkCPUendian() ? h : BigLittleSwap16(h);
}
 
unsigned short wechat_ntohs(unsigned short n)
{
       return checkCPUendian() ? n : BigLittleSwap16(n);
}

/*turn an unsigned short value to big-endian value					*/
/*for example 0x1234 in the memory of X86 is 0x34 and 0x12	*/
/*then turn it to Network Byte Order is 0x12 and 0x34				*/

