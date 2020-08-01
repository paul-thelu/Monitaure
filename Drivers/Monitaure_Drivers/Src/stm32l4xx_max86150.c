/**
 * \file			stm32l4xx_max86150.c
 * \author    		Paul THELU
 * \version			1.0
 * \date			21 January 2020
 * \brief			MAX86150 SpO2 & ECG sensor driver
 */

#include <math.h>
#include "../inc/global.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_max86150.h"

extern I2C_HandleTypeDef hi2c4;

/**
 * \brief			Initialize the max86150 heart rate, spo2 & ECG sensor
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Init(void)
{
	/* Checks that the sensor is reachable */
	if (MAX86150_IdCheck() != STATUS_OK) return STATUS_KO;

	/* Reset all internal registers to default */
	if (MAX86150_Reset()  != HAL_OK) return STATUS_KO;

	/* Shutdown the whole sensor */
	if (MAX86150_Stop()  != HAL_OK) return STATUS_KO;

	/* Sensor configuration */
	if (MAX86150_PPG_Configuration(_4096nA, _400Hz, _50us, _1samp) != HAL_OK) return STATUS_KO;
	if (MAX86150_ECG_Configuration(0,0,0,0) != HAL_OK) return STATUS_KO;

	/* Turn on the sensor again */
	if (MAX86150_Start() != HAL_OK) return STATUS_KO;

	return STATUS_OK;
}

/**
 * \brief			Check whether the device ID is correct or not
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_IdCheck(void)
{
	uint8_t partID;

	if (MAX86150_Read(MAX86150_PART_ID, (uint8_t *)&partID, 1) != HAL_OK) return STATUS_KO;

	if (partID != MAX86150_EXPECTEDPARTID) {
		return STATUS_KO;
	} else {
		return STATUS_OK;
	}
}

/**
 * \brief			MAX86150 reset registers
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Reset(void)
{
	uint8_t reset = 0x01;

	/* Reset the whole set of registers */
	if (MAX86150_Write(MAX86150_SYSCONTROL, (uint8_t *)&reset, 1) != HAL_OK) return STATUS_KO;

	/* Wait for the reset sequence to complete */
	while ((reset & 0x01) != 0) {
		if (MAX86150_Read(MAX86150_SYSCONTROL, (uint8_t *)&reset, 1) != HAL_OK) return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			MAX86150 stop mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Stop(void)
{
	uint8_t stop;

	/* step 1 : Read the SYSCONTROL register */
	if (MAX86150_Read(MAX86150_SYSCONTROL, (uint8_t *)&stop, 1) != HAL_OK) return STATUS_KO;

	/* step 2 : Toggle the SHUTDOWN bit only in the SYSCONTROL register */
	stop |= MAX86150_SHUTDOWN;

	/* step 3 : Rewrite the modified SYSCONTROL register */
	return MAX86150_Write(MAX86150_SYSCONTROL, (uint8_t *)&stop, 1);
}

/**
 * \brief			MAX86150 start mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Start(void)
{
	uint8_t start;

	/* step 1 : Read the SYSCONTROL register */
	if (MAX86150_Read(MAX86150_SYSCONTROL, (uint8_t *)&start, 1) != HAL_OK) return STATUS_KO;

	/* step 2 : Toggle the SHUTDOWN bit only in the SYSCONTROL register */
	start &= MAX86150_SHUTDOWN;

	/* step 3 : Rewrite the modified SYSCONTROL register */
	return MAX86150_Write(MAX86150_SYSCONTROL, (uint8_t *)&start, 1);
}

/**
 * \brief			Write I2C registers
 * \param[in]		MemAddress: I2C address of the register
 * \param[in]		*data: Data to transmit
 * \param[in]		count: Number of data to be written
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Write(uint16_t MemAddress, void *data, uint8_t count)
{
	if (HAL_I2C_Mem_Write(&hi2c4, MAX86150_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, data, count, I2C_MAX_DELAY) != HAL_OK) {
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
status_t MAX86150_Read(uint16_t MemAddress, void *data, uint8_t count)
{
	if (HAL_I2C_Mem_Read(&hi2c4, MAX86150_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, data, count, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	return STATUS_OK;
}

/**
 * \brief			Configure the FIFO slots
 * \param[in]		fd1: FIFO slot n°1
 * \param[in]		fd2: FIFO slot n°2
 * \param[in]		fd3: FIFO slot n°3
 * \param[in]		fd4: FIFO slot n°4
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_FIFO_DataCtrlReg(max86150_fifoData_t fd1, max86150_fifoData_t fd2, max86150_fifoData_t fd3, max86150_fifoData_t fd4)
{
	uint8_t fifoDataCtrlReg1 = (fd2 << 4) | fd1;
	uint8_t fifoDataCtrlReg2 = (fd4 << 4) | fd3;

	/* Write the interrupt bits into the corresponding intEn registers 1 & 2 */
	if (MAX86150_Write(MAX86150_FIFOCONTROL1, (uint8_t *)&fifoDataCtrlReg1, 1) != HAL_OK) return STATUS_KO;
	if (MAX86150_Write(MAX86150_FIFOCONTROL2, (uint8_t *)&fifoDataCtrlReg2, 1) != HAL_OK) return STATUS_KO;

	return STATUS_OK;
}

