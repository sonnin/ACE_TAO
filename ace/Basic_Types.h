// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Basic_Types.h
//
// = AUTHORS
//    David L. Levine
//
// = DESCRIPTION
//    #defines the list of preprocessor macros below.  The config.h file can
//    pre-define any of these to short-cut the definitions.  This is usually
//    only necessary if the preprocessor does all of its math using integers.
//
//    Sizes of built-in types:
//      ACE_SIZEOF_CHAR
//      ACE_SIZEOF_WCHAR
//      ACE_SIZEOF_SHORT
//      ACE_SIZEOF_INT
//      ACE_SIZEOF_LONG
//      ACE_SIZEOF_LONG_LONG
//      ACE_SIZEOF_VOID_P
//      ACE_SIZEOF_FLOAT
//      ACE_SIZEOF_DOUBLE
//      ACE_SIZEOF_LONG_DOUBLE
//
//    Wrappers for built-in types of specific sizes:
//      ACE_USHORT16 /* For backward compatibility.  Use ACE_UINT16 instead. */
//      ACE_INT16
//      ACE_UINT16
//      ACE_INT32
//      ACE_UINT32
//      ACE_UINT64
//    (Note: ACE_INT64 is not defined, because there is no ACE_LongLong for
//     platforms that don't have a native 8-byte integer type.)
//
//    Byte-order (endian-ness) determination:
//      ACE_BYTE_ORDER, to either ACE_BIG_ENDIAN or ACE_LITTLE_ENDIAN
//
// ============================================================================

#ifndef ACE_BASIC_TYPES_H
# define ACE_BASIC_TYPES_H

# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Pull in definitions
# include /**/ <limits.h>   // Integer limits
# include /**/ <float.h>    // Floating point limits
# include /**/ <stdlib.h>   // Other types
# include /**/ <sys/types.h>

# include "ace/ACE_export.h"

// A char always has 1 byte, by definition.
# define ACE_SIZEOF_CHAR 1

// Unfortunately, there isn't a portable way to determine the size of a wchar.
// So we just define them on a platform basis.
# if defined (ACE_HAS_WCHAR)
#   if defined (ACE_WIN32)
#     define ACE_SIZEOF_WCHAR 2
#   else /* ACE_WIN32 */
// 0 so the Basic_Types test will catch this.
#     define ACE_SIZEOF_WCHAR 0
#   endif /* ACE_WIN32 */
# endif /* ACE_HAS_WCHAR */

// The number of bytes in a short.
# if !defined (ACE_SIZEOF_SHORT)
#   if (USHRT_MAX) == 255U
#     define ACE_SIZEOF_SHORT 1
#   elif (USHRT_MAX) == 65535U
#     define ACE_SIZEOF_SHORT 2
#   elif (USHRT_MAX) == 4294967295U
#     define ACE_SIZEOF_SHORT 4
#   elif (USHRT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_SHORT 8
#   else
#     error: unsupported short size, must be updated for this platform!
#   endif /* USHRT_MAX */
# endif /* !defined (ACE_SIZEOF_SHORT) */

// The number of bytes in an int.
# if !defined (ACE_SIZEOF_INT)
#   if (UINT_MAX) == 65535U
#     define ACE_SIZEOF_INT 2
#   elif (UINT_MAX) == 4294967295U
#     define ACE_SIZEOF_INT 4
#   elif (UINT_MAX) == 18446744073709551615U
#     define ACE_SIZEOF_INT 8
#   else
#     error: unsupported int size, must be updated for this platform!
#   endif /* UINT_MAX */
# endif /* !defined (ACE_SIZEOF_INT) */

