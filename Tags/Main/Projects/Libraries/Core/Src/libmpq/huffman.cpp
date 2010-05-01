/*
*  huffman.c -- functions do decompress files in MPQ files which
*               uses a modified huffman version.
*
*  Copyright (C) 2003 Maik Broemme <mbroemme@plusserver.de>
*
*  Differences between C++ and C version:
*
*    - Removed the object oriented stuff.
*    - Replaced the goto things with some better C code.
*
*  This source was adepted from the C++ version of huffman.cpp included
*  in stormlib. The C++ version belongs to the following authors,
*
*  Ladislav Zezula <ladik.zezula.net>
*  ShadowFlare <BlakFlare@hotmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "Core.h"
#include <stdlib.h>

#include "mpq.h"
#include "huffman.h"
#include <string.h>

unsigned char table1502A630[] = {

	/* Data for compression type 0x00 */
	0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
	0x00, 0x00,

	/* Data for compression type 0x01 */
	0x54, 0x16, 0x16, 0x0D, 0x0C, 0x08, 0x06, 0x05, 0x06, 0x05, 0x06, 0x03, 0x04, 0x04, 0x03, 0x05,
	0x0E, 0x0B, 0x14, 0x13, 0x13, 0x09, 0x0B, 0x06, 0x05, 0x04, 0x03, 0x02, 0x03, 0x02, 0x02, 0x02,
	0x0D, 0x07, 0x09, 0x06, 0x06, 0x04, 0x03, 0x02, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02,
	0x09, 0x06, 0x04, 0x04, 0x04, 0x04, 0x03, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x04,
	0x08, 0x03, 0x04, 0x07, 0x09, 0x05, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02,
	0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02,
	0x06, 0x0A, 0x08, 0x08, 0x06, 0x07, 0x04, 0x03, 0x04, 0x04, 0x02, 0x02, 0x04, 0x02, 0x03, 0x03,
	0x04, 0x03, 0x07, 0x07, 0x09, 0x06, 0x04, 0x03, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x0A, 0x02, 0x02, 0x03, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x06, 0x03, 0x05, 0x02, 0x03,
	0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x04, 0x04, 0x04, 0x07, 0x09, 0x08, 0x0C, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x03,
	0x04, 0x01, 0x02, 0x04, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01,
	0x04, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x02, 0x06, 0x4B,
	0x00, 0x00,

	/* Data for compression type 0x02 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x27, 0x00, 0x00, 0x23, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x06, 0x0E, 0x10, 0x04,
	0x06, 0x08, 0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03, 0x01, 0x01, 0x02, 0x01, 0x01,
	0x01, 0x04, 0x02, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x04, 0x01, 0x01, 0x02, 0x03, 0x03, 0x02,
	0x03, 0x01, 0x03, 0x06, 0x04, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x01,
	0x01, 0x29, 0x07, 0x16, 0x12, 0x40, 0x0A, 0x0A, 0x11, 0x25, 0x01, 0x03, 0x17, 0x10, 0x26, 0x2A,
	0x10, 0x01, 0x23, 0x23, 0x2F, 0x10, 0x06, 0x07, 0x02, 0x09, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,

	/* Data for compression type 0x03 */
	0xFF, 0x0B, 0x07, 0x05, 0x0B, 0x02, 0x02, 0x02, 0x06, 0x02, 0x02, 0x01, 0x04, 0x02, 0x01, 0x03,
	0x09, 0x01, 0x01, 0x01, 0x03, 0x04, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01,
	0x05, 0x01, 0x01, 0x01, 0x0D, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
	0x0A, 0x04, 0x02, 0x01, 0x06, 0x03, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01,
	0x05, 0x02, 0x03, 0x04, 0x03, 0x03, 0x03, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x03, 0x03,
	0x01, 0x03, 0x01, 0x01, 0x02, 0x05, 0x01, 0x01, 0x04, 0x03, 0x05, 0x01, 0x03, 0x01, 0x03, 0x03,
	0x02, 0x01, 0x04, 0x03, 0x0A, 0x06, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x02, 0x01, 0x0A, 0x02, 0x05, 0x01, 0x01, 0x02, 0x07, 0x02, 0x17, 0x01, 0x05, 0x01, 0x01,
	0x0E, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x06, 0x02, 0x01, 0x04, 0x05, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x11,
	0x00, 0x00,

	/* Data for compression type 0x04 */
	0xFF, 0xFB, 0x98, 0x9A, 0x84, 0x85, 0x63, 0x64, 0x3E, 0x3E, 0x22, 0x22, 0x13, 0x13, 0x18, 0x17,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,

	/* Data for compression type 0x05 */
	0xFF, 0xF1, 0x9D, 0x9E, 0x9A, 0x9B, 0x9A, 0x97, 0x93, 0x93, 0x8C, 0x8E, 0x86, 0x88, 0x80, 0x82,
	0x7C, 0x7C, 0x72, 0x73, 0x69, 0x6B, 0x5F, 0x60, 0x55, 0x56, 0x4A, 0x4B, 0x40, 0x41, 0x37, 0x37,
	0x2F, 0x2F, 0x27, 0x27, 0x21, 0x21, 0x1B, 0x1C, 0x17, 0x17, 0x13, 0x13, 0x10, 0x10, 0x0D, 0x0D,
	0x0B, 0x0B, 0x09, 0x09, 0x08, 0x08, 0x07, 0x07, 0x06, 0x05, 0x05, 0x04, 0x04, 0x04, 0x19, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,

	/* Data for compression type 0x06 */
	0xC3, 0xCB, 0xF5, 0x41, 0xFF, 0x7B, 0xF7, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xBF, 0xCC, 0xF2, 0x40, 0xFD, 0x7C, 0xF7, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7A, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,

	/* Data for compression type 0x07 */
	0xC3, 0xD9, 0xEF, 0x3D, 0xF9, 0x7C, 0xE9, 0x1E, 0xFD, 0xAB, 0xF1, 0x2C, 0xFC, 0x5B, 0xFE, 0x17,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xBD, 0xD9, 0xEC, 0x3D, 0xF5, 0x7D, 0xE8, 0x1D, 0xFB, 0xAE, 0xF0, 0x2C, 0xFB, 0x5C, 0xFF, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x70, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,

	/* Data for compression type 0x08 */
	0xBA, 0xC5, 0xDA, 0x33, 0xE3, 0x6D, 0xD8, 0x18, 0xE5, 0x94, 0xDA, 0x23, 0xDF, 0x4A, 0xD1, 0x10,
	0xEE, 0xAF, 0xE4, 0x2C, 0xEA, 0x5A, 0xDE, 0x15, 0xF4, 0x87, 0xE9, 0x21, 0xF6, 0x43, 0xFC, 0x12,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xB0, 0xC7, 0xD8, 0x33, 0xE3, 0x6B, 0xD6, 0x18, 0xE7, 0x95, 0xD8, 0x23, 0xDB, 0x49, 0xD0, 0x11,
	0xE9, 0xB2, 0xE2, 0x2B, 0xE8, 0x5C, 0xDD, 0x15, 0xF1, 0x87, 0xE7, 0x20, 0xF7, 0x44, 0xFF, 0x13,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x5F, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

