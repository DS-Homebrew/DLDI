#ifndef CRC_INC
#define CRC_INC

// Sd data line CRC code 
// note the crc is calculated indivadually
#define DATA_CRC_SIZE 8	


u8 sdCRC7L(u8 *ptr, u32 len, u8 init_crc);

u16 sdCRC16L(u8 *ptr, u32 len, u16 init_crc);
u16 sdCRC16L_Table(u8 *ptr, u32 len, u16 init_crc);
void sdCRC16X (u8* buff, int buffLength, u8* crc16buff);

void sdDataLineRePack(u8 *src, u8 *des, u32 blocksize);
void sdDataCrcRePack(u8 *src, u8 *des);

int sdDataBlockCrcVerify(u8 *dat_unpack, u8* crc_unpack, u32 blocksize);


#endif // CRC_INC