// The number of bytes in a long.
// NOTE - since preprocessors only need to do integer math, this is a likely
// place for a preprocessor to not properly support being able to figure out
// the proper size.  HP aC++ and GNU gcc have this difficulty so watch out.
# if !defined (ACE_SIZEOF_LONG)
#   if (ULONG_MAX) == 65535UL
#     define ACE_SIZEOF_LONG 2
#   elif ((ULONG_MAX) == 4294967295UL)
#     define ACE_SIZEOF_LONG 4
#   elif ((ULONG_MAX) == 18446744073709551615UL)
#     define ACE_SIZEOF_LONG 8
#   else
#     error: unsupported long size, must be updated for this platform!
#   endif /* ULONG_MAX */
# endif /* !defined (ACE_SIZEOF_LONG) */

// The number of bytes in a long long.
# if !defined (ACE_SIZEOF_LONG_LONG)
#   if defined (ACE_LACKS_LONGLONG_T)
#     define ACE_SIZEOF_LONG_LONG 8
#   else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#     if ACE_SIZEOF_LONG == 8
#       define ACE_SIZEOF_LONG_LONG 8
       typedef unsigned long ACE_UINT64;
#     elif defined (ULLONG_MAX) && !defined (__GNUG__)
       // Some compilers use ULLONG_MAX and others, e.g. Irix, use
       // ULONGLONG_MAX.
#       if (ULLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     elif defined (ULONGLONG_MAX) && !defined (__GNUG__)
       // Irix 6.x, for example.
#       if (ULONGLONG_MAX) == 18446744073709551615ULL
#         define ACE_SIZEOF_LONG_LONG 8
#       elif (ULONGLONG_MAX) == 4294967295ULL
#         define ACE_SIZEOF_LONG_LONG 4
#       else
#         error Unsupported long long size needs to be updated for this platform
#       endif
       typedef unsigned long long ACE_UINT64;
#     else
       // ACE_SIZEOF_LONG_LONG is not yet known, but the platform doesn't
       // claim ACE_LACKS_LONGLONG_T, so assume it has 8-byte long longs.
#       define ACE_SIZEOF_LONG_LONG 8
#       if defined (sun) && !defined (ACE_LACKS_U_LONGLONG_T)
         // sun #defines u_longlong_t, maybe other platforms do also.
         // Use it, at least with g++, so that its -pedantic doesn't
         // complain about no ANSI C++ long long.
         typedef u_longlong_t ACE_UINT64;
#       else
         // LynxOS 2.5.0 and Linux don't have u_longlong_t.
         typedef unsigned long long ACE_UINT64;
#       endif /* sun */
#     endif /* ULLONG_MAX && !__GNUG__ */
#   endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
# endif /* !defined (ACE_SIZEOF_LONG_LONG) */


// The sizes of the commonly implemented types are now known.  Set up
// typedefs for whatever we can.  Some of these are needed for certain cases
// of ACE_UINT64, so do them before the 64-bit stuff.

# if ACE_SIZEOF_SHORT == 2
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# elif ACE_SIZEOF_INT == 2
  typedef int ACE_INT16;
  typedef unsigned int ACE_UINT16;
# elif (ACE_SIZEOF_SHORT) == 4 && defined(_CRAYMPP)
  // mpp cray - uses Alpha processors
  //   Use the real 32-bit quantity for ACE_INT32's, and use a "long"
  //   for ACE_INT16's.  This gets around conflicts with size_t in some ACE
  //   method signatures, among other things.
  typedef long ACE_INT16;
  typedef unsigned long ACE_UINT16;
  typedef short ACE_INT32;
  typedef unsigned short ACE_UINT32;
# elif (ACE_SIZEOF_SHORT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
  typedef short ACE_INT16;
  typedef unsigned short ACE_UINT16;
# else
#   error Have to add to the ACE_UINT16 type setting
# endif

typedef ACE_UINT16 ACE_USHORT16;

# if ACE_SIZEOF_INT == 4
  typedef int ACE_INT32;
  typedef unsigned int ACE_UINT32;
#   if defined (__KCC) && !defined (ACE_LACKS_LONGLONG_T)
  typedef unsigned long long ACE_UINT64;
