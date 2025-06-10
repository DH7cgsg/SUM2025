/* FILE NAME  : def.h
 * PROGRAMMER : DH7
 * LAST UPDATE: 09.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */
 
#ifndef __def_h_
#define __def_h_
 
/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>

#include "mth/mth.h"


 
#endif /* __def_h_ */
                          
/* END OF 'def.h' FILE */