/* Gets previous Huffman tree item (?) */
struct huffman_tree_item *libmpq_huff_get_prev_item(struct huffman_tree_item *hi, __int32 value) {
	if (PTR_INT(hi->prev) < 0) {
		return PTR_NOT(hi->prev);
	}
	if (value < 0) {
		value = __int32(hi - hi->next->prev);
	}
	return hi->prev + value;
}

/* 1500BC90 */
static void libmpq_huff_remove_item(struct huffman_tree_item *hi) {
	struct huffman_tree_item *temp;			/* EDX */

	if (hi->next != NULL) {
		temp = hi->prev;
		if (PTR_INT(temp) <= 0) {
			temp = PTR_NOT(temp);
		} else {
			temp += (hi - hi->next->prev);
		}
		temp->next          = hi->next;
		hi->next->prev      = hi->prev;
		hi->next = hi->prev = NULL;
	}
}

static void libmpq_huff_insert_item(struct huffman_tree_item **p_item, struct huffman_tree_item *item, unsigned long where, struct huffman_tree_item *item2) {
	struct huffman_tree_item *next = item->next;	/* EDI - next to the first item */
	struct huffman_tree_item *prev = item->prev;	/* ESI - prev to the first item */
	struct huffman_tree_item *prev2;		/* Pointer to previous item */
	long next2;					/* Pointer to the next item */

	/* The same code like in mpq_huff_remove_item(); */
	if (next != 0) {				/* If the first item already has next one */
		if (PTR_INT(prev) < 0) {
			prev = PTR_NOT(prev);
		} else {
			prev += (item - next->prev);
		}

		/*
		* 150083C1
		* Remove the item from the tree
		*/
		prev->next = next;
		next->prev = prev;

		/* Invalidate 'prev' and 'next' pointer */
		item->next = 0;
		item->prev = 0;
	}

	if (item2 == NULL) {				/* EDX - If the second item is not entered, */
		item2 = PTR_PTR(&p_item[1]);		/* take the first tree item */
	}

	switch (where) {
		case SWITCH_ITEMS:			/* Switch the two items */
			item->next  = item2->next;	/* item2->next (Pointer to pointer to first) */
			item->prev  = item2->next->prev;
			item2->next->prev = item;
			item2->next = item;		/* Set the first item */
			return;
		case INSERT_ITEM:			/* Insert as the last item */
			item->next = item2;		/* Set next item (or pointer to pointer to first item) */
			item->prev = item2->prev;	/* Set prev item (or last item in the tree) */
			next2 = PTR_INT(p_item[0]);	/* Usually NULL */
			prev2 = item2->prev;		/* Prev item to the second (or last tree item) */
			if (PTR_INT(prev2) < 0) {
				prev2 = PTR_NOT(prev);
				prev2->next = item;
				item2->prev = item;	/* Next after last item */
				return;
			}
			if (next2 < 0) {
				next2 = long(item2 - item2->next->prev);
			}
			prev2 += next2;
			prev2->next = item;
			item2->prev = item;		/* Set the next/last item */
			return;
		default:
			return;
	}
}

