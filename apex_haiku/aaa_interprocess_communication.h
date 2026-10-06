// created by Camilo 2021-02-14 16:56 BRT <3CamiloSasukeThomasBorregaardSoerensen
#pragma once


namespace apex_haiku
{


   class CLASS_DECL_APEX interprocess_communication_base :
      virtual public interprocess_communication::base
   {
   public:

#ifdef _UWP

      //i32              m_iSerial;

#elif defined(WINDOWS_DESKTOP)


      //HWND             m_hwnd;

      //m_osdataa[0] == = HWND;


#elif defined(APPLEOS)


      CFMessagePortRef     m_port;


#elif !defined(_UWP)

      key_t                m_key;
      int                  m_iQueue;

      struct data_struct
      {


         long     mtype;
         long     request;
         int      size;
         char     data[0];


      };


#endif


      string   m_strBaseChannel;


      interprocess_communication_base();
      virtual ~interprocess_communication_base();


      //HWND get_hwnd() const { return (HWND) get_os_data(); }
      //void set_hwnd(HWND hwnd) { set_os_data((void *) hwnd); }

   };


   class CLASS_DECL_APEX interprocess_communication_tx :
      virtual public interprocess_communication_base,
      virtual public interprocess_communication::tx
   {
   public:


      interprocess_communication_tx();
      ~interprocess_communication_tx() override;



      void open(const ::string & strChannel, ::launcher * plauncher = nullptr) override;
      void close() override;


      void send(const ::string & strMessage, const duration & durationTimeout) override;
      void send(int message, void * pdata, int len, const duration & durationTimeout) override;


      bool is_tx_ok() override;

   };


   class rx_private;


   class CLASS_DECL_APEX interprocess_communication_rx :
      virtual public interprocess_communication_base,
      virtual public interprocess_communication::rx
   {
   public:


      interprocess_communication_rx();
      ~interprocess_communication_rx() override;


      void create(const ::string & strChannel) override;
      void destroy() override;


      //virtual void on_interprocess_receive(::string && strMessage) override;
      //virtual void on_interprocess_receive(int message, memory && memory) override;
      //virtual void on_interprocess_post(i64 a, i64 b) override;


      bool on_idle() override;


      virtual bool start_receiving();

      virtual void * receive();

      bool is_rx_ok() override;


   };


   CLASS_DECL_APEX string app_install(string strPlatform = "");


} // namespace apex_haiku



