#define ALUOS_IMPLEMENTATION
#include "AluOS.h"

// ============================================
// 🎛️ UI 테스터용 전역 상태 변수
// ============================================
int win_x = 100, win_y = 50;
int win_w = 800, win_h = 600;
int is_dragging = 0, off_x = 0, off_y = 0;

int click_count = 0;
int slider_vol = 50;
int slider_bright = 80;
int toggle_state = 1;

char input_buffer[256] = "여기를 마우스로 누르고 입력하세요!";
int is_korean_input = 0;
float tick = 0.0f;

// 수학 함수 선언 (AluOS 내장 함수)
float efi_sin(float x);
float efi_cos(float x);

// ============================================
// ⚡ 이벤트 콜백 함수
// ============================================
void OnBtnPlus(void)  { click_count++; }
void OnBtnMinus(void) { click_count--; }
void OnBtnReset(void) { click_count = 0; }
void OnToggle(void)   { toggle_state = !toggle_state; }

// ============================================
// 🧊 3D 와이어프레임 렌더링 함수
// ============================================
void Draw3DCube(int cx, int cy, float size, float rot_x, float rot_y, u32 color) {
    float points[8][3] = { {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1}, {-1,-1,1}, {1,-1,1}, {1,1,1}, {-1,1,1} };
    int p2d[8][2];
    float sx = efi_sin(rot_x), cx_r = efi_cos(rot_x), sy = efi_sin(rot_y), cy_r = efi_cos(rot_y);
    
    for(int i=0; i<8; i++) {
        float x = points[i][0]*size, y = points[i][1]*size, z = points[i][2]*size;
        float xy = y*cx_r - z*sx, xz = y*sx + z*cx_r; y = xy; z = xz;
        float yx = x*cy_r + z*sy, yz = -x*sy + z*cy_r; x = yx; z = yz;
        float fov = 300.0f, proj = fov / (z + size*2.5f);
        p2d[i][0] = cx + (int)(x*proj); p2d[i][1] = cy + (int)(y*proj);
    }
    
    int edges[12][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
    for(int i=0; i<12; i++) Alu_DrawLine(p2d[edges[i][0]][0], p2d[edges[i][0]][1], p2d[edges[i][1]][0], p2d[edges[i][1]][1], color);
}

// ============================================
// 🚀 메인 OS 엔트리 포인트
// ============================================
#ifdef __cplusplus
extern "C"
#endif
EFI_STATUS EFIAPI EfiMain(EFI_HANDLE ImageHandle, EFI_ST *SystemTable) {
    
    Alu_Init(SystemTable, 1920, 1080);
    Alu_ChangeFont("\\EFI\\CarriOS\\Font\\Kr.ttf"); // 한글 폰트 로드

    while(1) {
        Alu_Update();
        
        // [F2] 키 입력으로 한/영 상태 전환
        if (AluKeyboardScanCode == SCAN_F2) is_korean_input = !is_korean_input;

        // 윈도우 창 드래그 처리 로직
        if (AluMouseClick && AluMouseX > (u32)win_x && AluMouseX < (u32)(win_x + win_w - 45) && 
            AluMouseY > (u32)win_y && AluMouseY < (u32)(win_y + 40)) {
            is_dragging = 1; off_x = AluMouseX - win_x; off_y = AluMouseY - win_y;
        }
        if (!AluMouseDown) is_dragging = 0;
        if (is_dragging) { win_x = AluMouseX - off_x; win_y = AluMouseY - off_y; }

        // ==========================================
        // 🎨 UI 렌더링 시작
        // ==========================================
        Alu_Begin(0xFFECEFF1); // 차분한 블루그레이 배경

        // 메인 윈도우 생성
        Alu_Window(win_x, win_y, win_w, win_h, "AluOS Native Widgets Showcase (C API)");
        Alu_DrawRect(win_x+5, win_y+40, win_w-10, win_h-45, COLOR_WHITE); // 내부 흰색 캔버스

        int cx1 = win_x + 25;  // 1단(좌측) 여백
        int cx2 = win_x + 400; // 2단(우측) 여백

        // ------------------------------------------
        // 섹션 1: Typography (텍스트 렌더링)
        // ------------------------------------------
        Alu_DrawString(cx1, win_y + 60, "1. Typography (글꼴 출력)", COLOR_DARK, 2);
        Alu_DrawRect(cx1, win_y + 100, 340, 2, COLOR_GRAY);

        Alu_DrawString(cx1, win_y + 115, "스케일(Scale) 1배율 텍스트입니다.", COLOR_GRAY, 1);
        Alu_DrawString(cx1, win_y + 145, "글씨 색상을 마음대로 바꿀 수 있습니다.", COLOR_BLUE, 1);
        Alu_DrawString(cx1, win_y + 175, "AluOS는 STB_TrueType을 사용합니다.", 0xFFE91E63, 1);

        // ------------------------------------------
        // 섹션 2: Shapes & Primitives (도형 그리기)
        // ------------------------------------------
        Alu_DrawString(cx2, win_y + 60, "2. Shapes (도형과 선)", COLOR_DARK, 2);
        Alu_DrawRect(cx2, win_y + 100, 340, 2, COLOR_GRAY);

        Alu_DrawRect(cx2, win_y + 120, 50, 50, 0xFFFF5722); // 네모
        Alu_DrawRoundedRect(cx2 + 70, win_y + 120, 50, 50, 15, 0xFF9C27B0); // 둥근 네모
        Alu_DrawCircle(cx2 + 165, win_y + 145, 25, 0xFF4CAF50); // 동그라미
        
        // 대각선 (X 표시)
        Alu_DrawLine(cx2 + 220, win_y + 120, cx2 + 270, win_y + 170, COLOR_BLACK);
        Alu_DrawLine(cx2 + 270, win_y + 120, cx2 + 220, win_y + 170, COLOR_BLACK);

        // ------------------------------------------
        // 섹션 3: Interactive Controls (입력 위젯)
        // ------------------------------------------
        Alu_DrawString(cx1, win_y + 230, "3. Controls (버튼 및 슬라이더)", COLOR_DARK, 2);
        Alu_DrawRect(cx1, win_y + 270, 340, 2, COLOR_GRAY);

        // 버튼들
        Alu_ButtonCb(cx1, win_y + 290, 90, 35, "+ Plus", COLOR_BLUE, OnBtnPlus);
        Alu_ButtonCb(cx1 + 100, win_y + 290, 90, 35, "- Minus", COLOR_GRAY, OnBtnMinus);
        Alu_ButtonCb(cx1 + 200, win_y + 290, 90, 35, "Reset", COLOR_PINK, OnBtnReset);

        char cnt_buf[32]; IntToString(click_count, cnt_buf);
        Alu_DrawString(cx1 + 305, win_y + 298, cnt_buf, COLOR_DARK, 1);

        // 슬라이더 바
        Alu_DrawString(cx1, win_y + 345, "볼륨 조절:", COLOR_GRAY, 1);
        Alu_Slider(cx1 + 80, win_y + 350, 250, &slider_vol, 100);

        Alu_DrawString(cx1, win_y + 385, "밝기 조절:", COLOR_GRAY, 1);
        Alu_Slider(cx1 + 80, win_y + 390, 250, &slider_bright, 100);

        // ------------------------------------------
        // 섹션 4: Text Input (한/영 텍스트 박스)
        // ------------------------------------------
        Alu_DrawString(cx2, win_y + 230, "4. Textbox (문자 오토마타)", COLOR_DARK, 2);
        Alu_DrawRect(cx2, win_y + 270, 340, 2, COLOR_GRAY);

        int tb_hover = (AluMouseX > (u32)cx2 && AluMouseX < (u32)(cx2+340) && AluMouseY > (u32)(win_y+290) && AluMouseY < (u32)(win_y+330));
        
        // 텍스트 박스 배경 및 밑줄
        Alu_DrawRoundedRect(cx2, win_y + 290, 340, 40, 6, tb_hover ? 0xFFE3F2FD : 0xFFF5F5F5);
        Alu_DrawRect(cx2, win_y + 328, 340, 2, tb_hover ? COLOR_BLUE : COLOR_GRAY); 
        Alu_DrawString(cx2 + 10, win_y + 300, input_buffer, COLOR_DARK, 1);

        // 마우스가 박스 위에 있을 때 키보드 입력 허용
        if (tb_hover) Alu_ProcessInput(input_buffer, 255, &is_korean_input);

        // 커스텀 토글 스위치 (직접 그리기)
        Alu_DrawString(cx2, win_y + 355, "토글 스위치 (직접 구현):", COLOR_GRAY, 1);
        Alu_ButtonCb(cx2 + 200, win_y + 345, 80, 35, toggle_state ? "ON" : "OFF", toggle_state ? COLOR_GREEN : COLOR_GRAY, OnToggle);

        // ------------------------------------------
        // 섹션 5: 3D Engine (소프트웨어 실시간 렌더링)
        // ------------------------------------------
        Alu_DrawString(cx1, win_y + 450, "5. 3D Renderer (CPU 실시간 렌더링)", COLOR_DARK, 2);
        Alu_DrawRect(cx1, win_y + 490, 715, 2, COLOR_GRAY);

        // 3D 모델 실시간 애니메이션 (tick 시간축 연동)
        Draw3DCube(cx1 + 100, win_y + 540, 30.0f, tick * 1.5f, tick * 2.0f, COLOR_BLUE);
        Draw3DCube(cx1 + 350, win_y + 540, 40.0f, tick * 1.0f, tick * 1.8f, COLOR_PINK);
        Draw3DCube(cx1 + 600, win_y + 540, 35.0f, tick * 2.2f, tick * 1.2f, COLOR_GREEN);

        tick += 0.03f; // 시간 진행

        // 커서 그리기 및 버퍼 플러시
        Alu_DrawCursor();
        Alu_End();
    }
    return 0;
} 