/* Builds Huffman tree. Called with the first 8 bits loaded from input stream. */
static void libmpq_huff_build_tree(struct huffman_tree *ht, unsigned int cmp_type) {
	unsigned long max_byte;				/* [ESP+10] - The greatest character found in table */
	unsigned char *byte_array;			/* [ESP+1C] - Pointer to unsigned char in table1502A630 */
	unsigned long i;				/* egcs in linux doesn't like multiple for loops without an explicit i */
	unsigned int found;				/* Thats needed to replace the goto stuff from original source :) */
	struct huffman_tree_item **p_item;		/* [ESP+14] - Pointer to Huffman tree item pointer array */
	struct huffman_tree_item *child1;

	/* Loop while pointer has a negative value. */
	while (PTR_INT(ht->last) > 0) {			/* ESI - Last entry */
		struct huffman_tree_item *temp;		/* EAX */

		if (ht->last->next != NULL) {		/* ESI->next */
			libmpq_huff_remove_item(ht->last);
		}
		ht->item3058   = PTR_PTR(&ht->item3054);/* [EDI+4] */
		ht->last->prev = ht->item3058;		/* EAX */
		temp           = libmpq_huff_get_prev_item(PTR_PTR(&ht->item3054), PTR_INT(&ht->item3050));
		temp->next     = ht->last;
		ht->item3054   = ht->last;
	}

	/* Clear all pointers in huffman tree item array. */
	memset(ht->items306C, 0, sizeof(ht->items306C));

	max_byte = 0;					/* Greatest character found init to zero. */
	p_item = (struct huffman_tree_item **)&ht->items306C;	/* Pointer to current entry in huffman tree item pointer array */

	/* Ensure we have low 8 bits only */
	cmp_type   &= 0xFF;
	byte_array  = table1502A630 + cmp_type * 258;	/* EDI also */

	for (i = 0; i < 0x100; i++, p_item++) {
		struct huffman_tree_item *item = ht->item3058;	/* Item to be created */
		struct huffman_tree_item *p_item3 = ht->item3058;
		unsigned char one_byte = byte_array[i];

		/* Skip all the bytes which are zero. */
		if (byte_array[i] == 0) {
			continue;
		}

		/* If not valid pointer, take the first available item in the array. */
		if (PTR_INT(item) <= 0) {
			item = &ht->items0008[ht->items++];
		}

		/* Insert this item as the top of the tree. */
		libmpq_huff_insert_item(&ht->item305C, item, SWITCH_ITEMS, NULL);

		item->parent    = NULL;			/* Invalidate child and parent */
		item->child     = NULL;
		*p_item         = item;			/* Store pointer into pointer array */

		item->dcmp_byte  = i;			/* Store counter */
		item->byte_value = one_byte;		/* Store byte value */
		if (one_byte >= max_byte) {
			max_byte = one_byte;
			continue;
		}

		/* Find the first item which has byte value greater than current one byte */
		found = 0;
		if (PTR_INT((p_item3 = ht->last)) > 0) {/* EDI - Pointer to the last item */

			/* 15006AF7 */
			if (p_item3 != NULL) {
				do {			/* 15006AFB */
					if (p_item3->byte_value >= one_byte) {
						found = 1;
						break;
					}
					p_item3 = p_item3->prev;
				} while (PTR_INT(p_item3) > 0);
			}
		}

		if (found == 0) {
			p_item3 = NULL;
		}

		/* 15006B09 */
		if (item->next != NULL) {
			libmpq_huff_remove_item(item);
		}

		/* 15006B15 */
		if (p_item3 == NULL) {
			p_item3 = PTR_PTR(&ht->first);
		}

		/* 15006B1F */
		item->next = p_item3->next;
		item->prev = p_item3->next->prev;
		p_item3->next->prev = item;
		p_item3->next = item;
	}

	/* 15006B4A */
	for (; i < 0x102; i++) {
		struct huffman_tree_item **p_item2 = &ht->items306C[i];	/* EDI */

		/* 15006B59  */
		struct huffman_tree_item *item2 = ht->item3058;	/* ESI */
		if (PTR_INT(item2) <= 0) {
			item2 = &ht->items0008[ht->items++];
		}
		libmpq_huff_insert_item(&ht->item305C, item2, INSERT_ITEM, NULL);

		/* 15006B89 */
		item2->dcmp_byte  = i;
		item2->byte_value = 1;
		item2->parent     = NULL;
		item2->child      = NULL;
		*p_item2++        = item2;
	}

	/* 15006BAA */
	if (PTR_INT((child1 = ht->last)) > 0) {		/* EDI - last item (first child to item */
		struct huffman_tree_item *child2;	/* EBP */
		struct huffman_tree_item *item;		/* ESI */

		/* 15006BB8 */
		while (PTR_INT((child2 = child1->prev)) > 0) {
			if (PTR_INT((item = ht->item3058)) <= 0) {
				item = &ht->items0008[ht->items++];
			}
			/* 15006BE3 */
			libmpq_huff_insert_item(&ht->item305C, item, SWITCH_ITEMS, NULL);

			/* 15006BF3 */
			item->parent = NULL;
			item->child  = NULL;

			/*
			* EDX = child2->byte_value + child1->byte_value;
			* EAX = child1->byte_value;
			* ECX = max_byte;		The greatest character (0xFF usually)
			*/
			item->byte_value = child1->byte_value + child2->byte_value;	/* 0x02 */
			item->child      = child1;	/* Prev item in the */
			child1->parent   = item;
			child2->parent   = item;

			/* EAX = item->byte_value; */
			if (item->byte_value >= max_byte) {
				max_byte = item->byte_value;
			} else {
				struct huffman_tree_item *p_item2 = child2->prev;	/* EDI */
				found = 0;
				if (PTR_INT(p_item2) > 0) {

					/* 15006C2D */
					do {
						if (p_item2->byte_value >= item->byte_value) {
							found = 1;
							break;
						}
						p_item2 = p_item2->prev;
					} while (PTR_INT(p_item2) > 0);
				}
				if (found == 0) {
					p_item2 = NULL;
				}
				if (item->next != 0) {
					struct huffman_tree_item *temp4 = libmpq_huff_get_prev_item(item, -1);
					temp4->next      = item->next;	/* The first item changed */
					item->next->prev = item->prev;	/* First->prev changed to negative value */
					item->next = NULL;
					item->prev = NULL;
				}

				/* 15006C62 */
				if (p_item2 == NULL) {
					p_item2 = PTR_PTR(&ht->first);
				}
				item->next = p_item2->next;		/* Set item with 0x100 byte value */
				item->prev = p_item2->next->prev;	/* Set item with 0x17 byte value */
				p_item2->next->prev = item;		/* Changed prev of item with */
				p_item2->next = item;
			}

			/* 15006C7B */
			if (PTR_INT((child1 = child2->prev)) <= 0) {
				break;
			}
		}
	}

	/* 15006C88 */
	ht->offs0004 = 1;
}

