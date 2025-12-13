/* 
 * AC46 Firmware Tool.cpp : Defines the entry point for the application.
 * Version 1.1
 * Date 2025/11/23
 * Author 0x0-nyan
 */


#include "framework.h"
#include "AC46 Firmware Tool.h"


#define MAX_LOADSTRING 100
#define CP_AC46 0xFFF0
#define CP_ENC 0xFFF1
#define CP_SFC 0xFFF2
#define CP_BRUTEFORCE 0xFFF3
#define CP_FW_AC46_ENCRYPTED 0xFFF4
#define CP_FW_AC46_RAW 0xFFF5
#define CP_FW_AC69 0xFFF6

#define GFN_OPEN 0
#define GFN_SAVE 1


// Global Variables:
HINSTANCE hInst;                                // current instance


// Forward declarations of functions included in this code module:
INT_PTR CALLBACK    CipherDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MainGUIProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


BOOL WINAPI JLEncCipher(BYTE* pData, BOOL Mode, u32 Length, const u16 Key, HWND OPTIONAL hWnd) {
    if (!pData) return 0;
    u16 Key1 = Key;
    for (long i = 0; i < Length; i++) {
        if(!Mode) if (!(i % 32)) Key1 = Key;
        pData[i] ^= Key1 & 0xFF;
        Key1 = ((Key1 << 1) ^ (Key1 & 0x8000 ? 0x1021 : 0)) & 0xFFFF;
        if (!i % 0xFF) if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, (int)(((double)i / (double)Length) * 100), 0);
    }
    if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
    return 1;
}


BOOL WINAPI JLSfcCipher(BYTE* pData, u32 Length, const u16 Key, HWND OPTIONAL hWnd) {
    if (!pData) return 0;
    u16 Key1 = Key ^ (0 >> 2);
    for (long i = 0; i < Length; i++) {
        if (!(i % 32)) Key1 = Key ^ (i >> 2);
        pData[i] ^= Key1 & 0xFF;
        Key1 = ((Key1 << 1) ^ (Key1 & 0x8000 ? 0x1021 : 0)) & 0xFFFF;
        if (!i % 0xFF) if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, (int)(((double)i / (double)Length) * 100), 0);
    }
    if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
    return 1;
}


BOOL WINAPI JLAC46Cipher(BYTE* pData, u32 Length, HWND OPTIONAL hWnd) {
    if (!pData) return 0;
    u16 Key = 0x0500;
    u16 Key1 = Key ^ (0 >> 2);
    u16 KeyArray[128] = { 0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00, 
                          0x0500, 0x0700, 0x0500, 0x1B00, 0x1D00, 0x1F00, 0x1D00, 0x1B00, 
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x3B00, 0x3D00, 0x3F00, 0x3D00, 0x3B00, 
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x1B00, 0x1D00, 0x1F00, 0x1D00, 0x1B00, 
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x7B00, 0x7D00, 0x7F00, 0x7D00, 0x7B00,
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x1B00, 0x1D00, 0x1F00, 0x1D00, 0x1B00,
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x3B00, 0x3D00, 0x3F00, 0x3D00, 0x3B00,
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0x1B00, 0x1D00, 0x1F00, 0x1D00, 0x1B00,
                          0x0500, 0x0700, 0x0500, 0x0B00, 0x0D00, 0x0F00, 0x0D00, 0x0B00,
                          0x0500, 0x0700, 0x0500, 0xFB00, 0xFD00, 0xFF00, 0xFD00, 0xFB00 };
    for (long i = 0; i < Length; i++) {
        //Key = KeyArray[(i / 0x0400) % 32] | (((i / 0x0400) % 8) > 2 ? (i / 0x2000) % 2 ? (i / 0x2000) << 12 : 0 : 0);
        Key = KeyArray[(i / 0x0400) % 128];
        if (!(i % 32)) Key1 = Key ^ (i >> 2);
        pData[i] ^= Key1 & 0xFF;
        Key1 = ((Key1 << 1) ^ (Key1 & 0x8000 ? 0x1021 : 0)) & 0xFFFF;
        if(!i % 0xFF) if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, (int)(((double)i / (double)Length) * 100), 0);
    }
    if (hWnd) SendMessageW(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
    return 1;
}



u16 WINAPI DecodeChipkey(BYTE* pBuffer) {
    if (!pBuffer) return 0;
    int sum1 = 0;
    u16 Key = 0b0000000000000000;
    for (int i = 0; i < 16; i++) {
        sum1 += pBuffer[i];
    }
    sum1 &= 0xFF;
    if (sum1 <= 0x10) sum1 = 0x55;
    else if (sum1 >= 0xE0) sum1 = 0xAA;
    for (int i = 0; i < 16; i++) {
        if ((pBuffer[16 + i] ^ pBuffer[15 - i]) < sum1) Key |= (1 << i);
    }
    return Key;
}


