#pragma once


// GDK, GTK :: C++


namespace user
{


   bool gsettings_get(string & str, const char * pszSchema, const char * pszKey);


} // namespace user


void kde_branch(::matter * preference, e_priority epriority = priority_normal);


template < typename PRED >
auto kde_fork(PRED pred, e_priority epriority = priority_normal)
{

   return async_pred(&gdk_branch, pred, epriority);

}