/* Gets the whole byte from the input stream. */
static unsigned long libmpq_huff_get_8bits(struct huffman_input_stream *is) {
	unsigned long one_byte;

	if (is->bits <= 8) {
		is->bit_buf |= *(unsigned short *)is->in_buf << is->bits;
		is->in_buf  += sizeof(unsigned short);
		is->bits    += 16;
	}

	one_byte      = (is->bit_buf & 0xFF);
	is->bit_buf >>= 8;
	is->bits     -= 8;

	return one_byte;
}

/* Gets 7 bits from the stream. */
static unsigned long libmpq_huff_get_7bits(struct huffman_input_stream *is) {
	if (is->bits <= 7) {
		is->bit_buf |= *(unsigned short *)is->in_buf << is->bits;
		is->in_buf  += sizeof(unsigned short);
		is->bits    += 16;
	}

	/* Get 7 bits from input stream. */
	return (is->bit_buf & 0x7F);
}

/* Gets one bit from input stream. */
unsigned long libmpq_huff_get_bit(struct huffman_input_stream *is) {
	unsigned long bit = (is->bit_buf & 1);

	is->bit_buf >>= 1;
	if (--is->bits == 0) {
		is->bit_buf  = *(unsigned long *)is->in_buf;
		is->in_buf  += sizeof(unsigned long);
		is->bits     = 32;
	}
	return bit;
}

