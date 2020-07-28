#include "common.h"

#define NEO_ASM

// this macro generates efficent code for all NEO2 ASIC commands
// the internal variable is required by the macro
static u16 _neo2_asic_command_dummy;
#define NEO2_ASIC_COMMAND( addr, data ) _neo2_asic_command_dummy = *(vu16*)( 0x08000000 + ( (addr<<16) * 2) + ( data * 2 ) )

extern bool write_data_asm (u8* buffer);

void neo2_asic_pre_command()	{
	NEO2_ASIC_COMMAND( 0xff, 0xd200 );
	NEO2_ASIC_COMMAND( 0x00, 0x1500 );
	NEO2_ASIC_COMMAND( 0x01, 0xd200 );
	NEO2_ASIC_COMMAND( 0x02, 0x1500 );
	NEO2_ASIC_COMMAND( 0xfe, 0x1500 );
}

void set_neo2_asic_cr( u16 cr )	{
	neo2_asic_pre_command();
	NEO2_ASIC_COMMAND( 0x37, cr );
}


#ifndef NEO_ASM

void wrMmcCmdBit( u32 bit )	{
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |			// always 1
		( 1<<19 ) |			// clk enable
		( 1<<18 ) |			// cs remap
		( 1<<17 ) |			// cmd remap
		( 1<<16 ) |			// d1-d3 remap
		( 1<<15 ) |			// d0 remap
		( 1<<14 ) |			// cs value (active high)
		( (bit&1)<<13 ) |		// cmd value
		( 1<<12 ) |			// d3 value
		( 1<<11 ) |			// d2 value
		( 1<<10 ) |			// d1 value
		( 1<<9 );				// d0 value
		data = *(vu16*)(addr);
}

void wrMmcCmdByte( u32 byte )	{
	register int i;
	for( i = 0; i < 8 ; i++ )
		wrMmcCmdBit( (byte>>(7-i)) & 1 );
}

#endif

void wrMmcDatBit( u32 bit )	{
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |			// always 1
		( 1<<19 ) |			// clk enable
		( 1<<18 ) |			// cs remap
		( 1<<17 ) |			// cmd remap
		( 1<<16 ) |			// d1-d3 remap
		( 1<<15 ) |			// d0 remap
		( 1<<14 ) |			// cs value (active high)
		( 1<<13 ) |		  // cmd value
		( 1<<12 ) |			// d3 value
		( 1<<11 ) |			// d2 value
		( 1<<10 ) |			// d1 value
		( (bit&1)<<9 );	// d0 value
		data = *(vu16*)(addr);
}

void wrMmcDatByte( u32 byte )	{
	register int i;
	for( i = 0; i < 8 ; i++ )
		wrMmcDatBit( (byte>>(7-i)) & 1 );
}

#ifndef NEO_ASM

u32 rdMmcCmdBit()	{
	static int i;
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |			// always 1
		( 1<<19 ) |			// clk enable
		( 1<<18 ) |			// cs remap
		( 1<<17 ) |			// cmd remap
		( 1<<16 ) |			// d1-d3 remap
		( 1<<15 ) |		// d0 remap
		( 1<<14 ) |		// cs value (active high)
		( 1<<13 ) |			// cmd value
		( 1<<12 ) |			// d3
		( 1<<11 ) |			// d2
		( 1<<10 ) |			// d1
		( 1<<9 ) |			// d0
		( 1<<7 );				// cmd input mode
	data = *(vu16*)(addr);
	return (data>>12) & 1;

}

#endif

// 1 to 8 bits please

#ifndef NEO_ASM

u8 rdMmcCmdBits( int num )	{
	register int i;
	u8 byte = 0;
	for( i = 0; i < num ; i++ )
		byte = ( byte << 1) | rdMmcCmdBit();
	return byte;
}

u8 rdMmcCmdByte()	{
	return rdMmcCmdBits( 8 );
}

#endif

u32 rdMmcDatBit()	{
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |		// always 1
		( 1<<19 ) |		// clk enable
		( 1<<18 ) |		// cs remap
		( 1<<17 ) |		// cmd remap
		( 1<<16 ) |		// d1-d3 remap
		//( 1<<15 ) |		// d0 remap
		( 1<<14 ) |		// cs value (active high)
		( 1<<13 ) |		// cmd value
		( 1<<12 ) |		// d3
		( 1<<11 ) |		// d2
		( 1<<10 ) |		// d1
		//( 1<<9 ) |		// d0
		//( 1<<6 ) |	// d3-d1 input (only needed in 4 bit mode)
		( 1<<5);			// d0 input




	data = *(vu16*)(addr);
	return (data>>8) & 1;

}



