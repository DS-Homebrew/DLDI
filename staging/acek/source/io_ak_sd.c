/*
    Copyright (C) 2007 Acekard, www.acekard.com
    SPDX-License-Identifier: MIT
*/

#include <nds.h>
#include <string.h>
#include "io_ak_sd.h"

static void dsd_4way_crc( const u8 * byte512, u8 * byte520 );

static void dsd_send_clock( u32 clock_count );

static void dsd_write_ds_cmd( const u8 * ds_cmd );


static u8 dsd_SDCRC7( const u8 * data, int cnt )
{
    int i, a;
    u8 crc, temp;

    crc = 0;
    for (a = 0; a < cnt; a++)
    {
        temp = data[a];
        for (i = 0; i < 8; i++)
        {
            crc <<= 1;
            if ((temp & 0x80) ^ (crc & 0x80)) crc ^= 0x09;
            temp <<= 1;
        }
    }
    crc = (crc << 1) | 1;
    return (crc);
}

static u16 dsd_SDCRC16( const u8 * data, u32 length )
{
    u32 crc = 0;
	u32 i = 0;
    for ( i = 0; i < length; ++i )
    {
        u32 temp = data[i];
		u32 j = 0;
        for ( j = 0; j < 8; ++j )
        {
            temp = temp << 1;
			crc = crc << 1;
			if( (temp & 0x100) ^ ((crc & 0x10000) >> 8) )
				crc ^= 0x1021;
        }
    }
    return crc & 0xffff;
}


void dsd_send_sd_cmd( u8 cmd, u32 arg )
{
	//dbg_printf( "SD CMD%02d, %08x\n", cmd, arg );
	u8 cmd_buf[6];
	cmd |= 0x40;
	u8 * parg = (u8 *)&arg;
	cmd_buf[0] = cmd;
	cmd_buf[1] = parg[3];
	cmd_buf[2] = parg[2];
	cmd_buf[3] = parg[1];
	cmd_buf[4] = parg[0];
	u8 crc7 = dsd_SDCRC7( cmd_buf, 5 );
	cmd_buf[5] = crc7;

	u32 i = 0;
	for( i = 0; i < 6; ++i )
	{
		u8 ds_cmd[8];
		u32 j = 0;
		for( j = 0; j < 8; ++j )
		{
			u8 cmd_bit = (cmd_buf[i] >> (7-j)) & 0x01;
			ds_cmd[j] = 0xAF | (cmd_bit<<4); // 0xA0 ��ʾ������ȫF��
			                                 // ����hzΪ1��ָ��hzΪ0��������normalΪ1
		}
		//wait_press_b();
		dsd_write_ds_cmd( ds_cmd );
	}
	//dbg_printf( "ready to send 128 clocks\n");
	dsd_send_clock( 128 );
}