static LPWSTR WINAPI GetWChar16(LPSTR pString) {    // Probably it's better not to use Unicode, but I use it anyway for future translation (Who will translate this program?)
    if (!pString) return NULL;
    static WCHAR pwString[16];
    int len = MultiByteToWideChar(CP_ACP, 0, pString, -1, pwString, 16);
    return pwString;
}


BOOL WINAPI GetFileNameW(HWND hWnd, LPWSTR Path, int BufferSize, LPCWSTR pwstrFilter, LPCWSTR pwstrTitle, DWORD dwMode) {
    OPENFILENAMEW ofnw;
    ZeroMemory(&ofnw, sizeof(OPENFILENAMEW));
    ofnw.lStructSize = sizeof(OPENFILENAMEW);
    ofnw.hwndOwner = hWnd;
    ofnw.hInstance = hInst;
    ofnw.lpstrFilter = pwstrFilter;
    ofnw.lpstrFile = Path;
    ofnw.nMaxFile = BufferSize;
    ofnw.lpstrTitle = pwstrTitle;
    BOOL res = FALSE;
    switch (dwMode) {
    case 0:     //GFN_OPEN
        res = GetOpenFileNameW(&ofnw);
        break;
    case 1:     //GFN_SAVE
		res = GetSaveFileNameW(&ofnw);
		break;
    }
    if (!res) {
        DWORD ErrCode = CommDlgExtendedError();
        WCHAR ErrMsg[256];
        wsprintfW(ErrMsg, L"GetFileNameW Error %x", ErrCode);
        if(ErrCode) MessageBoxW(hWnd, ErrMsg, NULL, MB_ICONERROR);
    }
    return res;
}

