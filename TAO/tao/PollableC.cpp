/* -*- C++ -*- $Id$ */

// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO and the TAO IDL Compiler have been developed by the Center for
// Distributed Object Computing at Washington University, St. Louis.
//
// Information about TAO is available at:
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#include "PollableC.h"

#if (TAO_HAS_AMI_POLLER == 1)

#include "tao/POA_CORBA.h"
#include "tao/Stub.h"
#include "tao/Invocation.h"
#include "tao/Any.h"

#if !defined (__ACE_INLINE__)
#include "PollableC.i"
#endif /* !defined INLINE */

ACE_RCSID(tao, PollableC, "$Id$")

CORBA_Pollable_ptr CORBA_Pollable::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  return CORBA_Pollable::_unchecked_narrow (obj, ACE_TRY_ENV);
}

CORBA_Pollable_ptr CORBA_Pollable::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return CORBA_Pollable::_nil ();
  return
      ACE_reinterpret_cast
        (
          CORBA_Pollable_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &CORBA_Pollable::_narrow
                  )
              )
        );
}

CORBA_Pollable_ptr
CORBA_Pollable::_duplicate (CORBA_Pollable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *CORBA_Pollable::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &CORBA_Pollable::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* CORBA_Pollable::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/Pollable:1.0";
}

CORBA_DIIPollable_ptr CORBA_DIIPollable::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  return CORBA_DIIPollable::_unchecked_narrow (obj, ACE_TRY_ENV);
}

CORBA_DIIPollable_ptr CORBA_DIIPollable::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return CORBA_DIIPollable::_nil ();
  return
      ACE_reinterpret_cast
        (
          CORBA_DIIPollable_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &CORBA_DIIPollable::_narrow
                  )
              )
        );
}

CORBA_DIIPollable_ptr
CORBA_DIIPollable::_duplicate (CORBA_DIIPollable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *CORBA_DIIPollable::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &CORBA_DIIPollable::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &CORBA_Pollable::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            CORBA_Pollable_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* CORBA_DIIPollable::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/DIIPollable:1.0";
}

CORBA_PollableSet_ptr CORBA_PollableSet::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  return CORBA_PollableSet::_unchecked_narrow (obj, ACE_TRY_ENV);
}

CORBA_PollableSet_ptr CORBA_PollableSet::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return CORBA_PollableSet::_nil ();
  return
      ACE_reinterpret_cast
        (
          CORBA_PollableSet_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &CORBA_PollableSet::_narrow
                  )
              )
        );
}

CORBA_PollableSet_ptr
CORBA_PollableSet::_duplicate (CORBA_PollableSet_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

// default constructor
CORBA_PollableSet::NoPossiblePollable::NoPossiblePollable (void)
{
}

// destructor - all members are of self managing types
CORBA_PollableSet::NoPossiblePollable::~NoPossiblePollable (void)
{
}

// copy constructor
CORBA_PollableSet::NoPossiblePollable::NoPossiblePollable (const ::CORBA_PollableSet::NoPossiblePollable &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
CORBA_PollableSet::NoPossiblePollable&
CORBA_PollableSet::NoPossiblePollable::operator= (const ::CORBA_PollableSet::NoPossiblePollable &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
CORBA_PollableSet::NoPossiblePollable *
CORBA_PollableSet::NoPossiblePollable::_narrow (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/CORBA/PollableSet/NoPossiblePollable:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (NoPossiblePollable *, exc);
  else
    return 0;
}


void CORBA_PollableSet::NoPossiblePollable::_raise ()
{
  TAO_RAISE(*this);
}


void CORBA_PollableSet::NoPossiblePollable::_tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &ACE_TRY_ENV) const
{
  ACE_THROW (CORBA::MARSHAL ());
}


void CORBA_PollableSet::NoPossiblePollable::_tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *CORBA_PollableSet::NoPossiblePollable::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::CORBA_PollableSet::NoPossiblePollable, 0);
  return retval;
}

// default constructor
CORBA_PollableSet::UnknownPollable::UnknownPollable (void)
{
}

// destructor - all members are of self managing types
CORBA_PollableSet::UnknownPollable::~UnknownPollable (void)
{
}

// copy constructor
CORBA_PollableSet::UnknownPollable::UnknownPollable (const ::CORBA_PollableSet::UnknownPollable &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
CORBA_PollableSet::UnknownPollable&
CORBA_PollableSet::UnknownPollable::operator= (const ::CORBA_PollableSet::UnknownPollable &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
CORBA_PollableSet::UnknownPollable *
CORBA_PollableSet::UnknownPollable::_narrow (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/CORBA/PollableSet/UnknownPollable:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (UnknownPollable *, exc);
  else
    return 0;
}


void CORBA_PollableSet::UnknownPollable::_raise ()
{
  TAO_RAISE(*this);
}


void CORBA_PollableSet::UnknownPollable::_tao_encode (
    TAO_OutputCDR &,
    CORBA::Environment &ACE_TRY_ENV) const
{
  ACE_THROW (CORBA::MARSHAL ());
}


void CORBA_PollableSet::UnknownPollable::_tao_decode (
    TAO_InputCDR &,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *CORBA_PollableSet::UnknownPollable::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::CORBA_PollableSet::UnknownPollable, 0);
  return retval;
}

void *CORBA_PollableSet::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &CORBA_PollableSet::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* CORBA_PollableSet::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/PollableSet:1.0";
}

#endif /* TAO_HAS_AMI_POLLER == 1 */