/**
 * \brief			Set interrupt sources
 * \param[in]		intEn1: Interrupt Enable register n°1
 * \param[in]		intEn2: Interrupt Enable register n°2
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_InterruptsEnabled(uint8_t intEn1, uint8_t intEn2)
{
	/* Write the interrupt bits into the corresponding intEn registers 1 & 2 */
	if (MAX86150_Write(MAX86150_INTENABLE1, (uint8_t *)&intEn1, 1) != HAL_OK) return STATUS_KO;
	if (MAX86150_Write(MAX86150_INTENABLE2, (uint8_t *)&intEn2, 1) != HAL_OK) return STATUS_KO;

	return STATUS_OK;
}

/**
 * \brief			Set LED range of power
 * \param[in]		ledType: LED type addressed
 * \param[in]		powerRange: Value of powerRange (_50mA or _100mA)
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Set_LedCurrentRange(max86150_led_t ledType, max86150_ledRange_t powerRange)
{
	switch (ledType)
	{
		case PPG_LED_IR :
			if (MAX86150_Write(MAX86150_LED_RANGE, (uint8_t *)&powerRange, 1) != HAL_OK) return STATUS_KO;
			break;
		case PPG_LED_RED :
			if (MAX86150_Write(MAX86150_LED_RANGE, (uint8_t *)&powerRange, 1) != HAL_OK) return STATUS_KO;
			break;
		default :
			return STATUS_KO;
			break;
	}

	return STATUS_OK;
}

/**
 * \brief			Set LED power
 * \param[in]		ledType: LED type addressed
 * \param[in]		power: Value of the LED power
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_Set_LedCurrentPulseAmplitude(max86150_led_t ledType, uint8_t power)
{
	switch (ledType)
	{
		case PPG_LED_IR :
			if (MAX86150_Write(MAX86150_LED1_PULSEAMP, (uint8_t *)&power, 1) != HAL_OK) return STATUS_KO;
			break;
		case PPG_LED_RED :
			if (MAX86150_Write(MAX86150_LED2_PULSEAMP, (uint8_t *)&power, 1) != HAL_OK) return STATUS_KO;
			break;
		case LED_PROXIMITY :
			if (MAX86150_Write(MAX86150_LED_PROX_AMP, (uint8_t *)&power, 1) != HAL_OK) return STATUS_KO;
			break;
		default :
			return STATUS_KO;
			break;
	}

	return STATUS_OK;
}

/**
 * \brief			Set the PPG configuration register
 * \param[in]		scale: Range of PPG ADC
 * \param[in]		rate: Sampling rate for PPG
 * \param[in]		pulseWidth: LED pulse width value
 * \param[in]		decim: Number of adjacent samples decimated
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_PPG_Configuration(max86150_scalePPG_t scale, max86150_sampleRatePPG_t rate, max86150_pulseWidthPPG_t pulseWidth, max86150_sampleDecimate_t decim)
{
	uint8_t temp;

	/* Concatenate the required values */
	temp = ((scale << 6) | rate << 2) | pulseWidth;

	/* Rewrite the whole 2 PPG registers containing the changes */
	if (MAX86150_Write(MAX86150_PPG_CONFIG1, (uint8_t *)&temp, 1) != HAL_OK) return STATUS_KO;
	if (MAX86150_Write(MAX86150_PPG_CONFIG2, (uint8_t *)&decim, 1) != HAL_OK) return STATUS_KO;

	return STATUS_OK;
}

/**
 * \brief			Set the ECG configuration register
 * \param[in]		adcCLK: ADC frequency clock select
 * \param[in]		adcOSR: ADC Over Sampling Ratio
 * \param[in]		ecgGain: Gain selection of ECG PGA
 * \param[in]		iaGain: Gain selection of ECG IA
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t MAX86150_ECG_Configuration(uint8_t adcCLK, uint8_t adcOSR, uint8_t ecgGain, uint8_t iaGain)
{
	uint8_t temp;

	/* Concatenate the ECG register1 required values */
	temp = (adcCLK << 2) | adcOSR;
	/* Rewrite the whole ECG register1 containing the changes */
	if (MAX86150_Write(MAX86150_ECG_CONFIG1, (uint8_t *)&temp, 1) != HAL_OK) return STATUS_KO;

	/* Concatenate the ECG register3 required values */
	temp = (ecgGain << 2) | iaGain;
	/* Rewrite the whole ECG register3 containing the changes */
	if (MAX86150_Write(MAX86150_ECG_CONFIG3, (uint8_t *)&temp, 1) != HAL_OK) return STATUS_KO;

	return STATUS_OK;
}
