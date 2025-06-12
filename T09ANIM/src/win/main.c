/* FILE NAME  : main.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              Startup entry-point module.
 */


#include "units/units.h"

#define WND_CLASS_NAME "window"

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  CONSOLE_FONT_INFOEX cfi = {0};

  SetDbgMemHooks();
  
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
                      "anim",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      NULL,
                      NULL,
                      hInstance,
                      NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);

  /* Units initialization */

  DH7_AnimAddUnit(DH7_UnitCreateBall());
  DH7_AnimAddUnit(DH7_UnitCreateCTRL());

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

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  INT W, H;
  

  static dh7PRIM Pr, PrCow;
  
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y += 100;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_CREATE:

    DH7_AnimInit(hWnd);
    
    SetTimer(hWnd, 0, 1, NULL);
    return 0;

  case WM_SIZE:

    H = HIWORD(lParam); //window sizes
    W = LOWORD(lParam);
    DH7_AnimResize(W, H);
    
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;

  case WM_MOUSEWHEEL:
    DH7_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DH7_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;

  case WM_TIMER:
    DH7_AnimRender();
    DH7_AnimCopyFrame();
    
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    DH7_AnimClose();
    KillTimer(hWnd, 0);
    fflush(stdout);
    FreeConsole();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MainWindowFunc' function */
 
/* END OF 'main.c' FILE *//* FILE NAME  : main.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              Startup entry-point module.
 */