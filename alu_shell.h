#pragma once

typedef unsigned long long UINTN;
typedef unsigned long long UINT64;
typedef unsigned short     CHAR16;
typedef unsigned char      UINT8;
typedef unsigned int       UINT32;
typedef unsigned short     UINT16;
typedef UINTN              EFI_STATUS;
typedef void*              EFI_HANDLE;

#define EFI_SUCCESS 0

typedef struct { UINT32 Data1; UINT16 Data2; UINT16 Data3; UINT8 Data4[8]; } EFI_GUID;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void *Reset;
    EFI_STATUS (*OutputString)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
    void *TestString; void *QueryMode; void *SetMode; void *SetAttribute;
    EFI_STATUS (*ClearScreen)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct { UINT16 ScanCode; CHAR16 UnicodeChar; } EFI_INPUT_KEY;
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    void *Reset;
    EFI_STATUS (*ReadKeyStroke)(struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, EFI_INPUT_KEY *Key);
    void *WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct { UINT16 Year; UINT8 Month; UINT8 Day; UINT8 Hour; UINT8 Minute; UINT8 Second; UINT8 Pad1; UINT32 Nanosecond; UINT16 TimeZone; UINT8 Daylight; UINT8 Pad2; } EFI_TIME;
typedef struct { UINT64 Size; UINT64 FileSize; UINT64 PhysicalSize; EFI_TIME CreateTime; EFI_TIME LastAccessTime; EFI_TIME ModificationTime; UINT64 Attribute; CHAR16 FileName[1]; } EFI_FILE_INFO;

typedef struct _EFI_FILE_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (*Open)(struct _EFI_FILE_PROTOCOL *This, struct _EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, UINT64 OpenMode, UINT64 Attributes);
    EFI_STATUS (*Close)(struct _EFI_FILE_PROTOCOL *This);
    EFI_STATUS (*Delete)(struct _EFI_FILE_PROTOCOL *This);
    EFI_STATUS (*Read)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
    EFI_STATUS (*Write)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
    void *GetPosition; EFI_STATUS (*SetPosition)(struct _EFI_FILE_PROTOCOL *This, UINT64 Position);
    void *GetInfo; void *SetInfo; void *Flush;
} EFI_FILE_PROTOCOL;

#define EFI_FILE_MODE_READ   0x0000000000000001
#define EFI_FILE_MODE_WRITE  0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000
#define EFI_FILE_DIRECTORY   0x0000000000000010

typedef EFI_STATUS (*EFI_VOLUME_OPEN)(void *This, EFI_FILE_PROTOCOL **Root);
typedef struct { UINT64 Revision; EFI_VOLUME_OPEN OpenVolume; } EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef EFI_STATUS (*EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface);
typedef struct { char Signature[8]; UINT32 Revision; UINT32 HeaderSize; UINT32 CRC32; UINT32 Reserved; void *RaiseTPL; void *RestoreTPL; void *AllocatePages; void *FreePages; void *GetMemoryMap; void *AllocatePool; void *FreePool; void *CreateEvent; void *SetTimer; void *WaitForEvent; void *SignalEvent; void *CloseEvent; void *CheckEvent; void *InstallProtocolInterface; void *ReinstallProtocolInterface; void *UninstallProtocolInterface; EFI_HANDLE_PROTOCOL HandleProtocol; } EFI_BOOT_SERVICES;
typedef struct _EFI_SYSTEM_TABLE { char Signature[8]; UINT32 Revision; UINT32 HeaderSize; UINT32 CRC32; UINT32 Reserved; CHAR16 *FirmwareVendor; UINT32 FirmwareRevision; EFI_HANDLE ConsoleInHandle; EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn; EFI_HANDLE ConsoleOutHandle; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; EFI_HANDLE StandardErrorHandle; void *StdErr; void *RuntimeServices; EFI_BOOT_SERVICES *BootServices; } EFI_SYSTEM_TABLE;

// -----------------------------------------------------------------------------
// 엔진 전역 변수 및 설정
// -----------------------------------------------------------------------------
EFI_SYSTEM_TABLE *gST;
EFI_FILE_PROTOCOL *gRoot = 0;
EFI_FILE_PROTOCOL *gCurrentDir = 0;

typedef struct {
    CHAR16 OSName[32]; CHAR16 CmdVersion[32]; CHAR16 CmdDir[32]; CHAR16 CmdCd[32];
    CHAR16 CmdOpen[32]; CHAR16 CmdEdit[32]; CHAR16 CmdBat[32]; CHAR16 CmdDelete[32];
    CHAR16 CmdMkdir[32]; CHAR16 CmdMkdoc[32];
} OS_CONFIG;
OS_CONFIG gConfig;

