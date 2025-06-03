/* FILE NAME  : t03clocks.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 03.06.2025
 * PURPOSE    : Animated clocks showing real time.
 *              Startup entry-point module.
 */

#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "my window"
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );


static INT Mx, My;


VOID DrawHand( HDC hDC )
{            
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  
};

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
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

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
                      "first window",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      NULL,
                      NULL,
                      hInstance,
                      NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
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
  HBRUSH hBr;
  POINT pt;
  static HDC hMemDC, hMemDCclock;
  static HBITMAP hBm, hBmClock; //bm for sizes
  static INT W, H;
  static BITMAP bm;
  

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCclock = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmClock = LoadImage(NULL, "kirbyclock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SetStretchBltMode(hDC, COLORONCOLOR);

    SetTimer(hWnd, 0, 0.01, NULL);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    return 0;

  case WM_PAINT:
    GetObject(hBm, sizeof(BITMAP), &bm);
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hMemDCclock, hBmClock);
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCclock, 0, 0, SRCCOPY);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_MOUSEMOVE:
    return 0;
  case WM_TIMER:
    //DrawHand
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmClock);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCclock);
    KillTimer(hWnd, 0);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MainWindowFunc' function */
 
/* END OF 't03clocks.c' FILE *//* FILE NAME  : t02eyes.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 03.06.2025
 * PURPOSE    : First windowed application project.
 *              Startup entry-point module.
 */