#include <nds.h>
#include "sdbasic.h"
#include "crc.h"
#include "io_ewsd.h"

/*
	The generator polynomial is a standard CCITT polynomial
	x^16 + x^12 + x^5 + 1

*/

#define POLYNOMIAL_CODE16 0x1021


u16 sdCRC16L(u8 *ptr, u32 len, u16 init_crc)
{
	u8 i;
	u16 crc = init_crc;

	while(len--)
	{
		for(i=0x80; i!=0; i>>=1)
		{
			if((crc&0x8000)!=0)
			{
				crc<<=1;
				crc^=POLYNOMIAL_CODE16;
			}
			else
				crc<<=1;

			if( (*ptr&i)!=0 )
				crc^=POLYNOMIAL_CODE16;
		}
		ptr++;
	}

	return(crc);
}

u16 crc16_tbl[256]={ // X16+X12+X5+1
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

// Calculate CRC16 CCITT using CRC table
u16 sdCRC16L_Table(u8 *ptr, u32 len, u16 init_crc)
{
	u8 da;
	u16 crc = init_crc;

	while(len-- != 0 )
	{
		da=(u8) (crc >> 8); // HI8 of CRC
		crc <<= 8; // LOW8 -> HI8
		crc ^= crc16_tbl[da ^ *ptr]; // XOR
		ptr++;
	}
	return(crc);
}

//----------------------------------------------------------------------------

/*
	The generator polynomial is a standard CCITT polynomial
	x^7 + x^3 + 1

*/

#define POLYNOMIAL_CODE7 0x09

u8 sdCRC7L(u8 *ptr, u32 len, u8 init_crc)
{
	u8 i;
	u8 crc = init_crc;

	while(len--)
	{
		for(i=0x80; i!=0; i>>=1)
		{

			if((crc&0x40)!=0)
			{
				crc<<=1;
				crc^=POLYNOMIAL_CODE7;
			}
			else
				crc<<=1;

			if( (*ptr&i) != 0 )
				crc^=POLYNOMIAL_CODE7;
		}
		ptr++;
	}

	return(crc & 0x7F);
}


u8 sdmHalf2Bits(u32 a)
{
	u8 ret = 0;
	int i;

	for ( i=0; i<8; i++)
	{
		u32 tmp;

		ret <<= 1;
		tmp = (a & 0xf0000000) >> 28;

		if (tmp==8)
			ret |= 1;

		a <<= 4;

	}

	return ret;
}



// repack 4 bytes
u32 sdCrcRePack32(u32 src)
{
	u32 a;
	u32 i;
	u32 b[4];

	u8 b0 = (u8)(src & 0x000000FF);
	u8 b1 = (u8)((src & 0x0000FF00) >> 8);
	u8 b2 = (u8)((src & 0x00FF0000) >> 16);
	u8 b3 = (u8)((src & 0xFF000000) >> 24);

	src =  LSwapINT32(b3, b2, b1, b0);

	for (i=0; i<4; i++)
	{
		a = src << i;

		a &= 0x88888888;


		b[i] = sdmHalf2Bits(a);

	}


	a = b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];

	return a;

}

void sdDataCrcRePack(u8 *src, u8 *des)
{
	u32 *src32 = (u32 *)src;
	u32 *des32 = (u32 *)des;

	*des32++ = sdCrcRePack32(*src32++);
	*des32 = sdCrcRePack32(*src32);
}

void sdDataLineRePack(u8 *src, u8 *des, u32 blocksize)
{
	u32 i;

	u32 *src32 = (u32 *)src;
	u32 *des32 = (u32 *)des;


	u32 blocksize32 = blocksize /= sizeof(u32);
	for ( i=0; i<blocksize32; i++)
	{
		*des32++ = sdCrcRePack32(*src32++);

	}

}

int sdDataBlockCrcVerify(u8 *dat_unpack, u8* crc_unpack, u32 blocksize)
{
	int ret = SD_ERROR;			// default SD_ERROR

	u8 DatBuffRepacked[SD_BLOCKSIZE];
	u8 DatCrcRepacked[DATA_CRC_SIZE];
	u8 DataPBuff[SD_BLOCKSIZE / 4];  // 4 dat line

	u32 i, j, k;
	u16 cal_crc;
    u16 rd_crc;

	sdDataLineRePack(dat_unpack, DatBuffRepacked, SD_BLOCKSIZE);
	sdDataCrcRePack(crc_unpack, DatCrcRepacked);





	for (k=0; k<4; k++)
	{
		for (i=0, j=0; i<SD_BLOCKSIZE / 4; i++, j+=4)
		{
			DataPBuff[i] = DatBuffRepacked[j+k];
		}

		cal_crc = sdCRC16L_Table(DataPBuff, SD_BLOCKSIZE / 4, 0);
		rd_crc = DatCrcRepacked[k] << 8 | DatCrcRepacked[k+4];

		if ( cal_crc != rd_crc)
			break;
	}

	if ( k == 4 ) // all data line checked!
		ret = SD_SUCCESS;

	return ret;


}



void sdCRC16X (u8* buff, int buffLength, u8* crc16buff)
{
	u32 a, b, c, d;
	int count;
	u32 bitPattern = 0x80808080;	// r7
	u32 crcConst = 0x1021;	// r8
	u32 dataByte = 0;	// r2

	a = 0;	// r3
	b = 0;	// r4
	c = 0;	// r5
	d = 0;	// r6

	buffLength = buffLength * 8;


	do {
		if (bitPattern & 0x80) dataByte = *buff++;

		a = a << 1;
		if ( a & 0x10000) a ^= crcConst;
		if (dataByte & (bitPattern >> 24)) a ^= crcConst;

		b = b << 1;
		if (b & 0x10000) b ^= crcConst;
		if (dataByte & (bitPattern >> 25)) b ^= crcConst;

		c = c << 1;
		if (c & 0x10000) c ^= crcConst;
		if (dataByte & (bitPattern >> 26)) c ^= crcConst;

		d = d << 1;
		if (d & 0x10000) d ^= crcConst;
		if (dataByte & (bitPattern >> 27)) d ^= crcConst;

		bitPattern = (bitPattern >> 4) | (bitPattern << 28);
	} while (buffLength-=4);

	count = 16;	// r8

	do {
		bitPattern = bitPattern << 4;
		if (a & 0x8000) bitPattern |= 8;
		if (b & 0x8000) bitPattern |= 4;
		if (c & 0x8000) bitPattern |= 2;
		if (d & 0x8000) bitPattern |= 1;

		a = a << 1;
		b = b << 1;
		c = c << 1;
		d = d << 1;

		count--;

		if (!(count & 0x01)) {
			*crc16buff++ = (u8)(bitPattern & 0xff);
		}
	} while (count != 0);

	return;
}

