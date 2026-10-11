#include <InterfaceDefs.h>
#include <Point.h>
#include <cstdlib>
#include <cstring>

extern "C" bool haiku_read_mouse_buttons(unsigned int * buttons)
{
   BPoint point;
   uint32 state = 0;
   if (get_mouse(&point, &state) != B_OK) return false;
   *buttons = state;
   return true;
}

extern "C" bool haiku_read_keyboard(unsigned char * states)
{
   key_info info;
   if (get_key_info(&info) != B_OK) return false;
   memcpy(states, info.key_states, sizeof(info.key_states));
   return true;
}

extern "C" int haiku_key_kind(int code)
{
   if (code < 0 || code >= 128) return 0;
   key_map * map = nullptr;
   char * buffer = nullptr;
   get_key_map(&map, &buffer);
   int kind = 0;
   if (map && buffer)
   {
      auto offset = map->normal_map[code];
      if (offset >= 0 && buffer[offset] == 1)
      {
         if (buffer[offset + 1] == B_ENTER) kind = 1;
         else if (buffer[offset + 1] == B_SPACE) kind = 2;
      }
   }
   free(map);
   free(buffer);
   return kind;
}
