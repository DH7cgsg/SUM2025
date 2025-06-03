/* FILE NAME  : t01fwin.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 02.06.2025
 * PURPOSE    : First windowed application project.
 *              Startup entry-point module.
 */

#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "my window"
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );
#define _WIN32_WININT 0x0500

static INT Mx, My;





VOID DrawEye( HDC hDC, int Xc, INT Yc, int R, INT R1, int Mx, int My )
{            
  INT Xcp, Ycp;
  double len, k;

  Ellipse(hDC, Xc - R, Yc + R, Xc + R, Yc - R);
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  //coordinates computation
  len = sqrt((Mx - Xc) * (Mx - Xc) + (My - Yc) * (My - Yc));
  if (len != 0)
    k = (R - R1) / len;
  if (len < (R - R1))
  {
    Xcp = Mx;
    Ycp = My;
  } 
  else
  {
    Xcp = Xc + (Mx - Xc) * k;
    Ycp = Yc + (My - Yc) * k;
  }
  Ellipse(hDC, Xcp - R1, Ycp + R1, Xcp + R1, Ycp - R1);
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  

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
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT W, H;
  static INT ix, iy; 

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
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
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_MOUSEMOVE:
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    Mx = pt.x, My = pt.y;
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Rectangle(hMemDC, 0, 0, W, H);
    for (ix = 100; ix <= 1500; ix += 100)
    {
      for (iy = 100; iy <= 600; iy += 100)
        DrawEye(hMemDC, ix, iy, 50, 20, Mx, My);
    }
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 0);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* End of 'MainWindowFunc' function */
 
/* END OF 't02eyes.c' FILE *//* FILE NAME  : t02eyes.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 02.06.2025
 * PURPOSE    : First windowed application project.
 *              Startup entry-point module.
 */

