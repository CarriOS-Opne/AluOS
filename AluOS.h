#ifndef ALUOS_H
#define ALUOS_H

#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")
#endif

// ============================================================================
// 1. MSVC Linker Error Prevention
// ============================================================================
#ifdef __cplusplus
extern "C" {
#endif
    void* memset(void* dest, int c, unsigned long long count);
    void* memcpy(void* dest, const void* src, unsigned long long count);
    extern int _fltused;
#ifdef __cplusplus
}
#endif

// ============================================================================
// 2. UEFI Core Structures & C Basic Types
// ============================================================================
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef u16                CHAR16;
typedef u64                UINTN;
typedef u64                EFI_STATUS;
typedef void* EFI_HANDLE;

#define EFI_SUCCESS 0
#define EFIAPI 

// 💡 키보드 스캔코드 매크로 
#define SCAN_UP    0x01
#define SCAN_DOWN  0x02
#define SCAN_F2    0x0C

typedef struct { u32 D1; u16 D2; u16 D3; u8 D4[8]; } EFI_GUID;
typedef struct { u32 Version; u32 HorizontalResolution; u32 VerticalResolution; u32 PixelFormat; u32 PixelInformation[4]; u32 PixelsPerScanLine; } EFI_GOP_MODE_INFO;
typedef struct { u32 MaxMode; u32 Mode; EFI_GOP_MODE_INFO *Info; UINTN SizeOfInfo; u64 FrameBufferBase; UINTN FrameBufferSize; } EFI_GOP_MODE;
typedef struct { EFI_STATUS(EFIAPI *QueryMode)(void*, u32, UINTN*, EFI_GOP_MODE_INFO**); EFI_STATUS(EFIAPI *SetMode)(void*, u32); void* B; EFI_GOP_MODE *Mode; } EFI_GOP;

typedef struct { u16 ScanCode; CHAR16 UnicodeChar; } EFI_INPUT_KEY;
typedef struct { EFI_INPUT_KEY Key; struct { u32 KeyToggleState; u32 KeyShiftState; } KeyState; } EFI_KEY_DATA;
typedef struct EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL { EFI_STATUS (EFIAPI *Reset)(void*, u8); EFI_STATUS (EFIAPI *ReadKeyStrokeEx)(void*, EFI_KEY_DATA*); void *WaitForKeyEx; EFI_STATUS (EFIAPI *SetState)(void*, u32*); EFI_STATUS (EFIAPI *RegisterKeyNotify)(void*, EFI_KEY_DATA*, EFI_STATUS(EFIAPI*)(EFI_KEY_DATA*), void**); EFI_STATUS (EFIAPI *UnregisterKeyNotify)(void*, void*); } EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

typedef struct { int RelativeMovementX; int RelativeMovementY; int RelativeMovementZ; u8 LeftButton; u8 RightButton; } EFI_SIMPLE_POINTER_STATE;
typedef struct { EFI_STATUS(EFIAPI *Reset)(void*, u8); EFI_STATUS(EFIAPI *GetState)(void*, EFI_SIMPLE_POINTER_STATE*); void* WaitForInput; struct { u64 ResolutionX; u64 ResolutionY; u8 LeftButton; u8 RightButton; } *Mode; } EFI_SIMPLE_POINTER_PROTOCOL;

#define EFI_FILE_MODE_READ   0x0000000000000001ULL
#define EFI_FILE_MODE_WRITE  0x0000000000000002ULL
#define EFI_FILE_MODE_CREATE 0x8000000000000000ULL
#define EFI_FILE_DIRECTORY   0x0000000000000010ULL

typedef struct EFI_FILE_PROTOCOL {
    u64 Revision;
    EFI_STATUS (EFIAPI *Open)(struct EFI_FILE_PROTOCOL*, struct EFI_FILE_PROTOCOL**, CHAR16*, u64, u64);
    EFI_STATUS (EFIAPI *Close)(struct EFI_FILE_PROTOCOL*);
    EFI_STATUS (EFIAPI *Delete)(struct EFI_FILE_PROTOCOL*);
    EFI_STATUS (EFIAPI *Read)(struct EFI_FILE_PROTOCOL*, UINTN*, void*);
    EFI_STATUS (EFIAPI *Write)(struct EFI_FILE_PROTOCOL*, UINTN*, void*);
    EFI_STATUS (EFIAPI *GetPosition)(struct EFI_FILE_PROTOCOL*, u64*);
    EFI_STATUS (EFIAPI *SetPosition)(struct EFI_FILE_PROTOCOL*, u64);
    EFI_STATUS (EFIAPI *GetInfo)(struct EFI_FILE_PROTOCOL*, EFI_GUID*, UINTN*, void*);
    EFI_STATUS (EFIAPI *SetInfo)(struct EFI_FILE_PROTOCOL*, EFI_GUID*, UINTN*, void*);
    EFI_STATUS (EFIAPI *Flush)(struct EFI_FILE_PROTOCOL*);
} EFI_FILE_PROTOCOL;
typedef struct { u64 Revision; EFI_STATUS (EFIAPI *OpenVolume)(void*, EFI_FILE_PROTOCOL**); } EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct { u16 Year; u8 Month; u8 Day; u8 Hour; u8 Minute; u8 Second; u8 Pad1; u32 Nanosecond; u16 TimeZone; u8 Daylight; u8 Pad2; } EFI_TIME;
typedef struct { u64 Size; u64 FileSize; u64 PhysicalSize; EFI_TIME CreateTime; EFI_TIME LastAccessTime; EFI_TIME ModificationTime; u64 Attribute; CHAR16 FileName[1]; } EFI_FILE_INFO;

