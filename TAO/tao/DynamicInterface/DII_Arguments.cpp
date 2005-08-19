// $Id$

#include "DII_Arguments.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/Exception.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/DynamicC.h"

ACE_RCSID (DynamicInterface,
           DII_Arguments,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "DII_Arguments.inl"
#endif /* ! __ACE_INLINE__ */

namespace TAO
{
  CORBA::Boolean
  NamedValue_Argument::demarshal (TAO_InputCDR &cdr)
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        if (this->x_ !=0 && this->x_->value ()->impl ())
          {
            this->x_->value ()->impl ()->_tao_decode (cdr
                                                      ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (false);

    this->byte_order_ = cdr.byte_order ();

    return 1;
  }

  void
  NamedValue_Argument::interceptor_value (CORBA::Any *any) const
  {
    (*any) <<= *this->x_->value ();
  }

  // ========================================================================

  CORBA::Boolean
  NVList_Argument::marshal (TAO_OutputCDR &cdr)
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        this->x_->_tao_encode (cdr,
                               CORBA::ARG_IN | CORBA::ARG_INOUT
                               ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (false);

    return 1;
  }

  CORBA::Boolean
  NVList_Argument::demarshal (TAO_InputCDR &cdr)
  {

    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
      {
        // Now, get all the "return", "out", and "inout" parameters
        // from the response message body ... return parameter is
        // first, the rest are in the order defined in the IDL spec
        // (which is also the order that DII users are required to
        // use).

        this->x_->_tao_incoming_cdr (
            cdr,
            CORBA::ARG_OUT | CORBA::ARG_INOUT,
            this->lazy_evaluation_
            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHANY
      {
        return 0;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (false);

    return 1;
  }

  void
  NVList_Argument::interceptor_paramlist (Dynamic::ParameterList *lst)
  {
    const CORBA::ULong len = this->x_->count ();
    lst->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        if (!this->x_->item (i)->value ())
          return;

        (*lst)[i].argument.replace (
          this->x_->item (i)->value ()->impl ());

        switch (this->x_->item (i)->flags ())
          {
          case CORBA::ARG_IN:
            {
              (*lst)[i].mode = CORBA::PARAM_IN;
              break;
            }
          case CORBA::ARG_INOUT:
            {
              (*lst)[i].mode = CORBA::PARAM_INOUT;
              break;
            }
          case CORBA::ARG_OUT:
            {
              (*lst)[i].mode = CORBA::PARAM_OUT;
              break;
            }
          default:
            break;
          }
      }
  }
}