void dsd_write_sd_sector( u32 address, const u8 * data )
{
	dsd_send_sd_cmd( 24, address );

	u8 sd_data[520];

	dsd_4way_crc( data, sd_data );

	// START bit
	u8 start_bit_cmd[] = { 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xD0 };
	dsd_write_ds_cmd( start_bit_cmd );

	// Totally 520 bytes, include 4 line's crc16 code\n");
	u32 i = 0;
	for( i = 0; i < 520; i += 4 )
	{
		u8 ds_cmd[8];
		u8 j = 0;
		for( j = 0; j < 4; ++j)
		{
			ds_cmd[j*2] = 0xD0 | (sd_data[i+j] >> 4);
			ds_cmd[j*2+1] = 0xD0 | (sd_data[i+j] & 0x0f);
		}
		dsd_write_ds_cmd( ds_cmd );
	}

	// END bit
	u8 cmd[] = { 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	dsd_write_ds_cmd( cmd );
}

static void dsd_write_ds_cmd( const u8 * ds_cmd )
{
	//dbg_printf("write_ds_cmd %02x%02x%02x%02x%02x%02x%02x%02x\n", 
	//	ds_cmd[0], ds_cmd[1], ds_cmd[2], ds_cmd[3], 
	//	ds_cmd[4], ds_cmd[5], ds_cmd[6], ds_cmd[7] );
	u8 cmd[8];
	u32 i = 0;
	for( i = 0; i < 8; ++i )
	{
		cmd[i] = ds_cmd[7-i];
	}
	cardWriteCommand( cmd );
	CARD_CR2 = CARD_ACTIVATE | CARD_nRESET | 0x00000000 | 0x00406000/* | CARD_27*/;
	while( CARD_CR2 & CARD_BUSY ) {}	
}


static void dsd_4way_crc( const u8 * byte512, u8 * byte520 )
{
	u8 d0[130] = {};
	u8 d1[130] = {};
	u8 d2[130] = {};
	u8 d3[130] = {};

	u32 i = 0;
	for( i = 0; i < 128; ++i )
	{
		u32 j = 0;
		for( j = 0; j < 4; ++j )
		{
			u8 d = byte512[i*4+j];
			d0[i] |= ((d & 0x01) >> 0) << ((3-j)*2);
			d0[i] |= ((d & 0x10) >> 4) << ((3-j)*2+1);

			d1[i] |= ((d & 0x02) >> 1) << ((3-j)*2);
			d1[i] |= ((d & 0x20) >> 5) << ((3-j)*2+1);

			d2[i] |= ((d & 0x04) >> 2) << ((3-j)*2);
			d2[i] |= ((d & 0x40) >> 6) << ((3-j)*2+1);

			d3[i] |= ((d & 0x08) >> 3) << ((3-j)*2);
			d3[i] |= ((d & 0x80) >> 7) << ((3-j)*2+1);
		}
	}
	
	u16 crc = 0;
	u8 * pcrc = NULL;
/////// data line 0
	crc = dsd_SDCRC16( d0, 128 );
	pcrc = (u8 *)&crc;
	d0[128] = pcrc[1];
	d0[129] = pcrc[0];
	//dbg_printf("D0 crc16 is %04x, [128] %02x [129] %02x\n", crc, d0[128], d0[129] );
/////// data line 1
	crc = dsd_SDCRC16( d1, 128 );
	//dbg_printf("D1 crc16 is %04x\n", crc );
	pcrc = (u8 *)&crc;
	d1[128] = pcrc[1];
	d1[129] = pcrc[0];
////// data line 2
	crc = dsd_SDCRC16( d2, 128 );
	//dbg_printf("D2 crc16 is %04x\n", crc );
	pcrc = (u8 *)&crc;
	d2[128] = pcrc[1];
	d2[129] = pcrc[0];
////// data line 3
	crc = dsd_SDCRC16( d3, 128 );
	//dbg_printf("D3 crc16 is %04x\n", crc );
	pcrc = (u8 *)&crc;
	d3[128] = pcrc[1];
	d3[129] = pcrc[0];
	
	memset( byte520, 0, 520 );

	for( i = 0; i < 130; ++i )
	{
		u32 j = 0;
		for( j = 0; j < 4; ++j )
		{
			u32 index = i*4+j;
			u8 * d = &(byte520[index]);
			*d |= ((d0[i] & (0x01 << (3-j)*2)) >> (3-j)*2) << 0;
			*d |= ((d1[i] & (0x01 << (3-j)*2)) >> (3-j)*2) << 1;
			*d |= ((d2[i] & (0x01 << (3-j)*2)) >> (3-j)*2) << 2;
			*d |= ((d3[i] & (0x01 << (3-j)*2)) >> (3-j)*2) << 3;

			*d |= ((d0[i] & (0x01 << ((3-j)*2+1))) >> ((3-j)*2+1)) << 4;
			*d |= ((d1[i] & (0x01 << ((3-j)*2+1))) >> ((3-j)*2+1)) << 5;
			*d |= ((d2[i] & (0x01 << ((3-j)*2+1))) >> ((3-j)*2+1)) << 6;
			*d |= ((d3[i] & (0x01 << ((3-j)*2+1))) >> ((3-j)*2+1)) << 7;
		}
	}
}

// clock count will be align with times of 8
static void dsd_send_clock( u32 clock_count )
{
	u32 i = 0;
	for( i = 0; i < clock_count / 8; ++i )
	{
		u8 cmd[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		cardWriteCommand( cmd );
		CARD_CR2 = CARD_ACTIVATE | CARD_nRESET | 0x00000000 | 0x00406000/* | CARD_27*/;
		while( CARD_CR2 & CARD_BUSY ) {}
	}
}

bool dsd_is_sd_busy()
{
	u8 cmd[] = { 0,0,0,0,0,0,0,0xd5 };
	cardWriteCommand( cmd );
	CARD_CR2 = CARD_ACTIVATE | CARD_nRESET | 0x07000000 | 0x00406000/* | CARD_27*/;
	u32 data = 0;
	while( CARD_CR2 & CARD_BUSY )
	{
		while (!(CARD_CR2 & CARD_DATA_READY)) {}
		data = CARD_DATA_RD;
	}
	if( data & 0x80808080 )
		return false;
	else
		return true;
}