BOOL WINAPI CipherGUI(HWND hWnd, DWORD dwCipherMode) {
    BYTE* Data0 = nullptr;
    //DWORD BytesRead = 0;
	WCHAR Path[512] = { 0 };
    CIPHERSTRUCT cs;
	ZeroMemory(&cs, sizeof(CIPHERSTRUCT));
    cs.ChipKey = NULL;
    cs.dwCipherMode = dwCipherMode;
    //cs.hWndDialog = hWnd;
    cs.nBufferLength = 1048576;
    //cs.szOutputPath = (LPWSTR)L"user.app";
    if (GetFileNameW(hWnd, Path, sizeof(Path), L"All Files\0*.*\0\0", NULL, GFN_OPEN)) {
		HANDLE hRFile = CreateFileW(Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        if (hRFile) {
            Data0 = new BYTE[1048576];
            ZeroMemory(Data0, 1048576);
            if (!ReadFile(hRFile, Data0, 1048576, &cs.nBytesToProcess, NULL)) {
                MessageBoxW(NULL, L"Failed to read file", NULL, MB_ICONERROR);
                delete[] Data0;
                CloseHandle(hRFile);
                return 0;
            }
            cs.pbBuffer = Data0;
            //cs.nBufferLength = BytesRead;
            cs.szInputPath = Path;
            WCHAR szCurDir[512] = { 0 }, szOutPath[512] = {0};
            GetCurrentDirectoryW(512, szCurDir);
            wsprintfW(szOutPath, L"%s\\Cipher.bin", szCurDir);
			cs.szOutputPath = szOutPath;
            Sleep(500);
            CloseHandle(hRFile);
            
		}
        else {
            MessageBoxW(NULL, L"Failed to open file", NULL, MB_ICONERROR);
            return 0;
        }
    }
    else {
        return 0;
    }
    INT_PTR Result = DialogBoxParamW(hInst, MAKEINTRESOURCEW(IDD_DIALOG_CIPHER), hWnd, CipherDlgProc, (LPARAM)&cs);
    switch (Result) {
    case 1:
        return 0;
    case 4:
        delete[] Data0;
        Sleep(500);
        return 0;
    }
    delete[] Data0;
    Sleep(2000);
    return 0;
}


void WINAPI ShowChipkey(HWND hWnd) {
    BYTE Data0[128];
    ZeroMemory(Data0, 128);
    DWORD BytesRead = 0;
    WCHAR Path[1024] = { 0 };
    if (GetFileNameW(NULL, Path, sizeof(Path), L"chip_key.bin\0*.*\0\0", L"Decode Chipkey...", GFN_OPEN)) {
        HANDLE hRFile = CreateFileW(Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        if (hRFile) {
            if (!ReadFile(hRFile, Data0, 128, &BytesRead, NULL)) {
                MessageBoxW(NULL, L"Failed to read file", NULL, MB_ICONERROR);
                CloseHandle(hRFile);
                return;
            }
            Sleep(200);
            CloseHandle(hRFile);
        }
        else MessageBoxW(NULL, L"Failed to open file", NULL, MB_ICONERROR);
    }
    else MessageBoxW(NULL, L"Failed to open file", NULL, MB_ICONERROR);
    u16 ChipKey = DecodeChipkey(Data0);
	WCHAR Msg[256];
    wsprintfW(Msg, L"Chip Key: %04X", ChipKey);
	MessageBoxW(hWnd, Msg, L"Chip Key", MB_ICONINFORMATION);
    return;
}

void WINAPI ShowCrc16(HWND hWnd) {
    BYTE* Data0 = nullptr;
    DWORD BytesRead = 0;
    WCHAR Path[1024] = { 0 };
    if (GetFileNameW(NULL, Path, sizeof(Path), L"All Files\0*.*\0\0", L"Calculate CRC16 (Max. 1MB) ...", GFN_OPEN)) {
        HANDLE hRFile = CreateFileW(Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
        if (hRFile) {
            Data0 = new BYTE[1048576];
            ZeroMemory(Data0, 1048576);
            if (!ReadFile(hRFile, Data0, 1048576, &BytesRead, NULL)) {
                MessageBoxW(NULL, L"Failed to read file", NULL, MB_ICONERROR);
                delete[] Data0;
                CloseHandle(hRFile);
                return;
            }
            CloseHandle(hRFile);
        }
        else MessageBoxW(NULL, L"Failed to open file", NULL, MB_ICONERROR);
    }
    else MessageBoxW(NULL, L"Failed to open file", NULL, MB_ICONERROR);
    u16 CRC16 = crc16_ccitt(Data0, BytesRead);
    WCHAR Msg[256];
    wsprintfW(Msg, L"CRC16: 0x%04X", CRC16);
    delete[] Data0;
    MessageBoxW(hWnd, Msg, L"CRC16", MB_ICONINFORMATION);
    return;
}


DWORD WINAPI CipherThreadProc(LPVOID lpParam) {
    CIPHERSTRUCT cs;
    PCIPHERSTRUCT pCS = (PCIPHERSTRUCT)lpParam;
    ZeroMemory(&cs, sizeof(CIPHERSTRUCT));
    memcpy(&cs, pCS, sizeof(CIPHERSTRUCT));
    BYTE* Buffer = cs.pbBuffer;
    SendMessageW(GetDlgItem(cs.hWndDialog, IDC_PROGRESS1), PBM_SETPOS, 10, 0);
    BOOL cpResult = FALSE;
    switch (cs.dwCipherMode) {
    case CP_AC46:
        cpResult = JLAC46Cipher(Buffer, cs.nBytesToProcess, cs.hWndDialog);
        break;
    case CP_ENC:
        cpResult = JLEncCipher(Buffer, 0, cs.nBytesToProcess, cs.ChipKey, cs.hWndDialog);
        break;
    case CP_SFC:
        cpResult = JLSfcCipher(Buffer, cs.nBytesToProcess, cs.ChipKey, cs.hWndDialog);
        break;
    default:
		MessageBoxW(cs.hWndDialog, L"Invalid cipher mode", NULL, MB_ICONERROR);
        break;
    }

    if (cpResult) {
        HANDLE hFile = CreateFileW(cs.szOutputPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD BytesWritten;
            WriteFile(hFile, Buffer, cs.nBytesToProcess, &BytesWritten, NULL);
            CloseHandle(hFile);
            SendMessageW(GetDlgItem(cs.hWndDialog, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
            MessageBeep(MB_ICONINFORMATION);
        }
        else {
            DWORD dwError = GetLastError();
            WCHAR ErrMsg[256];
            wsprintfW(ErrMsg, L"CreateFile Error %x", dwError);
            MessageBoxW(NULL, ErrMsg, NULL, MB_ICONERROR);
        }
    }
    else {
        MessageBoxW(cs.hWndDialog, L"ぬるぽ", NULL, MB_ICONERROR);
    }
    EnableWindow(GetDlgItem(cs.hWndDialog, IDFINISH), TRUE);
    Sleep(5000);
    delete[] Buffer;
    return 0;
}



DWORD WINAPI AC46FWUnpackProc(LPVOID lpParam) {
    CIPHERSTRUCT cs;
    memcpy(&cs, lpParam, sizeof(CIPHERSTRUCT));
    WCHAR *Msg = new WCHAR[8192];
    WCHAR szInput[256], szOutput[256], tMsg[512];
	memcpy(szInput, cs.szInputPath, 256 * sizeof(WCHAR));
	memcpy(szOutput, cs.szOutputPath, 256 * sizeof(WCHAR));
	szInput[255] = 0;
	szOutput[255] = 0;
    FLASHHEADER FlashHeader;
    PFILEHEAD FileHeads;
    DWORD BytesRead = 0;
    BYTE *pFlash, *pData;
    Sleep(100);

    SendMessageW(GetDlgItem(cs.hWndDialog, IDC_MESBOX), EM_SETLIMITTEXT, 8192, 0);
	wsprintfW(Msg, L"Input File Path: %s\r\nOutput Path: %s\r\nReading File...", szInput, szOutput);
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);

    // Read input file
	HANDLE hFile = CreateFileW(szInput, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
    if(hFile != INVALID_HANDLE_VALUE) {
        pFlash = new BYTE[1048576];
		ZeroMemory(pFlash, 1048576);
        if (ReadFile(hFile, pFlash, 1048576, &BytesRead, NULL)) {
            CloseHandle(hFile);
            wsprintfW(tMsg, L"\r\nFlash Image Size: %d Bytes", BytesRead);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        }
        else {
            DWORD dwError = GetLastError();
            CloseHandle(hFile);
            wsprintfW(tMsg, L"\r\nERROR: Cannot read input file. Code: %d\r\nOperation Failed.", dwError);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            delete[] pFlash;
            delete[] Msg;
            return 1;
        }
    }
    else {
        DWORD dwError = GetLastError();
        wsprintfW(tMsg, L"\r\nERROR: Cannot open input file. Code: %d\r\nOperation Failed.", dwError);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
        MessageBeep(MB_ICONERROR);
        delete[] Msg;
        return 1;
	}

    // Auto detect scrambling from 0xFFFFFFFD signature
    if(pFlash[0x8] == 0xFD && pFlash[0x9] == 0xFF && pFlash[0xA] == 0xFF && pFlash[0xB] == 0xFF) {
        cs.dwCipherMode = CP_FW_AC46_RAW;
        wcscat_s(Msg, 8192, L"\r\nScrambled: False");
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    }
    else {
        JLEncCipher(pFlash, 0, 32, 0xFFFF, NULL);
        if (pFlash[0x8] == 0xFD && pFlash[0x9] == 0xFF && pFlash[0xA] == 0xFF && pFlash[0xB] == 0xFF) {
            cs.dwCipherMode = CP_FW_AC46_ENCRYPTED;
            wcscat_s(Msg, 8192, L"\r\nScrambled: True");
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        }
        else {
            wcscat_s(Msg, 8192, L"\r\nERROR: Invalid AC46 flash image file.\r\nOperation Failed.");
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            delete[] pFlash;
            delete[] Msg;
			return 1;
        }
	}
	
    // Read flash header
    memcpy(&FlashHeader, pFlash, sizeof(FLASHHEADER));
	wsprintfW(tMsg, L"\r\nFlash Header CRC: 0x%04X\r\nFile List CRC: 0x%04X\r\nFile Count: %d\r\n", FlashHeader.CRC16, FlashHeader.CRC16FileHead, FlashHeader.nFiles);
    wcscat_s(Msg, 8192, tMsg);
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);

    // Check CRCs
    u16 ActualCRC = crc16_ccitt(&pFlash[2], 32 - 2);
    if (ActualCRC != FlashHeader.CRC16) {
        wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of flash header (0x%04X) doesn't match the listed one (0x%04X).\r\nThere might be some data corruption.", ActualCRC, FlashHeader.CRC16);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    }
    ActualCRC = crc16_ccitt(&pFlash[32], 32 * FlashHeader.nFiles);
    if (ActualCRC != FlashHeader.CRC16FileHead) {
        wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of file list (0x%04X) doesn't match the listed one (0x%04X).\r\nThere might be some data corruption.", ActualCRC, FlashHeader.CRC16FileHead);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    }

    // Descramble file list if scrambled
    if (cs.dwCipherMode == CP_FW_AC46_ENCRYPTED) JLEncCipher(&pFlash[32], 0, sizeof(FILEHEAD) * FlashHeader.nFiles, 0xFFFF, NULL);

    // Allocate memory and read file headers
	FileHeads = new FILEHEAD[FlashHeader.nFiles];
    for (int i = 0; i < FlashHeader.nFiles; i++) {
		memcpy(&FileHeads[i], &pFlash[sizeof(FLASHHEADER) + (i * sizeof(FILEHEAD))], sizeof(FILEHEAD));    
        wsprintfW(tMsg, L"\r\n%s     Address: 0x%X   Size: %d Bytes     CRC16: 0x%04X     Type: %d", GetWChar16(FileHeads[i].Name), FileHeads[i].Address, FileHeads[i].Length, FileHeads[i].CRC16, FileHeads[i].Filetype);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        if(FileHeads[i].Address + FileHeads[i].Length > BytesRead) {
            wsprintfW(tMsg, L"\r\nERROR: File %s exceeds flash image size (0x%X).\r\nOperation failed.", GetWChar16(FileHeads[i].Name), (FileHeads[i].Address + FileHeads[i].Length));
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            delete[] pFlash;
            delete[] Msg;
            delete[] FileHeads;
            return 1;
		}
        //if (!strcmp(FileHeads[i].Name, "_____.____1")) addrChipkey = FileHeads[i].Address + 0x80;
    }

    // Create output directory
    wcscat_s(Msg, 8192, L"\r\n\r\nCreating Output Directory...");
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
	BOOL res = CreateDirectoryW(szOutput, NULL);
    if(!res) {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_ALREADY_EXISTS) {
            wsprintfW(tMsg, L"ERROR: Fialed to create output directory. Code: %d\r\nOperation Failed.", dwError);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            delete[] pFlash;
            delete[] Msg;
            delete[] FileHeads;
            return 1;
        }
	}
    SetCurrentDirectoryW(szOutput);

    // Allocate memory and save uboot.boot
    pData = new BYTE[1048576];
    for (int i = 0; i < FlashHeader.nFiles; i++) {
        if (FileHeads[i].Filetype == 1) {
            wcscat_s(Msg, 8192, L"\r\nExtracting uboot.boot ...");
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            ZeroMemory(pData, 1048576);
            memcpy(pData, &pFlash[FileHeads[i].Address], FileHeads[i].Length);

            // Descramble it if scrambled
            if (cs.dwCipherMode == CP_FW_AC46_ENCRYPTED) {
                wcscat_s(Msg, 8192, L"\r\nDescrambling uboot.boot ...");
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg); 
                JLEncCipher(pData, 0, 16, 0xFFFF, NULL);                                // For first BANKCB header
                JLEncCipher(&pData[16], 1, FileHeads[i].Length - 16, 0xFFFF, NULL);     // For code itself, with a mode 1
            }

            // Check CRC16
            ActualCRC = crc16_ccitt(pData, FileHeads[i].Length);
            if (ActualCRC != FileHeads[i].CRC16) {
                wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of uboot.boot (0x%04X) doesn't match the listed one (0x%04X).\r\nUsing this bootloader is not recommended.", ActualCRC, FileHeads[i].CRC16);
                wcscat_s(Msg, 8192, tMsg);
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            }

            // Write to file
            hFile = CreateFileW(L"uboot.boot", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD BytesWritten;
                WriteFile(hFile, pData, FileHeads[i].Length, &BytesWritten, NULL);
                CloseHandle(hFile);
                break;
            }
            else {
                DWORD dwError = GetLastError();
                wsprintfW(tMsg, L"\r\nERROR: Failed to create file. Code: %d\r\nOperation Failed.", dwError);
                wcscat_s(Msg, 8192, tMsg);
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
                SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
                MessageBeep(MB_ICONERROR);
                delete[] pFlash;
                delete[] pData;
                delete[] Msg;
                delete[] FileHeads;
                return 1;
            }
        }
    }

	// Crack user.app
    u32 resOffset = 0, ResSize = 0;
    for (int i = 0; i < FlashHeader.nFiles; i++) {
        if(FileHeads[i].Filetype == 2){
            wsprintfW(tMsg, L"\r\nCopying %s to buffer...", GetWChar16(FileHeads[i].Name));
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            ZeroMemory(pData, 1048576);
            memcpy(pData, &pFlash[FileHeads[i].Address], FileHeads[i].Length);
			ResSize = FileHeads[i].Length;
            delete[] pFlash;

            // Descramble it if scrambled
            if (cs.dwCipherMode == CP_FW_AC46_ENCRYPTED) {
                wcscat_s(Msg, 8192, L"\r\nDescrambling user.app ...");
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
                JLEncCipher(pData, 0, FileHeads[i].Length, 0xFFFF, NULL);
            }

            // Perform AC46 Cipher
            wcscat_s(Msg, 8192, L"\r\nPerforming AC46 Cipher...");
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            JLAC46Cipher(pData, FileHeads[i].Length, NULL);

            // Check CRC16
            ActualCRC = crc16_ccitt(pData, FileHeads[i].Length);
            if (ActualCRC != FileHeads[i].CRC16) {
                wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of user.app (0x%04X) doesn't match the listed one (0x%04X).\r\nThere might be some data corruption.", ActualCRC, FileHeads[i].CRC16);
                wcscat_s(Msg, 8192, tMsg);
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            }

            // Find resource file list
            BYTE chiptype[16] = {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x98, 0x00, 0xF0, 0xF0,
                0x0A, 0x01, 0x00, 0x00
            };
            for (resOffset = 0; resOffset < FileHeads[i].Length; resOffset += 16) {
                if (!memcmp(&pData[resOffset], chiptype, 16)) {
                    resOffset -= 16;
					wsprintfW(tMsg, L"\r\nResource file list found at offset 0x%X.", resOffset);
                    wcscat_s(Msg, 8192, tMsg);
                    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
                    break;
                }
            }
            if (resOffset >= FileHeads[i].Length - 32) {
                wcscat_s(Msg, 8192, L"\r\nERROR: No resource file list found. \r\nOperation Failed.");
                SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
                SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
                MessageBeep(MB_ICONERROR);
                //delete[] pFlash;
                delete[] pData;
                delete[] Msg;
                delete[] FileHeads;
                return 1;
            }
            break;
		}
    }

    // Read resource header
    FLASHHEADER ResHeader;
	memcpy(&ResHeader, &pData[resOffset], sizeof(FLASHHEADER));
    wsprintfW(tMsg, L"\r\nResource Header CRC: 0x%04X\r\nResource File List CRC: 0x%04X\r\nFile Count: %d\r\n", ResHeader.CRC16, ResHeader.CRC16FileHead, ResHeader.nFiles);
    wcscat_s(Msg, 8192, tMsg);
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);

    // Check CRCs of headers
    ActualCRC = crc16_ccitt(&pData[resOffset + 2], 30);
    if (ActualCRC != ResHeader.CRC16) {
        wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of resource header (0x%04X) doesn't match the listed one (0x%04X).\r\nThere might be some data corruption.", ActualCRC, ResHeader.CRC16);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    }
    ActualCRC = crc16_ccitt(&pData[resOffset + 32], 32 * ResHeader.nFiles);
    if (ActualCRC != ResHeader.CRC16FileHead) {
        wsprintfW(tMsg, L"\r\nINFO: Actual CRC16 of resource file list (0x%04X) doesn't match the listed one (0x%04X).\r\nThis problem is still being investigated.", ActualCRC, ResHeader.CRC16FileHead);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    }

    // Allocate memory and read resource file headers
    PFILEHEAD ResFileHeads = new FILEHEAD[ResHeader.nFiles];
    for (int i = 0; i < ResHeader.nFiles; i++) {
        memcpy(&ResFileHeads[i], &pData[resOffset + 32 + (i * sizeof(FILEHEAD))], sizeof(FILEHEAD));    
        wsprintfW(tMsg, L"\r\n%s   Address: 0x%X     Size: %d Bytes     CRC16: 0x%04X     Type: %d", GetWChar16(ResFileHeads[i].Name), (ResFileHeads[i].Address - ResFileHeads[0].Address), ResFileHeads[i].Length, ResFileHeads[i].CRC16, ResFileHeads[i].Filetype);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        if ((ResFileHeads[i].Address - ResFileHeads[0].Address) + ResFileHeads[i].Length > ResSize) {
            wsprintfW(tMsg, L"\r\nERROR: File %s exceeds user.app size (0x%X).\r\nOperation failed.", GetWChar16(ResFileHeads[i].Name), ((ResFileHeads[i].Address - ResFileHeads[0].Address) + ResFileHeads[i].Length));
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            //delete[] pFlash;
            delete[] pData;
            delete[] FileHeads;
            delete[] ResFileHeads;
            delete[] Msg;
            return 1;
        }
    }
    u32 ExtraOffset = ResFileHeads[0].Address;      // For some reason all resource files have extra offset (usually 0x1400) while sdram actually starts from 0.
	
    // Create resource directory and repacker batch
    wcscat_s(Msg, 8192, L"\r\n\r\nCreating Resource Directory...");
    wcscat_s(szOutput, 256, L"\\Resources");
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    res = CreateDirectoryW(szOutput, NULL);
    if (!res) {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_ALREADY_EXISTS) {
            wsprintfW(tMsg, L"\r\nERROR: Failed to create resource directory. Code: %d\r\nOperation Failed.", dwError);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            delete[] pData;
            delete[] FileHeads;
            delete[] ResFileHeads;
            delete[] Msg;
            return 1;
        }
    }
    SetCurrentDirectoryW(szOutput);
    char *pRepacker = new char[4096];
    ZeroMemory(pRepacker, 4096);
    strcat_s(pRepacker, 4096, "@echo AC46 Firmware Repacker (reflasher)\r\n");
    strcat_s(pRepacker, 4096, "@echo Before you continue, make sure that there are no any JieLi devices are connected via USB, unless if you want to flash them.\r\n");
    strcat_s(pRepacker, 4096, "pause\r\n\r\n");
    strcat_s(pRepacker, 4096, "isd_download.exe -f uboot.boot ");

    // Extract each files
    for (int i = 0; i < ResHeader.nFiles; i++) {
		wsprintfW(tMsg, L"\r\nExtracting resource file %s...", GetWChar16(ResFileHeads[i].Name));
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);

        // Check CRC16
		ActualCRC = crc16_ccitt(&pData[ResFileHeads[i].Address - ExtraOffset], ResFileHeads[i].Length);
        if (ActualCRC != ResFileHeads[i].CRC16) {
            wsprintfW(tMsg, L"\r\nWARNING: Actual CRC16 of file %s (0x%04X) doesn't match the listed one (0x%04X).\r\nThere might be some data corruption.", GetWChar16(ResFileHeads[i].Name), ActualCRC, ResFileHeads[i].CRC16);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        }
        // Write to file
        hFile = CreateFileW(GetWChar16(ResFileHeads[i].Name), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD BytesWritten;
            char tFile[64];
            WriteFile(hFile, &pData[ResFileHeads[i].Address - ExtraOffset], ResFileHeads[i].Length, &BytesWritten, NULL);
            CloseHandle(hFile);
            wsprintfA(tFile, "Resources\\%s ", ResFileHeads[i].Name);
            strcat_s(pRepacker, 4096, tFile);
        }
        else {
            DWORD dwError = GetLastError();
            wsprintfW(tMsg, L"\r\nERROR: Failed to create file. Code: %d\r\nOperation Failed.", dwError);
            wcscat_s(Msg, 8192, tMsg);
            SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
            SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
            MessageBeep(MB_ICONERROR);
            //delete[] pFlash;
            delete[] pData;
            delete[] FileHeads;
            delete[] ResFileHeads;
            delete[] Msg;
            delete[] pRepacker;
            return 1;
		}
    }
	
    // Add rest of commands to batch and save it
    wcscat_s(Msg, 8192, L"\r\nExtraction completed!\r\nCreating repacker...");
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    strcat_s(pRepacker, 4096, "-tonorflash -dev bt15 -div6 -updata -runaddr 0x1000000 -wait 300\r\n\r\n");
    strcat_s(pRepacker, 4096, "del jl_ac46.bin\r\nrename jl_isd.bin jl_ac46.bin\r\n\r\n");
    strcat_s(pRepacker, 4096, "bfumake.exe -fi jl_ac46.bin -ld 0x0000 -rd 0x0000 -fo jl_bt46.bfu\r\n");
    strcat_s(pRepacker, 4096, "pause");
    hFile = CreateFileW(L"Repacker.bat.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD BytesWritten;
        WriteFile(hFile, pRepacker, strlen(pRepacker), &BytesWritten, NULL);
        CloseHandle(hFile);
        
    }
    else {
        DWORD dwError = GetLastError();
        wsprintfW(tMsg, L"\r\nERROR: Failed to create repacker. Code: %d\r\nOperation Failed.", dwError);
        wcscat_s(Msg, 8192, tMsg);
        SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
        SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
        MessageBeep(MB_ICONERROR);
        //delete[] pFlash;
        delete[] pData;
        delete[] FileHeads;
        delete[] ResFileHeads;
        delete[] Msg;
        delete[] pRepacker;
        return 1;
    }

    wcscat_s(Msg, 8192, L"\r\nAll tasks completed!");
    SetDlgItemTextW(cs.hWndDialog, IDC_MESBOX, Msg);
    SetDlgItemTextW(cs.hWndDialog, IDC_INPUTPATH, L"No file selected, drag an AC46 flash image here to open");
    MessageBeep(MB_ICONINFORMATION);
    Sleep(10000);
    //delete[] pFlash;
    delete[] pData;
    delete[] FileHeads;
    delete[] ResFileHeads;
    delete[] Msg;
    delete[] pRepacker;
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    hInst = hInstance;
    HWND hWnd = CreateDialogParamW(hInstance, MAKEINTRESOURCEW(IDD_MAIN_GUI), NULL, MainGUIProc, NULL);
    if (!hWnd) {
        DWORD dwError = GetLastError();
        WCHAR ErrMsg[256];
        wsprintfW(ErrMsg, L"CreateDialogParamW Error %d", dwError);
        MessageBoxW(NULL, ErrMsg, NULL, MB_ICONERROR);
        return 1;
    }
	ShowWindow(hWnd, nCmdShow);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AC46FIRMWARETOOL));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}


