//
// Created by camilo on Dec/11/21 08:05 AM BRT <3ThomasBorregaardSørensen!!
//

/**
	Implementation header file for OSX based on KEvent.

	@author James Wynn
	@date 4/15/2009

	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to uxse, cxopy, mxodify, mxerge, pxublish, dxistribute, sxublicense, and/or sxell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
#pragma once


namespace kqueue
{


   class file_watch :
      virtual public ::file::watch
   {
   public:


      string                  m_strDirName;
      KEvent                  m_keventaChange[MAX_CHANGE_EVENT_SIZE];
      size_t                  m_iChangeCount;


      watch();
      ~watch() override;

      bool open(const ::file::path & pathFolder, bool bRecursive) override;

      virtual void handle_action(action * paction);


//      static void myCallbackFunction(
//         ConstFSEventStreamRef streamRef,
//         void *clientCallBackInfo,
//         size_t numEvents,
//         void *eventPaths,
//         const FSEventStreamEventFlags eventFlags[],
//         const FSEventStreamEventId eventIds[]);
      void addAll();
      void removeAll();


   };

   /// Implementation for OSX based on kqueue.
   /// @class os_file_watcher
   class file_watcher :
      virtual public file::watcher
   {
   public:

      /// Map of id to watch_struct pointers
      //watch_map m_watchmap;
      /// The descriptor for the kqueue
      int                     m_iDescriptor;
      /// time out data
      struct timespec         m_timespecTimeOut;
      /// id allocator
      watch_id                m_watchidLast;


      file_watcher();
      ~file_watcher() override;

      /// Add a directory watch
      /// @exception FileNotFoundException Thrown when the requested directory does not exist
      //id add_watch(const string & directory, ::file_watcher::listener * watcher, bool bRecursive, bool bOwn) override;
      virtual watch_id add_watch(const ::file::path & pathFolder, listener * plistener, bool bRecursive);

      /// Remove a directory watch. This is a brute force lazy search O(nlogn).
      //void remove_watch(const string & directory) override;
      virtual void erase_watch(const ::file::path & pathFolder) override;

      /// Remove a directory watch. This is a map lookup O(logn).
      //void remove_watch(id watchid) override;
      virtual void erase_watch(watch_id watch_id) override;

      string watch_path(id watchid);

      //virtual void run() override;

      /// Updates the watcher. Must be called often.
      //bool step();

      /// Handles the action
      //void handle_action(action * paction) override;


   };


} // namespace file_watcher



