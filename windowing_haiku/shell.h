#pragma once
#include "_.h"
#include "aura/user/user/shell.h"
namespace windowing_haiku {
class CLASS_DECL_WINDOWING_HAIKU shell : virtual public ::user::shell {
public:
   shell();
   bool m_iconLoading = false;
   image_key m_loadingKey;
   using ::user::shell::get_file_image;
   ::i32 get_file_image(const image_key &) override;
   bool _get_file_image(_get_file_image_ &) override;
   bool defer_get_file_image_by_file_image(_get_file_image_ &) override;
   bool get_image_by_file_extension(_get_file_image_ &) override;
   enum_folder get_folder_type(::particle *, const ::scoped_string &) override;
};
}