#ifndef NEO_ASM

u32 rdMmcDatBit4()	{
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |		// always 1
		( 1<<19 ) |		// clk enable
		( 1<<18 ) |		// cs remap
		( 1<<17 ) |		// cmd remap
		//( 1<<16 ) |		// d1-d3 remap
		//( 1<<15 ) |		// d0 remap
		( 1<<14 ) |		// cs value (active high)
		( 1<<13 ) |		// cmd value
		//( 1<<12 ) |		// d3
		//( 1<<11 ) |		// d2
		//( 1<<10 ) |		// d1
		//( 1<<9 ) |		// d0
		( 1<<6 ) |	// d3-d1 input (only needed in 4 bit mode)
		( 1<<5);			// d0 input
	data = *(vu16*)(addr);
	return (data>>8) & 15;

}

#endif

#ifndef NEO_ASM

void wrMmcDatBit4( u8 dat )	{
	register u16 data;
	register u32 addr =
		0x08000000 |
		( 1<<24 ) |		// always 1
		( 1<<19 ) |		// clk enable
		( 1<<18 ) |		// cs remap
		( 1<<17 ) |		// cmd remap
		( 1<<14 ) |		// cs value (active high)
		( 1<<13 );
	*(vu16*)(addr) = ( ( dat & 15 ) << 8 );
}

void wrMmcDatByte4( u8 val )	{
	wrMmcDatBit4( (val >> 4) & 15 );
	wrMmcDatBit4( val & 15 );
}

// 4 to 8 bits please
u8 rdMmcDatByte4()	{
	u8 byte = rdMmcDatBit4();
	byte = ( byte << 4) | rdMmcDatBit4();
	return byte;
}

#endif

// 1 to 8 bits please

u8 rdMmcDatByte()	{
	register int i;
	u8 byte = 0;
	for( i = 0; i < 8 ; i++ )
		byte = ( byte << 1) | rdMmcDatBit();
	return byte;
}

#ifndef NEO_ASM

 /*
* Computation of CRC-7 0x48 polynom (x**7 + x**3 + 1)
* Used in MMC commands and responses.
*/

const unsigned char crcTab[256]={
	0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77,
	0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26, 0x51, 0x58, 0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E,
	0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D, 0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45,
	0x2B, 0x22, 0x39, 0x30, 0x0F, 0x06, 0x1D, 0x14, 0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C,
	0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B, 0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01, 0x1A, 0x13,
	0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42, 0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A,
	0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69, 0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21,
	0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70, 0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E, 0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36,
	0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67, 0x10, 0x19, 0x02, 0x0B, 0x34, 0x3D, 0x26, 0x2F,
	0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C, 0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04,
	0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47, 0x5C, 0x55, 0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D,
	0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A, 0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52,
	0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03, 0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B,
	0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28, 0x5F, 0x56, 0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60,
	0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31, 0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79
};


u8 crc7 (unsigned char *buf)
{
  register unsigned int i;
  u8 crc7_accum = 0;
  register u8 crc=0;
  for (i = 0;  i < 5;  ++i)	{
      crc7_accum = crcTab[(crc7_accum << 1) ^ buf[i]];
  }
  crc ^= crc7_accum;
  return crc;
}

#endif

void debugMmcResp( u8 cmd, u8 *resp )	{
		/*swiWaitForVBlank();
		clearFrameBuf();
		drawFwTitle();
		drawFwToolbar();
		drawMessage( "CMD%u: %x %x %x %x %x %x", cmd, resp[0], resp[1], resp[2], resp[3], resp[4], resp[5] );
		flushFrameBuf();
		while( 1 )	{
			scanKeys();
			if( keysDown() & 1 )				{
				break;
			}
		}*/
}

void debugMmcPrint( char *str )	{
		/*swiWaitForVBlank();
		clearFrameBuf();
		drawFwTitle();
		drawFwToolbar();
		drawMessage( "%s", str );
		flushFrameBuf();
		while( 1 )	{
			scanKeys();
			if( keysDown() & 1 )				{
				break;
			}
		}*/
}

#ifndef NEO_ASM

void sendMmcCmd( u8 cmd, u32 arg )	{
	u32 i;
	u8 p[6];
	p[0]=cmd|0x40;
	p[1]=(u8)(arg >> 24);
	p[2]=(u8)(arg >> 16);
	p[3]=(u8)(arg >> 8);
	p[4]=(u8)(arg);
	p[5]=( crc7(p) << 1 ) | 1;
	wrMmcCmdByte(0xFF);
	wrMmcCmdByte(0xFF);
	wrMmcCmdByte( p[0] );
	wrMmcCmdByte( p[1] );
	wrMmcCmdByte( p[2] );
	wrMmcCmdByte( p[3] );
	wrMmcCmdByte( p[4] );
	wrMmcCmdByte( p[5] );
}