int StrCmp(const CHAR16 *s1, const CHAR16 *s2) { while (*s1 && (*s1 == *s2)) { s1++; s2++; } return *s1 - *s2; }
void StrCpy(CHAR16 *dest, const CHAR16 *src) { while (*src) { *dest++ = *src++; } *dest = '\0'; }
void PrintOut(CHAR16 *Str) { gST->ConOut->OutputString(gST->ConOut, Str); }
void ParseArgs(CHAR16 *Input, CHAR16 *Cmd, CHAR16 *Arg1, CHAR16 *Arg2) {
    UINTN i = 0, j = 0; while(Input[i] == ' ') i++;
    while (Input[i] != ' ' && Input[i] != '\0') { Cmd[j++] = Input[i++]; } Cmd[j] = '\0';
    while (Input[i] == ' ') i++; j = 0;
    while (Input[i] != ' ' && Input[i] != '\0') { Arg1[j++] = Input[i++]; } Arg1[j] = '\0';
    while (Input[i] == ' ') i++; j = 0;
    while (Input[i] != '\0') { Arg2[j++] = Input[i++]; } Arg2[j] = '\0';
}

// -----------------------------------------------------------------------------
// 기능 구현부
// -----------------------------------------------------------------------------
void ExecVersion() { PrintOut(gConfig.OSName); PrintOut(L" Core System\r\n"); }

void ExecDir() {
    if (!gCurrentDir) return;
    gCurrentDir->SetPosition(gCurrentDir, 0);
    UINT8 InfoBuffer[512]; UINTN BufferSize;
    while (1) {
        BufferSize = sizeof(InfoBuffer);
        if (gCurrentDir->Read(gCurrentDir, &BufferSize, InfoBuffer) != EFI_SUCCESS || BufferSize == 0) break;
        EFI_FILE_INFO *FileInfo = (EFI_FILE_INFO *)InfoBuffer;
        if (FileInfo->Attribute & EFI_FILE_DIRECTORY) PrintOut(L"<DIR> "); else PrintOut(L"      ");
        PrintOut(FileInfo->FileName); PrintOut(L"\r\n");
    }
}

void ExecCd(CHAR16 *Dir) {
    if (!gCurrentDir || Dir[0] == '\0') return;
    if (StrCmp(Dir, L"\\") == 0 || StrCmp(Dir, L"/") == 0) {
        if (gCurrentDir != gRoot) gCurrentDir->Close(gCurrentDir); gCurrentDir = gRoot; return;
    }
    EFI_FILE_PROTOCOL *NewDir;
    if (gCurrentDir->Open(gCurrentDir, &NewDir, Dir, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        if (gCurrentDir != gRoot) gCurrentDir->Close(gCurrentDir); gCurrentDir = NewDir;
    } else PrintOut(L"Path not found\r\n");
}

void ExecOpen(CHAR16 *File) {
    if (!gCurrentDir || File[0] == '\0') return;
    EFI_FILE_PROTOCOL *F;
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        UINT8 ReadBuf[64]; UINTN ReadSize; CHAR16 CharStr[2] = {0,0};
        while (1) {
            ReadSize = sizeof(ReadBuf);
            if (F->Read(F, &ReadSize, ReadBuf) != EFI_SUCCESS || ReadSize == 0) break;
            for (UINTN k = 0; k < ReadSize; k++) {
                if (ReadBuf[k] == '\n') PrintOut(L"\r\n");
                else if (ReadBuf[k] >= 32 && ReadBuf[k] <= 126) { CharStr[0] = ReadBuf[k]; PrintOut(CharStr); }
            }
        }
        PrintOut(L"\r\n"); F->Close(F);
    } else PrintOut(L"File not found\r\n");
}

