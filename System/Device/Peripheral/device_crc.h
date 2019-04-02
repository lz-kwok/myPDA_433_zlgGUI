/*
 * ======== CRC_Modbus.h ========
 */
#ifndef _DEVICECRC_H_
#define _DEVICECRC_H_

#define RegBase         40001




uint16_t Crc16(uint8_t *bufferpoint,int16_t sum);
void ModbusMasterAsk(uint8_t SlaveAddr,uint32_t RegisAddr,uint8_t FuncCode,uint8_t DataH,uint8_t DataL);
void ModbusMasterRead(uint8_t SlaveAddr,uint32_t RegisAddr,uint8_t FuncCode,uint8_t DataH,uint8_t DataL);


#endif
