#include "platform.h"
#include "input.h"
#include "acme/constant/user_message.h"
#include "acme/constant/user_key.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/parallelization/task.h"
#include "aura/message/user.h"

extern "C" bool haiku_read_mouse_buttons(unsigned int * buttons);
extern "C" bool haiku_read_keyboard(unsigned char * states);
extern "C" int haiku_key_kind(int code);

namespace input_haiku
{
   ::e_status input::is_keyboard_message_handling_enabled(::user::interaction_base *)
   {
      return ::success;
   }

   void input::__input_task()
   {
      unsigned int previous = 0;
      bool initialized = false;
      unsigned char previousKeys[16] = {};
      bool keysInitialized = false;
      while (task_get_run())
      {
         ::pointer_array<::particle> keyboardHandlers;
         bool mouseEnabled;
         {
            synchronous_lock lock(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);
            keyboardHandlers = m_particleaKeyboardHandler;
            mouseEnabled = m_particleaMouseHandler.has_element();
         }
         unsigned char keys[16] = {};
         if (keyboardHandlers.has_element() && haiku_read_keyboard(keys))
         {
            if (keysInitialized)
            {
               for (int code = 0; code < 128; ++code)
               {
                  unsigned int mask = 0x80u >> (code % 8);
                  if (!((keys[code / 8] ^ previousKeys[code / 8]) & mask)) continue;
                  auto message = create_newø<::message::key>();
                  message->m_eusermessage = (keys[code / 8] & mask) ? ::user::e_message_key_down : ::user::e_message_key_up;
                  message->m_nScanCode = code;
                  auto kind = haiku_key_kind(code);
                  message->m_ekey = kind == 1 ? ::user::e_key_return : kind == 2 ? ::user::e_key_space : ::user::e_key_none;
                  for (auto & handler : keyboardHandlers) handler->handle_message(message);
               }
            }
            ::memory_copy(previousKeys, keys, sizeof(keys));
            keysInitialized = true;
         }
         else if (keyboardHandlers.is_empty()) keysInitialized = false;
         unsigned int buttons = 0;
         if (mouseEnabled && haiku_read_mouse_buttons(&buttons))
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
         else if (!mouseEnabled) initialized = false;
         preempt(5_ms);
      }
   }
}
