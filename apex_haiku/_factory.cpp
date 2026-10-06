#include "platform.h"
#include "apex/platform/launcher.h"
#include "launcher.h"
//#include "os_context.h"
//#include "ip_enum.h"
//#include "interprocess_communication.h"
#include "service_handler.h"
#include "node.h"
#include "fen/watch.h"
#include "apex/parallelization/service.h"
#include "apex/parallelization/service_handler.h"
#include "service_handler.h"


DECLARE_FACTORY(acme_haiku);
DECLARE_FACTORY(apex_posix);


IMPLEMENT_FACTORY(apex_haiku)
{

   // Register the POSIX Apex services, including System V interprocess communication.
   apex_posix_factory(pfactory);

   acme_haiku_factory(pfactory);


   //add_factory_item < ::haiku::stdio_file, ::file::text_file >();
   //add_factory_item < ::haiku::file, ::file::file >();
   //pfactory->add_factory_item < ::apex_haiku::os_context, ::os_context >();
   //pfactory->add_factory_item < ::haiku::pipe, ::process::pipe >();
   //pfactory->add_factory_item < ::haiku::process, ::process::process >();

   //add_factory_item < ::haiku::console, ::console::console >();
   //pfactory->add_factory_item < ::haiku::crypto, ::crypto::crypto >();
   //pfactory->add_factory_item < ::apex_haiku::ip_enum, ::networking::ip_enum >();


   //pfactory->add_factory_item < ::apex_haiku::interprocess_communication_base, ::interprocess_communication::base >();
   //pfactory->add_factory_item < ::apex_haiku::interprocess_communication_rx, ::interprocess_communication::rx >();
   //pfactory->add_factory_item < ::apex_haiku::interprocess_communication_tx, ::interprocess_communication::tx >();
   //add_factory_item < ::haiku::interprocess_communication, ::interprocess_communication::interprocess_communication >();


   //add_factory_item < ::haiku::buffer, ::graphics::graphics >();
   //add_factory_item < ::haiku::interaction_impl, ::user::interaction_impl >();

   //pfactory->add_factory_item < ::file::os_watcher, ::file::watcher >();
   pfactory->add_factory_item<::apex_haiku::fen::watcher, ::file::watcher>();
   pfactory->add_factory_item<::apex_haiku::fen::watch, ::file::watch>();
   //pfactory->add_factory_item < ::file::os_watch, ::file::watch >();

   //pfactory->add_factory_item < ::apex_haiku::file_context, ::file_context >();
   pfactory->add_factory_item < ::apex_haiku::service_handler, ::service_handler >();

   pfactory->add_factory_item < ::apex_haiku::node, ::platform::node >();

   //add_factory_item < ::haiku::copydesk, ::user::cop
   // 
   // 
   // ydesk >();
   ////add_factory_item < ::haiku::shell, ::user::shell >();


}




