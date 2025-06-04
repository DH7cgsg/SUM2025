/* FILE NAME  : t03clocks.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 03.06.2025
 * PURPOSE    : Animated clocks showing real time.
 *              Startup entry-point module.
 */

#include <windows.h>
#include <math.h>
#include <time.h>
#define WND_CLASS_NAME "clock window"
#define DOUBLE double
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                              WPARAM wParam, LPARAM lParam );


static INT Mx, My;


VOID DrawHand( HDC hDC, INT Xc, INT Yc, DOUBLE len, DOUBLE time, INT wPen, COLORREF color, INT W, INT L)
{            
  DOUBLE pi = 3.1415;
  HPEN hPen, hOldPen;
  HBRUSH hBr;
  DOUBLE angle;
  DOUBLE si, co;
  INT i;

  POINT points[] = {
    {0, L}, {W, 0}, {0, -W}, {-W, 0}
  };
  POINT points1[sizeof(points) / sizeof(points[0])];

  hPen = CreatePen(PS_SOLID, wPen, color);
  hOldPen = SelectObject(hDC, hPen);
  angle = time * 2 * pi / 360;
  si = sin(angle);
  co = cos(angle);
  //MoveToEx(hDC, (DOUBLE)Xc + (DOUBLE)len * sin(angle), (DOUBLE)Yc - (DOUBLE)len * cos(angle), NULL);
  //LineTo(hDC, Xc, Yc);
  for (i = 0; i < sizeof(points) / sizeof(points[0]); i++)
  {
    points1[i].x = Xc + points[i].x * co + points[i].y * si;
    points1[i].y = Yc - points[i].y * co - points[i].x * si;
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, color);

  Polygon(hDC, points1, sizeof(points1) / sizeof(points[0]));


  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);

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
                      "clock",
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
  static HDC hMemDC, hMemDCclock, hMemDCAND, hMemDCXOR;
  static HBITMAP hBm, hBmClock, hBmAND, hBmXOR; 
  static INT W, H;
  static BITMAP bm, bmsprite; //bm for sizes
  static INT size;
  static SYSTEMTIME st;
  static DOUBLE t; //for clock
  static HFONT hFnt, hFntOld;
  static CHAR buff[100];
  static RECT rc;
  static CHAR *WD[] =
  {
    "бя", "ом", "бр", "яп", "вр", "ор", "яа"
  };
  INT i;
  INT k;
  

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCclock = CreateCompatibleDC(hDC);
    hMemDCAND = CreateCompatibleDC(hDC);
    hMemDCXOR = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmClock = LoadImage(NULL, "kirbyclock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAND = LoadImage(NULL, "ANDmask2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXOR = LoadImage(NULL, "XORmask2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SetStretchBltMode(hMemDC, COLORONCOLOR); //mode for stretch 
    SelectObject(hMemDCclock, hBmClock);
    SelectObject(hMemDCAND, hBmAND);
    SelectObject(hMemDCXOR, hBmXOR);    //only once

    SetTextColor(hMemDC, RGB(30, 90, 180));
    SetBkColor(hMemDC, RGB(220, 220, 180));
    SetBkMode(hMemDC, OPAQUE);   //text out settings
    rc.left = 20;
    rc.top = 300;
    rc.right = 320;
    rc.bottom = 800;

    SetTimer(hWnd, 0, 1, NULL);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam); //window
    W = LOWORD(lParam);
    size = (W < H ? W : H);
    
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    return 0;

  case WM_PAINT:
    GetObject(hBmClock, sizeof(BITMAP), &bm);
    GetObject(hBmAND, sizeof(BITMAP), &bmsprite);  //width height pic
    hDC = BeginPaint(hWnd, &ps);
    GetLocalTime(&st);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W + 1, H + 1);

    StretchBlt(hMemDC, (W - size) / 2, (H - size) / 2, size, size, hMemDCclock, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    DrawHand(hMemDC, W / 2, H / 2, size * 0.4, (st.wSecond + st.wMilliseconds / 1000) * 360 / 60, 2, RGB(176, 0, 81), 5, size * 0.435); //seconds 
    DrawHand(hMemDC, W / 2, H / 2, size * 0.425, (st.wMinute + st.wSecond / 60) * 360 / 60, 2, RGB(0, 144, 158), 7, size * 0.4); //minutes
    DrawHand(hMemDC, W / 2, H / 2, size * 0.35, (st.wHour % 12 + st.wMinute / 60) * 360 / 12, 2, RGB(250, 157, 12), 10, size * 0.35); //hours
    
    t = (DOUBLE)clock() / CLOCKS_PER_SEC;
    for (i = 0; i < 20; i++)
    {
      BitBlt(hMemDC, 1600 + 150 * sin(t - i), H / 2 + 150 * cos(2 * t - i), bmsprite.bmWidth, bmsprite.bmHeight, hMemDCAND, 0, 0, SRCAND);
      BitBlt(hMemDC, 1600 + 150 * sin(t - i), H / 2 + 150 * cos(2 * t - i), bmsprite.bmWidth, bmsprite.bmHeight, hMemDCXOR, 0, 0, SRCINVERT);
    }

    hFntOld = SelectObject(hMemDC, hFnt);
    hFnt = CreateFont(200, 100, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      RUSSIAN_CHARSET,
      OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "Comis Sans");

    GetLocalTime(&st);
    k = wsprintf(buff, "%02i.%02i.%02i.%02i.%02i %s", st.wHour, st.wMinute, st.wDay, st.wMonth, st.wYear, WD[st.wDayOfWeek]);
    
    DrawText(hMemDC, buff, k, &rc, DT_CENTER | DT_SINGLELINE);

    SelectObject(hMemDC, hFntOld);
    DeleteObject(hFnt);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
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