// $Id$

#include "connect.h"
#include "roa.h"
#include "debug.h"

TAO_OA_Connection_Handler::TAO_OA_Connection_Handler (ACE_Thread_Manager* t)
  : SUPERCLASS (t)
{
  // Grab the singleton...at some later point in time we can provide
  // an argumented CTOR to have per-instance parameters.
  params_ = TAO_OA_PARAMS::instance ();
  ACE_ASSERT  (params_ != 0);
}

int
TAO_OA_Connection_Handler::open (void*)
{
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  const int MAX_SOCK_BUF_SIZE = 65536;
  if (this->peer ().set_option(SOL_SOCKET, SO_SNDBUF, (void
						       *)&MAX_SOCK_BUF_SIZE,
			       sizeof (MAX_SOCK_BUF_SIZE)) == -1)
    return -1;
  if (this->peer ().set_option(SOL_SOCKET, SO_RCVBUF, (void
						       *)&MAX_SOCK_BUF_SIZE,
			       sizeof(MAX_SOCK_BUF_SIZE)) == -1) 
    return -1;

  ACE_DEBUG ((LM_DEBUG, 
	      " (%P|%t) %sconnection from client %s\n", 
	      params_->using_threads () ? "threaded " : "",
	      addr.get_host_name ())); 
  return 0;
}

int
TAO_OA_Connection_Handler::handle_close (ACE_HANDLE handle,
			   ACE_Reactor_Mask rm)
{
  ACE_DEBUG  ((LM_DEBUG, " (%P|%t) TAO_OA_Connection_Handler::handle_close (%d,%d)\n", handle, rm));
  return SUPERCLASS::handle_close (handle, rm);
}

int
TAO_OA_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e., turned
  // into an active object.  Presumably, activation spawns a thread with this
  // method as the "worker function".
  int result = 0;

  ACE_DEBUG  ((LM_DEBUG, " (%P|%t) TAO_OA_Connection_Handler::svc begin\n"));

  // Here we simply synthesize the "typical" event loop one might find
  // in a reactive handler, except that this can simply block waiting for
  // input.
  while ((result = handle_input ()) >= 0)
    continue;
  ACE_DEBUG  ((LM_DEBUG, " (%P|%t) TAO_OA_Connection_Handler::svc end\n"));
  
  return result;
}

int
TAO_OA_Connection_Handler::handle_input (ACE_HANDLE handle)
{
  // CJCXXX The tasks of this method should change to something like
  // the following:
  // 1. call into GIOP to pull off the header
  // 2. construct a complete request
  // 3. dispatch that request and return any required reply and errors

  CORBA_Environment env;
  TAO_Dispatch_Context ctx;

  ctx.oa_ = params_->oa ();
  ctx.endpoint_ = peer ();

  // CJCXXX Knowledge of these will move into the OA so they don't
  // have to be copied every time.  Also, these should be set in the
  // by a call to register_object () or somesuch, because there will
  // be a different upcall and forwarder for every object (or is it
  // for every TYPE of object?).  I need to rename "context" so that
  // it has a more meaningful name.
  ctx.skeleton_ = params_->upcall ();
  ctx.context_ = params_->context ();
  ctx.check_forward_ = params_->forwarder ();

  int ret;

  switch (params_->oa ()->handle_message  (ctx, env))
    {
    case 1:
    default:
      ret = 0;
      break;

    case 0:
    case -1:
      ret = -1;
      break;
    }

  // Don't report any errors from the application/skeleton back to the
  // top level ... the application already has a variety of means to
  // pass whatever data needs passing, and we don't need to confuse
  // things by mixing ORB and application errors here.

  if  (env.exception  () != 0) 
    {
      dexc  (env, "TAO_OA_Connection_Handler, handle incoming message");
      env.clear  ();
    }
  return ret;
}

#if ! defined (__ACE_INLINE__)
#  include "connect.i"
#endif

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
// Direct
template class ACE_Acceptor<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
// Indirect
template class ACE_Task<ACE_NULL_SYNCH>;// ACE_Svc_Handler
template class ACE_TSS<ACE_Dynamic>; // ACE_Svc_Handler
template class ACE_Message_Queue<ACE_NULL_SYNCH>; // ACE_Task
template class ACE_Module<ACE_NULL_SYNCH>; // ACE_Task
template class ACE_Thru_Task<ACE_NULL_SYNCH>; // ACE_Module
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
