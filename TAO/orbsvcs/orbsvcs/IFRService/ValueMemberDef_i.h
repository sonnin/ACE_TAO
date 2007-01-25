// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ValueMemberDef_i.h
//
// = DESCRIPTION
//    ValueMemberDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_VALUEMEMBERDEF_I_H
#define TAO_VALUEMEMBERDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_ValueMemberDef_i : public virtual TAO_Contained_i
{
  // = TITLE
  //    TAO_ValeMemberDef_i
  //
  // = DESCRIPTION
  //    Represents the definition of a valuemember structure.
  //
public:
  TAO_ValueMemberDef_i (TAO_Repository_i *repoy);
  // Constructor

  virtual ~TAO_ValueMemberDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA::Contained::Description *describe ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr type_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::IDLType_ptr type_def ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::IDLType_ptr type_def_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void type_def (
      CORBA::IDLType_ptr type_def)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void type_def_i (
      CORBA::IDLType_ptr type_def)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Visibility access ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Visibility access_i ()

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void access (
      CORBA::Visibility access)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void access_i (
      CORBA::Visibility access)

    ACE_THROW_SPEC ((CORBA::SystemException));
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_VALUEMEMBERDEF_I_H */
