/* FILE NAME  : t07globe.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 07.06.2025
 * PURPOSE    : 3d globe
 *              Startup entry-point module.
 */
  
#include <windows.h>
#include <math.h>
#include <time.h>
#include "globe.h"
#include "mth.h"
#include "timer.h"
#include <stdio.h>
#define WND_CLASS_NAME "clock window"
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );




/* Main startup program function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstance;
 *   - previouse application instance handle
 *     (not used, alway NULL):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - command line window show parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) system error level value.
 */

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRc;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    GetWindowRect(hWnd, &SaveRc);
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    rc  = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom + 201, SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else 
  {
    SetWindowPos(hWnd, HWND_TOP, SaveRc.left, SaveRc.top, SaveRc.right - SaveRc.left, 
                                              SaveRc.bottom - SaveRc.top, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;

  }


}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /*MATR m1 = MatrSet(1, 2, 3, 4,
                    5, 6, 7, 8,
                    9, 10, 11, 12,
                    13, 14, 15, 16);
  MATR m2 = MatrSet(3, 3, 2, 1,
                    0, 2, 5, 6,
                    0, 0, 1, 3,
                    0, 0, 0, 7); */


  CONSOLE_FONT_INFOEX cfi = {0};
 
  AllocConsole();
 
  SetConsoleTitle("debug");
 
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 30;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 255, 255, 0, 0, 0, 0);
  


  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbWndExtra = 0;
  wc.cbClsExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_MENUTEXT;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "error trying to register window class", "ERROR", MB_OK);
    return 0;
  }
  hWnd = CreateWindow(WND_CLASS_NAME,
                      "globe",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      NULL,
                      NULL,
                      hInstance,
                      NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  

   /* Message loop */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 47, 0);
  }
  return msg.wParam;


} 
/* End of 'WinMain' function */
 
 
/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message type (see WM_***):
 *       UINT Msg;
 *   - message 'word' parameter:
 *       WPARAM wParam;
 *   - message 'long' parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT W, H; //window sizes
  static BITMAP bm; //bm for sizes
  
  

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    GLB_Init(1.0);
    GLB_TimerInit();
    SetTimer(hWnd, 0, 1, NULL);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam); //window sizes
    W = LOWORD(lParam);

    GLB_Resize(W, H);
    
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);

    SendMessage(hWnd, WM_TIMER, 47, 0);
    SelectObject(hMemDC, hBm);
    return 0;

  case WM_PAINT:
    //GetObject(hBmClock, sizeof(BITMAP), &bm);
    //GetObject(hBmAND, sizeof(BITMAP), &bmsprite);  //width height pic
    hDC = BeginPaint(hWnd, &ps);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 144, 158));
    Rectangle(hMemDC, 0, 0, W + 1, H + 1);
    GLB_Draw(hMemDC, RGB(176, 0, 81), 3, 30);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_F11)
      FlipFullScreen(hWnd);
    /* if (wParam == VK_F1)
    {
      ChangeParam(1, 3);
      printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 0, 255, 0, 0, 0);
      printf("log : Distancing by 1....\n");
    }
    if (wParam == VK_F2)
    {
      ChangeParam(-1, 3);
      printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 0, 255, 0, 0, 0);
      printf("log : Zooming in by 1....\n");
    } */
    if (wParam == VK_F3)
    {
      
      printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 255, 0, 0, 0, 0, 0);
      if (!GetParam(0))
      {
        printf("log : Random colors enabled....\n");
        ChangeParam(1, 0);
      }
      else
      {
        printf("log : Random colors disabled....\n");
        ChangeParam(0, 0);
      }
    }
    if (wParam == VK_F4)
    {
      printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 255, 0, 0, 0, 0);
      if (!GetParam(1))
      {
        printf("log : Form has been changed to shrinked....\n");
        ChangeParam(1, 1);
      }
      else
      {
        printf("log : Form has been changed to default....\n");
        ChangeParam(0, 1);
      }
    }
    if (wParam == VK_F5)
    {
      printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 255, 0, 255, 0, 0, 0);
      if (!GetParam(2))
      {
        printf("log : Lines have been enabled....\n");
        ChangeParam(1, 2);
      }
      else
      {
        printf("log : Lines have been disabled....\n");
        ChangeParam(0, 2);
      }
    }
  case WM_TIMER:
    GLB_TimerResponse();
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 0);
    fflush(stdout);
    FreeConsole();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MainWindowFunc' function */
 
/* END OF 't07globe.c' FILE *//* FILE NAME  : t07globe.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 06.06.2025
 * PURPOSE    : 3d globe
 *              Startup entry-point module.
 */