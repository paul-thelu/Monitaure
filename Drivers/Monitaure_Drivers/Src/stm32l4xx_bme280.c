/**
 * \file			stm32l4xx_bme280.c
 * \author    		Paul THELU
 * \version			1.0
 * \date			21 January 2020
 * \brief			BME280 barometric & humidity sensor driver
 */

#include <math.h>
#include "global.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_bme280.h"

extern I2C_HandleTypeDef hi2c4;

/**
 * \brief			Initialize the bme280 barometer
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Init(void)
{
	uint8_t temp;

	/* Checks that the sensor is reachable */
	if (BME280_IdCheck() != STATUS_OK) {
		return STATUS_KO;
	}

	/* Reset all internal registers to default */
	BME280_Reset();

	/* Shutdown the whole sensor */
	BME280_Stop();

	/* No humidity oversampling (x1 sample) */
	temp = 0x01;
	BME280_Write(BME280_CTRL_HUM, (uint8_t *)&temp, 1);

	/* No temp & pressure oversampling (x1 sample), sleep mode enabled */
	temp = 0x24;
	BME280_Write(BME280_CTRL_MEAS, (uint8_t *)&temp, 1);

	/* No active IIR filter, no SPI, no normal mode standby time */
	temp = 0x00;
	BME280_Write(BME280_CONFIG, (uint8_t *)&temp, 1);

	/* Turn on the sensor again */
	BME280_Start();

	return STATUS_OK;
}

/**
 * \brief			Check whether the device ID is correct or not
 * \param[in]		hi2c: I2C handler
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_IdCheck(void)
{
	uint8_t partID;

	BME280_Read(BME280_PART_ID, (uint8_t *)&partID, 1);

	if (partID != BME280_EXPECTEDPARTID) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			BME280 reset registers
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Reset(void)
{
	/* Reset value to be written = 0xB6 */
	uint8_t reset = 0xB6;

	/* Reset the whole set of registers */
	BME280_Write(BME280_RESET, (uint8_t *)&reset, 1);

	/* Wait for the reset sequence to complete */
	while (reset != 0) {
		BME280_Read(BME280_RESET, (uint8_t *)&reset, 1);
	}

	return STATUS_OK;
}

/**
 * \brief			BME280 stop mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Stop(void)
{
	uint8_t stop;

	/* step 1 : Read the CTRL_MEAS register */
	BME280_Read(BME280_CTRL_MEAS, (uint8_t *)&stop, 1);

	/* step 2 : Toggle the SLEEP bit only in the SYSCONTROL register */
	stop &= BME280_SLEEP;

	/* step 3 : Rewrite the modified CTRL_MEAS register */
	return BME280_Write(BME280_CTRL_MEAS, (uint8_t *)&stop, 1);
}

/**
 * \brief			BME280 start normal mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Start(void)
{
	uint8_t start;

	/* step 1 : Read the CTRL_MEAS register */
	BME280_Read(BME280_CTRL_MEAS, (uint8_t *)&start, 1);

	/* step 2 : Toggle the SLEEP bit only in the CTRL_MEAS register */
	start |= ~BME280_SLEEP;

	/* step 3 : Rewrite the modified CTRL_MEAS register */
	return BME280_Write(BME280_CTRL_MEAS, (uint8_t *)&start, 1);
}

