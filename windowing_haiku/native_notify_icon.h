#pragma once
extern "C" {
void * haiku_notify_create(const char *, const void *, void (*)(void *, const char *), void *);
void haiku_notify_menu(void *, int, int, const char *, const char *, bool, bool);
int haiku_notify_install(void *);
void haiku_notify_destroy(void *);
bool haiku_notify_exists(void *);
void haiku_notify_bitmap(void *, const void *);
}
