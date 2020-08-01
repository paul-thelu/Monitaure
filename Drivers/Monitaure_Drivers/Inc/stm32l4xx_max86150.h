/*
 * File:   stm32l4xx_max86150.h
 * Author: Paul
 */

#ifndef MONITAURE_DRIVERS_INC_STM32L4XX_MAX86150_H_
#define	MONITAURE_DRIVERS_INC_STM32L4XX_MAX86150_H_

/* Private defines -----------------------------------------------------------*/
#define MAX86150_INTSTAT1		0x00
#define MAX86150_INTSTAT2   	0x01
#define MAX86150_INTENABLE1   	0x02
#define MAX86150_INTENABLE2   	0x03
#define MAX86150_FIFOWRITEPTR   0x04
#define MAX86150_FIFOOVERFLOW   0x05
#define MAX86150_FIFOREADPTR    0x06
#define MAX86150_FIFODATA 		0x07
#define MAX86150_FIFOCONFIG    	0x08
#define MAX86150_FIFOCONTROL1   0x09
#define MAX86150_FIFOCONTROL2   0x0A
#define MAX86150_SYSCONTROL    	0x0D
#define MAX86150_PPG_CONFIG1   	0x0E
#define MAX86150_PPG_CONFIG2   	0x0F
#define MAX86150_LED_PROX_AMP   0x10
#define MAX86150_LED1_PULSEAMP  0x11
#define MAX86150_LED2_PULSEAMP  0x12
#define MAX86150_LED_RANGE 		0x14
#define MAX86150_LED_PILOT_PA 	0x15
#define MAX86150_ECG_CONFIG1 	0x3C
#define MAX86150_ECG_CONFIG3 	0x3E
#define MAX86150_PROXINTTHRESH  0x10

/* Interrupt Status 1 */
#define MAX86150_INT_A_FULL 	0x80
#define MAX86150_PPG_RDY 		0x40
#define MAX86150_ALC_OVF 		0x20
#define MAX86150_PROX_INT 		0x10
#define MAX86150_PWR_RDY 		0x01

/* Interrupt Status 2 */
#define MAX86150_VDD_OOR 		0x80
#define MAX86150_ECG_RDY 		0x04

#define MAX86150_INT_A_FULL_ENABLE    	0x80
#define MAX86150_INT_A_FULL_DISABLE    	0x00
#define MAX86150_INT_DATA_RDY_ENABLE   	0x40
#define MAX86150_INT_DATA_RDY_DISABLE   0x00
#define MAX86150_INT_ALC_OVF_ENABLE    	0x20
#define MAX86150_INT_ALC_OVF_DISABLE    0x00
#define MAX86150_INT_PROX_INT_ENABLE    0x10
#define MAX86150_INT_PROX_INT_DISABLE   0x00
#define MAX86150_SAMPLEAVG_1    	0x00
#define MAX86150_SAMPLEAVG_2    	0x20
#define MAX86150_SAMPLEAVG_4    	0x40
#define MAX86150_SAMPLEAVG_8    	0x60
#define MAX86150_SAMPLEAVG_16    	0x80
#define MAX86150_SAMPLEAVG_32    	0xA0
#define MAX86150_ROLLOVER_ENABLE    0x10
#define MAX86150_ROLLOVER_DISABLE   0x00
#define MAX86150_SHUTDOWN    		0x02
#define MAX86150_RESET    			0x01
#define MAX86150_MODE_REDONLY    	0x02
#define MAX86150_MODE_REDIRONLY    	0x03
#define MAX86150_MODE_MULTILED    	0x07
#define MAX86150_ADCRANGE_2048    	0x00
#define MAX86150_ADCRANGE_4096    	0x20
#define MAX86150_ADCRANGE_8192    	0x40
#define MAX86150_ADCRANGE_16384    	0x60
#define MAX86150_SAMPLERATE_50    	0x00
#define MAX86150_SAMPLERATE_100    	0x04
#define MAX86150_SAMPLERATE_200    	0x08
#define MAX86150_SAMPLERATE_400    	0x0C
#define MAX86150_SAMPLERATE_800    	0x10
#define MAX86150_SAMPLERATE_1000    0x14
#define MAX86150_SAMPLERATE_1600    0x18
#define MAX86150_SAMPLERATE_3200    0x1C
#define MAX86150_PULSEWIDTH_MASK    0xFC
#define MAX86150_PULSEWIDTH_69    	0x00
#define MAX86150_PULSEWIDTH_118    	0x01
#define MAX86150_PULSEWIDTH_215    	0x02
#define MAX86150_PULSEWIDTH_411    	0x03

#define MAX86150_PART_ID			0xFF
#define MAX86150_EXPECTEDPARTID    	0x1E
#define MAX86150_ADDRESS          	0x5E //7-bit I2C Address

typedef enum max86150_led {
    PPG_LED_IR = 1,
    PPG_LED_RED = 2,
    LED_PROXIMITY = 3
} max86150_led_t;

typedef enum max86150_ledRange {
	_50MA,
	_100MA
} max86150_ledRange_t;

typedef enum max86150_fifoData {
	SLOT_NONE,
	SLOT_PPG_LED1,
	SLOT_PPG_LED2,
	SLOT_PILOT_LED1 = 5,
	SLOT_PILOT_LED2 = 6,
	SLOT_ECG = 9
} max86150_fifoData_t;

typedef enum max86150_sampleRatePPG {
	_10Hz,
	_20Hz,
	_50Hz,
	_84Hz,
	_100Hz,
	_200Hz,
	_400Hz,
	_800Hz,
	_1000Hz,
	_1600Hz,
	_3200Hz
} max86150_sampleRatePPG_t;

typedef enum max86150_scalePPG {
	_4096nA,
	_8192nA,
	_16384nA,
	_32768nA
} max86150_scalePPG_t;

typedef enum max86150_pulseWidthPPG {
	_50us,
	_100us,
	_200us,
	_400us
} max86150_pulseWidthPPG_t;

typedef enum max86150_sampleDecimate {
	_1samp,
	_2samp,
	_4samp,
	_8samp,
	_16samp,
	_32samp
} max86150_sampleDecimate_t;

status_t MAX86150_Init(void);
status_t MAX86150_IdCheck(void);
status_t MAX86150_Reset(void);
status_t MAX86150_Stop(void);
status_t MAX86150_Start(void);

status_t MAX86150_Write(uint16_t MemAddress, void *data, uint8_t count);
status_t MAX86150_Read(uint16_t MemAddress, void *data, uint8_t count);

status_t MAX86150_FIFO_DataCtrlReg(max86150_fifoData_t fd1, max86150_fifoData_t fd2, max86150_fifoData_t fd3, max86150_fifoData_t fd4);
status_t MAX86150_InterruptsEnabled(uint8_t intEn1, uint8_t intEn2);
status_t MAX86150_Set_LedCurrentRange(max86150_led_t ledType, max86150_ledRange_t powerRange);
status_t MAX86150_Set_LedCurrentPulseAmplitude(max86150_led_t ledType, uint8_t power);
status_t MAX86150_PPG_Configuration(max86150_scalePPG_t scale, max86150_sampleRatePPG_t rate, max86150_pulseWidthPPG_t pulseWidth, max86150_sampleDecimate_t decim);
status_t MAX86150_ECG_Configuration(uint8_t adcCLK, uint8_t adcOSR, uint8_t ecgGain, uint8_t iaGain);

#endif	/* MONITAURE_DRIVERS_INC_STM32L4XX_MAX86150_H_ */