static struct huffman_tree_item *libmpq_huff_call1500E740(struct huffman_tree *ht, unsigned int value) {
	struct huffman_tree_item *p_item1 = ht->item3058;	/* EDX */
	struct huffman_tree_item *p_item2;			/* EAX */
	struct huffman_tree_item *p_next;
	struct huffman_tree_item *p_prev;
	struct huffman_tree_item **pp_item;

	if (PTR_INT(p_item1) <= 0 || (p_item2 = p_item1) == NULL) {
		if((p_item2 = &ht->items0008[ht->items++]) != NULL) {
			p_item1 = p_item2;
		} else {
			p_item1 = ht->first;
		}
	} else {
		p_item1 = p_item2;
	}

	p_next = p_item1->next;
	if (p_next != NULL) {
		p_prev = p_item1->prev;
		if (PTR_INT(p_prev) <= 0) {
			p_prev = PTR_NOT(p_prev);
		} else {
			p_prev += (p_item1 - p_item1->next->prev);
		}

		p_prev->next = p_next;
		p_next->prev = p_prev;
		p_item1->next = NULL;
		p_item1->prev = NULL;
	}
	pp_item = &ht->first;				/* ESI */
	if (value > 1) {

		/* ECX = ht->first->next; */
		p_item1->next = *pp_item;
		p_item1->prev = (*pp_item)->prev;

		(*pp_item)->prev = p_item2;
		*pp_item = p_item1;

		p_item2->parent = NULL;
		p_item2->child  = NULL;
	} else {
		p_item1->next = (struct huffman_tree_item *)pp_item;
		p_item1->prev = pp_item[1];
		/* EDI = ht->item305C; */
		p_prev = pp_item[1];			/* ECX */
		if (p_prev <= 0) {
			p_prev = PTR_NOT(p_prev);
			p_prev->next = p_item1;
			p_prev->prev = p_item2;

			p_item2->parent = NULL;
			p_item2->child  = NULL;
		} else {
			if (PTR_INT(ht->item305C) < 0) {
				p_prev += (struct huffman_tree_item *)pp_item - (*pp_item)->prev;
			} else {
				p_prev += PTR_INT(ht->item305C);
			}

			p_prev->next    = p_item1;
			pp_item[1]      = p_item2;
			p_item2->parent = NULL;
			p_item2->child  = NULL;
		}
	}
	return p_item2;
}

