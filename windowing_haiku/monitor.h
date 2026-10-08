#pragma once
#include "aura/windowing/monitor.h"
#include "acme_windowing_haiku/monitor.h"
namespace windowing_haiku {
class monitor : virtual public ::windowing::monitor, public ::haiku::acme::windowing::monitor {
public:
 void initialize_monitor(::windowing::display *,int) override;
 void update_cache() override;
 ::i32_rectangle monitor_rectangle() override;
 ::i32_rectangle _workspace_rectangle() override;
};
}
