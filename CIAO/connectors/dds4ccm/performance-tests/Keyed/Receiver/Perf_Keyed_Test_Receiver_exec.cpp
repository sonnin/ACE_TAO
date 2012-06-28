// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v1.8.3
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

#include "Perf_Keyed_Test_Receiver_exec.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_Perf_Keyed_Test_Receiver_Impl
{

  /**
   * Facet Executor Implementation Class: info_listen_data_listener_exec_i
   */

  info_listen_data_listener_exec_i::info_listen_data_listener_exec_i (
        ::Perf_Keyed_Test::CCM_Receiver_Context_ptr ctx,
        Receiver_exec_i &callback)
    : ciao_context_ (
        ::Perf_Keyed_Test::CCM_Receiver_Context::_duplicate (ctx))
      , callback_ (callback)
  {
  }

  info_listen_data_listener_exec_i::~info_listen_data_listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PerfKeyedTest::Listener

  void
  info_listen_data_listener_exec_i::on_one_data (const ::PerfKeyedTest & datum,
  const ::CCM_DDS::ReadInfo & /* info */)
  {
    // Record time, not for ping messages , already pinged back.
    if (datum.latency_ping != -1L)
      {
        this->callback_.record_time (datum.data_len);
      }

    // Send back a packet if this is a ping
    if (datum.latency_ping == 1L)
      {
        this->callback_.write_one (const_cast<PerfKeyedTest&> (datum));
      }
  }

  void
  info_listen_data_listener_exec_i::on_many_data (const ::PerfKeyedTestSeq & /* data */,
  const ::CCM_DDS::ReadInfoSeq & /* infos */)
  {
    /* Your code here. */
  }

  /**
   * Component Executor Implementation Class: Receiver_exec_i
   */

  Receiver_exec_i::Receiver_exec_i (void)
    : count_ (0L)
      , interval_time_(0L)
      , interval_messages_received_(0L)
      , messages_received_(0L)
      , interval_bytes_received_(0L)
      , bytes_received_(0L)
      , interval_data_length_(0L)
      , first_time_ (0L)
      , finished_(false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::write_one (PerfKeyedTest & an_instance)
  {
    an_instance.latency_ping = -1L;
    this->writer_->write_one (an_instance, ::DDS::HANDLE_NIL);
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc =
            this->ciao_context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::record_time (unsigned long datalen)
  {
    ++this->count_;
    if( datalen == INITIALIZE_SIZE)
      {
        // store the info for this interval
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (this->first_time_);
        this->messages_received_=1;
      }
    else if ( datalen == FINISHED_SIZE)
      {  // store the info for this interval
        ACE_UINT64 last_time;
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (last_time);
        this->interval_time_ =  (last_time  - this->first_time_);
        this->interval_messages_received_ = ++this->messages_received_;
        this->interval_bytes_received_ = this->bytes_received_;
        this->finished_ = true;
      }
    else
      {
        ++this->messages_received_;
        this->interval_data_length_ = datalen + OVERHEAD_BYTES;
        this->bytes_received_ += datalen + OVERHEAD_BYTES;
      }
  }
  // Component attributes and port operations.

  ::CCM_DDS::PerfKeyedTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_info_listen_data_listener_.in ()))
      {
        info_listen_data_listener_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          info_listen_data_listener_exec_i (
            this->ciao_context_.in (),
            *this),
            ::CCM_DDS::PerfKeyedTest::CCM_Listener::_nil ());

          this->ciao_info_listen_data_listener_ = tmp;
      }

    return
      ::CCM_DDS::PerfKeyedTest::CCM_Listener::_duplicate (
        this->ciao_info_listen_data_listener_.in ());
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Perf_Keyed_Test::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    (void) ACE_High_Res_Timer::global_scale_factor ();
    // enable the datalistener
    this->start();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
     this->writer_ = this->ciao_context_->get_connection_write_ping_data ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    if (!this->finished_.value ())  // proces ended before received last message
      {
        ACE_UINT64 last_time;
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (last_time);
        this->interval_time_ =  (last_time  - this->first_time_);
        this->interval_messages_received_ = ++this->messages_received_;
        this->interval_bytes_received_ = this->bytes_received_;
      }
    if ((this->count_.value () > 0) && (this->interval_time_ > 0))
      {
        double per_sec = (double)1000000/ this->interval_time_;
        double mbps =
          (this->interval_bytes_received_.value()* per_sec)* (8.0/1000.0/1000.0);
        ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                             "Data Length: %u  Messages: %u  Messages/s(ave): "
                             "%6.01f,   Mbps(ave): %7.01f \n",
                       this->interval_data_length_.value(),
                       this->interval_messages_received_.value(),
                       this->interval_messages_received_.value()* per_sec,
                       mbps));
      }
    else
      {
         ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                           "No samples received\n "));
      }
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Perf_Keyed_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
