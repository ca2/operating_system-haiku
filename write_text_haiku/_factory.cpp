#include "acme/_start.h"
#include "aura/_.h"
#include "aura/graphics/write_text/write_text.h"
#include "aura/graphics/write_text/fonts.h"
#include "aura/graphics/write_text/font_enumeration.h"
#include "aura/graphics/write_text/font_enumeration_item.h"

extern "C" void haiku_enumerate_font_families(void *, void (*)(void *, const char *));
namespace write_text_haiku
{
class font_enumeration : virtual public ::write_text::font_enumeration
{
public:
   void on_enumerate_fonts() override
   {
      defer_construct_newø(m_pfontenumerationitema);
      m_pfontenumerationitema->erase_all();
      haiku_enumerate_font_families(this, [](void *context, const char *family)
      {
         auto self = static_cast<font_enumeration *>(context);
         auto item = allocateø ::write_text::font_enumeration_item();
         item->m_strName = family;
         self->m_pfontenumerationitema->add(item);
      });
   }
};
}
IMPLEMENT_FACTORY(write_text_haiku)
{
   pfactory->add_factory_item<::write_text::fonts>();
   pfactory->add_factory_item<::write_text::write_text>();
   pfactory->add_factory_item<::write_text_haiku::font_enumeration, ::write_text::font_enumeration>();
}