INT_PTR CALLBACK CipherDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    //UNREFERENCED_PARAMETER(lParam);
    static CIPHERSTRUCT cs;
    switch (message)
    {
    case WM_INITDIALOG: {
        PCIPHERSTRUCT pCS = (PCIPHERSTRUCT)lParam;
        ZeroMemory(&cs, sizeof(CIPHERSTRUCT));
        memcpy(&cs, pCS, sizeof(CIPHERSTRUCT));
        cs.hWndDialog = hDlg;
        SetDlgItemTextW(hDlg, IDC_INPUTPATH, cs.szInputPath);
        SetDlgItemTextW(hDlg, IDC_OUTPATH, cs.szOutputPath);
        SendMessageW(GetDlgItem(hDlg, IDC_KEY), EM_SETLIMITTEXT, 4, 0);
        switch (cs.dwCipherMode) {
        case CP_ENC:
            SetWindowTextW(hDlg, L"ENC Cipher");
            SetDlgItemTextW(hDlg, IDC_KEY, L"FFFF");
            break;
        case CP_SFC:
            SetWindowTextW(hDlg, L"SFC Cipher");
            SetDlgItemTextW(hDlg, IDC_KEY, L"FFFF");
            break;
        case CP_AC46:
            SetWindowTextW(hDlg, L"AC46 Cipher");
            SetDlgItemTextW(hDlg, IDC_KEY, L"<No key required>");
            SendDlgItemMessageW(hDlg, IDC_KEY, WM_ENABLE, FALSE, 0);
            //SetDlgItemTextW(hDlg, IDC_OUTPATH, L"C:\\user.app");
            break;
        //case CP_BRUTEFORCE:
        //    SetWindowTextW(hDlg, L"ENC Bruteforcer");
        //    SetDlgItemTextW(hDlg, IDC_STATIC, L"Initial Key (Increased by every 32 bytes, 16 bit Unsigned Hexadecimal)");
        //    SetDlgItemTextW(hDlg, IDC_KEY, L"0000");
        }
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDFINISH:
            EndDialog(hDlg, 1);
            return (INT_PTR)TRUE;
        case IDCANCEL:
            EndDialog(hDlg, 4);
            return (INT_PTR)TRUE;
        case IDSTART: {
            GetDlgItemTextW(hDlg, IDC_OUTPATH, cs.szOutputPath, MAX_PATH);
            if (cs.dwCipherMode == CP_ENC || cs.dwCipherMode == CP_SFC || cs.dwCipherMode == CP_BRUTEFORCE) {
                WCHAR szKey[8] = { 0 };
                u16 Key = 0;
                GetDlgItemTextW(hDlg, IDC_KEY, szKey, 6);
                if (swscanf_s(szKey, L"%hx", &Key)) {
                    cs.ChipKey = Key;
                }
                else {
                    MessageBoxW(hDlg, L"Invalid Key", NULL, MB_ICONERROR);
                    return (INT_PTR)TRUE;
                }
            }
            SendDlgItemMessageW(hDlg, IDC_KEY, WM_ENABLE, FALSE, 0);
            SendDlgItemMessageW(hDlg, IDC_OUTPATH, WM_ENABLE, FALSE, 0);
			EnableWindow(GetDlgItem(hDlg, IDSELECT), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDSTART), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
            CreateThread(NULL, 0, CipherThreadProc, &cs, 0, NULL);
            Sleep(500);
            return (INT_PTR)TRUE;
        }
        case IDSELECT: {
            WCHAR Path[1024] = { 0 };
            if (GetFileNameW(hDlg, Path, sizeof(Path), L"All Files\0*.*\0\0", L"Select Output File", GFN_SAVE)) {
                SetDlgItemTextW(hDlg, IDC_OUTPATH, Path);
                cs.szOutputPath = Path;
            }
            return (INT_PTR)TRUE;
        }
      }
      break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainGUIProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
			SetDlgItemTextW(hWnd, IDC_MESBOX, L"Drag an AC46 Flash Image here, or select via menu.");
            return (INT_PTR)TRUE;

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                ExitProcess(0);
                break;
            case ID_FILE_OPEN: {
				WCHAR Path[512] = { 0 };
                if (GetFileNameW(hWnd, Path, sizeof(Path), L"Firmware / Flash Image;All Files\0*.bin;*.*\0\0", L"Open AC46 Firmware Image...", GFN_OPEN)) {
                    SetDlgItemTextW(hWnd, IDC_INPUTPATH, Path);
                    EnableWindow(GetDlgItem(hWnd, IDEXTRACT), TRUE);
                }
                break;
            }
            case ID_TOOLS_ENCCIPHER:
                CipherGUI(hWnd, CP_ENC);
                break;
            case ID_TOOLS_AC46CIPHER:
                CipherGUI(hWnd, CP_AC46);
                break;
            case ID_TOOLS_SFCCIPHER:
                CipherGUI(hWnd, CP_SFC);
                break;
            case ID_TOOLS_CALCULATECRC16:
                ShowCrc16(hWnd);
                break;
            case ID_TOOLS_DECODECHIPKEY:
                ShowChipkey(hWnd);
                break;
            //case ID_TOOLS_BRUTEFORCE:
            //    CipherGUI(hWnd, CP_BRUTEFORCE);
            //    break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_HELP_VISITMYGITHUB:
                ShellExecuteW(NULL, L"open", L"https://github.com/0x0-nyan", NULL, NULL, SW_SHOW);
                break;

            case IDEXTRACT: {
                WCHAR InputPath[512] = { 0 }, OutputPath[512] = { 0 };
                GetDlgItemTextW(hWnd, IDC_INPUTPATH, InputPath, 512);
                if (!lstrcmpiW(InputPath, L"No file selected, drag an AC46 flash image here to open")) {
                    MessageBoxW(hWnd, L"No File Selected", NULL, MB_ICONERROR);
                    break;
                }
                EnableWindow(GetDlgItem(hWnd, IDEXTRACT), FALSE);
				wsprintfW(OutputPath, L"%s_Contents", InputPath);
				static CIPHERSTRUCT cs;
				ZeroMemory(&cs, sizeof(CIPHERSTRUCT));
				cs.szInputPath = InputPath;
				cs.szOutputPath = OutputPath;
                cs.hWndDialog = hWnd;
                cs.dwCipherMode = CP_FW_AC46_RAW;
                CreateThread(NULL, 0, AC46FWUnpackProc, &cs, 0, NULL);
                Sleep(500);
                break;
            }
            }
            return (INT_PTR)TRUE;
        }
        case WM_DROPFILES: {
            HDROP hDrop = (HDROP)wParam;
			WCHAR DroppedFile[MAX_PATH] = { 0 };
            UINT nFiles = DragQueryFileW(hDrop, 0, DroppedFile, MAX_PATH);
            if (nFiles) {
                SetDlgItemTextW(hWnd, IDC_INPUTPATH, DroppedFile);
                EnableWindow(GetDlgItem(hWnd, IDEXTRACT), TRUE);
            }
            else {
                MessageBoxW(hWnd, L"Drag and Drop failed.\nYou can still select file via Menu.", NULL, MB_ICONERROR);
            }
            DragFinish(hDrop);
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hWnd);
            ExitProcess(0);
			break;
    }
    return (INT_PTR)FALSE;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