static void libmpq_huff_call1500E820(struct huffman_tree *ht, struct huffman_tree_item *p_item) {
	struct huffman_tree_item *p_item1;		/* EDI */
	struct huffman_tree_item *p_item2 = NULL;	/* EAX */
	struct huffman_tree_item *p_item3;		/* EDX */
	struct huffman_tree_item *p_prev;		/* EBX */

	for (; p_item != NULL; p_item = p_item->parent) {
		p_item->byte_value++;

		for (p_item1 = p_item; ; p_item1 = p_prev) {
			p_prev = p_item1->prev;
			if (PTR_INT(p_prev) <= 0) {
				p_prev = NULL;
				break;
			}
			if (p_prev->byte_value >= p_item->byte_value) {
				break;
			}
		}

		if (p_item1 == p_item) {
			continue;
		}

		if (p_item1->next != NULL) {
			p_item2 = libmpq_huff_get_prev_item(p_item1, -1);
			p_item2->next = p_item1->next;
			p_item1->next->prev = p_item1->prev;
			p_item1->next = NULL;
			p_item1->prev = NULL;
		}
		p_item2 = p_item->next;
		p_item1->next = p_item2;
		p_item1->prev = p_item2->prev;
		p_item2->prev = p_item1;
		p_item->next = p_item1;
		if ((p_item2 = p_item1) != NULL) {
			p_item2 = libmpq_huff_get_prev_item(p_item, -1);
			p_item2->next = p_item->next;
			p_item->next->prev = p_item->prev;
			p_item->next = NULL;
			p_item->prev = NULL;
		}

		if (p_prev == NULL) {
			p_prev = PTR_PTR(&ht->first);
		}
		p_item2       = p_prev->next;
		p_item->next  = p_item2;
		p_item->prev  = p_item2->prev;
		p_item2->prev = p_item;
		p_prev->next  = p_item;

		p_item3 = p_item1->parent->child;
		p_item2 = p_item->parent;
		if (p_item2->child == p_item) {
			p_item2->child = p_item1;
		}

		if (p_item3 == p_item1) {
			p_item1->parent->child = p_item;
		}

		p_item2 = p_item->parent;
		p_item->parent  = p_item1->parent;
		p_item1->parent = p_item2;
		ht->offs0004++;
	}
}