typedef struct { 
    char _hdr[24]; EFI_STATUS (EFIAPI *GetTime)(EFI_TIME *Time, void *Capabilities);
    void *SetTime, *GetWakeupTime, *SetWakeupTime, *SetVirtualAddressMap, *ConvertPointer, *GetVariable, *GetNextVariableName, *SetVariable, *GetNextHighMonotonicCount;
    void (EFIAPI *ResetSystem)(UINTN ResetType, EFI_STATUS ResetStatus, UINTN DataSize, void *ResetData);
} EFI_RT;

typedef struct { 
    char _h[24]; void *R1, *R2, *A1, *F1;
    EFI_STATUS (EFIAPI *GetMemoryMap)(UINTN*, void*, UINTN*, UINTN*, u32*);
    EFI_STATUS (EFIAPI *AllocatePool)(UINTN PoolType, UINTN Size, void **Buffer);
    EFI_STATUS (EFIAPI *FreePool)(void *Buffer);
    void *C1, *S1, *W1, *S2, *C2, *C3, *I1, *R3, *U1, *H1, *P1, *R4, *L1, *L2, *I2, *L3, *S3, *E1, *U2;
    EFI_STATUS (EFIAPI *ExitBootServices)(EFI_HANDLE, UINTN);
    void *G1, *S4, *S5, *C4, *D1, *O1, *C5, *O2, *P2, *L4;
    EFI_STATUS (EFIAPI *LocateProtocol)(EFI_GUID*, void**, void**);
    void* I3; EFI_STATUS (EFIAPI *Stall)(UINTN);
    EFI_STATUS (EFIAPI *SetWatchdogTimer)(UINTN Timeout, u64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
} EFI_BS;

typedef struct { 
    char _hdr[24]; void* V1; u32 R1, P1; void *H1; void *ConIn; void *H2; 
    struct { void* R; EFI_STATUS (EFIAPI *OutputString)(void*, CHAR16*); } *ConOut; 
    void *H3, *E1; EFI_RT* RuntimeServices; EFI_BS* BootServices; 
} EFI_ST;

// ============================================================================
// 3. AluOS API (Global English Interface)
// ============================================================================
#define COLOR_BLUE       0xFF2196F3
#define COLOR_LIGHT_BLUE 0xFF90CAF9
#define COLOR_PINK       0xFFE91E63
#define COLOR_GREEN      0xFF4CAF50
#define COLOR_WHITE      0xFFFFFFFF
#define COLOR_BLACK      0xFF000000
#define COLOR_GRAY       0xFFE0E0E0
#define COLOR_DARK       0xFF202124
#define COLOR_BG         0xFFF1F3F4

extern u32 AluMouseX, AluMouseY;
extern int AluMouseClick;     
extern int AluMouseDown;      
extern int AluMouseRightClick;
extern int AluKeyboardChar;
extern int AluKeyboardScanCode; 
extern int AluMouseScroll;
extern u32 AluScreenWidth, AluScreenHeight;

// Core Engine API
void Alu_Init(EFI_ST *st, u32 req_width, u32 req_height);
void Alu_Update(void);
void Alu_Begin(u32 color);
void Alu_End(void);

// Text Input Engine
void Alu_ProcessInput(char* buf, int max_len, int* is_korean);

// Drawing API
void Alu_DrawPixel(int x, int y, u32 color);
void Alu_DrawRect(int x, int y, int w, int h, u32 color);
void Alu_DrawRoundedRect(int x, int y, int w, int h, int r, u32 color);
void Alu_DrawCircle(int cx, int cy, int r, u32 color);
void Alu_DrawLine(int x0, int y0, int x1, int y1, u32 color);
void Alu_DrawString(int x, int y, const char* s, u32 color, int scale);
void Alu_DrawCursor(void);

// UI Controls
void Alu_Window(int x, int y, int w, int h, const char* title);
int  Alu_Button(int x, int y, int w, int h, const char* text, u32 color);
void Alu_ButtonCb(int x, int y, int w, int h, const char* text, u32 color, void (*callback)(void));
void Alu_Slider(int x, int y, int w, int* val, int max_val);

// Extended System API (File, Font, BMP)
void Alu_FileWrite(const char* path, const char* data);
void Alu_FileRead(const char* path, char* out_buf, int max_len);
void Alu_FileDelete(const char* path);
void Alu_FileRename(const char* old_path, const char* new_path);
void Alu_GetFileList(const char* path, char names[32][64], int is_dir[32], int* count);
void Alu_ChangeFont(const char* path);
void Alu_DrawBMP(int x, int y, const char* path);

void IntToString(int n, char* out);
unsigned long long efi_strlen(const char* str);

// ============================================================================
// 🌟 3.5. Ultra-Simple API (파이썬보다 쉬운 매크로 엔진!)
// ============================================================================
// 1. 화면 크기 및 OS 초기화
#define OS_INIT(w, h) EFI_STATUS EFIAPI EfiMain(EFI_HANDLE ImageHandle, EFI_ST *SystemTable) { Alu_Init(SystemTable, w, h);
#define OS_LOOP() while(1) { Alu_Update();
#define OS_CLEAR(color) Alu_Begin(color);
#define OS_RENDER() Alu_DrawCursor(); Alu_End(); } return 0; }

// 2. 폰트 및 사진 출력
#define FONT(path) Alu_ChangeFont(path)
#define IMAGE(x, y, path) Alu_DrawBMP(x, y, path)

// 3. 창, 텍스트, 버튼 UI
#define WINDOW(x, y, w, h, title) Alu_Window(x, y, w, h, title)
#define TEXT(x, y, color, scale, str) Alu_DrawString(x, y, str, color, scale)
#define BUTTON(x, y, w, h, color, text, func) Alu_ButtonCb(x, y, w, h, text, color, func)
#define SLIDER(x, y, w, val_ptr, max) Alu_Slider(x, y, w, val_ptr, max)

// 4. 그림 그리기
#define RECT(x, y, w, h, color) Alu_DrawRect(x, y, w, h, color)
#define CIRCLE(x, y, r, color) Alu_DrawCircle(x, y, r, color)
#define LINE(x1, y1, x2, y2, color) Alu_DrawLine(x1, y1, x2, y2, color)

// ============================================================================
// 4. AluOS Implementation (Included once via ALUOS_IMPLEMENTATION)
// ============================================================================
#ifdef ALUOS_IMPLEMENTATION

#if defined(_MSC_VER)
#ifdef __cplusplus
extern "C" {
#endif
    int _fltused = 0;
#ifdef __cplusplus
}
#endif
#pragma function(memset)
void* memset(void* dest, int c, unsigned long long count) { 
    unsigned char* p = (unsigned char*)dest; while(count--) *p++=(unsigned char)c; return dest; 
}
#pragma function(memcpy)
void* memcpy(void* dest, const void* src, unsigned long long count) { 
    unsigned char* d = (unsigned char*)dest; const unsigned char* s = (const unsigned char*)src; while(count--) *d++=*s++; return dest; 
}
#endif

#ifndef NULL
#define NULL 0
#endif

EFI_BS *g_bs = 0; EFI_RT *g_rt = 0;
u32 AluScreenWidth = 1024, AluScreenHeight = 768;
u32 AluMouseX = 512, AluMouseY = 384;
int AluMouseClick = 0, AluMouseDown = 0, AluMouseRightClick = 0, AluKeyboardChar = 0, AluKeyboardScanCode = 0, AluMouseScroll = 0;
static int prev_mouse_down = 0;

static u32 back_buffer[2560 * 1440];
static u32* fb_ptr; static u32 fb_stride;
static EFI_SIMPLE_POINTER_PROTOCOL *mouse_prot = 0;
static EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *kb_prot = 0;

void* efi_malloc(unsigned long long size) { void* ptr = 0; if (g_bs && g_bs->AllocatePool(2, size, &ptr) == 0) return ptr; return 0; }
void efi_free(void* ptr) { if (g_bs && ptr) g_bs->FreePool(ptr); }
unsigned long long efi_strlen(const char* str) { unsigned long long len=0; if(!str) return 0; while(str[len]) len++; return len; }

float efi_fabs(float x) { return x < 0.0f ? -x : x; }
int efi_ifloor(float x) { int i = (int)x; return (x < 0.0f && x != (float)i) ? i - 1 : i; }
int efi_iceil(float x) { int i = (int)x; return (x > 0.0f && x != (float)i) ? i + 1 : i; }
float efi_sqrt(float x) { if (x <= 0.0f) return 0.0f; float z = x; for (int i = 0; i < 20; i++) { if (z == 0.0f) break; z = 0.5f * (z + x / z); } return z; }
float efi_fmod(float x, float y) { return x - (float)((int)(x / y)) * y; }
float efi_sin(float x) { x = efi_fmod(x, 2.0f * 3.14159265f); if (x < 0.0f) x += 2.0f * 3.14159265f; if (x > 3.14159265f) return -efi_sin(x - 3.14159265f); float x2 = x * x; return x - (x * x2) / 6.0f + (x * x2 * x2) / 120.0f - (x * x2 * x2 * x2) / 5040.0f; }
float efi_cos(float x) { return efi_sin(x + 3.14159265f / 2.0f); }

#define STBTT_malloc(x,u)  ((void)(u), efi_malloc((unsigned long long)(x)))
#define STBTT_free(x,u)    ((void)(u), efi_free(x))
#define STBTT_assert(x)    ((void)(0))
#define STBTT_ifloor(x)    efi_ifloor(x)
#define STBTT_iceil(x)     efi_iceil(x)
#define STBTT_sqrt(x)      efi_sqrt(x)
#define STBTT_pow(x,y)     (x)
#define STBTT_fmod(x,y)    efi_fmod(x,y)
#define STBTT_cos(x)       (0.0f)
#define STBTT_acos(x)      (0.0f)
#define STBTT_fabs(x)      efi_fabs(x)
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
#define STBTT_strlen       efi_strlen

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#pragma warning(disable: 4244 4389 4146 4242 4189)
#include "stb_truetype.h"

stbtt_fontinfo global_font; int font_loaded = 0;

u32 DecodeUTF8(const char **s) {
    u8 c = (u8)**s; if (c == 0) return 0;
    if (c < 0x80) { (*s)++; return c; }
    if ((c & 0xE0) == 0xC0) { u32 u = (u32)(((c & 0x1F) << 6) | (((u8)*(*s+1)) & 0x3F)); (*s) += 2; return u; }
    if ((c & 0xF0) == 0xE0) { u32 u = (u32)(((c & 0x0F) << 12) | ((((u8)*(*s+1)) & 0x3F) << 6) | (((u8)*(*s+2)) & 0x3F)); (*s) += 3; return u; }
    (*s)++; return c; 
}
void IntToString(int n, char* out) { if(n == 0) { out[0] = '0'; out[1] = 0; return; } int i = 0, sign = (n < 0); if(sign) n = -n; while(n > 0) { out[i++] = (char)((n % 10) + '0'); n /= 10; } if(sign) out[i++] = '-'; out[i] = 0; for(int j = 0; j < i / 2; j++) { char t = out[j]; out[j] = out[i - 1 - j]; out[i - 1 - j] = t; } }

// 한글 오토마타 엔진 통합 구현
int GetChoIndex(char c) { switch(c) { case 'r': return 0; case 'R': return 1; case 's': return 2; case 'e': return 3; case 'E': return 4; case 'f': return 5; case 'a': return 6; case 'q': return 7; case 'Q': return 8; case 't': return 9; case 'T': return 10; case 'd': return 11; case 'w': return 12; case 'W': return 13; case 'c': return 14; case 'z': return 15; case 'x': return 16; case 'v': return 17; case 'g': return 18; } return -1; }
int GetJungIndex(char c) { switch(c) { case 'k': return 0; case 'o': return 1; case 'i': return 2; case 'O': return 3; case 'j': return 4; case 'p': return 5; case 'u': return 6; case 'P': return 7; case 'h': return 8; case 'y': return 12; case 'n': return 13; case 'b': return 17; case 'm': return 18; case 'l': return 20; } return -1; }
int MergeJung(int jung1, char c2) { if(jung1 == 8 && c2 == 'k') return 9; if(jung1 == 8 && c2 == 'o') return 10; if(jung1 == 8 && c2 == 'l') return 11; if(jung1 == 13 && c2 == 'j') return 14; if(jung1 == 13 && c2 == 'p') return 15; if(jung1 == 13 && c2 == 'l') return 16; if(jung1 == 18 && c2 == 'l') return 19; return -1; }
int GetJongIndex(char c) { switch(c) { case 'r': return 1; case 'R': return 2; case 's': return 4; case 'e': return 7; case 'f': return 8; case 'a': return 16; case 'q': return 17; case 't': return 19; case 'T': return 20; case 'd': return 21; case 'w': return 22; case 'c': return 23; case 'z': return 24; case 'x': return 25; case 'v': return 26; case 'g': return 27; } return -1; }
int MergeJong(int jong1, char c2) { if(jong1 == 1 && c2 == 't') return 3; if(jong1 == 4 && c2 == 'w') return 5; if(jong1 == 4 && c2 == 'g') return 6; if(jong1 == 8 && c2 == 'r') return 9; if(jong1 == 8 && c2 == 'a') return 10; if(jong1 == 8 && c2 == 'q') return 11; if(jong1 == 8 && c2 == 't') return 12; if(jong1 == 8 && c2 == 'x') return 13; if(jong1 == 8 && c2 == 'v') return 14; if(jong1 == 8 && c2 == 'g') return 15; if(jong1 == 17 && c2 == 't') return 18; return -1; }
u32 PopLastUTF8(char* buf, int* len) { if (*len <= 0) return 0; int start = *len - 1; while(start > 0 && (buf[start] & 0xC0) == 0x80) start--; const char* ptr = &buf[start]; u32 c = DecodeUTF8(&ptr); *len = start; buf[*len] = 0; return c; }
void AppendRawUTF8(char* buf, int* len, int max, u32 u) { if (u < 0x80) { if (*len < max - 1) { buf[(*len)++] = (char)(u & 0xFF); buf[*len] = 0; } } else if (u < 0x800) { if (*len < max - 2) { buf[(*len)++] = (char)(0xC0 | ((u >> 6) & 0x1F)); buf[(*len)++] = (char)(0x80 | (u & 0x3F)); buf[*len] = 0; } } else { if (*len < max - 3) { buf[(*len)++] = (char)(0xE0 | ((u >> 12) & 0x0F)); buf[(*len)++] = (char)(0x80 | ((u >> 6) & 0x3F)); buf[(*len)++] = (char)(0x80 | (u & 0x3F)); buf[*len] = 0; } } }

void Alu_ProcessInput(char* buf, int max_len, int* is_korean) {
    if (AluKeyboardScanCode == SCAN_F2) { *is_korean = !(*is_korean); return; }
    if (AluKeyboardChar == 0) return;
    
    int len = (int)efi_strlen(buf);
    if (AluKeyboardChar == 0x08) { PopLastUTF8(buf, &len); return; } 
    if (AluKeyboardChar == '\r' || AluKeyboardChar == '\n') { AppendRawUTF8(buf, &len, max_len, '\n'); return; }
    if (AluKeyboardChar < 32) return;

    char key = (char)AluKeyboardChar;
    if (!(*is_korean) || key < 'A' || key > 'z') { AppendRawUTF8(buf, &len, max_len, (u32)key); return; } 
    
    u32 last_c = PopLastUTF8(buf, &len); int cho = -1, jung = -1, jong = -1, is_hangul = 0;
    if (last_c >= 0xAC00U && last_c <= 0xD7A3U) {
        is_hangul = 1; int code = (int)(last_c - 0xAC00U); jong = code % 28; jung = ((code - jong) / 28) % 21; cho = ((code - jong) / 28) / 21;
    }
    int new_cho = GetChoIndex(key), new_jung = GetJungIndex(key), new_jong = GetJongIndex(key);
    if (is_hangul) {
        if (jong == 0 && new_jung != -1) { int merged = MergeJung(jung, key); if (merged != -1) { AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(cho * 21 * 28) + (u32)(merged * 28)); return; } }
        if (jong == 0 && new_jong != -1) { AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(cho * 21 * 28) + (u32)(jung * 28) + (u32)new_jong); return; }
        if (jong != 0 && new_jung != -1) { int merged = MergeJong(jong, key); if (merged != -1) { AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(cho * 21 * 28) + (u32)(jung * 28) + (u32)merged); return; } }
        if (jong != 0 && new_jung != -1) {
            int move_cho = -1, keep_jong = 0;
            if(jong==3){keep_jong=1; move_cho=9;} else if(jong==5){keep_jong=4; move_cho=12;} else if(jong==6){keep_jong=4; move_cho=18;}
            else if(jong==9){keep_jong=8; move_cho=0;} else if(jong==10){keep_jong=8; move_cho=6;} else if(jong==11){keep_jong=8; move_cho=7;}
            else if(jong==12){keep_jong=8; move_cho=9;} else if(jong==13){keep_jong=8; move_cho=16;} else if(jong==14){keep_jong=8; move_cho=17;}
            else if(jong==15){keep_jong=8; move_cho=18;} else if(jong==18){keep_jong=17; move_cho=9;} else {
                int jong_to_cho[] = {-1, 0, 1, -1, 2, -1, -1, 3, 5, -1, -1, -1, -1, -1, -1, -1, 6, 7, -1, 9, 10, 11, 12, 14, 15, 16, 17, 18};
                move_cho = jong_to_cho[jong]; keep_jong = 0;
            }
            if (move_cho != -1) { AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(cho * 21 * 28) + (u32)(jung * 28) + (u32)keep_jong); AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(move_cho * 21 * 28) + (u32)(new_jung * 28)); return; }
        }
    }
    if (last_c != 0) AppendRawUTF8(buf, &len, max_len, last_c);
    if (new_cho != -1) { int c2j[] = {0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E}; AppendRawUTF8(buf, &len, max_len, (u32)c2j[new_cho]); return; }
    if (new_jung != -1) {
        int j2j[] = {0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163};
        if (last_c >= 0x3131U && last_c <= 0x314EU) {
            int prev_cho = -1; int c2j[] = {0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E};
            for(int i=0; i<19; i++) if(c2j[i] == (int)last_c) { prev_cho = i; break; }
            if (prev_cho != -1) { PopLastUTF8(buf, &len); AppendRawUTF8(buf, &len, max_len, 0xAC00U + (u32)(prev_cho * 21 * 28) + (u32)(new_jung * 28)); return; }
        }
        AppendRawUTF8(buf, &len, max_len, (u32)j2j[new_jung]); return;
    }
    AppendRawUTF8(buf, &len, max_len, (u32)key);
}