#   endif /* __KCC */
# elif ACE_SIZEOF_LONG == 4
  typedef long ACE_INT32;
  typedef unsigned long ACE_UINT32;
# elif (ACE_SIZEOF_INT) == 8 && defined(_UNICOS)
  // vector cray - hard 64-bit, all 64 bit types
#   if !defined(_CRAYMPP)
      typedef int ACE_INT32;
      typedef unsigned int ACE_UINT32;
#   endif
  typedef unsigned long long ACE_UINT64;
# else
#   error Have to add to the ACE_UINT32 type setting
# endif

// The number of bytes in a void *.
# ifndef ACE_SIZEOF_VOID_P
#   define ACE_SIZEOF_VOID_P ACE_SIZEOF_LONG
# endif /* ACE_SIZEOF_VOID_P */

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that unsigned versions of a type are the same size as the signed
// version of the same type.
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_INT
# if defined (__SUNPRO_CC)
    // For unknown reasons, Sun CC 5.0 won't allow a reintepret cast
    // of a 64-bit pointer to a 64-bit int.
    typedef u_long ptr_arith_t;
# else  /* ! __SUNPRO_CC */
    typedef u_int ptr_arith_t;
# endif /* ! __SUNPRO_CC */
#elif ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG
  typedef u_long ptr_arith_t;
#elif ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
  typedef u_long long ptr_arith_t;
#else
# error "Can't find a suitable type for doing pointer arithmetic."
#endif /* ACE_SIZEOF_VOID_P */

#if defined (ACE_LACKS_LONGLONG_T)
  // This throws away the high 32 bits.  It's very unlikely that a
  // pointer will be more than 32 bits wide if the platform does not
  // support 64-bit integers.
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, L.lo ())
#else  /* ! ACE_LACKS_LONGLONG_T */
# define ACE_LONGLONG_TO_PTR(PTR_TYPE, L) \
  ACE_reinterpret_cast (PTR_TYPE, ACE_static_cast (ptr_arith_t, L))
#endif /* ! ACE_LACKS_LONGLONG_T */

