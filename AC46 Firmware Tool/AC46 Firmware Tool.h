#pragma once

#include "resource.h"

typedef UINT8 u8;
typedef UINT16 u16;
typedef UINT32 u32;


typedef struct _FLASHHEADER
{
    u16 CRC16;
    u16 CRC16FileHead;
    u8 Info[8];
    u32 nFiles;
    u32 Version;
    u32 Version1;
    u8 Chiptype[8];

} FLASHHEADER, *PFLASHHEADER;


typedef struct _FILEHEAD
{
    u8 Filetype;
    u8 Reserved;
    u16 CRC16;
    u32 Address;
    u32 Length;
    u32 Index;
    char Name[16];

} FILEHEAD, *PFILEHEAD;


typedef struct _BANKCB
{
    u16 BankNum;
    u16 Size;
    u32 BankAddress;
    u32 Address;
    u16 CRC16;
    u16 CRC16Bank;
} BANKCB, *PBANKCB;

/*
typedef struct _IMAGEHEADER512 {
    FLASHHEADER FlashHeader;
    FLASHFILEHEAD File0;
    FLASHFILEHEAD File1;
    FLASHFILEHEAD File2;
    FLASHFILEHEAD File3;
    FLASHFILEHEAD File4;
    FLASHFILEHEAD File5;
    FLASHFILEHEAD File6;
    FLASHFILEHEAD File7;
    FLASHFILEHEAD File8;
    FLASHFILEHEAD File9;
    FLASHFILEHEAD FileA;
    FLASHFILEHEAD FileB;
    FLASHFILEHEAD FileC;
    FLASHFILEHEAD FileD;
    FLASHFILEHEAD FileE;
} IMAGEHEADER512, *PIMAGEHEADER512;
*/

typedef struct _CIPHERSTRUCT {
    PBYTE pbBuffer;
    DWORD nBufferLength;
    DWORD nBytesToProcess;
    u16 OPTIONAL ChipKey;
    DWORD dwCipherMode;
    LPWSTR szInputPath;
    LPWSTR szOutputPath;
    HWND hWndDialog;
} CIPHERSTRUCT, *PCIPHERSTRUCT;


typedef struct _AC46STRUCT {    // I'm not sure about its byte alignment
    u16 CRC16;
    u16 CRC16User;
    u32 Address;
    u16 Unknown1;
    u32 SDRAMOffset;            // I guess so
    u32 Unknown2;
    u16 Unknown3;
    u8  Unknown4;
} AC46STRUCT, *PAC46STRUCT;