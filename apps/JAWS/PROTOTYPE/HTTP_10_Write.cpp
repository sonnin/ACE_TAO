// $Id$

#include "ace/streams.h"

#include "JAWS/JAWS.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

#include "HTTP_10.h"

#include "HTTP_10_Request.h"

ACE_RCSID(PROTOTYPE, HTTP_10_Write, "$Id$")

// --------------- WRITE TASK ----------------------

JAWS_HTTP_10_Write_Task::JAWS_HTTP_10_Write_Task (void)
{
}

JAWS_HTTP_10_Write_Task::~JAWS_HTTP_10_Write_Task (void)
{
}

int
JAWS_HTTP_10_Write_Task::handle_put (JAWS_Data_Block *data, ACE_Time_Value *)
{
  JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put");

  JAWS_IO_Handler *handler = data->io_handler ();
  JAWS_Dispatch_Policy *policy = this->policy ();
  if (policy == 0) policy = data->policy ();

  JAWS_IO *io = policy->io ();

  JAWS_HTTP_10_Request *info = ACE_static_cast (JAWS_HTTP_10_Request *,
                                                data->payload ());

  if (info->status () == (int) JAWS_HTTP_10_Request::STATUS_QUIT)
    {
      delete info;
      return -1;
    }

#if 0
  ACE_DEBUG ((LM_DEBUG, " (%t) request %s::%s::%s parsed\n",
              (info->method () ? info->method () : "-"),
              (info->uri () ? info->uri () : "="),
              (info->version () ? info->version () : "HTTP/0.9")));
#endif

  if (info->type () != (int) JAWS_HTTP_10_Request::GET)
    info->set_status (JAWS_HTTP_10_Request::STATUS_NOT_IMPLEMENTED);

  if (info->type () == (int) JAWS_HTTP_10_Request::QUIT)
    info->set_status (JAWS_HTTP_10_Request::STATUS_QUIT);

  if (info->status () != (int) JAWS_HTTP_10_Request::STATUS_OK)
    {
      JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, ! STATUS OK");

      char msg[] =
        "<html><head><title>HTTP/1.0 500 Internal Server Error</title>"
        "<body><h1>Server Error</h1>HTTP/1.0 500 Internal Server Error"
        "</body></html>";

      io->send_error_message (handler, msg, sizeof (msg));
      switch (handler->status ())
        {
        case JAWS_IO_Handler::WRITE_OK:
          delete info;

          if (info->status () == (int) JAWS_HTTP_10_Request::STATUS_QUIT)
            return -1;

          return 0;

        case JAWS_IO_Handler::WRITE_ERROR:
          return -1;

        default:
          return 2;
        }

      return 2;
    }
  else
    {
      io->transmit_file (handler,
                         info->path (),
                         "",
                         0,
                         "",
                         0);

      switch (handler->status ())
        {
        case JAWS_IO_Handler::TRANSMIT_OK:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, OK");
            delete info;
            return 0;
          }
        case JAWS_IO_Handler::TRANSMIT_ERROR:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, ERROR");
            delete info;
            return -1;
          }
        default:
          {
            JAWS_TRACE ("JAWS_HTTP_10_Write_Task::handle_put, DEFAULT");
            delete info;
            return 1;
          }
        }
    }

  delete info;
  return -1;
}
