#include "native.h"
#include <Looper.h>
#include <Message.h>
#include <Messenger.h>
#include <NodeMonitor.h>
#include <OS.h>
#include <sys/stat.h>
#include <errno.h>
#include <map>
#include <mutex>
#include <new>

namespace {
class monitor : public BLooper {
public:
 sem_id changed;
 monitor(sem_id s) : BLooper("ca2-node-monitor"), changed(s) {}
 void MessageReceived(BMessage *m) override {
  if(m->what!=B_NODE_MONITOR){BLooper::MessageReceived(m);return;}
  int32 opcode=0;m->FindInt32("opcode",&opcode);
  if(opcode==B_STAT_CHANGED){int32 fields=0;if(m->FindInt32("fields",&fields)==B_OK && (fields&~B_STAT_ACCESS_TIME)==0)return;}
  release_sem(changed);
 }
};
struct entry { node_ref ref{}; bool watching=false; };
std::mutex registry_mutex;
std::map<int,monitor *> monitors;
monitor *lookup(int id){std::lock_guard<std::mutex> guard(registry_mutex);auto i=monitors.find(id);return i==monitors.end()?nullptr:i->second;}
int error(status_t s){if(s==B_OK)return 0;if(s==B_ENTRY_NOT_FOUND)return ENOENT;if(s==B_NO_MEMORY)return ENOMEM;return EIO;}
}
extern "C" int apex_haiku_fen_port(){
 sem_id id=create_sem(0,"ca2-node-changed");if(id<0){errno=ENOMEM;return -1;}
 auto *m=new(std::nothrow) monitor(id);if(!m){delete_sem(id);errno=ENOMEM;return -1;}
 if(m->Run()<0){delete m;delete_sem(id);errno=EIO;return -1;}
 {std::lock_guard<std::mutex> guard(registry_mutex);monitors[id]=m;}return id;
}
extern "C" void apex_haiku_fen_close(int id){
 monitor *m=nullptr;{std::lock_guard<std::mutex> guard(registry_mutex);auto i=monitors.find(id);if(i!=monitors.end()){m=i->second;monitors.erase(i);}}
 if(m){stop_watching(BMessenger(m));if(m->Lock())m->Quit();}if(id>=0)delete_sem(id);
}
extern "C" int apex_haiku_fen_stat(const char *path,apex_haiku_fen_status *out){
 struct stat s{};out->exists=0;if(lstat(path,&s)!=0)return errno;out->exists=1;
 out->device=s.st_dev;out->inode=s.st_ino;out->size=s.st_size;
 out->atime_sec=s.st_atim.tv_sec;out->atime_nsec=s.st_atim.tv_nsec;
 out->mtime_sec=s.st_mtim.tv_sec;out->mtime_nsec=s.st_mtim.tv_nsec;
 out->ctime_sec=s.st_ctim.tv_sec;out->ctime_nsec=s.st_ctim.tv_nsec;
 out->directory=S_ISDIR(s.st_mode);out->symlink=S_ISLNK(s.st_mode);return 0;
}
extern "C" void *apex_haiku_fen_entry(const char *){return new(std::nothrow) entry;}
extern "C" int apex_haiku_fen_arm(int id,void *p,const apex_haiku_fen_status *s){
 auto *e=static_cast<entry *>(p);auto *m=lookup(id);if(!e || !m)return EINVAL;
 node_ref ref{};ref.device=s->device;ref.node=s->inode;
 if(e->watching && e->ref.device==ref.device && e->ref.node==ref.node)return 0;
 if(e->watching)watch_node(&e->ref,B_STOP_WATCHING,BMessenger(m));
 uint32 flags=B_WATCH_NAME|B_WATCH_STAT|B_WATCH_INTERIM_STAT|B_WATCH_ATTR;if(s->directory)flags|=B_WATCH_DIRECTORY;
 status_t result=watch_node(&ref,flags,BMessenger(m));e->watching=result==B_OK;e->ref=ref;return error(result);
}
extern "C" void apex_haiku_fen_remove(int id,void *p){
 auto *e=static_cast<entry *>(p);if(!e)return;auto *m=lookup(id);
 if(e->watching && m)watch_node(&e->ref,B_STOP_WATCHING,BMessenger(m));delete e;
}
extern "C" int apex_haiku_fen_next(int id,int timeout_ms){
 status_t result=acquire_sem_etc(id,1,B_RELATIVE_TIMEOUT,bigtime_t(timeout_ms)*1000);
 if(result==B_OK)return 1;if(result==B_TIMED_OUT || result==B_WOULD_BLOCK || result==B_INTERRUPTED)return 0;return -EIO;
}
