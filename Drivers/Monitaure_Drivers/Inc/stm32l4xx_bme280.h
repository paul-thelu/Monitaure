/*
 * stm32l4xx_bme280.h
 *
 *  Created on: 11 juil. 2019
 *      Author: PaulT
 */

#ifndef TICKTRACK_DRIVERS_INC_STM32L4XX_BME280_H_
#define	TICKTRACK_DRIVERS_INC_STM32L4XX_BME280_H_

/* Private defines -----------------------------------------------------------*/
#define BME280_ADDRESS              0xEC
#define BME280_CTRL_HUM             0xF2
#define BME280_STATUS               0xF3
#define BME280_CTRL_MEAS            0xF4
#define BME280_CONFIG               0xF5
#define BME280_PART_ID              0xD0
#define BME280_RESET                0xE0
#define BME280_PRESS_MSB            0xF7
#define BME280_PRESS_LSB            0xF8
#define BME280_PRESS_XLSB           0xF9
#define BME280_TEMP_MSB             0xFA
#define BME280_TEMP_LSB             0xFB
#define BME280_TEMP_XLSB            0xFC
#define BME280_HUM_MSB              0xFD
#define BME280_HUM_LSB              0xFE
#define BME280_OVER_SAMPLING        0

#define BME280_SLEEP				0xFC
#define BME280_EXPECTEDPARTID    	0x60

#define BME280_DIG_T				0x88
#define BME280_DIG_H				0xE1

typedef struct bme280_calibration
{
  // Temperature
  uint16_t t1;
  int16_t t2;
  int16_t t3;

  // Pressure
  uint16_t p1;
  int16_t p2;
  int16_t p3;
  int16_t p4;
  int16_t p5;
  int16_t p6;
  int16_t p7;
  int16_t p8;
  int16_t p9;

  // Humidity
  uint8_t h1;
  int16_t h2;
  uint8_t h3;
  int16_t h4;
  int16_t h5;
  int8_t h6;
} bme280_calibration_t;

typedef struct bme280_data
{
  int32_t rawPressure;
  int32_t rawTemperature;
  int16_t rawHumidity;
  int32_t temperatureFine;
  float pressure;
  float temperature;
  float humidity;
  float altitude;
  float dewPointTemp;
  float heatIndex;
} bme280_data_t;

// Altimètre - Température - Humidité
status_t BME280_Init(void);
status_t BME280_IdCheck(void);
status_t BME280_Reset(void);
status_t BME280_Stop(void);
status_t BME280_Start(void);
status_t BME280_Write(uint16_t MemAddress, void *data, uint8_t count);
status_t BME280_Read(uint16_t MemAddress, void *data, uint8_t count);

status_t BME280_Read_Calibration(bme280_calibration_t *BME280);
status_t BME280_Read_Raw_Values(bme280_data_t *BME280);
float BME280_Read_Temperature(bme280_data_t *BME280, bme280_calibration_t *BME280cal);
float BME280_Read_Pressure(bme280_data_t *BME280, bme280_calibration_t *BME280cal);
float BME280_Read_Humidity(bme280_data_t *BME280, bme280_calibration_t *BME280cal);
uint16_t BME280_Read_Heat_Index(float temperature, float humidity);
void BME280_Read_Environmental(bme280_data_t *BME280, bme280_calibration_t *BME280cal);

#endif	/* TICKTRACK_DRIVERS_INC_STM32L4XX_BME280_H_ */

