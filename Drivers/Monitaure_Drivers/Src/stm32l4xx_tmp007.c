/**
 * \file			stm32l4xx_tmp007.c
 * \author    		Paul THELU
 * \version			1.0
 * \date			21 January 2020
 * \brief			TMP007 thermopile temperature sensor driver
 */

#include <math.h>
#include "../inc/global.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_tmp007.h"

extern I2C_HandleTypeDef hi2c4;

/**
 * \brief			Initialize the tmp007 contactless temperature sensor
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Init(void)
{
	/* Checks that the sensor is reachable */
	if (TMP007_IdCheck() != STATUS_OK) {
		return STATUS_KO;
	}

	/* Reset all internal registers to default */
	TMP007_Reset();

	/* Shutdown the whole sensor */
	TMP007_Stop();

	/* Initialize sensor according to user's settings */
	/* Enable Conversion, ALERT interrupt and Transient Compensation at a rate of 1 sample/sec */
	TMP007_Write16bits(TMP007_CONFIG, 0x0A00 | TMP007_CFG_ALERTEN | TMP007_CFG_TRANSC);

	/* Enable Conversion Done Flag */
	TMP007_Write16bits(TMP007_STATMASK, TMP007_STAT_CRTEN);

	/* Turn on the sensor again */
	TMP007_Start();

	return STATUS_OK;
}

/**
 * \brief			Check whether the device ID is correct or not
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_IdCheck(void)
{
	uint16_t partID;

	TMP007_Read16bits(TMP007_DEVID, &partID);

	if ((partID & 0x00FF) != TMP007_EXPECTEDPARTID) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			TMP007 reset registers
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Reset(void)
{
	uint16_t reset = 0x8000;

	/* Reset the whole set of registers */
	TMP007_Write16bits(TMP007_CONFIG, reset);

	/* Wait for the reset sequence to complete */
	while ((reset & 0x8000) != 0) {
		TMP007_Read16bits(TMP007_CONFIG, (uint16_t *)&reset);
	}

	return STATUS_OK;
}

/**
 * \brief			TMP007 stop mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Stop(void)
{
	uint16_t stop;

	/* step 1 : Read the TMP007_CONFIG register */
	TMP007_Read16bits(TMP007_CONFIG, &stop);

	/* step 2 : Toggle the MOD bit only in the TMP007_CONFIG register */
	return TMP007_Write16bits(TMP007_CONFIG, stop & ~TMP007_CFG_MODEON);
}

/**
 * \brief			TMP007 start mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Start(void)
{
	uint16_t start;

	/* step 1 : Read the TMP007_CONFIG register */
	TMP007_Read16bits(TMP007_CONFIG, &start);

	/* step 2 : Toggle the MOD bit only in the TMP007_CONFIG register */
	return TMP007_Write16bits(TMP007_CONFIG, start | TMP007_CFG_MODEON);
}

/**
 * \brief			Read the die raw temperature
 * \return			Die raw temperature
 */
int16_t TMP007_ReadRawDieTemperature(void)
{
	uint16_t rawDieTemp;

	TMP007_Read16bits(TMP007_TDIE, &rawDieTemp);

	/* Read Object temperature and ignore last 2 LSB */
	return (int16_t)((rawDieTemp & 0xFFFC) >> 2);
}

/**
 * \brief			Read the die temperature of TMP007
 * \return			Die temperature in degree C
 */
float TMP007_ReadDieTempC(void)
{
	return (float)(TMP007_ReadRawDieTemperature() * 0.03125);
}

/**
 * \brief			Read the object temperature in degree celsius
 * \return			Object temperature in degree C
 */
float TMP007_ReadObjTempC(void)
{
	uint16_t buf;

	TMP007_Read16bits(TMP007_TOBJ, &buf);

	if (buf & 0x1) {
		return NAN;
	} else {
		return (float)(((int16_t)(buf >> 2)) * 0.03125);
	}
}

/**
 * \brief			Read the raw voltage
 * \return			16bits raw voltage
 */
int16_t TMP007_ReadRawVoltage(void)
{
	uint16_t rawVolt;

	TMP007_Read16bits(TMP007_VOBJ, &rawVolt);

	return (int16_t)rawVolt;
}

/**
 * \brief			Write 16bits long I2C registers
 * \param[in]		MemAddress: I2C address register
 * \param[in]		data: Data to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Write16bits(uint16_t MemAddress, uint16_t data)
{
	uint8_t dataBuf[2] = {(uint8_t)((data & 0xFF00) >> 8), (uint8_t)(data & 0x00FF)};

	if (HAL_I2C_Mem_Write(&hi2c4, TMP007_I2CADDR, MemAddress, I2C_MEMADD_SIZE_8BIT, dataBuf, 2, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			Read 16bits long I2C registers
 * \param[in]		MemAddress: I2C address register
 * \param[out]		*data: Data to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t TMP007_Read16bits(uint16_t MemAddress, uint16_t *data)
{
	uint8_t buff[2];

	if (HAL_I2C_Mem_Read(&hi2c4, TMP007_I2CADDR, MemAddress, I2C_MEMADD_SIZE_8BIT, buff, 2, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	*data = (buff[0] << 8) | buff[1];

	return STATUS_OK;
}
