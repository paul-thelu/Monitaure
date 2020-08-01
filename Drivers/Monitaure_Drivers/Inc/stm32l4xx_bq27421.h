/*
 * stm32l4xx_bq27421.h
 *
 *  Created on: 11 juil. 2019
 *      Author: PaulT
 */

#ifndef MONITAURE_DRIVERS_INC_STM32L4XX_BQ27421_H_
#define MONITAURE_DRIVERS_INC_STM32L4XX_BQ27421_H_

#define BQ27421_DELAY                       1
#define BQ27421_I2C_ADDRESS                 (uint16_t)0xAA
#define BQ27421_CONTROL_LOW                 0x00
#define BQ27421_CONTROL_HIGH                0x01
#define BQ27421_TEMP_LOW                    0x02
#define BQ27421_TEMP_HIGH                   0x03
#define BQ27421_VOLTAGE_LOW                 0x04
#define BQ27421_VOLTAGE_HIGH                0x05
#define BQ27421_FLAGS_LOW                   0x06
#define BQ27421_FLAGS_HIGH                  0x07
#define BQ27421_NOM_AVAILABLE_CAP_LOW       0x08
#define BQ27421_NOM_AVAILABLE_CAP_HIGH      0x09
#define BQ27421_FULL_AVAILABLE_CAP_LOW      0x0A
#define BQ27421_FULL_AVAILABLE_CAP_HIGH     0x0B
#define BQ27421_REMAINING_CAP_LOW           0x0C
#define BQ27421_REMAINING_CAP_HIGH          0x0D
#define BQ27421_FULL_CHARGE_CAP_LOW         0x0E
#define BQ27421_FULL_CHARGE_CAP_HIGH        0x0F
#define BQ27421_AVG_CURRENT_LOW             0x10
#define BQ27421_AVG_CURRENT_HIGH            0x11
#define BQ27421_STANDBY_CURRENT_LOW         0x12
#define BQ27421_STANDBY_CURRENT_HIGH        0x13
#define BQ27421_MAX_LOAD_CURRENT_LOW        0x14
#define BQ27421_MAX_LOAD_CURRENT_HIGH       0x15
#define BQ27421_AVG_POWER_LOW               0x18
#define BQ27421_AVG_POWER_HIGH              0x19
#define BQ27421_STATE_OF_CHARGE_LOW         0x1C
#define BQ27421_STATE_OF_CHARGE_HIGH        0x1D
#define BQ27421_INT_TEMP_LOW                0x1E
#define BQ27421_INT_TEMP_HIGH               0x1F
#define BQ27421_STATE_OF_HEALTH_LOW         0x20
#define BQ27421_STATE_OF_HEALTH_HIGH        0x21
#define BQ27421_REMAINING_CAP_UNFILT_LOW    0x28
#define BQ27421_REMAINING_CAP_UNFILT_HIGH   0x29
#define BQ27421_REMAINING_CAP_FILT_LOW      0x2A
#define BQ27421_REMAINING_CAP_FILT_HIGH     0x2B
#define BQ27421_FULL_CHARGE_UNFILT_CAP_LOW  0x2C
#define BQ27421_FULL_CHARGE_UNFILT_CAP_HIGH 0x2D
#define BQ27421_FULL_CHARGE_FILT_CAP_LOW    0x2E
#define BQ27421_FULL_CHARGE_FILT_CAP_HIGH   0x2F
#define BQ27421_STATE_OF_CHARGE_UNFILT_LOW  0x30
#define BQ27421_STATE_OF_CHARGE_UNFILT_HIGH 0x31
#define BQ27421_OPCONFIG_LOW                0x3A
#define BQ27421_OPCONFIG_HIGH               0x3B
#define BQ27421_DESIGN_CAP_LOW              0x3C
#define BQ27421_DESIGN_CAP_HIGH             0x3D
#define BQ27421_DATA_CLASS                  0x3E
#define BQ27421_DATA_BLOCK                  0x3F
#define BQ27421_BLOCK_DATA_START            0x40
#define BQ27421_BLOCK_DATA_END              0x5F
#define BQ27421_BLOCK_DATA_CHECKSUM         0x60
#define BQ27421_BLOCK_DATA_CONTROL          0x61
#define BQ27421_CONTROL_STATUS              0x0000
#define BQ27421_CONTROL_DEVICE_TYPE         0x0001
#define BQ27421_CONTROL_FW_VERSION          0x0002
#define BQ27421_CONTROL_DM_CODE             0x0004
#define BQ27421_CONTROL_PREV_MACWRITE       0x0007
#define BQ27421_CONTROL_CHEM_ID             0x0008
#define BQ27421_CONTROL_BAT_INSERT          0x000C
#define BQ27421_CONTROL_BAT_REMOVE          0x000D
#define BQ27421_CONTROL_SET_HIBERNATE       0x0011
#define BQ27421_CONTROL_CLEAR_HIBERNATE     0x0012
#define BQ27421_CONTROL_SET_CFGUPDATE       0x0013
#define BQ27421_CONTROL_SHUTDOWN_ENABLE     0x001B
#define BQ27421_CONTROL_SHUTDOWN            0x001C
#define BQ27421_CONTROL_SEALED              0x0020
#define BQ27421_CONTROL_TOGGLE_GPOUT        0x0023
#define BQ27421_CONTROL_RESET               0x0041
#define BQ27421_CONTROL_SOFT_RESET          0x0042
#define BQ27421_CONTROL_EXIT_CFGUPDATE      0x0043
#define BQ27421_CONTROL_EXIT_RESIM          0x0044
#define BQ27421_CONTROL_UNSEAL              0x8000

