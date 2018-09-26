#ifndef _TW8835_H_
#define	_TW8835_H_

//==<<PAGE & TWBASE>>=================================================
#define PAGE0_GENERAL		0x00
#define PAGE0_OUTPUT		0x00	//0x007::0x00F & 0x01F
#define PAGE0_INPUT			0x00	//0x040::0x046
#define PAGE0_BT656			0x00	//0x047::0x04A
#define PAGE0_DTV			0x00	//0x050::0x05F
#define PAGE0_GPIO			0x00	//0x080::0x09E
#define PAGE0_MBIST			0x00	//0x0A0::0x0A4
#define PAGE0_TOUCH			0x00	//0x0B0::0x0B4
#define PAGE0_LOPOR			0x00	//0x0D4::0x0DF
#define PAGE0_LEDC			0x00	//0x0E0::0x0E6
#define PAGE0_DCDC			0x00	//0x0E8::0x0EC
#define PAGE0_VCOM			0x00	//0x0ED::0x0EE
#define PAGE0_SSPLL			0x00	//0x0F6::0x0FD
#define PAGE1_DECODER		0x01	//0x101::0x142
#define PAGE1_VADC			0x01	//0x1C0::0x1E7	include LLPLL
#define PAGE1_LLPLL			0x01	//0x1C0::0x1E7	include LLPLL
#define PAGE2_SCALER		0x02	//0x201::0x21E
#define PAGE2_TCON			0x02	//0x240::0x24E
#define PAGE2_IMAGE			0x02	//0x280::0x2BF
#define PAGE2_GAMMA		0x02	//0x2E0::0x2E3 include LCDC
#define PAGE2_DITHER		0x02	//0x2E4::0x2E4
#define PAGE2_RGBLEVEL		0x02	//0x2F0::0x2F5
#define PAGE2_8BITPANEL		0x02	//0x2F8::0x2F9
#define PAGE3_FOSD			0x03	//0x300::0x354
#define PAGE4_SOSD			0x04	//0x400::0x3BE
#define PAGE4_SPI			0x04	//0x4C0::0x4DA include MCU
#define PAGE4_CLOCK			0x04	//0x4E0::0x4EB
#define PAGE5_MEAS			0x05	//0x500::0x545


//===========================
// PAGE 0
//===========================
	#define REG000	0x00
	#define REG001	0x01
	#define REG002	0x02
	#define REG003	0x03
	#define REG004	0x04
	#define REG005	0x05
	#define REG006	0x06
	#define REG007	0x07
	#define REG008	0x08
	#define REG009	0x09
	#define REG00A	0x0A
	#define REG00B	0x0B
	#define REG00C	0x0C
	#define REG00D	0x0D
	#define REG00E	0x0E
	#define REG00F	0x0F

	#define REG010	0x10
	#define REG011	0x11
	#define REG012	0x12
	#define REG013	0x13
	#define REG014	0x14
	#define REG015	0x15
	#define REG016	0x16
	#define REG017	0x17
	#define REG018	0x18
	#define REG019	0x19
	#define REG01A	0x1A
	#define REG01B	0x1B
	#define REG01C	0x1C
	#define REG01D	0x1D
	#define REG01E	0x1E
	#define REG01F	0x1F

	#define REG020	0x20
	#define REG021	0x21
	#define REG022	0x22
	#define REG023	0x23
	#define REG024	0x24
	#define REG025	0x25
	#define REG026	0x26
	#define REG027	0x27
	#define REG028	0x28
	#define REG029	0x29
	#define REG02A	0x2A
	#define REG02B	0x2B
	#define REG02C	0x2C
	#define REG02D	0x2D
	#define REG02E	0x2E
	#define REG02F	0x2F

	#define REG030	0x30
	#define REG031	0x31
	#define REG032	0x32
	#define REG033	0x33
	#define REG034	0x34
	#define REG035	0x35
	#define REG036	0x36
	#define REG037	0x37
	#define REG038	0x38
	#define REG039	0x39
	#define REG03A	0x3A
	#define REG03B	0x3B
	#define REG03C	0x3C
	#define REG03D	0x3D
	#define REG03E	0x3E
	#define REG03F	0x3F


	//#define REG0FF	0x0FF

	#define REG106	0x06

