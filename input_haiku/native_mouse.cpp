#include <InterfaceDefs.h>
#include <Point.h>

extern "C" bool haiku_read_mouse_buttons(unsigned int * buttons)
{
   BPoint point;
   uint32 state = 0;
   if (get_mouse(&point, &state) != B_OK) return false;
   *buttons = state;
   return true;
}
