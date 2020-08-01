/**
 * \file			stm32l4xx_bq27421.c
 * \author    		Paul THELU
 * \version			1.0
 * \date			21 January 2020
 * \brief			BQ27421 battery supervisor driver
 */

#include <stdbool.h>
#include <string.h>
#include "../inc/global.h"
#include "stm32l4xx_hal.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_bq27421.h"

extern I2C_HandleTypeDef hi2c4;

/**
 * \brief			Initializes the bq27421 battery supervisor device
 * \param[in]		hi2c: I2C handler
 * \param[in]		designCapacity_mAh: Battery designed capacity in mAh
 * \param[in]		terminateVoltage_mV:
 * \param[in]		taperCurrent_mA:
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BQ27421_Init(uint16_t designCapacity_mAh, uint16_t terminateVoltage_mV, uint16_t taperCurrent_mA)
{
	uint8_t checksumNew;
	uint16_t designEnergy_mWh, taperRate, flags, checksumOld, checksumRead;

	designEnergy_mWh = 3.7 * designCapacity_mAh;
	taperRate = designCapacity_mAh / (0.1 * taperCurrent_mA);
	// Unseal gauge
	BQ27421_Control_Write(BQ27421_CONTROL_UNSEAL);
	BQ27421_Control_Write(BQ27421_CONTROL_UNSEAL);

	// Send CFG_UPDATE
	BQ27421_Control_Write(BQ27421_CONTROL_SET_CFGUPDATE);

	// Poll flags
	do {
		BQ27421_Command_Read(BQ27421_FLAGS_LOW, &flags);
		if (!(flags & 0x0010)) {
			HAL_Delay(50);
		}
	} while (!(flags & 0x0010));

	// Enable Block Data Memory Control
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CONTROL, 0x0000);

	HAL_Delay(BQ27421_DELAY);

	// Access State subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0052);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Read block checksum
	BQ27421_Command_Read(BQ27421_BLOCK_DATA_CHECKSUM, &checksumOld);

	// Read 32-byte block of data
	uint8_t block[32];
	for (uint8_t i = 0; i < 32; ++i) {
		block[i] = 0x00;
	}

	BQ27421_Read_Data_Block(0x00, block, 32);

	// Calculate checksum
	uint8_t checksumCalc = 0x00;
	for (uint8_t i = 0; i < 32; ++i) {
		checksumCalc += block[i];
	}
	checksumCalc = 0xFF - checksumCalc;

	// Update design capacity
	block[10] = (uint8_t)(designCapacity_mAh >> 8);
	block[11] = (uint8_t)(designCapacity_mAh & 0x00FF);
	// Update design energy
	block[12] = (uint8_t)(designEnergy_mWh >> 8);
	block[13] = (uint8_t)(designEnergy_mWh & 0x00FF);
	// Update terminate voltage
	block[16] = (uint8_t)(terminateVoltage_mV >> 8);
	block[17] = (uint8_t)(terminateVoltage_mV & 0x00FF);
	// Update taper rate
	block[27] = (uint8_t)(taperRate >> 8);
	block[28] = (uint8_t)(taperRate & 0x00FF);

	// Calculate new checksum
	checksumNew = 0x00;
	for (int i = 0; i < 32; ++i) {
		checksumNew += block[i];
	}
	checksumNew = 0xFF - checksumNew;

	// Enable Block Data Memory Control
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CONTROL, 0x0000);

	HAL_Delay(BQ27421_DELAY);

	// Access State subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0052);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Write 32-byte block of updated data
	BQ27421_Write_Data_Block(0x00, block, 32);

	// Write new checksum
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CHECKSUM, checksumNew);

	// Access State subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0052);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Read block checksum
	BQ27421_Command_Read(BQ27421_BLOCK_DATA_CHECKSUM, &checksumRead);

	// Verify
	if (checksumRead != (uint8_t)checksumNew) {
		return STATUS_KO;
	}

	// Enable Block Data Memory Control
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CONTROL, 0x0000);

	HAL_Delay(BQ27421_DELAY);

	// Access Registers subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0040);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Read block checksum
	BQ27421_Command_Read(BQ27421_BLOCK_DATA_CHECKSUM, &checksumOld);

	// Read 32-byte block of data
	for (uint8_t i = 0; i < 32; ++i) {
		block[i] = 0x00;
	}

	BQ27421_Read_Data_Block(0x00, block, 32);

	// Calculate checksum
	checksumCalc = 0x00;
	for(uint8_t i = 0; i < 32; ++i) {
		checksumCalc += block[i];
	}
	checksumCalc = 0xFF - checksumCalc;

	// Update OpConfig
	block[0] = 0x05;

	// Calculate new checksum
	checksumNew = 0x00;
	for (int i = 0; i < 32; ++i) {
		checksumNew += block[i];
	}
	checksumNew = 0xFF - checksumNew;

	// Enable Block Data Memory Control
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CONTROL, 0x0000);

	HAL_Delay(BQ27421_DELAY);

	// Access Registers subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0040);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Write 32-byte block of updated data
	BQ27421_Write_Data_Block(0x00, block, 32);

	// Write new checksum
	BQ27421_Command_Write(BQ27421_BLOCK_DATA_CHECKSUM, checksumNew);

	// Access Registers subclass
	BQ27421_Command_Write(BQ27421_DATA_CLASS, 0x0040);

	// Write the block offset
	BQ27421_Command_Write(BQ27421_DATA_BLOCK, 0x0000);

	// Read block checksum
	BQ27421_Command_Read(BQ27421_BLOCK_DATA_CHECKSUM, &checksumRead);

	// Verify
	if (checksumRead != (uint8_t)checksumNew) {
		return STATUS_KO;
	}

	// Configure BAT_DET
	BQ27421_Control_Write(BQ27421_CONTROL_BAT_INSERT);

	// Send Soft Reset
	BQ27421_Control_Write(BQ27421_CONTROL_SOFT_RESET);

	// Poll flags
	do {
		BQ27421_Command_Read(BQ27421_FLAGS_LOW, &flags);
		if (!(flags & 0x0010)) {
			HAL_Delay(50);
		}
	} while ((flags & 0x0010));

	// Seal gauge
	BQ27421_Control_Write(BQ27421_CONTROL_SEALED);

	return STATUS_OK;
}

status_t BQ27421_Update(bq27421_info_t *battery)
{
	uint16_t temp;

	if (BQ27421_ReadVoltage_mV(&(battery->voltage_mV)) != STATUS_OK) 		return STATUS_KO;
	if (BQ27421_ReadAvgCurrent_mA(&(battery->current_mA)) != STATUS_OK) 	return STATUS_KO;
	if (BQ27421_ReadTemp_degK(&temp) != STATUS_OK) 						return STATUS_KO;

	battery->temp_degC = ((double)temp / 10) - 273.15;

	if (BQ27421_ReadStateofCharge_percent(&(battery->soc_percent)) != STATUS_OK) 				return STATUS_KO;
	if (BQ27421_ReadStateofHealth_percent(&(battery->soh_percent)) != STATUS_OK) 				return STATUS_KO;
	if (BQ27421_ReadDesignCapacity_mAh(&(battery->designCapacity_mAh)) != STATUS_OK) 			return STATUS_KO;
	if (BQ27421_ReadRemainingCapacity_mAh(&(battery->remainingCapacity_mAh)) != STATUS_OK) 	return STATUS_KO;
	if (BQ27421_ReadFullChargeCapacity_mAh(&(battery->fullChargeCapacity_mAh)) != STATUS_OK) 	return STATUS_KO;
	if (BQ27421_ReadFlagsReg(&temp) != STATUS_OK) 											return STATUS_KO;

	battery->isCritical = temp & 0x0002;
	battery->isLow = temp & 0x0004;
	battery->isFull = temp & 0x0200;

	if (battery->current_mA <= 0) {
		battery->isDischarging = 1;
		battery->isCharging = 0;
	} else {
		battery->isDischarging = 0;
		battery->isCharging = 1;
	}
	return STATUS_OK;
}

/**
 * \brief			Writes commands to the bq27421 battery supervisor device
 * \param[in]		hi2c: I2C handler
 * \param[in]		command: Command register to write to
 * \param[in]		data: Data to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BQ27421_Command_Write(uint8_t command, uint16_t data)
{
	uint8_t i2c_data[3];

	i2c_data[0] = command;
	i2c_data[1] = (uint8_t)(data & 0x00FF);
	i2c_data[2] = (uint8_t)((data >> 8) & 0x00FF);

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 3, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	HAL_Delay(BQ27421_DELAY);

	return STATUS_OK;
}

/**
 * \brief			Reads commands from the bq27421 battery supervisor device
 * \param[in]		hi2c: I2C handler
 * \param[in]		command: Command register to read from
 * \param[in]		data: Data to receive
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BQ27421_Command_Read(uint8_t command, uint16_t *data)
{
	uint8_t i2c_data[2];

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, &command, 1, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	HAL_Delay(BQ27421_DELAY);

	if (HAL_I2C_Master_Receive(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) {
		return STATUS_KO;
	}

	HAL_Delay(BQ27421_DELAY);

	/* Stores the new 16bits long data */
	*data = (i2c_data[1] << 8) | i2c_data[0];

	return STATUS_OK;
}