// =====================================================================
// [NEW] 완벽하게 새로 만든 메모리 버퍼 에디터 (기존 글자 지우기 가능)
// =====================================================================
void ExecEdit(CHAR16 *File) {
    if (!gCurrentDir || File[0] == '\0') return;
    EFI_FILE_PROTOCOL *F;
    CHAR16 EditBuf[4096]; // 파일 전체를 담을 RAM 메모리 버퍼
    UINTN EditLen = 0;
    EditBuf[0] = '\0';

    // 1. 기존 파일 내용을 메모리(EditBuf)로 전부 싹 불러오기
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        UINT8 ReadBuf[2048]; UINTN ReadSize;
        while (1) {
            ReadSize = sizeof(ReadBuf);
            if (F->Read(F, &ReadSize, ReadBuf) != EFI_SUCCESS || ReadSize == 0) break;
            for (UINTN k = 0; k < ReadSize && EditLen < 4094; k++) {
                if (ReadBuf[k] >= 32 && ReadBuf[k] <= 126) { EditBuf[EditLen++] = ReadBuf[k]; }
                else if (ReadBuf[k] == '\n') { EditBuf[EditLen++] = '\r'; EditBuf[EditLen++] = '\n'; }
            }
        }
        F->Close(F);
    }
    EditBuf[EditLen] = '\0';

    // 2. 실시간 에디터 화면 루프
    EFI_INPUT_KEY Key;
    while (1) {
        // 매번 화면을 지우고 새로 그려서 진짜 에디터처럼 동작하게 만듦
        gST->ConOut->ClearScreen(gST->ConOut);
        PrintOut(L"--- EDITOR MODE : "); PrintOut(File); PrintOut(L" ---\r\n");
        PrintOut(L"([ESC] 키를 누르면 자동 저장하고 나갑니다)\r\n");
        PrintOut(L"----------------------------------------------------\r\n");
        PrintOut(EditBuf); // 지금까지 쓴 내용 전체 출력
        PrintOut(L"_");   // 커서 깜빡임(가짜)

        // 키보드 입력 대기
        while (gST->ConIn->ReadKeyStroke(gST->ConIn, &Key) != EFI_SUCCESS);

        // [ESC] 키 (저장 및 종료)
        if (Key.ScanCode == 0x17) {
            break;
        }
        // [백스페이스] (기존 글자든 새 글자든 무조건 지움!)
        else if (Key.UnicodeChar == '\b') {
            if (EditLen > 0) {
                // 엔터(\r\n)를 지울 땐 두 칸씩 지워야 함
                if (EditLen >= 2 && EditBuf[EditLen-1] == '\n' && EditBuf[EditLen-2] == '\r') {
                    EditLen -= 2;
                } else {
                    EditLen--;
                }
                EditBuf[EditLen] = '\0';
            }
        }
        // [엔터]
        else if (Key.UnicodeChar == '\r') {
            if (EditLen < 4093) {
                EditBuf[EditLen++] = '\r';
                EditBuf[EditLen++] = '\n';
                EditBuf[EditLen] = '\0';
            }
        }
        // [일반 문자열 입력]
        else if (Key.UnicodeChar >= 32 && Key.UnicodeChar <= 126) {
            if (EditLen < 4095) {
                EditBuf[EditLen++] = Key.UnicodeChar;
                EditBuf[EditLen] = '\0';
            }
        }
    }

    // 3. 편집 끝! 메모리 버퍼 내용을 파일로 덮어쓰기 (저장)
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) {
        F->Delete(F); // 기존 파일 흔적 없애기
    }
    
    // 새 파일 생성해서 완성된 텍스트 넣기
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) {
        UINT8 WriteBuf[4096];
        UINTN WSize = 0;
        for (UINTN i = 0; i < EditLen; i++) {
            WriteBuf[WSize++] = (UINT8)(EditBuf[i] & 0xFF);
        }
        F->Write(F, &WSize, WriteBuf);
        F->Close(F);
    }
    
    gST->ConOut->ClearScreen(gST->ConOut);
    PrintOut(L"[File Successfully Saved!]\r\n");
}
// =====================================================================

void ExecMkdir(CHAR16 *Dir) {
    if (!gCurrentDir || Dir[0] == '\0') return;
    EFI_FILE_PROTOCOL *NewDir;
    if (gCurrentDir->Open(gCurrentDir, &NewDir, Dir, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_FILE_DIRECTORY) == EFI_SUCCESS) NewDir->Close(NewDir);
}
void ExecMkdoc(CHAR16 *File) {
    if (!gCurrentDir || File[0] == '\0') return;
    EFI_FILE_PROTOCOL *NewFile;
    if (gCurrentDir->Open(gCurrentDir, &NewFile, File, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) NewFile->Close(NewFile);
}
void ExecDelete(CHAR16 *File) {
    if (!gCurrentDir || File[0] == '\0') return;
    EFI_FILE_PROTOCOL *F;
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) F->Delete(F);
}

void AluOS_RunLine(CHAR16 *LineBuffer) {
    CHAR16 Cmd[64], Arg1[64], Arg2[64]; ParseArgs(LineBuffer, Cmd, Arg1, Arg2);
    if (Cmd[0] == '\0') return;
    if (StrCmp(Cmd, gConfig.CmdVersion) == 0)      ExecVersion();
    else if (StrCmp(Cmd, gConfig.CmdDir) == 0)     ExecDir();
    else if (StrCmp(Cmd, gConfig.CmdCd) == 0)      ExecCd(Arg1);
    else if (StrCmp(Cmd, gConfig.CmdOpen) == 0)    ExecOpen(Arg1);
    else if (StrCmp(Cmd, gConfig.CmdEdit) == 0)    ExecEdit(Arg1);
    else if (StrCmp(Cmd, gConfig.CmdDelete) == 0)  ExecDelete(Arg1);
    else if (StrCmp(Cmd, gConfig.CmdMkdir) == 0)   ExecMkdir(Arg1);
    else if (StrCmp(Cmd, gConfig.CmdMkdoc) == 0)   ExecMkdoc(Arg1);
    else { PrintOut(L"Unknown Command\r\n"); }
}

