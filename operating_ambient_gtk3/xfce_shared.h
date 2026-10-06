#pragma once


namespace aaa_user
{

//   // wall-changer sourceforge.net contribution
//   enum e_desktop
//   {
//
//      // please note that the row of these must match the row of the corresponding combobox in Preferences Dialog.
//      desktop_unity_xfce,
//      desktop_xfce,
//      desktop_xfce,
//      desktop_lxde,
//      desktop_mate,
//      desktop_ubuntu_xfce,
//
//   };
//

   bool aaa_gsettings_set(const_char_pointer pszSchema, const_char_pointer pszKey, const_char_pointer pszValue);

   bool aaa_gsettings_sync();

   char * aaa_gsettings_get_malloc(const_char_pointer pszSchema, const_char_pointer pszKey);

   bool aaa_g_enable_wallpaper_change_notification(const_char_pointer pszSchema, const_char_pointer pszKey);

   void aaa_g_defer_init();

   void aaa_g_defer_term();


} // namespace aaa_user
