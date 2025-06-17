/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */


#include "rnd.h"
#include <wglew.h>
#include <gl/wglext.h>
 
#pragma comment(lib, "opengl32")

static COLORREF color = RGB(255, 0, 255);

/* Base render functions */

VOID DH7_RndInit( HWND hWnd )
{
  INT i, num;
  HGLRC hRC;
  PIXELFORMATDESCRIPTOR pfd = {0};
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  /* Store window handle */
  DH7_hRndWnd = hWnd;
  /* Get window DC */
  DH7_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DH7_hRndDC, &pfd);
  DescribePixelFormat(DH7_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(DH7_hRndDC, i, &pfd);
 
  /* OpenGL init: setup rendering context */
  DH7_hRndGLRC = wglCreateContext(DH7_hRndDC);
  wglMakeCurrent(DH7_hRndDC, DH7_hRndGLRC);
 
  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(DH7_hRndWnd, "Error extensions initializing", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }
  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(DH7_hRndDC, PixelAttribs, NULL, 1, &i, &num);
  hRC = wglCreateContextAttribsARB(DH7_hRndDC, NULL, ContextAttribs);
 
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DH7_hRndGLRC);
 
  DH7_hRndGLRC = hRC;
  wglMakeCurrent(DH7_hRndDC, DH7_hRndGLRC);

  #ifndef NDEBUG
  OutputDebugString(glGetString(GL_VERSION));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_VENDOR));
  OutputDebugString("\n");
  OutputDebugString(glGetString(GL_RENDERER));
  OutputDebugString("\n");
 
  printf("Version : %s\n", glGetString(GL_VERSION));
  printf("Vendor  : %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  #endif /* NDEBUG */

  #ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, NULL);
  #endif /* NDEBUG */
  

  wglSwapIntervalEXT(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPrimitiveRestartIndex(-1);

  /* Set default render parameters */
  DH7_RndResize(100, 100);
  DH7_RndCamLoc = VecSet1(11);
  DH7_RndCamAt = VecSet(0, 2, 0);
  DH7_RndCamUp = VecSet(0, 1, 0);
  DH7_RndCamSet(DH7_RndCamLoc, DH7_RndCamAt, DH7_RndCamUp);
  DH7_RndCamDir = VecSet(-DH7_RndMatrView.A[0][2],
                         -DH7_RndMatrView.A[1][2],
                         -DH7_RndMatrView.A[2][2]);
  DH7_RndCamRight = VecSet(DH7_RndMatrView.A[0][0],
                           DH7_RndMatrView.A[1][0],
                           DH7_RndMatrView.A[2][0]);                                             
  DH7_RndResInit();
}

VOID DH7_RndClose( VOID )
{
  DH7_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DH7_hRndGLRC);
  ReleaseDC(DH7_hRndWnd, DH7_hRndDC);
}

VOID DH7_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

   /* Setup projection */
  DH7_RndFrameW = W;
  DH7_RndFrameH = H;
  DH7_RndProjSet();
}
VOID DH7_RndCopyFrame()
{
  SwapBuffers(DH7_hRndDC);  
}

VOID DH7_RndStart( VOID )
{
  FLT ClearColor[4] = {0.30, 0.47, 0.8, 1};
  FLT DepthClearValue = 1;
 
  DH7_RndShdUpdate();

  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, ClearColor);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);

  
}

VOID DH7_RndEnd( VOID )
{
  glFinish();
}

/* Camera/projaction functions */

VOID DH7_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = DH7_RndProjSize;
  /* Correct aspect ratio */
  if (DH7_RndFrameW > DH7_RndFrameH)
    rx *= (DBL)DH7_RndFrameW / DH7_RndFrameH;
  else
    ry *= (DBL)DH7_RndFrameH / DH7_RndFrameW;
 
  DH7_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      DH7_RndProjDist, DH7_RndProjFarClip);
  DH7_RndMatrVP = MatrMulMatr(DH7_RndMatrView, DH7_RndMatrProj);
}
VOID DH7_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  DH7_RndMatrView = MatrView(Loc, At, Up);
  DH7_RndMatrVP = MatrMulMatr(DH7_RndMatrView, DH7_RndMatrProj);
  DH7_RndCamLoc = Loc;
  DH7_RndCamAt = At;
  DH7_RndCamUp = Up;
  DH7_RndCamDir = VecSet(-DH7_RndMatrView.A[0][2],
                         -DH7_RndMatrView.A[1][2],
                         -DH7_RndMatrView.A[2][2]);
  DH7_RndCamRight = VecSet(DH7_RndMatrView.A[0][0],
                           DH7_RndMatrView.A[1][0],
                           DH7_RndMatrView.A[2][0]);
}
