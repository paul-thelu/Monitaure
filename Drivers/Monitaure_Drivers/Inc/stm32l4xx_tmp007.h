/*
 * File:   stm32l4xx_TMP007.h
 * Author: Paul
 */

#ifndef MONITAURE_DRIVERS_INC_STM32L4XX_TMP007_H_
#define	MONITAURE_DRIVERS_INC_STM32L4XX_TMP007_H_

#define TMP007_VOBJ       0x00
#define TMP007_TDIE       0x01
#define TMP007_CONFIG     0x02
#define TMP007_TOBJ       0x03
#define TMP007_STATUS     0x04
#define TMP007_STATMASK   0x05

#define TMP007_CFG_RESET    0x8000
#define TMP007_CFG_MODEON   0x1000
#define TMP007_CFG_1SAMPLE  0x0000
#define TMP007_CFG_2SAMPLE  0x0200
#define TMP007_CFG_4SAMPLE  0x0400
#define TMP007_CFG_8SAMPLE  0x0600
#define TMP007_CFG_16SAMPLE 0x0800
#define TMP007_CFG_ALERTEN  0x0100
#define TMP007_CFG_ALERTF   0x0080
#define TMP007_CFG_TRANSC   0x0040

#define TMP007_STAT_ALERTEN 0x8000
#define TMP007_STAT_CRTEN   0x4000

#define TMP007_I2CADDR 		0x80
#define TMP007_DEVID 		0x1F

#define TMP007_EXPECTEDPARTID	0x78


status_t TMP007_Init(void);
status_t TMP007_IdCheck(void);
status_t TMP007_Reset(void);
status_t TMP007_Stop(void);
status_t TMP007_Start(void);

int16_t TMP007_ReadRawDieTemperature(void);
float TMP007_ReadDieTempC(void);
float TMP007_ReadObjTempC(void);
int16_t TMP007_ReadRawVoltage(void);

status_t TMP007_Write16bits(uint16_t MemAddress, uint16_t data);
status_t TMP007_Read16bits(uint16_t MemAddress, uint16_t *data);

#endif	/* MONITAURE_DRIVERS_INC_STM32L4XX_TMP007_H_ */
