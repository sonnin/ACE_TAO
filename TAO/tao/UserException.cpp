// $Id$

#include "UserException.h"

#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "tao/UserException.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           UserException,
           "$Id$")


CORBA::UserException::UserException (void)
{
}

CORBA::UserException::UserException (char const * repository_id,
                                     char const * local_name)
  : CORBA::Exception (repository_id,
                      local_name)
{
}

CORBA::UserException::~UserException (void)
{
}

CORBA::UserException &
CORBA::UserException::operator= (CORBA::UserException const & rhs)
{
  this->Exception::operator= (rhs);
  return *this;
}

int
CORBA::UserException::_is_a (char const * interface_id) const
{
  return ACE_OS::strcmp (interface_id,
                         "IDL:omg.org/CORBA/UserException:1.0") == 0
    || this->Exception::_is_a (interface_id);
}

ACE_CString
CORBA::UserException::_info (void) const
{
  // @@ we can use the exception's typecode to dump all the data held
  // within it ...

  ACE_CString user_exception_info = "user exception, ID '";
  user_exception_info += this->_rep_id ();
  user_exception_info += "'";
  return user_exception_info;
}