void Alu_Init(EFI_ST *st, u32 req_width, u32 req_height) {
    g_bs = st->BootServices; g_rt = st->RuntimeServices;
    g_bs->SetWatchdogTimer(0, 0, 0, NULL); 
    
    EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
    EFI_GUID mouse_guid = {0x31878c87, 0x0b75, 0x11d5, {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
    EFI_GUID input_ex_guid = {0xdd9e7534, 0x7762, 0x4698, {0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa}};
    
    EFI_GOP *gop = 0; g_bs->LocateProtocol(&gop_guid, 0, (void**)&gop);
    g_bs->LocateProtocol(&mouse_guid, 0, (void**)&mouse_prot); if(mouse_prot) mouse_prot->Reset(mouse_prot, 1);
    g_bs->LocateProtocol(&input_ex_guid, 0, (void**)&kb_prot);
    
    if (gop) {
        u32 target_mode = 0, found = 0, max_pixels = 0, best_mode = 0;
        for (u32 i = 0; i < gop->Mode->MaxMode; i++) {
            UINTN sz; EFI_GOP_MODE_INFO *info;
            if (gop->QueryMode(gop, i, &sz, &info) == EFI_SUCCESS) {
                if (info->HorizontalResolution == req_width && info->VerticalResolution == req_height) { target_mode = i; found = 1; break; }
                if (info->HorizontalResolution <= 2560 && info->VerticalResolution <= 1440) {
                    u32 px = info->HorizontalResolution * info->VerticalResolution;
                    if (px > max_pixels) { max_pixels = px; best_mode = i; }
                }
            }
        }
        gop->SetMode(gop, found ? target_mode : best_mode);
        fb_ptr = (u32*)gop->Mode->FrameBufferBase; fb_stride = gop->Mode->Info->PixelsPerScanLine;
        AluScreenWidth = gop->Mode->Info->HorizontalResolution; AluScreenHeight = gop->Mode->Info->VerticalResolution;
        AluMouseX = AluScreenWidth/2; AluMouseY = AluScreenHeight/2;
    }
}

void Alu_Update(void) {
    prev_mouse_down = AluMouseDown;
    AluMouseDown = 0; AluMouseRightClick = 0; AluKeyboardChar = 0; AluKeyboardScanCode = 0; AluMouseScroll = 0;
    
    if (mouse_prot) {
        EFI_SIMPLE_POINTER_STATE st;
        if (mouse_prot->GetState(mouse_prot, &st) == EFI_SUCCESS) {
            AluMouseX += st.RelativeMovementX; AluMouseY += st.RelativeMovementY;
            AluMouseScroll = st.RelativeMovementZ;
            if ((int)AluMouseX < 0) AluMouseX = 0; if (AluMouseX >= AluScreenWidth) AluMouseX = AluScreenWidth - 1;
            if ((int)AluMouseY < 0) AluMouseY = 0; if (AluMouseY >= AluScreenHeight) AluMouseY = AluScreenHeight - 1;
            if (st.LeftButton) AluMouseDown = 1;
            if (st.RightButton) AluMouseRightClick = 1;
        }
    }
    if (kb_prot) {
        EFI_KEY_DATA kd; 
        if (kb_prot->ReadKeyStrokeEx(kb_prot, &kd) == EFI_SUCCESS) {
            AluKeyboardChar = (int)kd.Key.UnicodeChar;
            AluKeyboardScanCode = (int)kd.Key.ScanCode;
        }
    }
    AluMouseClick = (AluMouseDown && !prev_mouse_down); // Edge Trigger!
}

void Alu_Begin(u32 color) { for (u64 i = 0; i < (u64)fb_stride * AluScreenHeight; i++) back_buffer[i] = color; }
void Alu_End(void) { for (u64 i = 0; i < (u64)fb_stride * AluScreenHeight; i++) fb_ptr[i] = back_buffer[i]; if(g_bs) g_bs->Stall(5000); }

void Alu_DrawPixel(int x, int y, u32 c) {
    if(x < 0 || x >= (int)AluScreenWidth || y < 0 || y >= (int)AluScreenHeight) return;
    u8 a = (u8)((c >> 24) & 0xFF); if (a == 0) return;
    if (a == 255) { back_buffer[(u64)y * fb_stride + x] = c; return; }
    u32 bg = back_buffer[(u64)y * fb_stride + x];
    u8 r1 = (u8)((c >> 16) & 0xFF), g1 = (u8)((c >> 8) & 0xFF), b1 = (u8)(c & 0xFF);
    u8 r2 = (u8)((bg >> 16) & 0xFF), g2 = (u8)((bg >> 8) & 0xFF), b2 = (u8)(bg & 0xFF);
    back_buffer[(u64)y * fb_stride + x] = (0xFF << 24) | (((r1*a + r2*(255-a))/255)<<16) | (((g1*a + g2*(255-a))/255)<<8) | ((b1*a + b2*(255-a))/255);
}
void Alu_DrawRect(int x, int y, int w, int h, u32 c) { for(int i=0; i<h; i++) for(int j=0; j<w; j++) Alu_DrawPixel(x+j, y+i, c); }
void Alu_DrawRoundedRect(int x, int y, int w, int h, int r, u32 c) {
    for(int i=0; i<h; i++) for(int j=0; j<w; j++) {
        if((i<r && j<r && (r-i)*(r-i)+(r-j)*(r-j)>r*r) || (i<r && j>w-r && (r-i)*(r-i)+(j-(w-r))*(j-(w-r))>r*r) || (i>h-r && j<r && (i-(h-r))*(i-(h-r))+(r-j)*(r-j)>r*r) || (i>h-r && j>w-r && (i-(h-r))*(i-(h-r))+(j-(w-r))*(j-(w-r))>r*r)) continue;
        Alu_DrawPixel(x+j, y+i, c);
    }
}
void Alu_DrawCircle(int cx, int cy, int r, u32 c) { for(int y=-r; y<=r; y++) for(int x=-r; x<=r; x++) if(x*x+y*y <= r*r) Alu_DrawPixel(cx+x, cy+y, c); }
void Alu_DrawLine(int x0, int y0, int x1, int y1, u32 color) { 
    int dx = (int)efi_fabs((float)(x1 - x0)), dy = (int)efi_fabs((float)(y1 - y0)); int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1; int err = (dx > dy ? dx : -dy) / 2, e2; 
    for(;;) { Alu_DrawPixel(x0, y0, color); if (x0 == x1 && y0 == y1) break; e2 = err; if (e2 > -dx) { err -= dy; x0 += sx; } if (e2 < dy) { err += dx; y0 += sy; } } 
}

void Alu_ChangeFont(const char* path) {
    CHAR16 w_path[256]; memset(w_path, 0, 256 * 2);
    for(int i=0; i<256; i++) { w_path[i] = path[i]; if(!path[i]) break; }
    EFI_GUID sfsp_guid = {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FS; EFI_FILE_PROTOCOL *Root, *File;
    if (g_bs->LocateProtocol(&sfsp_guid, 0, (void**)&FS) == EFI_SUCCESS) {
        if (FS->OpenVolume(FS, &Root) == EFI_SUCCESS) {
            if (Root->Open(Root, &File, w_path, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
                u8 info_buf[512]; UINTN info_sz = 512; EFI_GUID InfoGuid = { 0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} };
                if (File->GetInfo(File, &InfoGuid, &info_sz, info_buf) == EFI_SUCCESS) {
                    u32 size = (u32)((EFI_FILE_INFO*)info_buf)->FileSize; u8* buffer = (u8*)efi_malloc(size);
                    if (buffer) { UINTN rsz = size; File->Read(File, &rsz, buffer); stbtt_InitFont(&global_font, buffer, stbtt_GetFontOffsetForIndex(buffer,0)); font_loaded = 1; }
                } File->Close(File);
            } Root->Close(Root);
        }
    }
}

void Alu_DrawString(int x, int y, const char* s, u32 color, int scale) {
    if (!font_loaded) return;
    int start_x = x; float fscale = stbtt_ScaleForPixelHeight(&global_font, 16.0f * scale);
    int asc, dsc, gap; stbtt_GetFontVMetrics(&global_font, &asc, &dsc, &gap); asc = (int)(asc * fscale);
    while (s && *s) {
        if (*s == '\n') { y += 20 * scale; x = start_x; s++; continue; }
        const char* next_s = s; u32 c = DecodeUTF8(&next_s); if (c == 0U) break; if (c == 32U) { x += 6 * scale; s = next_s; continue; }
        int adv, lsb, x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(&global_font, c, &adv, &lsb);
        stbtt_GetCodepointBitmapBox(&global_font, c, fscale, fscale, &x0, &y0, &x1, &y1);
        int bw = x1 - x0, bh = y1 - y0; u8* bmp = stbtt_GetCodepointBitmap(&global_font, 0, fscale, c, &bw, &bh, 0, 0);
        if (bmp) {
            for (int by=0; by<bh; by++) for (int bx=0; bx<bw; bx++) {
                u8 alpha = bmp[by * bw + bx];
                if (alpha > 0) Alu_DrawPixel(x + x0 + bx, y + asc + y0 + by, (u32)(alpha << 24) | (color & 0xFFFFFF));
            } efi_free(bmp);
        } x += (int)(adv * fscale); s = next_s;
    }
}

int Alu_Button(int x, int y, int w, int h, const char* text, u32 color) {
    int hov = (AluMouseX > (u32)x && AluMouseX < (u32)(x+w) && AluMouseY > (u32)y && AluMouseY < (u32)(y+h));
    u32 render_color = hov ? 0xFF42A5F5 : color; 
    Alu_DrawRoundedRect(x, y, w, h, 6, render_color); 
    int t_w = (int)efi_strlen(text) * 8; 
    Alu_DrawString(x + (w/2) - (t_w/2), y+(h/2)-8, text, COLOR_WHITE, 1);
    if (hov && AluMouseClick) return 1;
    return 0;
}

void Alu_ButtonCb(int x, int y, int w, int h, const char* text, u32 color, void (*callback)(void)) {
    if (Alu_Button(x, y, w, h, text, color)) { if (callback) callback(); }
}

void Alu_Slider(int x, int y, int w, int* val, int max_val) {
    int hov = (AluMouseX > (u32)(x-12) && AluMouseX < (u32)(x+w+12) && AluMouseY > (u32)(y-12) && AluMouseY < (u32)(y+12));
    if (hov && AluMouseDown) { *val = ((int)AluMouseX - x) * max_val / w; if(*val<0) *val=0; if(*val>max_val) *val=max_val; }
    int thx = x + (*val * w / max_val);
    Alu_DrawRoundedRect(x, y - 5, w, 10, 5, COLOR_LIGHT_BLUE);
    if (thx > x) Alu_DrawRoundedRect(x, y - 5, thx - x, 10, 5, COLOR_BLUE);
    Alu_DrawCircle(thx, y, 10, COLOR_BLUE);
}

void Alu_Window(int x, int y, int w, int h, const char* title) {
    Alu_DrawRoundedRect(x-4, y-4, w+8, h+8, 12, 0x22000000); 
    Alu_DrawRoundedRect(x, y, w, h, 12, COLOR_BG); 
    Alu_DrawRoundedRect(x, y, w, 40, 12, COLOR_GRAY); 
    Alu_DrawRect(x, y + 20, w, 20, COLOR_GRAY); 
    Alu_DrawString(x+15, y+12, title, COLOR_DARK, 1); 
}

void Alu_DrawCursor() {
    static const u8 cur[16][12] = {
        {1,0,0,0,0,0,0,0,0,0,0,0}, {1,1,0,0,0,0,0,0,0,0,0,0}, {1,2,1,0,0,0,0,0,0,0,0,0}, {1,2,2,1,0,0,0,0,0,0,0,0},
        {1,2,2,2,1,0,0,0,0,0,0,0}, {1,2,2,2,2,1,0,0,0,0,0,0}, {1,2,2,2,2,2,1,0,0,0,0,0}, {1,2,2,2,2,2,2,1,0,0,0,0},
        {1,2,2,2,2,2,2,2,1,0,0,0}, {1,2,2,2,2,2,2,2,2,1,0,0}, {1,2,2,2,2,2,2,2,2,2,1,0}, {1,2,2,2,2,2,2,1,1,1,1,1},
        {1,2,2,2,1,2,2,1,0,0,0,0}, {1,2,2,1,0,1,2,2,1,0,0,0}, {1,2,1,0,0,0,1,2,2,1,0,0}, {1,1,0,0,0,0,0,1,1,0,0,0}
    };
    for (int i=0; i<16; i++) {
        for (int j=0; j<12; j++) {
            if (cur[i][j] == 1) Alu_DrawPixel(AluMouseX+j, AluMouseY+i, COLOR_BLACK);
            else if (cur[i][j] == 2) Alu_DrawPixel(AluMouseX+j, AluMouseY+i, COLOR_WHITE);
        }
    }
}

EFI_FILE_PROTOCOL* Alu_GetRootDir() {
    EFI_GUID sfsp_guid = {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FS; EFI_FILE_PROTOCOL *Root = 0;
    if (g_bs && g_bs->LocateProtocol(&sfsp_guid, 0, (void**)&FS) == EFI_SUCCESS) {
        FS->OpenVolume(FS, &Root);
    } return Root;
}
void Alu_FileWrite(const char* path, const char* data) {
    EFI_FILE_PROTOCOL *Root = Alu_GetRootDir(); if(!Root) return;
    CHAR16 wp[256]; memset(wp, 0, 512); for(int i=0; i<256 && path[i]; i++) wp[i]=path[i];
    EFI_FILE_PROTOCOL *File;
    if (Root->Open(Root, &File, wp, EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) File->Delete(File);
    if (Root->Open(Root, &File, wp, EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE, 0) == EFI_SUCCESS) {
        UINTN sz = (UINTN)efi_strlen(data); File->Write(File, &sz, (void*)data); File->Close(File);
    } Root->Close(Root);
}
void Alu_FileRead(const char* path, char* buf, int max_len) {
    EFI_FILE_PROTOCOL *Root = Alu_GetRootDir(); if(!Root) return;
    CHAR16 wp[256]; memset(wp, 0, 512); for(int i=0; i<256 && path[i]; i++) wp[i]=path[i];
    EFI_FILE_PROTOCOL *File;
    if (Root->Open(Root, &File, wp, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        UINTN sz = (UINTN)(max_len - 1); File->Read(File, &sz, buf); buf[sz] = 0; File->Close(File);
    } Root->Close(Root);
}
void Alu_FileDelete(const char* path) {
    EFI_FILE_PROTOCOL *Root = Alu_GetRootDir(); if(!Root) return;
    CHAR16 wp[256]; memset(wp, 0, 512); for(int i=0; i<256 && path[i]; i++) wp[i]=path[i];
    EFI_FILE_PROTOCOL *File;
    if (Root->Open(Root, &File, wp, EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE, 0) == EFI_SUCCESS) File->Delete(File);
    Root->Close(Root);
}
void Alu_FileRename(const char* oldp, const char* newp) {
    char buf[4096]; memset(buf, 0, 4096); Alu_FileRead(oldp, buf, 4096);
    if (efi_strlen(buf) > 0) { Alu_FileWrite(newp, buf); Alu_FileDelete(oldp); }
}
void Alu_GetFileList(const char* path, char names[32][64], int is_dir[32], int* count) {
    *count = 0; EFI_FILE_PROTOCOL *Root = Alu_GetRootDir(); if(!Root) return;
    CHAR16 wp[256]; memset(wp, 0, 512); for(int i=0; i<256 && path[i]; i++) wp[i]=path[i];
    EFI_FILE_PROTOCOL *Dir;
    if (Root->Open(Root, &Dir, wp, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        u8 buffer[512];
        while (*count < 32) {
            UINTN rsz = sizeof(buffer); if (Dir->Read(Dir, &rsz, buffer) != EFI_SUCCESS || rsz == 0) break;
            EFI_FILE_INFO *info = (EFI_FILE_INFO*)buffer;
            if (info->FileName[0] == '.') continue; 
            int i=0; while(info->FileName[i] && i<63) { names[*count][i] = (char)info->FileName[i]; i++; } names[*count][i] = 0;
            is_dir[*count] = (info->Attribute & EFI_FILE_DIRECTORY) ? 1 : 0;
            (*count)++;
        } Dir->Close(Dir);
    } Root->Close(Root);
}

void Alu_DrawBMP(int x, int y, const char* path) {
    CHAR16 wp[256]; memset(wp, 0, 512); for(int i=0; i<256 && path[i]; i++) wp[i]=path[i];
    EFI_FILE_PROTOCOL *Root = Alu_GetRootDir(); if(!Root) return;
    EFI_FILE_PROTOCOL *File;
    if (Root->Open(Root, &File, wp, EFI_FILE_MODE_READ, 0) == EFI_SUCCESS) {
        u8 info_buf[512]; UINTN info_sz = 512; EFI_GUID InfoGuid = { 0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} };
        if (File->GetInfo(File, &InfoGuid, &info_sz, info_buf) == EFI_SUCCESS) {
            u32 sz = (u32)((EFI_FILE_INFO*)info_buf)->FileSize; u8* bmp = (u8*)efi_malloc(sz);
            if (bmp) {
                UINTN rsz = sz; File->Read(File, &rsz, bmp);
                if (bmp[0] == 'B' && bmp[1] == 'M') {
                    u32 off = bmp[10]|(bmp[11]<<8)|(bmp[12]<<16)|(bmp[13]<<24);
                    int w = bmp[18]|(bmp[19]<<8)|(bmp[20]<<16)|(bmp[21]<<24);
                    int h = bmp[22]|(bmp[23]<<8)|(bmp[24]<<16)|(bmp[25]<<24);
                    int bpp = bmp[28]|(bmp[29]<<8); int top_down = (h < 0); if(h < 0) h = -h;
                    if (bpp == 24 || bpp == 32) {
                        int r_sz = ((w * (bpp / 8)) + 3) & ~3;
                        for (int cy = 0; cy < h; cy++) {
                            int sy = top_down ? cy : (h - 1 - cy);
                            u8* row = &bmp[off + sy * r_sz];
                            for (int cx = 0; cx < w; cx++) {
                                u32 col = 0xFF000000 | (row[cx*(bpp/8)+2]<<16) | (row[cx*(bpp/8)+1]<<8) | row[cx*(bpp/8)];
                                Alu_DrawPixel(x + cx, y + cy, col);
                            }
                        }
                    }
                } efi_free(bmp);
            }
        } File->Close(File);
    } Root->Close(Root);
}

#endif // ALUOS_IMPLEMENTATION
#endif // ALUOS_H 
