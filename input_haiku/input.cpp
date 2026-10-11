#include "platform.h"
#include "input.h"
#include "acme/constant/user_message.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/parallelization/task.h"
#include "aura/message/user.h"

extern "C" bool haiku_read_mouse_buttons(unsigned int * buttons);

namespace input_haiku
{
   ::e_status input::is_keyboard_message_handling_enabled(::user::interaction_base *)
   {
      return error_not_supported;
   }

   void input::__input_task()
   {
      unsigned int previous = 0;
      bool initialized = false;
      while (task_get_run())
      {
         unsigned int buttons = 0;
         if (haiku_read_mouse_buttons(&buttons))
         {
            if (initialized)
            {
               const unsigned int masks[] = {1, 2, 4};
               const ::user::enum_message down[] = {::user::e_message_left_button_down, ::user::e_message_right_button_down, ::user::e_message_middle_button_down};
               const ::user::enum_message up[] = {::user::e_message_left_button_up, ::user::e_message_right_button_up, ::user::e_message_middle_button_up};
               for (int i = 0; i < 3; ++i)
               {
                  if (!((previous ^ buttons) & masks[i])) continue;
                  auto message = create_newø<::message::mouse>();
                  message->m_eusermessage = (buttons & masks[i]) ? down[i] : up[i];
                  ::pointer_array<::particle> handlers;
                  {
                     synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
                     handlers = m_particleaMouseHandler;
                  }
                  for (auto & handler : handlers) handler->handle_message(message);
               }
            }
            previous = buttons;
            initialized = true;
         }
         preempt(5_ms);
      }
   }
}