/**
 * \brief			Write I2C registers
 * \param[in]		MemAddress: I2C address of the register
 * \param[in]		*data: Data to transmit
 * \param[in]		count: Number of data to be written
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Write(uint16_t MemAddress, void *data, uint8_t count)
{
	if (HAL_I2C_Mem_Write(&hi2c4, BME280_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, data, count, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			Read I2C registers
 * \param[in]		MemAddress: I2C address of the register
 * \param[out]		*data: Data buffer
 * \param[in]		count: Number of data to be read
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BME280_Read(uint16_t MemAddress, void *data, uint8_t count)
{
	if (HAL_I2C_Mem_Read(&hi2c4, BME280_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, data, count, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

status_t BME280_Read_Calibration(bme280_calibration_t *BME280)
{
	uint8_t buff1[25] = {};
	uint8_t buff2[7] = {};

	// Reading register raw values
	BME280_Read(BME280_DIG_T, (uint8_t *)&buff1, sizeof(buff1));
	BME280_Read(BME280_DIG_H, (uint8_t *)&buff2, sizeof(buff2));

	BME280->t1 = ((uint16_t)buff1[1] << 8) | (uint16_t)buff1[0];
	BME280->t2 = ((int16_t)buff1[3] << 8) | (int16_t)buff1[2];
	BME280->t3 = ((int16_t)buff1[5] << 8) | (int16_t)buff1[4];

	BME280->p1 = ((uint16_t)(buff1[7] << 8) | (uint16_t)buff1[6]);
	BME280->p2 = ((int16_t)(buff1[9] << 8) | (int16_t)buff1[8]);
	BME280->p3 = ((int16_t)(buff1[11] << 8) | (int16_t)buff1[10]);
	BME280->p4 = ((int16_t)(buff1[13] << 8) | (int16_t)buff1[12]);
	BME280->p5 = ((int16_t)(buff1[15] << 8) | (int16_t)buff1[14]);
	BME280->p6 = ((int16_t)(buff1[17] << 8) | (int16_t)buff1[16]);
	BME280->p7 = ((int16_t)(buff1[19] << 8) | (int16_t)buff1[18]);
	BME280->p8 = ((int16_t)(buff1[21] << 8) | (int16_t)buff1[20]);
	BME280->p9 = ((int16_t)(buff1[23] << 8) | (int16_t)buff1[22]);

	BME280->h1 = (int8_t)buff1[24];
	BME280->h2 = ((int16_t)(buff2[1] << 8) | (int16_t)buff2[0]);
	BME280->h3 = (int8_t)buff2[2];
	BME280->h4 = ((int16_t)buff2[3] << 4) | ((int16_t)buff2[4] & 0xF);
	BME280->h5 = ((int16_t)(buff2[5] << 4) | ((int16_t)(buff2[4] & 0xF0) >> (int16_t)4));
	BME280->h6 = (int8_t)buff2[6];

	return STATUS_OK;
}

status_t BME280_Read_Raw_Values(bme280_data_t *BME280)
{
	uint8_t buff[8] = {};

	BME280_Read(BME280_PRESS_MSB, (uint8_t *)&buff, sizeof(buff));

	BME280->rawPressure = ((int32_t)buff[3] << 12) | ((int32_t)buff[4] << 4) | ((int32_t)buff[5] >> 4);  // Pressure
	BME280->rawTemperature = ((int32_t)buff[0] << 12) | ((int32_t)buff[1] << 4) | ((int32_t)buff[2] >> 4);  // Temperature
	BME280->rawHumidity = (((int32_t)buff[6] << 8) | (int32_t)buff[7]);  // Humidity

	return STATUS_OK;
}

float BME280_Read_Temperature(bme280_data_t *BME280, bme280_calibration_t *BME280cal)
{
	int32_t var1, var2, T;

	var1  = ((((BME280->rawTemperature >> 3) - ((int32_t)BME280cal->t1 << 1))) * ((int32_t)BME280cal->t2)) >> 11;
	var2  = (((((BME280->rawTemperature >> 4) - ((int32_t)BME280cal->t1)) * ((BME280->rawTemperature >> 4) - ((int32_t)BME280cal->t1))) >> 12) * ((int32_t)BME280cal->t3)) >> 14;
	BME280->temperatureFine = var1 + var2;
	T = ((BME280->temperatureFine * 5) + 128) >> 8;

	return (T/100.0);
}

float BME280_Read_Pressure(bme280_data_t *BME280, bme280_calibration_t *BME280cal)
{
	int64_t var1, var2, p;

	var1 = ((int64_t)BME280->temperatureFine) - 128000;
	var2 = var1 * var1 * (int64_t)BME280cal->p6;
	var2 = var2 + ((var1 * (int64_t)BME280cal->p5) << 17 );
	var2 = var2 + (((int64_t)BME280cal->p4) << 35);
	var1 = ((var1 * var1 * (int64_t)BME280cal->p3) >> 8) + ((var1 * (int64_t)BME280cal->p2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)BME280cal->p1) >> 33;

	// Avoid exception caused by division by zero
	if (var1 == 0)  {return 0;}

	p = 1048576 - BME280->rawPressure;
	p = (((p << 31) - var2)*3125) / var1;
	var1 = (((int64_t)BME280cal->p9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)BME280cal->p8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)BME280cal->p7) << 4);

	return ((float)p/256.0);
}

float BME280_Read_Humidity(bme280_data_t *BME280, bme280_calibration_t *BME280cal)
{
	int32_t v_x1_u32r;

	v_x1_u32r = (BME280->temperatureFine - ((int32_t)76800));
	v_x1_u32r = (((((BME280->rawHumidity << 14) - (((int32_t)BME280cal->h4) << 20) - (((int32_t)BME280cal->h5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)BME280cal->h6)) >> 10) * (((v_x1_u32r * ((int32_t)BME280cal->h3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)BME280cal->h2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)BME280cal->h1)) >> 4));

	if (v_x1_u32r < 0) {
		v_x1_u32r = 0;
	}
	if (v_x1_u32r > 419430400) {
		v_x1_u32r = 419430400;
	}

	float h = v_x1_u32r >> 12;

	return (h / 1024.0);
}

uint16_t BME280_Read_Heat_Index(float temperature, float humidity)
{
	uint16_t heatIndex;

	/* degree °F conversion */
	temperature = (temperature * 1.8) + 32.0;

	/* Heat Index formula */
	heatIndex = -42.379
	+ 2.04901523 * temperature
	+ 10.14333127 * humidity
	- 0.22475541 * temperature * humidity
	- 0.00683783 * temperature * temperature
	- 0.05481717 * humidity * humidity
	+ 0.00122874 * temperature * temperature * humidity
	+ 0.00085282 * temperature * humidity * humidity
	- 0.00000199 * temperature * temperature * humidity * humidity;

	/* degree °C conversion */
	return (uint16_t)((heatIndex - 32.0) / 1.8);
}

void BME280_Read_Environmental(bme280_data_t *BME280, bme280_calibration_t *BME280cal)
{
	uint8_t temp, status;

	temp = 0x25;
	BME280_Write(BME280_CTRL_MEAS, (uint8_t *)&temp, 1);

	/* wait for the conversion to end */
	do {
		BME280_Read(BME280_STATUS, (uint8_t *)&status, 1);
	} while(status & 0x08);

	BME280_Read_Raw_Values(BME280);
	BME280->temperature = BME280_Read_Temperature(BME280, BME280cal);
	BME280->pressure = BME280_Read_Pressure(BME280, BME280cal);
	BME280->humidity = BME280_Read_Humidity(BME280, BME280cal);
	BME280->altitude = 44330.0 * (1.0 - pow(BME280->pressure / 101325, 0.1903));
	BME280->dewPointTemp = pow((BME280->humidity / 100), 0.125) * (112 + 0.9 * BME280->temperature) + (0.1 * BME280->temperature) - 112;
	BME280->heatIndex = BME280_Read_Heat_Index(BME280->temperature, BME280->humidity);
}