bool recvMmcCmdResp( u8 *resp, u32 len )
{
    u32 i,j;
    for (i=0; i<1024; i++) {
        if(rdMmcCmdBit()==0) {
            *resp++ = rdMmcCmdBits(7);
            len--;
            for(j=0; j<len; j++) {
            	*resp++ = rdMmcCmdByte();
    				}
    				return 1;
        }
    }
    return 0;
}

bool recvMmcCmdResp1( u8 *resp )	{
	return recvMmcCmdResp( resp, 48/8 );
}

#endif

bool recvMmcCmdResp2( u8 *resp )	{
	return recvMmcCmdResp( resp, 136/8 );
}

bool recvMmcCmdResp3( u8 *resp )	{
	return recvMmcCmdResp( resp, 48/8 );
}


void sendMmcGoIdleStateCmd()	{
	sendMmcCmd( 0, 0 );
}

bool sendMmcSendOpCondCmd( u32 ocr, u32 *ocr_out )	{
	u8 resp[48/8];
	sendMmcCmd( 1, ocr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 0x3f) && (resp[1] & 0x80)  )	{
		if (ocr_out) *ocr_out = ((resp[1] << 24) | (resp[2] << 16) | (resp[3] << 8) | resp[4]);
		return true;
	}
	debugMmcResp( 1, resp );
	return false;
}

bool sendSdAppCmdCmd( u16 rca )	{
	u8 resp[48/8];
	sendMmcCmd( 55, (((rca>>8)&0xFF)<<24) | ((rca&0xFF)<<16) | 0xFFFF );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 55) )
		return true;
	//debugMmcResp( 55, resp );
	return false;
}

bool sendSdSdAppOpCondCmd( u32 ocr, u32 *ocr_out )	{
	u8 resp[48/8];
	if( sendSdAppCmdCmd(0) )	{
		sendMmcCmd( 41, ocr );
		if( recvMmcCmdResp1( resp ) && (resp[0] == 0x3f) && (resp[1] & 0x80)  )	{
			if (ocr_out) *ocr_out = ((resp[1] << 24) | (resp[2] << 16) | (resp[3] << 8) | resp[4]);
			return true;
		}
	}
	//debugMmcResp( 41, resp );
	return false;
}

bool sendSdSendRelativeAddrCmd( u16 *rca )	{
	u8 resp[48/8];
	sendMmcCmd( 3, 1 );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 3) )	{
		*rca = (resp[1]<<8) | resp[2];
		return true;
	}
	debugMmcResp( 3, resp );
	return false;
}

bool sendMmcAllSendCidCmd()	{
	u8 resp[136/8];
	sendMmcCmd( 2, 0xFFFFFFFF );
	if( recvMmcCmdResp2( resp ) && (resp[0] == 0x3f) )	{
		return true;
	}
	debugMmcResp( 2, resp );
	return false;
}

bool sendMmcSelectDeselectCardCmd( u16 rca )	{
	u8 resp[48/8];
	sendMmcCmd( 7, (((rca>>8)&0xFF)<<24) | ((rca&0xFF)<<16) | 0xFFFF );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 7) )	{
		return true;
	}
	debugMmcResp( 7, resp );
	return false;
}

bool sendSdSetBusWidth( u16 rca, u8 width )	{
	u8 resp[48/8];
	if( width == 4 )	width = 2;
	else if( width == 1 )	width = 0;
	else return false;
	if( sendSdAppCmdCmd(rca) )	{
		sendMmcCmd( 6, width );
		if( recvMmcCmdResp1( resp ) && (resp[0] == 6) )
			return true;
	}
	debugMmcResp( 6, resp );
	return false;
}