__int32 libmpq_huff_do_decompress(struct huffman_tree *ht, struct huffman_input_stream *is, unsigned char *out_buf, unsigned int out_length) {
	unsigned int n8bits;				/* 8 bits loaded from input stream */
	unsigned int n7bits;				/* 7 bits loaded from input stream */
	unsigned int found;				/* Thats needed to replace the goto stuff from original source :) */
	unsigned int dcmp_byte = 0;
	unsigned long bit_count;
	struct huffman_decompress *qd;
	unsigned int has_qd;				/* Can we use quick decompression? */
	struct huffman_tree_item *p_item1 = NULL;
	struct huffman_tree_item *p_item2 = NULL;
	unsigned char *out_pos = out_buf;

	/* Test the output length. Must not be non zero. */
	if (out_length == 0) {
		return 0;
	}

	/* Get the compression type from the input stream. */
	n8bits = libmpq_huff_get_8bits(is);

	/* Build the Huffman tree */
	libmpq_huff_build_tree(ht, n8bits);
	ht->cmp0 = (n8bits == 0) ? TRUE : FALSE;

	for(;;) {
		n7bits = libmpq_huff_get_7bits(is);	/* Get 7 bits from input stream */

		/*
		* Try to use quick decompression. Check huffman_decompress array for corresponding item.
		* If found, use the result byte instead.
		*/
		qd = &ht->qd3474[n7bits];

		/* If there is a quick-pass possible (ebx) */
		has_qd = (qd->offs00 >= ht->offs0004) ? TRUE : FALSE;

		/* If we can use quick decompress, use it. */
		if (has_qd) {
			found = 0;
			if (qd->bits > 7) {
				is->bit_buf >>= 7;
				is->bits -= 7;
				p_item1 = qd->p_item;
				found = 1;
			}
			if (found == 0) {
				is->bit_buf >>= qd->bits;
				is->bits     -= qd->bits;
				dcmp_byte     = qd->dcmp_byte;
			}
		} else {
			found = 1;
			p_item1 = ht->first->next->prev;
			if (PTR_INT(p_item1) <= 0) {
				p_item1 = NULL;
			}
		}

		if (found == 1) {
			bit_count = 0;
			p_item2 = NULL;
			do {
				p_item1 = p_item1->child;	/* Move down by one level */
				if (libmpq_huff_get_bit(is)) {	/* If current bit is set, move to previous */
					p_item1 = p_item1->prev;
				}
				if (++bit_count == 7) {		/* If we are at 7th bit, save current huffman tree item. */
					p_item2 = p_item1;
				}
			} while (p_item1->child != NULL);	/* Walk until tree has no deeper level */

			if (has_qd == FALSE) {
				if (bit_count > 7) {
					qd->offs00 = ht->offs0004;
					qd->bits   = bit_count;
					qd->p_item = p_item2;
				} else {
					unsigned long index = n7bits & (0xFFFFFFFF >> (32 - bit_count));
					unsigned long add   = (1 << bit_count);

					for (qd = &ht->qd3474[index]; index <= 0x7F; index += add, qd += add) {
						qd->offs00    = ht->offs0004;
						qd->bits      = bit_count;
						qd->dcmp_byte = p_item1->dcmp_byte;
					}
				}
			}
			dcmp_byte = p_item1->dcmp_byte;
		}

		if (dcmp_byte == 0x101)	{		/* Huffman tree needs to be modified */
			n8bits  = libmpq_huff_get_8bits(is);
			p_item1 = (ht->last <= 0) ? NULL : ht->last;

			p_item2 = libmpq_huff_call1500E740(ht, 1);
			p_item2->parent     = p_item1;
			p_item2->dcmp_byte  = p_item1->dcmp_byte;
			p_item2->byte_value = p_item1->byte_value;
			ht->items306C[p_item2->dcmp_byte] = p_item2;

			p_item2 = libmpq_huff_call1500E740(ht, 1);
			p_item2->parent     = p_item1;
			p_item2->dcmp_byte  = n8bits;
			p_item2->byte_value = 0;
			ht->items306C[p_item2->dcmp_byte] = p_item2;

			p_item1->child = p_item2;
			libmpq_huff_call1500E820(ht, p_item2);
			if (ht->cmp0 == 0) {
				libmpq_huff_call1500E820(ht, ht->items306C[n8bits]);
			}
			dcmp_byte = n8bits;
		}

		if (dcmp_byte == 0x100) {
			break;
		}

		*out_pos++ = (unsigned char)dcmp_byte;
		if (--out_length == 0) {
			break;
		}
		if (ht->cmp0) {
			libmpq_huff_call1500E820(ht, ht->items306C[dcmp_byte]);
		}
	}
	return __int32(out_pos - out_buf);
}

int libmpq_huff_init_tree(struct huffman_tree *ht, struct huffman_tree_item *hi, unsigned int cmp) {
	int count;

	/* Clear links for all the items in the tree */
	for (hi = ht->items0008, count = 0x203; count != 0; hi++, count--) {
		hi->next = hi->prev = NULL;
	}

	ht->item3050 = NULL;
	ht->item3054 = PTR_PTR(&ht->item3054);
	ht->item3058 = PTR_NOT(ht->item3054);

	ht->item305C = NULL;
	ht->first    = PTR_PTR(&ht->first);
	ht->last     = PTR_NOT(ht->first);

	ht->offs0004 = 1;
	ht->items    = 0;

	/* Clear all huffman_decompress items. Do this only if preparing for decompression */
	if (cmp == LIBMPQ_HUFF_DECOMPRESS) {
		for (count = 0; count < sizeof(ht->qd3474) / sizeof(struct huffman_decompress); count++) {
			ht->qd3474[count].offs00 = 0;
		}
	}
	return 0;
}
