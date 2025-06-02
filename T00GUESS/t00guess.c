/* Hmelevskiy Daniil */

#include <stdio.h>
#include <windows.h>

void main( void )
{
  int a = 0, b = 100, m, code;
  char buf[100];
  /*
  "Yes" - lesser
  "No" - larger
  "Cancel" - guessed
  */

  while (1)
  {
    m = (a + b) / 2;
    sprintf(buf, "i guess its: %i\n"
                 "'Yes' - smaller, 'No' - larger, 'Cancel' - guessed", m);
    code = MessageBox(NULL, buf, "num guesser", MB_YESNOCANCEL | MB_ICONQUESTION);
    if (code == IDCANCEL)
      break;
    else if (code == IDYES)
      b = m - 1;
    else
      a = m + 1;
  }
  sprintf(buf, "your number is: %i", m);
  MessageBox(NULL, buf, "answer", MB_OK | MB_ICONEXCLAMATION);


}