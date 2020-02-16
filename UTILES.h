#ifndef utils_h
#define utils_h
#define reset_all(reg) reg=0 ;
#define set_all(reg)  reg=~0 ;
#define set_bit(reg,bit) reg|=(uint32)(1<<(bit))  //set a bit change it to 1
#define reset_bit(reg,bit) reg&=~(uint32)(1<<(bit)) //reset a bit change it to 0
#define toggle_bit(reg,bit) reg^=(1<<(bit)) //toggol a bit 
#define get_bit(reg,bit) (reg&(1<<(bit)))>>(bit) //find the value in the bit eather 1 or 0 
#define set_nibh(reg)  reg|=(15<<(4)) //nibble is 4bits where h stand for most segnificant nibble
#define set_nibl(reg)  reg|=(15) //nibble is 4bits where l stand for least segnificant nibble
#define reset_nibh(reg) reg&=(15)
#define reset_nibl(reg) reg&=(15<<4)
#define get_nibh(reg)  reg=(reg&(15<<4))>>4
#define get_nibl(reg)  reg&=(15)
#define set_nibh_val(reg,val) reg=((reg&(15))+(val<<4)) 
#define set_nibl_val(reg,val) reg=((reg&(15<<4))+(val))
#endif