/**
 * \brief			Writes control to the bq27421 battery supervisor device
 * \param[in]		hi2c: I2C handler
 * \param[in]		subcommand: Subcommand to write to
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t BQ27421_Control_Write(uint16_t subcommand)
{
	uint8_t i2c_data[2];

	i2c_data[0] = BQ27421_CONTROL_LOW;
	i2c_data[1] = (uint8_t)((subcommand) & 0x00FF);

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	i2c_data[0] = BQ27421_CONTROL_HIGH;
	i2c_data[1] = (uint8_t)((subcommand >> 8) & 0x00FF);

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	return STATUS_OK;
}

status_t BQ27421_Control_Read(uint16_t subcommand, uint16_t *data)
{
	uint8_t i2c_data[2];

	i2c_data[0] = BQ27421_CONTROL_LOW;
	i2c_data[1] = (uint8_t)((subcommand) & 0x00FF);

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	i2c_data[0] = BQ27421_CONTROL_HIGH;
	i2c_data[1] = (uint8_t)((subcommand >> 8) & 0x00FF);

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	if (HAL_I2C_Master_Receive(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	*data = (i2c_data[1] << 8) | i2c_data[0];

	return STATUS_OK;
}

status_t BQ27421_Write_Data_Block(uint8_t offset, uint8_t *data, uint8_t bytes)
{
	uint8_t i2c_data[2];

	for (int i = 0; i < bytes; ++i) {
		i2c_data[0] = BQ27421_BLOCK_DATA_START + offset + i;
		i2c_data[1] = data[i];

		if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, i2c_data, 2, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
		HAL_Delay(BQ27421_DELAY);
	}
	return STATUS_OK;
}

status_t BQ27421_Read_Data_Block(uint8_t offset, uint8_t *data, uint8_t bytes)
{
	uint8_t i2c_data;

	i2c_data = BQ27421_BLOCK_DATA_START + offset;

	if (HAL_I2C_Master_Transmit(&hi2c4, BQ27421_I2C_ADDRESS, &i2c_data, 1, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(5);

	if (HAL_I2C_Master_Receive(&hi2c4, BQ27421_I2C_ADDRESS, data, bytes, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	return STATUS_OK;
}

status_t BQ27421_ReadDeviceType(uint16_t *deviceType)
{
	if (BQ27421_Control_Write(BQ27421_CONTROL_DEVICE_TYPE) != STATUS_OK) {
		return STATUS_KO;
	}
	if (BQ27421_Command_Read(BQ27421_CONTROL_LOW, deviceType) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadDeviceFWver(uint16_t *deviceFWver)
{
	if (!BQ27421_Control_Write(BQ27421_CONTROL_FW_VERSION) != STATUS_OK) {
		return STATUS_KO;
	}
	if (BQ27421_Command_Read(BQ27421_CONTROL_LOW, deviceFWver) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadDesignCapacity_mAh(uint16_t *capacity_mAh)
{
	if (BQ27421_Command_Read(BQ27421_DESIGN_CAP_LOW, capacity_mAh) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadVoltage_mV(uint16_t *voltage_mV)
{
	if (BQ27421_Command_Read(BQ27421_VOLTAGE_LOW, voltage_mV) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadTemp_degK(uint16_t *temp_degKbyTen)
{
	if (BQ27421_Command_Read(BQ27421_TEMP_LOW, temp_degKbyTen) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadAvgCurrent_mA(int16_t *avgCurrent_mA)
{
	if (BQ27421_Command_Read(BQ27421_AVG_CURRENT_LOW, (uint16_t *)avgCurrent_mA) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadStateofCharge_percent(uint16_t *soc_percent)
{
	if (BQ27421_Command_Read(BQ27421_STATE_OF_CHARGE_LOW, soc_percent) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadControlReg(uint16_t *control)
{
	if (BQ27421_Control_Write(BQ27421_CONTROL_STATUS) != STATUS_OK) {
		return STATUS_KO;
	}
	if (BQ27421_Command_Read(BQ27421_CONTROL_LOW, control) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadFlagsReg(uint16_t *flags)
{
	if (BQ27421_Command_Read(BQ27421_FLAGS_LOW, flags) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadOpConfig(uint16_t *opConfig)
{
	if (BQ27421_Command_Read(BQ27421_OPCONFIG_LOW, opConfig) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadRemainingCapacity_mAh(uint16_t *capacity_mAh)
{
	if (BQ27421_Command_Read(BQ27421_REMAINING_CAP_LOW, capacity_mAh) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadFullChargeCapacity_mAh(uint16_t *capacity_mAh)
{
	if (BQ27421_Command_Read(BQ27421_FULL_CHARGE_CAP_LOW, capacity_mAh) != STATUS_OK) {
		return STATUS_KO;
	}
	return STATUS_OK;
}

status_t BQ27421_ReadStateofHealth_percent(uint16_t *soh_percent)
{
	if (BQ27421_Command_Read(BQ27421_STATE_OF_HEALTH_LOW, soh_percent) != STATUS_OK) {
		return STATUS_KO;
	}
	*soh_percent = *soh_percent & 0x00FF;

	return STATUS_OK;
}

status_t BQ27421_Write(unsigned char nRegister, uint8_t *pData, int nDataLength)
{
	if (HAL_I2C_Mem_Write(&hi2c4, BQ27421_I2C_ADDRESS, nRegister, I2C_MEMADD_SIZE_8BIT, pData, nDataLength, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	return STATUS_OK;
}

status_t BQ27421_Read(unsigned char nRegister, uint8_t *pDataFromGauge, int nDataLength)
{
	if (HAL_I2C_Mem_Read(&hi2c4, BQ27421_I2C_ADDRESS, nRegister, I2C_MEMADD_SIZE_8BIT, pDataFromGauge, nDataLength, I2C_MAX_DELAY) != HAL_OK) return STATUS_KO;
	HAL_Delay(BQ27421_DELAY);

	return STATUS_OK;
}