bool initSd()	{
	int i;
	u16 rca;
	u32 ocr;
	set_neo2_asic_cr( 1<<7 | 1<<10 );
	for( i = 0; i < 80; i++ )	{
		wrMmcCmdBit(1);
	}
	sendMmcGoIdleStateCmd();
	for( i = 0; i < (1024 * 8); i++ )	{
		if( sendSdSdAppOpCondCmd(0x00300000, &ocr) )	{
			if (!(ocr & 0x00300000)) {
				debugMmcPrint("This SD Card is not compatible");
				break;
			}
			if( sendMmcAllSendCidCmd() )	{
				if( sendSdSendRelativeAddrCmd( &rca ) )	{
					if( sendMmcSelectDeselectCardCmd( rca ) )	{
						if( sendSdSetBusWidth( rca, 4 ) )	{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool sendMmcReadSingleBlockCmd( u8 *buf, u32 addr )	{
	int i;
	u8 resp[6];
	sendMmcCmd( 17, addr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 17) )	{

		while( rdMmcDatBit() != 0 )	{
			;
		}

		for (i=0; i<512; i++)	{
			buf[i] = rdMmcDatByte();
		}

		// Clock out CRC
		rdMmcDatByte();
		rdMmcDatByte();

		// Clock out end bit
		rdMmcDatBit();

		return true;

	}
	debugMmcResp( 17, resp );
	return false;
}

////////////////////////////////////////////////////////////////////////////////

void sdCrc16(u8 *p_crc, u8 *data, int len) {
	int i;
	u8 nybble;

	u64 poly = 0x0001000000100001LL;
	u64 crc = 0;
	u64 n_crc; // This can probably be u32

	// Load crc from array
	for (i = 0; i < 8; i++) {
		crc <<= 8;
		crc |= p_crc[i];
	}

	for (i = 0; i < (len * 2); i++) {
		if (i & 1) nybble = (data[i >> 1] & 0x0F);
		else nybble = (data[i >> 1] >> 4);

		n_crc = (crc >> (15 * 4));
		crc <<= 4;
		if ((nybble ^ n_crc) & 1) crc ^= (poly << 0);
		if ((nybble ^ n_crc) & 2) crc ^= (poly << 1);
		if ((nybble ^ n_crc) & 4) crc ^= (poly << 2);
		if ((nybble ^ n_crc) & 8) crc ^= (poly << 3);
	}

	// Output crc to array
	for (i = 7; i >= 0; i--) {
		p_crc[i] = crc;
		crc >>= 8;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool sendSdWriteMultipleBlockCmd4( u32 addr )	{
	int i;
	u8 resp[6];
	sendMmcCmd( 25, addr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 25) )	{
		return true;
	}
	//debugMmcResp( 25, resp );
	return false;
}

// this can also be used to send a single block!

bool sendSdWriteMultipleBlock4( u8 *buf )	{
	int i;
	u8 crcbuf[8];
	u8 crc_stat = 0;

	//memset(crcbuf, 0, 8); // crc in = 0
	*(u32*)&crcbuf[0]=0;
	*(u32*)&crcbuf[4]=0;
	sdCrc16(crcbuf, buf, 512); // Calculate crcs

//	for (i = 0; i < 20; i++)
	wrMmcDatByte4(0xFF);	// write 40 P bits... should be more than enough

	wrMmcDatBit4(0) ;	// write 4 start bits

#ifndef NEO_ASM

	// write out data and crc bytes
	for (i=0; i<512; i++)	{
		wrMmcDatByte4( buf[i] );
	}
	for (i=0; i<8; i++)	{
		wrMmcDatByte4( crcbuf[i] );
	}

#else

	wrSdDat4( buf, 512 );
	wrSdDat4( crcbuf, 8 );

#endif

	wrMmcDatBit4(15);	// write 4 end bits

	rdMmcDatByte4();	// clock out two bits on D0

///////////////////////////////////////////////////////////////////////////

	// Check CRC Status
	//while (rdMmcDatBit4() & 1);		// wait for start bit

	// this part is almost definetly wrong

	// in the case of CRC Status 111 - (Card did not respond at all on the
	// data line, as mentioned in the spec) - it will not be sending a low start bit

	// page 80 of the spec

///////////////////////////////////////////////////////////////////////////

	/*if( rdMmcDatBit4() & 1 )	{
		return false;
	} else {
		for(i=0; i<3; i++)	{
			crc_stat <<= 1;
			crc_stat |= (rdMmcDatBit4() & 1);	// read crc status on D0
		}
		rdMmcDatBit4();		// end bit

		// If CRC Status is not positive (010b) return error
		if(crc_stat!=2)
			return false;


		// at this point the card is definetly cooperating so wait for the start bit

		while( rdMmcDatBit4() & 1 );

		// Do not time out? Writes take an unpredictable amount of time to complete.
		for(i=(1024*128);i>0;i--)	{
				if((rdMmcDatBit4()&1) != 0 )
					break;
		}
		// check for busy timeout
		if(i==0)
			return false;
	}*/

	// although all of the above follows SD spec lets just clock out some bits and wait
	// for the busy to clear...it works well on many non working cards


	rdMmcDatByte4();	// clock out two bits on D0
	rdMmcDatByte4();	// clock out two bits on D0

	while( rdMmcDatBit4() & 1 );
	while( (rdMmcDatBit4()&1) == 0 );

	// todo: if this is a multiple block write we must learn about and send
	// ACMD22 to verify 1 block was written, if not return false instead of true

	return true;

}

bool sendMmcWriteBlockCmd4( u8 *buf, u32 addr )	{
	int i;
	u8 resp[6];
	sendMmcCmd( 24, addr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 24) )	{
		if( write_data_asm( buf ) == false )	{
			return false;
		}
		return true;
	}
	//debugMmcResp( 24, resp );
	return false;

}

////////////////////////////////////////////////////////////////////////////////

#ifndef NEO_ASM

bool sendSdReadSingleBlockCmd4( u8 *buf, u32 addr )	{
	int i;
	u8 resp[6];
	sendMmcCmd( 17, addr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 17) )	{

		while( (rdMmcDatBit4()&1) != 0 )	{
			;
		}

		for (i=0; i<512; i++)	{
			buf[i] = rdMmcDatByte4();
		}

		// Clock out CRC
		for (i=0; i<8; i++)	{
			rdMmcDatByte4();
		}

		// Clock out end bit
		rdMmcDatBit4();
		return true;

	}
	return false;
}

bool sendSdReadMultipleBlockCmd4( u32 addr )	{
	int i;
	u8 resp[6];
	sendMmcCmd( 18, addr );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 18) )	{
		return true;
	}
	return false;
}

int recvSdReadMultipleBlock4( u8 *buf )	{
	int i;

	while( (rdMmcDatBit4()&1) != 0 )	{
		;
	}

	for (i=0; i<512; i++)	{
		buf[i] = rdMmcDatByte4();
	}

	// Clock out CRC
	for (i=0; i<8; i++)	{
		rdMmcDatByte4();
	}

	// Clock out end bit
	rdMmcDatBit4();

	return true;
}

#endif

bool sendSdStopTransmissionCmd4()	{
	int i;
	u8 resp[6];
	sendMmcCmd( 12, 0 );
	if( recvMmcCmdResp1( resp ) && (resp[0] == 12) )	{
		return true;
	}
	debugMmcResp( 12, resp );
	return false;
}


// Neo MMC driver functions

bool NSD2_IsInserted(void)	{
	return true;
}

bool NSD2_ClearStatus (void) {
	return true;
}

bool NSD2_Shutdown(void) {
	return true;
}

bool NSD2_StartUp(void) {
	return initSd();
}



bool NSD2_WriteSectors (u32 sector, u8 numSecs, void* buffer)
{
	int i;
	int inumSecs=numSecs;
	u8 *p=(u8*)buffer;
	if( numSecs == 0 )	inumSecs = 256;
	for( i=0; i<inumSecs; i++ )	{
		if( sendMmcWriteBlockCmd4( p, sector * 512 ) == false )	{
			return false;
		}
		sector++;
		p+=512;
	}

	return true;

	/*int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;

	if( numSecs == 0 )	inumSecs = 256;
	if( sendSdWriteMultipleBlockCmd4( sector * 512 ) == false )	{
		return false;
	}
	for( i=0; i<inumSecs; i++ )		{
		if( sendSdWriteMultipleBlock4( p ) == false )	{
			return false;
		}
		p+=512;
	}

	if( sendSdStopTransmissionCmd4() == false )
		return false;

	return true;*/

/*
	int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;
	if( numSecs == 0 )	inumSecs = 256;
	for( i=0; i<inumSecs; i++ )		{
		if(	write_neo2_spi_mmc( p, sector * 512 ) == false )	{
			return false;
		}
		p+=512;
		sector++;
	}
	return true;
*/
}

bool NSD2_ReadSectors (u32 sector, u8 numSecs, void* buffer)
{

	int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;
	if( numSecs == 0 )	inumSecs = 256;
	for( i=0; i<inumSecs; i++ )		{
		if(	sendSdReadSingleBlockCmd4( p, sector * 512 ) == false )	{
			return false;
		}
		p+=512;
		sector++;
	}
	return true;

/*
	int inumSecs=numSecs;
	int i;
	u8 *p=(u8*)buffer;

	if( numSecs == 0 )	inumSecs = 256;
	if( sendSdReadMultipleBlockCmd4( sector * 512 ) == false )	{
		return false;
	}
	for( i=0; i<inumSecs; i++ )		{
		if( recvSdReadMultipleBlock4( p ) == false )	{
			return false;
		}
		p+=512;
	}

	if( sendSdStopTransmissionCmd4() == false )
		return false;

	return true;
*/
}