//===========================
// PAGE 2
//===========================
	#define REG200	0x00
	#define REG201	0x01
	#define REG202	0x02
	#define REG203	0x03
	#define REG204	0x04
	#define REG205	0x05
	#define REG206	0x06
	#define REG207	0x07
	#define REG208	0x08
	#define REG209	0x09
	#define REG20A	0x0A
	#define REG20B	0x0B
	#define REG20C	0x0C
	#define REG20D	0x0D
	#define REG20E	0x0E
	#define REG20F	0x0F

	#define REG210	0x10
	#define REG211	0x11
	#define REG212	0x12
	#define REG213	0x13
	#define REG214	0x14
	#define REG215	0x15
	#define REG216	0x16
	#define REG217	0x17
	#define REG218	0x18
	#define REG219	0x19
	#define REG21A	0x1A
	#define REG21B	0x1B
	#define REG21C	0x1C
	#define REG21D	0x1D
	#define REG21E	0x1E
	#define REG21F	0x1F

	#define REG220	0x20
	#define REG221	0x21
	#define REG222	0x22
	#define REG223	0x23
	#define REG224	0x24
	#define REG225	0x25
	#define REG226	0x26
	#define REG227	0x27
	#define REG228	0x28
	#define REG229	0x29
	#define REG22A	0x2A
	#define REG22B	0x2B
	#define REG22C	0x2C
	#define REG22D	0x2D
	#define REG22E	0x2E
	#define REG22F	0x2F

	#define REG230	0x30
	#define REG231	0x31
	#define REG232	0x32
	#define REG233	0x33
	#define REG234	0x34
	#define REG235	0x35
	#define REG236	0x36
	#define REG237	0x37
	#define REG238	0x38
	#define REG239	0x39
	#define REG23A	0x3A
	#define REG23B	0x3B
	#define REG23C	0x3C
	#define REG23D	0x3D
	#define REG23E	0x3E
	#define REG23F	0x3F

	#define REG240	0x40
	#define REG241	0x41
	#define REG242	0x42
	#define REG243	0x43
	#define REG244	0x44
	#define REG245	0x45
	#define REG246	0x46
	#define REG247	0x47
	#define REG248	0x48
	#define REG249	0x49
	#define REG24A	0x4A
	#define REG24B	0x4B
	#define REG24C	0x4C
	#define REG24D	0x4D
	#define REG24E	0x4E
	#define REG24F	0x4F

	#define REG250	0x50
	#define REG251	0x51
	#define REG252	0x52
	#define REG253	0x53
	#define REG254	0x54
	#define REG255	0x55
	#define REG256	0x56
	#define REG257	0x57
	#define REG258	0x58
	#define REG259	0x59
	#define REG25A	0x5A
	#define REG25B	0x5B
	#define REG25C	0x5C
	#define REG25D	0x5D
	#define REG25E	0x5E
	#define REG25F	0x5F

	#define REG260	0x60
	#define REG261	0x61
	#define REG262	0x62
	#define REG263	0x63
	#define REG264	0x64
	#define REG265	0x65
	#define REG266	0x66
	#define REG267	0x67
	#define REG268	0x68
	#define REG269	0x69
	#define REG26A	0x6A
	#define REG26B	0x6B
	#define REG26C	0x6C
	#define REG26D	0x6D
	#define REG26E	0x6E
	#define REG26F	0x6F

	#define REG270	0x70
	#define REG271	0x71
	#define REG272	0x72
	#define REG273	0x73
	#define REG274	0x74
	#define REG275	0x75
	#define REG276	0x76
	#define REG277	0x77
	#define REG278	0x78
	#define REG279	0x79
	#define REG27A	0x7A
	#define REG27B	0x7B
	#define REG27C	0x7C
	#define REG27D	0x7D
	#define REG27E	0x7E
	#define REG27F	0x7F

	#define REG280	0x80
	#define REG281	0x81
	#define REG282	0x82
	#define REG283	0x83
	#define REG284	0x84
	#define REG285	0x85
	#define REG286	0x86
	#define REG287	0x87
	#define REG288	0x88
	#define REG289	0x89
	#define REG28A	0x8A
	#define REG28B	0x8B
	#define REG28C	0x8C
	#define REG28D	0x8D
	#define REG28E	0x8E
	#define REG28F	0x8F

	#define REG290	0x90
	#define REG291	0x91
	#define REG292	0x92
	#define REG293	0x93
	#define REG294	0x94
	#define REG295	0x95
	#define REG296	0x96
	#define REG297	0x97
	#define REG298	0x98
	#define REG299	0x99
	#define REG29A	0x9A
	#define REG29B	0x9B
	#define REG29C	0x9C
	#define REG29D	0x9D
	#define REG29E	0x9E
	#define REG29F	0x9F

	#define REG2A0	0xA0
	#define REG2A1	0xA1
	#define REG2A2	0xA2
	#define REG2A3	0xA3
	#define REG2A4	0xA4
	#define REG2A5	0xA5
	#define REG2A6	0xA6
	#define REG2A7	0xA7
	#define REG2A8	0xA8
	#define REG2A9	0xA9
	#define REG2AA	0xAA
	#define REG2AB	0xAB
	#define REG2AC	0xAC
	#define REG2AD	0xAD
	#define REG2AE	0xAE
	#define REG2AF	0xAF

	#define REG2B0	0xB0
	#define REG2B1	0xB1
	#define REG2B2	0xB2
	#define REG2B3	0xB3
	#define REG2B4	0xB4
	#define REG2B5	0xB5
	#define REG2B6	0xB6
	#define REG2B7	0xB7
	#define REG2B8	0xB8
	#define REG2B9	0xB9
	#define REG2BA	0xBA
	#define REG2BB	0xBB
	#define REG2BC	0xBC
	#define REG2BD	0xBD
	#define REG2BE	0xBE
	#define REG2BF	0xBF

	#define REG2C0	0xC0
	#define REG2C1	0xC1
	#define REG2C2	0xC2
	#define REG2C3	0xC3
	#define REG2C4	0xC4
	#define REG2C5	0xC5
	#define REG2C6	0xC6
	#define REG2C7	0xC7
	#define REG2C8	0xC8
	#define REG2C9	0xC9
	#define REG2CA	0xCA
	#define REG2CB	0xCB
	#define REG2CC	0xCC
	#define REG2CD	0xCD
	#define REG2CE	0xCE
	#define REG2CF	0xCF

	#define REG2D0	0xD0
	#define REG2D1	0xD1
	#define REG2D2	0xD2
	#define REG2D3	0xD3
	#define REG2D4	0xD4
	#define REG2D5	0xD5
	#define REG2D6	0xD6
	#define REG2D7	0xD7
	#define REG2D8	0xD8
	#define REG2D9	0xD9
	#define REG2DA	0xDA
	#define REG2DB	0xDB
	#define REG2DC	0xDC
	#define REG2DD	0xDD
	#define REG2DE	0xDE
	#define REG2DF	0xDF

	#define REG2E0	0xE0
	#define REG2E1	0xE1
	#define REG2E2	0xE2
	#define REG2E3	0xE3
	#define REG2E4	0xE4
	#define REG2E5	0xE5
	#define REG2E6	0xE6
	#define REG2E7	0xE7
	#define REG2E8	0xE8
	#define REG2E9	0xE9
	#define REG2EA	0xEA
	#define REG2EB	0xEB
	#define REG2EC	0xEC
	#define REG2ED	0xED
	#define REG2EE	0xEE
	#define REG2EF	0xEF

	#define REG2F0	0xF0
	#define REG2F1	0xF1
	#define REG2F2	0xF2
	#define REG2F3	0xF3
	#define REG2F4	0xF4
	#define REG2F5	0xF5
	#define REG2F6	0xF6
	#define REG2F7	0xF7
	#define REG2F8	0xF8
	#define REG2F9	0xF9
	#define REG2FA	0xFA
	#define REG2FB	0xFB
	#define REG2FC	0xFC
	#define REG2FD	0xFD
	#define REG2FE	0xFE
	//#define REG2FF	0x2FF


