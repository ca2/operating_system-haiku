#include <Font.h>
extern "C" void haiku_enumerate_font_families(void *context, void (*add)(void *, const char *))
{
   for (int32 i = 0; i < count_font_families(); ++i)
   {
      font_family family;
      if (get_font_family(i, &family) == B_OK)
         add(context, family);
   }
}