typedef struct bq27421_info {
    uint16_t    voltage_mV;
    int16_t     current_mA;
    double      temp_degC;
    uint16_t    soc_percent;
    uint16_t    soh_percent;
    uint16_t    designCapacity_mAh;
    uint16_t    remainingCapacity_mAh;
    uint16_t    fullChargeCapacity_mAh;

    bool        isCritical;
    bool        isLow;
    bool        isFull;
    bool        isCharging;
    bool        isDischarging;
} bq27421_info_t;

status_t BQ27421_Init(uint16_t designCapacity_mAh, uint16_t terminateVoltage_mV, uint16_t taperCurrent_mA);
status_t BQ27421_Update(bq27421_info_t *battery);
status_t BQ27421_ReadDeviceType(uint16_t *deviceType);
status_t BQ27421_ReadDeviceFWver(uint16_t *deviceFWver);
status_t BQ27421_ReadDesignCapacity_mAh(uint16_t *capacity_mAh);

status_t BQ27421_ReadVoltage_mV(uint16_t *voltage_mV);
status_t BQ27421_ReadTemp_degK(uint16_t *temp_degKbyTen);
status_t BQ27421_ReadAvgCurrent_mA(int16_t *avgCurrent_mA);
status_t BQ27421_ReadStateofCharge_percent(uint16_t *soc_percent);

status_t BQ27421_ReadControlReg(uint16_t *control);
status_t BQ27421_ReadFlagsReg(uint16_t *flags);
status_t BQ27421_ReadOpConfig(uint16_t *opConfig);
status_t BQ27421_ReadRemainingCapacity_mAh(uint16_t *capacity_mAh);
status_t BQ27421_ReadFullChargeCapacity_mAh(uint16_t *capacity_mAh);
status_t BQ27421_ReadStateofHealth_percent(uint16_t *soh_percent);

status_t BQ27421_Command_Write(uint8_t command, uint16_t data);
status_t BQ27421_Command_Read(uint8_t command, uint16_t *data);

status_t BQ27421_Control_Write(uint16_t subcommand);
status_t BQ27421_Control_Read(uint16_t subcommand, uint16_t *data);
status_t BQ27421_Write_Data_Block(uint8_t offset, uint8_t *data, uint8_t bytes);
status_t BQ27421_Read_Data_Block(uint8_t offset, uint8_t *data, uint8_t bytes);

#endif /* MONITAURE_DRIVERS_INC_STM32L4XX_BQ27421_H_ */