void ExecBat(CHAR16 *File) {
    if (!gCurrentDir || File[0] == '\0') return;
    EFI_FILE_PROTOCOL *F;
    if (gCurrentDir->Open(gCurrentDir, &F, File, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        UINT8 ReadBuf[1024]; UINTN ReadSize = sizeof(ReadBuf); F->Read(F, &ReadSize, ReadBuf); F->Close(F);
        CHAR16 LineBuf[128]; UINTN LineIdx = 0;
        for (UINTN k = 0; k < ReadSize; k++) {
            if (ReadBuf[k] == '\r') continue;
            if (ReadBuf[k] == '\n') {
                LineBuf[LineIdx] = '\0';
                if (LineIdx > 0) { PrintOut(L"> "); PrintOut(LineBuf); PrintOut(L"\r\n"); AluOS_RunLine(LineBuf); }
                LineIdx = 0;
            } else if (ReadBuf[k] >= 32 && ReadBuf[k] <= 126) {
                if (LineIdx < 127) LineBuf[LineIdx++] = ReadBuf[k];
            }
        }
        if (LineIdx > 0) { LineBuf[LineIdx] = '\0'; PrintOut(L"> "); PrintOut(LineBuf); PrintOut(L"\r\n"); AluOS_RunLine(LineBuf); }
    } else PrintOut(L"Script not found\r\n");
}

void AluOS_Init(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    gST = SystemTable; StrCpy(gConfig.OSName, L"DefaultOS");
    EFI_GUID GuidLoadedImage = { 0x5B1B31A1, 0x9562, 0x11D2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } };
    EFI_GUID GuidFileSystem = { 0x964E5B22, 0x6459, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } };
    typedef struct { UINT32 Rev; EFI_HANDLE Parent; EFI_SYSTEM_TABLE *ST; EFI_HANDLE DeviceHandle; } L_IMAGE;
    L_IMAGE *LoadedImage; EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    gST->BootServices->HandleProtocol(ImageHandle, &GuidLoadedImage, (void **)&LoadedImage);
    gST->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &GuidFileSystem, (void **)&FileSystem);
    if (FileSystem) { FileSystem->OpenVolume(FileSystem, &gRoot); gCurrentDir = gRoot; }
}

void AluOS_SetOSName(CHAR16 *Name) { StrCpy(gConfig.OSName, Name); }
void AluOS_SetCmd_Version(CHAR16 *Cmd) { StrCpy(gConfig.CmdVersion, Cmd); }
void AluOS_SetCmd_Dir(CHAR16 *Cmd) { StrCpy(gConfig.CmdDir, Cmd); }
void AluOS_SetCmd_Cd(CHAR16 *Cmd) { StrCpy(gConfig.CmdCd, Cmd); }
void AluOS_SetCmd_Open(CHAR16 *Cmd) { StrCpy(gConfig.CmdOpen, Cmd); }
void AluOS_SetCmd_Edit(CHAR16 *Cmd) { StrCpy(gConfig.CmdEdit, Cmd); }
void AluOS_SetCmd_Bat(CHAR16 *Cmd) { StrCpy(gConfig.CmdBat, Cmd); }
void AluOS_SetCmd_Delete(CHAR16 *Cmd) { StrCpy(gConfig.CmdDelete, Cmd); }
void AluOS_SetCmd_Mkdir(CHAR16 *Cmd) { StrCpy(gConfig.CmdMkdir, Cmd); }
void AluOS_SetCmd_Mkdoc(CHAR16 *Cmd) { StrCpy(gConfig.CmdMkdoc, Cmd); }

void AluOS_StartShell() {
    EFI_INPUT_KEY Key; CHAR16 Buffer[256];
    gST->ConOut->ClearScreen(gST->ConOut);
    while (1) {
        PrintOut(gConfig.OSName); PrintOut(L"> ");
        UINTN BufIdx = 0; CHAR16 CharStr[2] = {0, 0};
        while (1) {
            if (gST->ConIn->ReadKeyStroke(gST->ConIn, &Key) == EFI_SUCCESS) {
                if (Key.UnicodeChar == '\r') { PrintOut(L"\r\n"); Buffer[BufIdx] = '\0'; break; }
                else if (Key.UnicodeChar == '\b' && BufIdx > 0) { BufIdx--; PrintOut(L"\b \b"); }
                else if (Key.UnicodeChar >= 32) { Buffer[BufIdx++] = Key.UnicodeChar; CharStr[0] = Key.UnicodeChar; PrintOut(CharStr); }
            }
        }
        if (BufIdx > 0) {
            if (StrCmp(Buffer, gConfig.CmdBat) == 0) ExecBat(Buffer);
            else AluOS_RunLine(Buffer);
        }
    }
}