// If the platform lacks a long long, define one.
# if defined (ACE_LACKS_LONGLONG_T)
  class ACE_Export ACE_U_LongLong
  // = TITLE
  //     Unsigned long long for platforms that don't have one.
  //
  // = DESCRIPTION
  //     Provide our own unsigned long long.  This is intended to be
  //     use with ACE_High_Res_Timer, so the division operator assumes
  //     that the quotient fits into a u_long.
  //     Please note that the constructor takes (optionally) two values.
  //     The high one contributes 0x100000000 times its value.  So,
  //     for example, (0, 2) is _not_ 20000000000, but instead
  //     0x200000000.  To emphasize this, the default values are expressed
  //     in hex, and output () dumps the value in hex.
  {
  public:
    // = Initialization and termination methods.
    ACE_U_LongLong (const ACE_UINT32 lo = 0x0, const ACE_UINT32 hi = 0x0);
    ACE_U_LongLong (const ACE_U_LongLong &);
    ACE_U_LongLong &operator= (const ACE_U_LongLong &);
    ~ACE_U_LongLong (void);

    // = Overloaded relation operators.
    int operator== (const ACE_U_LongLong &) const;
    int operator== (const ACE_UINT32) const;
    int operator!= (const ACE_U_LongLong &) const;
    int operator!= (const ACE_UINT32) const;
    int operator< (const ACE_U_LongLong &) const;
    int operator< (const ACE_UINT32) const;
    int operator<= (const ACE_U_LongLong &) const;
    int operator<= (const ACE_UINT32) const;
    int operator> (const ACE_U_LongLong &) const;
    int operator> (const ACE_UINT32) const;
    int operator>= (const ACE_U_LongLong &) const;
    int operator>= (const ACE_UINT32) const;

    ACE_U_LongLong operator+ (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator+ (const ACE_UINT32) const;
    ACE_U_LongLong operator- (const ACE_U_LongLong &) const;
    ACE_U_LongLong operator- (const ACE_UINT32) const;
    ACE_U_LongLong operator* (const ACE_UINT32);
    ACE_U_LongLong &operator*= (const ACE_UINT32);

    ACE_U_LongLong operator<< (const u_int) const;
    ACE_U_LongLong &operator<<= (const u_int);
    ACE_U_LongLong operator>> (const u_int) const;
    ACE_U_LongLong &operator>>= (const u_int);

    double operator/ (const double) const;

    ACE_U_LongLong &operator+= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator+= (const ACE_UINT32);
    ACE_U_LongLong &operator-= (const ACE_U_LongLong &);
    ACE_U_LongLong &operator-= (const ACE_UINT32);
    ACE_U_LongLong &operator++ ();
    ACE_U_LongLong &operator-- ();
    ACE_U_LongLong &operator|= (const ACE_U_LongLong);
    ACE_U_LongLong &operator|= (const ACE_UINT32);
    ACE_U_LongLong &operator&= (const ACE_U_LongLong);
    ACE_U_LongLong &operator&= (const ACE_UINT32);

    // Note that the following take ACE_UINT32 arguments.  These are
    // typical use cases, and easy to implement.  But, they limit the
    // return values to 32 bits as well.  There are no checks for
    // overflow.
    ACE_UINT32 operator/ (const ACE_UINT32) const;
    ACE_UINT32 operator% (const ACE_UINT32) const;

    // The following only operate on the lower 32 bits (they take only
    // 32 bit arguments).
    ACE_UINT32 operator| (const ACE_INT32) const;
    ACE_UINT32 operator& (const ACE_INT32) const;

    // The following operators convert their arguments to
    // ACE_UINT32.  So, there may be information loss if they are
    // used.
    ACE_U_LongLong operator* (const ACE_INT32);
    ACE_U_LongLong &operator*= (const ACE_INT32);
    ACE_UINT32 operator/ (const ACE_INT32) const;
#   if ACE_SIZEOF_INT == 4
    ACE_UINT32 operator/ (const u_long) const;
    ACE_UINT32 operator/ (const long) const;
#   else  /* ACE_SIZEOF_INT != 4 */
    ACE_UINT32 operator/ (const u_int) const;
    ACE_UINT32 operator/ (const int) const;
#   endif /* ACE_SIZEOF_INT != 4 */

    // = Helper methods.
    void output (FILE * = stdout) const;
    // Outputs the value to the FILE, in hex.

    ACE_UINT32 hi (void) const;
    ACE_UINT32 lo (void) const;

    void hi (const ACE_UINT32 hi);
    void lo (const ACE_UINT32 lo);

    ACE_ALLOC_HOOK_DECLARE;

  private:
    union
      {
        struct
          {
            ACE_UINT32 hi_;
            // High 32 bits.

            ACE_UINT32 lo_;
            // Low 32 bits.
          } data_;
        // To ensure alignment on 8-byte boundary.

        // double isn't usually usable with ACE_LACKS_FLOATING_POINT,
        // but this seems OK.
        double for_alignment_;
        // To ensure alignment on 8-byte boundary.
      };

    // NOTE:  the following four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    const ACE_UINT32 &h_ () const { return data_.hi_; }
    // Internal utility function to hide access through struct.

    ACE_UINT32 &h_ () { return data_.hi_; }
    // Internal utility function to hide access through struct.

    const ACE_UINT32 &l_ () const { return data_.lo_; }
    // Internal utility function to hide access through struct.

    ACE_UINT32 &l_ () { return data_.lo_; }
    // Internal utility function to hide access through struct.

    // NOTE:  the above four accessors are inlined here in
    // order to minimize the extent of the data_ struct.  It's
    // only used here; the .i and .cpp files use the accessors.

    ACE_UINT32 ul_shift (ACE_UINT32 a, ACE_UINT32 c_in, ACE_UINT32 *c_out);
    ACE_U_LongLong ull_shift (ACE_U_LongLong a, ACE_UINT32 c_in,
                              ACE_UINT32 *c_out);
    ACE_U_LongLong ull_add (ACE_U_LongLong a, ACE_U_LongLong b,
                            ACE_UINT32 *carry);
    ACE_U_LongLong ull_mult (ACE_U_LongLong a, ACE_UINT32 b,
                             ACE_UINT32 *carry);
    // These functions are used to implement multiplication.
  };

  typedef ACE_U_LongLong ACE_UINT64;

# endif /* ACE_LACKS_LONGLONG_T */

// Conversions from ACE_UINT64 to ACE_UINT32.  ACE_CU64_TO_CU32 should
// be used on const ACE_UINT64's.
# if defined (ACE_LACKS_LONGLONG_T)
#   define ACE_U64_TO_U32(n) ((n).lo ())
#   define ACE_CU64_TO_CU32(n) ((n).lo ())
# else  /* ! ACE_LACKS_LONGLONG_T */
#   define ACE_U64_TO_U32(n) (ACE_static_cast (ACE_UINT32, (n)))
#   define ACE_CU64_TO_CU32(n) \
     (ACE_static_cast (ACE_CAST_CONST ACE_UINT32, (n)))
# endif /* ! ACE_LACKS_LONGLONG_T */

// 64-bit literals require special marking on some platforms.
# if defined (ACE_WIN32)
#   if defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#     define ACE_UINT64_LITERAL(n) n ## LL
#     define ACE_INT64_LITERAL(n) n ## LL
#else
#   define ACE_UINT64_LITERAL(n) n ## ui64
#   define ACE_INT64_LITERAL(n) n ## i64
#endif /* defined (__IBMCPP__) && (__IBMCPP__ >= 400) */
# elif defined (ACE_LACKS_LONGLONG_T)
    // Can only specify 32-bit arguments.
#   define ACE_UINT64_LITERAL(n) (ACE_U_LongLong (n))
      // This one won't really work, but it'll keep
      // some compilers happy until we have better support
#   define ACE_INT64_LITERAL(n) (ACE_U_LongLong (n))
# else  /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_LITERAL(n) n ## ull
#   define ACE_INT64_LITERAL(n) n ## ll
# endif /* ! ACE_WIN32  &&  ! ACE_LACKS_LONGLONG_T */

#if !defined (ACE_UINT64_FORMAT_SPECIFIER)
# define ACE_UINT64_FORMAT_SPECIFIER ACE_TEXT ("%llu")
#endif /* ACE_UINT64_FORMAT_SPECIFIER */

#if !defined (ACE_INT64_FORMAT_SPECIFIER)
# define ACE_INT64_FORMAT_SPECIFIER ACE_TEXT ("%lld")
#endif /* ACE_INT64_FORMAT_SPECIFIER */

// Cast from UINT64 to a double requires an intermediate cast to INT64
// on some platforms.
# if defined (ACE_WIN32)
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_static_cast (__int64, n)
# elif defined (ACE_LACKS_LONGLONG_T)
   // Only use the low 32 bits.
#   define ACE_UINT64_DBLCAST_ADAPTER(n) ACE_U64_TO_U32 (n)
# else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
#   define ACE_UINT64_DBLCAST_ADAPTER(n) (n)
# endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */


// The number of bytes in a float.
# ifndef ACE_SIZEOF_FLOAT
#   if FLT_MAX_EXP == 128
#     define ACE_SIZEOF_FLOAT 4
#   elif FLT_MAX_EXP == 1024
#     define ACE_SIZEOF_FLOAT 8
#   else
#     error: unsupported float size, must be updated for this platform!
#   endif /* FLT_MAX_EXP */
# endif /* ACE_SIZEOF_FLOAT */

// The number of bytes in a double.
# ifndef ACE_SIZEOF_DOUBLE
#   if DBL_MAX_EXP == 128
#     define ACE_SIZEOF_DOUBLE 4
#   elif DBL_MAX_EXP == 1024
#     define ACE_SIZEOF_DOUBLE 8
#   else
#     error: unsupported double size, must be updated for this platform!
#   endif /* DBL_MAX_EXP */
# endif /* ACE_SIZEOF_DOUBLE */

// The number of bytes in a long double.
# ifndef ACE_SIZEOF_LONG_DOUBLE
#   if LDBL_MAX_EXP == 128
#     define ACE_SIZEOF_LONG_DOUBLE 4
#   elif LDBL_MAX_EXP == 1024
#     define ACE_SIZEOF_LONG_DOUBLE 8
#   elif LDBL_MAX_EXP == 16384
#     if defined (LDBL_DIG)  &&  LDBL_DIG == 18
#       define ACE_SIZEOF_LONG_DOUBLE 12
#     else  /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#       define ACE_SIZEOF_LONG_DOUBLE 16
#     endif /* ! LDBL_DIG  ||  LDBL_DIG != 18 */
#   else
#     error: unsupported double size, must be updated for this platform!
#   endif /* LDBL_MAX_EXP */
# endif /* ACE_SIZEOF_LONG_DOUBLE */

// Max and min sizes for the ACE integer types.
#define ACE_CHAR_MAX 0x7F
#define ACE_CHAR_MIN -(ACE_CHAR_MAX)-1
#define ACE_OCTET_MAX 0xFF
#define ACE_INT16_MAX 0x7FFF
#define ACE_INT16_MIN -(ACE_INT16_MAX)-1
#define ACE_UINT16_MAX 0xFFFF
#define ACE_WCHAR_MAX ACE_UINT16_MAX
#define ACE_INT32_MAX 0x7FFFFFFF
#define ACE_INT32_MIN -(ACE_INT32_MAX)-1
#define ACE_UINT32_MAX 0xFFFFFFFF
#define ACE_INT64_MAX ACE_INT64_LITERAL(0x7FFFFFFFFFFFFFFF)
#define ACE_INT64_MIN -(ACE_INT64_MAX)-1
#define ACE_UINT64_MAX ACE_UINT64_LITERAL(0xFFFFFFFFFFFFFFFF)
// These use ANSI/IEEE format.
#define ACE_FLT_MAX 3.402823466e+38F
#define ACE_DBL_MAX 1.7976931348623158e+308

// Byte-order (endian-ness) determination.
# if defined (BYTE_ORDER)
#   if (BYTE_ORDER == LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0123X
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (BYTE_ORDER == BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 3210X
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown BYTE_ORDER!
#   endif /* BYTE_ORDER */
# elif defined (__BYTE_ORDER)
#   if (__BYTE_ORDER == __LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0123X
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (__BYTE_ORDER == __BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 3210X
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown __BYTE_ORDER!
#   endif /* __BYTE_ORDER */
# else /* ! BYTE_ORDER && ! __BYTE_ORDER */
  // We weren't explicitly told, so we have to figure it out . . .
#   if defined (i386) || defined (__i386__) || defined (_M_IX86) || \
     defined (vax) || defined (__alpha)
    // We know these are little endian.
#     define ACE_LITTLE_ENDIAN 0123X
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   else
    // Otherwise, we assume big endian.
#     define ACE_BIG_ENDIAN 3210X
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   endif
# endif /* ! BYTE_ORDER && ! __BYTE_ORDER */

# if defined (__ACE_INLINE__)
#   include "ace/Basic_Types.i"
# endif /* __ACE_INLINE__ */

# include "ace/post.h"
#endif /* ACE_BASIC_TYPES_H */
