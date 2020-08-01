/*
 * File:   sst26vf064b.h
 * Author: Paul
 */

#ifndef MONITAURE_DRIVERS_INC_NVM_SST26VF0XXB_H_
#define	MONITAURE_DRIVERS_INC_NVM_SST26VF0XXB_H_

/* Private defines -----------------------------------------------------------*/
#define SST26_SPI_CHANNEL       1
#define SST26_CMD_READ          0x03    // read memory
#define SST26_CMD_WRITE         0x02    // program a page (can be any number between 1 - 256 bytes)
#define SST26_CMD_SER           0x20    // erase sector (4 KByte of memory)
#define SST26_CMD_ERASE         0xC7    // chip erase
#define SST26_CMD_RDSR          0x05    // read status register
#define SST26_CMD_WRSR          0x01    // write status register
#define SST26_CMD_WREN          0x06    // write enable
#define SST26_CMD_WRDI          0x04    // write disable
#define SST26_CMD_RSTEN         0x66    // reset enable
#define SST26_CMD_RST           0x99    // reset command
#define SST26_CMD_RSTQIO        0xFF    // reset mode to SPI
#define SST26_CMD_WBPR          0x42    // write to block protect register
#define SST26_CMD_LBPR          0x8D    // global block protection lock command
#define SST26_CMD_ULBPR         0x98    // global block protection unlock command
#define SST26_CMD_NVWLDR        0xE8    // non-volatile write lockdown register write command
#define SST26_CMD_JEDECID       0x9F    // JEDEC ID read command

// Non Volatile Memory - SST26VF064B
// Communication SPI NVM
unsigned char NVM_SST26VF0XXB_Write(unsigned long int address, unsigned char *pData, unsigned int nCount);
void NVM_SST26VF0XXB_Read(unsigned long int address, unsigned char *pData, unsigned int nCount);
void NVM_SST26VF0XXB_ChipErase(void);
void NVM_SST26VF0XXB_SectorErase(unsigned long int address);
unsigned char NVM_SST26VF0XXB_ReadStatusRegister(void);
void NVM_SST26VF0XXB_WriteStatusRegister(unsigned char newStatus);
void NVM_SST26VF0XXB_Reset(void);
unsigned char NVM_SST26VF0XXB_JEDEC_ID_Check(void);

#endif	/* MONITAURE_DRIVERS_INC_NVM_SST26VF0XXB_H_ */