//===========================
// PAGE 4
//===========================
	#define REG400	0x00
	#define REG401	0x01
	#define REG402	0x02
	#define REG403	0x03
	#define REG404	0x04
	#define REG405	0x05
	#define REG406	0x06
	#define REG407	0x07
	#define REG408	0x08
	#define REG409	0x09
	#define REG40A	0x0A
	#define REG40B	0x0B
	#define REG40C	0x0C
	#define REG40D	0x0D
	#define REG40E	0x0E
	#define REG40F	0x0F

	#define REG410	0x10
	#define REG411	0x11
	#define REG412	0x12
	#define REG413	0x13
	#define REG414	0x14
	#define REG415	0x15
	#define REG416	0x16
	#define REG417	0x17
	#define REG418	0x18
	#define REG419	0x19
	#define REG41A	0x1A
	#define REG41B	0x1B
	#define REG41C	0x1C
	#define REG41D	0x1D
	#define REG41E	0x1E
	#define REG41F	0x1F

	#define REG420	0x20
	#define REG421	0x21
	#define REG422	0x22
	#define REG423	0x23
	#define REG424	0x24
	#define REG425	0x25
	#define REG426	0x26
	#define REG427	0x27
	#define REG428	0x28
	#define REG429	0x29
	#define REG42A	0x2A
	#define REG42B	0x2B
	#define REG42C	0x2C
	#define REG42D	0x2D
	#define REG42E	0x2E
	#define REG42F	0x2F

	#define REG430	0x30
	#define REG431	0x31
	#define REG432	0x32
	#define REG433	0x33
	#define REG434	0x34
	#define REG435	0x35
	#define REG436	0x36
	#define REG437	0x37
	#define REG438	0x38
	#define REG439	0x39
	#define REG43A	0x3A
	#define REG43B	0x3B
	#define REG43C	0x3C
	#define REG43D	0x3D
	#define REG43E	0x3E
	#define REG43F	0x3F

	#define REG440	0x40
	#define REG441	0x41
	#define REG442	0x42
	#define REG443	0x43
	#define REG444	0x44
	#define REG445	0x45
	#define REG446	0x46
	#define REG447	0x47
	#define REG448	0x48
	#define REG449	0x49
	#define REG44A	0x4A
	#define REG44B	0x4B
	#define REG44C	0x4C
	#define REG44D	0x4D
	#define REG44E	0x4E
	#define REG44F	0x4F

	#define REG450	0x50
	#define REG451	0x51
	#define REG452	0x52
	#define REG453	0x53
	#define REG454	0x54
	#define REG455	0x55
	#define REG456	0x56
	#define REG457	0x57
	#define REG458	0x58
	#define REG459	0x59
	#define REG45A	0x5A
	#define REG45B	0x5B
	#define REG45C	0x5C
	#define REG45D	0x5D
	#define REG45E	0x5E
	#define REG45F	0x5F

	#define REG460	0x60
	#define REG461	0x61
	#define REG462	0x62
	#define REG463	0x63
	#define REG464	0x64
	#define REG465	0x65
	#define REG466	0x66
	#define REG467	0x67
	#define REG468	0x68
	#define REG469	0x69
	#define REG46A	0x6A
	#define REG46B	0x6B
	#define REG46C	0x6C
	#define REG46D	0x6D
	#define REG46E	0x6E
	#define REG46F	0x6F

	#define REG470	0x70
	#define REG471	0x71
	#define REG472	0x72
	#define REG473	0x73
	#define REG474	0x74
	#define REG475	0x75
	#define REG476	0x76
	#define REG477	0x77
	#define REG478	0x78
	#define REG479	0x79
	#define REG47A	0x7A
	#define REG47B	0x7B
	#define REG47C	0x7C
	#define REG47D	0x7D
	#define REG47E	0x7E
	#define REG47F	0x7F

	#define REG480	0x80
	#define REG481	0x81
	#define REG482	0x82
	#define REG483	0x83
	#define REG484	0x84
	#define REG485	0x85
	#define REG486	0x86
	#define REG487	0x87
	#define REG488	0x88
	#define REG489	0x89
	#define REG48A	0x8A
	#define REG48B	0x8B
	#define REG48C	0x8C
	#define REG48D	0x8D
	#define REG48E	0x8E
	#define REG48F	0x8F

	#define REG490	0x90
	#define REG491	0x91
	#define REG492	0x92
	#define REG493	0x93
	#define REG494	0x94
	#define REG495	0x95
	#define REG496	0x96
	#define REG497	0x97
	#define REG498	0x98
	#define REG499	0x99
	#define REG49A	0x9A
	#define REG49B	0x9B
	#define REG49C	0x9C
	#define REG49D	0x9D
	#define REG49E	0x9E
	#define REG49F	0x9F

	#define REG4A0	0xA0
	#define REG4A1	0xA1
	#define REG4A2	0xA2
	#define REG4A3	0xA3
	#define REG4A4	0xA4
	#define REG4A5	0xA5
	#define REG4A6	0xA6
	#define REG4A7	0xA7
	#define REG4A8	0xA8
	#define REG4A9	0xA9
	#define REG4AA	0xAA
	#define REG4AB	0xAB
	#define REG4AC	0xAC
	#define REG4AD	0xAD
	#define REG4AE	0xAE
	#define REG4AF	0xAF

	#define REG4B0	0xB0
	#define REG4B1	0xB1
	#define REG4B2	0xB2
	#define REG4B3	0xB3
	#define REG4B4	0xB4
	#define REG4B5	0xB5
	#define REG4B6	0xB6
	#define REG4B7	0xB7
	#define REG4B8	0xB8
	#define REG4B9	0xB9
	#define REG4BA	0xBA
	#define REG4BB	0xBB
	#define REG4BC	0xBC
	#define REG4BD	0xBD
	#define REG4BE	0xBE
	#define REG4BF	0xBF

	#define REG4C0	0xC0
	#define REG4C1	0xC1
	#define REG4C2	0xC2
	#define REG4C3	0xC3
	#define REG4C4	0xC4
	#define REG4C5	0xC5
	#define REG4C6	0xC6
	#define REG4C7	0xC7
	#define REG4C8	0xC8
	#define REG4C9	0xC9
	#define REG4CA	0xCA
	#define REG4CB	0xCB
	#define REG4CC	0xCC
	#define REG4CD	0xCD
	#define REG4CE	0xCE
	#define REG4CF	0xCF

	#define REG4D0	0xD0
	#define REG4D1	0xD1
	#define REG4D2	0xD2
	#define REG4D3	0xD3
	#define REG4D4	0xD4
	#define REG4D5	0xD5
	#define REG4D6	0xD6
	#define REG4D7	0xD7
	#define REG4D8	0xD8
	#define REG4D9	0xD9
	#define REG4DA	0xDA
	#define REG4DB	0xDB
	#define REG4DC	0xDC
	#define REG4DD	0xDD
	#define REG4DE	0xDE
	#define REG4DF	0xDF

	#define REG4E0	0xE0
	#define REG4E1	0xE1
	#define REG4E2	0xE2
	#define REG4E3	0xE3
	#define REG4E4	0xE4
	#define REG4E5	0xE5
	#define REG4E6	0xE6
	#define REG4E7	0xE7
	#define REG4E8	0xE8
	#define REG4E9	0xE9
	#define REG4EA	0xEA
	#define REG4EB	0xEB
	#define REG4EC	0xEC
	#define REG4ED	0xED
	#define REG4EE	0xEE
	#define REG4EF	0xEF

	#define REG4F0	0xF0
	#define REG4F1	0xF1
	#define REG4F2	0xF2
	#define REG4F3	0xF3
	#define REG4F4	0xF4
	#define REG4F5	0xF5
	#define REG4F6	0xF6
	#define REG4F7	0xF7
	#define REG4F8	0xF8
	#define REG4F9	0xF9
	#define REG4FA	0xFA
	#define REG4FB	0xFB
	#define REG4FC	0xFC
	#define REG4FD	0xFD
	#define REG4FE	0xFE
	//#define REG4FF	0xFF




#endif	//.._TW8836_H_





