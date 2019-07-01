
/**
 * Basic Data Type Definations.s
 *
 */

#if defined(_WIN32)
#  if _MSC_VER  <= 1200 ///< VC6
#    define __FUNCTION__ ""
#  endif
#  define _INTPTR (1) ///< Default 32b Pointer Size.
#  define __PRETTY_FUNCTION__           __FUNCTION__
#  define __func__                      __FUNCTION__
#  define _CRT_SECURE_NO_WARNINGS
#endif

/**
 * C++ Mixed Programming Symbol.
 */
#if defined(__cplusplus)
#  define NK_CPP_EXTERN_BEGIN       extern "C" {
#  define NK_CPP_EXTERN_END         }
#else
#  define NK_CPP_EXTERN_BEGIN
#  define NK_CPP_EXTERN_END
#endif

/**
 * Windows Export Symbol.
 */
#if defined(_WIN32)
#  define NK_API __declspec(dllexport)
#else
#  define NK_API extern
#endif


#ifndef __NK_TYPES_H__
#define __NK_TYPES_H__

NK_CPP_EXTERN_BEGIN


/**
 * Multi-Platform Common Basic Type Definations.
 */
typedef  char           NK_Int8;
typedef  unsigned char  NK_UInt8;
typedef  char           NK_Char;
typedef  NK_Char*       NK_PChar;
typedef  NK_UInt8       NK_Byte;
typedef  NK_Byte*       NK_PByte;
typedef  int            NK_Int;
typedef  NK_Int         NK_Integer;
typedef  void           NK_Void;
typedef  void*          NK_PVoid;
typedef  NK_Int         NK_Boolean;


/**
 * @brief
 *  Type Definition.\n
 */
#if defined(_WIN32)
#  if _MSC_VER  <= 1200 ///< VC6
#    include <WINNT.H>
typedef     SHORT       NK_Int16;
typedef     INT32       NK_Int32;
typedef     INT64       NK_Int64;
typedef     USHORT      NK_UInt16;
typedef     UINT32      NK_UInt32;
typedef     UINT64      NK_UInt64;
typedef     INT_PTR     NK_PtrInt;
#  else
#  include <stdint.h>
typedef     int16_t     NK_Int16;
typedef     int32_t     NK_Int32;
typedef     int64_t     NK_Int64;
typedef     uint16_t    NK_UInt16;
typedef     uint32_t    NK_UInt32;
typedef     uint64_t    NK_UInt64;
typedef     uintptr_t   NK_PtrInt;
#  endif
#elif defined(_ECOS)
#  include <cyg_type.h>

typedef     cyg_halint16        NK_Int16;
typedef     cyg_halint32        NK_Int32;
typedef     cyg_halint64        NK_Int64;
typedef     cyg_uint16          NK_UInt16;
typedef     cyg_uint32          NK_UInt32;
typedef     cyg_uint64          NK_UInt64;
typedef     cyg_haladdress      NK_PtrInt;


#else
#  include <stdint.h>
typedef     int16_t     NK_Int16;
typedef     int32_t     NK_Int32;
typedef     int64_t     NK_Int64;
typedef     uint16_t    NK_UInt16;
typedef     uint32_t    NK_UInt32;
typedef     uint64_t    NK_UInt64;
typedef     uintptr_t   NK_PtrInt;
#endif

/**
 * Some Type Redefinition.
 */
typedef     NK_Int32    NK_SSize;
typedef     NK_UInt32   NK_Size;
typedef     NK_Int64    NK_SSize64;
typedef     NK_UInt64   NK_Size64;

/**
 * All Caption Letter Defination.\n
 * Someone Prefer to User this Method for Their Own Code-Style.
 */
#define     NK_INT8     NK_Int8
#define     NK_UINT8    NK_UInt8
#define     NK_INT16    NK_Int16
#define     NK_UINT16   NK_UInt16
#define     NK_INT32    NK_Int32
#define     NK_UINT32   NK_UInt32
#define     NK_INT64    NK_Int64
#define     NK_UINT64   NK_UInt64
#define     NK_CHAR     NK_Char
#define     NK_PCHAR    NK_PChar
#define     NK_BYTE     NK_Byte
#define     NK_PBYTE    NK_PByte
#define     NK_INTEGER  NK_Integer
#define     NK_INT      NK_Int
#define     NK_PTRINT   NK_PtrInt
#define     NK_SSIZE    NK_SSize
#define     NK_SIZE     NK_Size
#define     NK_SSIZE64  NK_SSize64
#define     NK_SIZE64   NK_Size64
#define     NK_VOID     NK_Void
#define     NK_PVOID    NK_PVoid
#define     NK_BOOLEAN  NK_Boolean

/**
 * @brief
 *  Boolean True or False Defination.\n
 */
#define     NK_False    (0)
#define     NK_True     (!NK_False)
#define     NK_FALSE    (NK_False)
#define     NK_TRUE     (NK_True)

#if !defined(TRUE)
#define TRUE NK_True
#endif
#if !defined(FALSE)
#define FALSE NK_False
#endif
#if !defined(true)
#define true NK_True
#endif
#if !defined(false)
#define false NK_False
#endif



/**
 * @brief
 *  Nil Pointer Defination.\n
 */
#if defined(NULL)
#  define   NK_Nil      (NULL)
#else
#if defined(__cplusplus)
#define NK_Nil 0
#else
#define NK_Nil ((void *)0)
#endif
#endif
#define     NK_NIL      (NK_Nil)

/**
 * @brief
 *  Single Float Defination.\n
 */
typedef     float       NK_Float;
#define     NK_FLOAT    NK_Float

/**
 * @brief
 *  Double Float Defination.\n
 */
typedef     double      NK_DFloat;
#define     NK_DFLOAT   NK_DFloat

#define inline __inline

#endif /* __NK_TYPES_H__ */