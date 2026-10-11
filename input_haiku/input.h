#pragma once
#include "apex/input/input.h"

namespace input_haiku
{
   class input : virtual public ::input::input
   {
   public:
      void __input_task() override;
      ::e_status is_keyboard_message_handling_enabled(::user::interaction_base *) override;
   };
}
