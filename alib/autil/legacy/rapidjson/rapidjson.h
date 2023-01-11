// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

#ifndef AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_H_
#define AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_H_

/*!\file rapidjson.h
    \brief common definitions and configuration
    
    \see AUTIL_RAPIDJSON_CONFIG
 */

/*! \defgroup AUTIL_RAPIDJSON_CONFIG RapidJSON configuration
    \brief Configuration macros for library features

    Some RapidJSON features are configurable to adapt the library to a wide
    variety of platforms, environments and usage scenarios.  Most of the
    features can be configured in terms of overriden or predefined
    preprocessor macros at compile-time.

    Some additional customization is available in the \ref AUTIL_RAPIDJSON_ERRORS APIs.

    \note These macros should be given on the compiler command-line
          (where applicable)  to avoid inconsistent values when compiling
          different translation units of a single application.
 */

#include <cstdlib>  // malloc(), realloc(), free(), size_t
#include <cstring>  // memset(), memcpy(), memmove(), memcmp()

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_VERSION_STRING
//
// ALWAYS synchronize the following 3 macros with corresponding variables in /CMakeLists.txt.
//

//!@cond AUTIL_RAPIDJSON_HIDDEN_FROM_DOXYGEN
// token stringification
#define AUTIL_RAPIDJSON_STRINGIFY(x) AUTIL_RAPIDJSON_DO_STRINGIFY(x)
#define AUTIL_RAPIDJSON_DO_STRINGIFY(x) #x
//!@endcond

/*! \def AUTIL_RAPIDJSON_MAJOR_VERSION
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Major version of RapidJSON in integer.
*/
/*! \def AUTIL_RAPIDJSON_MINOR_VERSION
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Minor version of RapidJSON in integer.
*/
/*! \def AUTIL_RAPIDJSON_PATCH_VERSION
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Patch version of RapidJSON in integer.
*/
/*! \def AUTIL_RAPIDJSON_VERSION_STRING
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Version of RapidJSON in "<major>.<minor>.<patch>" string format.
*/
#define AUTIL_RAPIDJSON_MAJOR_VERSION 1
#define AUTIL_RAPIDJSON_MINOR_VERSION 0
#define AUTIL_RAPIDJSON_PATCH_VERSION 2
#define AUTIL_RAPIDJSON_VERSION_STRING \
    AUTIL_RAPIDJSON_STRINGIFY(AUTIL_RAPIDJSON_MAJOR_VERSION.AUTIL_RAPIDJSON_MINOR_VERSION.AUTIL_RAPIDJSON_PATCH_VERSION)

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_NAMESPACE_(BEGIN|END)
/*! \def AUTIL_RAPIDJSON_NAMESPACE
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace

    In order to avoid symbol clashes and/or "One Definition Rule" errors
    between multiple inclusions of (different versions of) RapidJSON in
    a single binary, users can customize the name of the main RapidJSON
    namespace.

    In case of a single nesting level, defining \c AUTIL_RAPIDJSON_NAMESPACE
    to a custom name (e.g. \c MyRapidJSON) is sufficient.  If multiple
    levels are needed, both \ref AUTIL_RAPIDJSON_NAMESPACE_BEGIN and \ref
    AUTIL_RAPIDJSON_NAMESPACE_END need to be defined as well:

    \code
    // in some .cpp file
    #define AUTIL_RAPIDJSON_NAMESPACE my::rapidjson
    #define AUTIL_RAPIDJSON_NAMESPACE_BEGIN namespace my { namespace rapidjson {
    #define AUTIL_RAPIDJSON_NAMESPACE_END   } }
    #include "rapidjson/..."
    \endcode

    \see rapidjson
 */
/*! \def AUTIL_RAPIDJSON_NAMESPACE_BEGIN
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace (opening expression)
    \see AUTIL_RAPIDJSON_NAMESPACE
*/
/*! \def AUTIL_RAPIDJSON_NAMESPACE_END
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace (closing expression)
    \see AUTIL_RAPIDJSON_NAMESPACE
*/
#ifndef AUTIL_RAPIDJSON_NAMESPACE
#define AUTIL_RAPIDJSON_NAMESPACE autil_rapidjson
#endif
#ifndef AUTIL_RAPIDJSON_NAMESPACE_BEGIN
#define AUTIL_RAPIDJSON_NAMESPACE_BEGIN namespace AUTIL_RAPIDJSON_NAMESPACE {
#endif
#ifndef AUTIL_RAPIDJSON_NAMESPACE_END
#define AUTIL_RAPIDJSON_NAMESPACE_END }
#endif

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_HAS_STDSTRING

#ifndef AUTIL_RAPIDJSON_HAS_STDSTRING
#define AUTIL_RAPIDJSON_HAS_STDSTRING 1 // always use stdstring
/*! \def AUTIL_RAPIDJSON_HAS_STDSTRING
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Enable RapidJSON support for \c std::string

    By defining this preprocessor symbol to \c 1, several convenience functions for using
    \ref rapidjson::GenericValue with \c std::string are enabled, especially
    for construction and comparison.

    \hideinitializer
*/
#endif // !defined(AUTIL_RAPIDJSON_HAS_STDSTRING)

#if AUTIL_RAPIDJSON_HAS_STDSTRING
#include <string>
#endif // AUTIL_RAPIDJSON_HAS_STDSTRING

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_NO_INT64DEFINE

/*! \def AUTIL_RAPIDJSON_NO_INT64DEFINE
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Use external 64-bit integer types.

    RapidJSON requires the 64-bit integer types \c int64_t and  \c uint64_t types
    to be available at global scope.

    If users have their own definition, define AUTIL_RAPIDJSON_NO_INT64DEFINE to
    prevent RapidJSON from defining its own types.
*/
#ifndef AUTIL_RAPIDJSON_NO_INT64DEFINE
//!@cond AUTIL_RAPIDJSON_HIDDEN_FROM_DOXYGEN
#ifdef _MSC_VER
#include "msinttypes/stdint.h"
#include "msinttypes/inttypes.h"
#else
// Other compilers should have this.
#include <stdint.h>
#include <inttypes.h>
#endif
//!@endcond
#ifdef AUTIL_RAPIDJSON_DOXYGEN_RUNNING
#define AUTIL_RAPIDJSON_NO_INT64DEFINE
#endif
#endif // AUTIL_RAPIDJSON_NO_INT64TYPEDEF

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_FORCEINLINE

#ifndef AUTIL_RAPIDJSON_FORCEINLINE
//!@cond AUTIL_RAPIDJSON_HIDDEN_FROM_DOXYGEN
#if defined(_MSC_VER) && defined(NDEBUG)
#define AUTIL_RAPIDJSON_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ >= 4 && defined(NDEBUG)
#define AUTIL_RAPIDJSON_FORCEINLINE __attribute__((always_inline))
#else
#define AUTIL_RAPIDJSON_FORCEINLINE
#endif
//!@endcond
#endif // AUTIL_RAPIDJSON_FORCEINLINE

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_ENDIAN
#define AUTIL_RAPIDJSON_LITTLEENDIAN  0   //!< Little endian machine
#define AUTIL_RAPIDJSON_BIGENDIAN     1   //!< Big endian machine

//! Endianness of the machine.
/*!
    \def AUTIL_RAPIDJSON_ENDIAN
    \ingroup AUTIL_RAPIDJSON_CONFIG

    GCC 4.6 provided macro for detecting endianness of the target machine. But other
    compilers may not have this. User can define AUTIL_RAPIDJSON_ENDIAN to either
    \ref AUTIL_RAPIDJSON_LITTLEENDIAN or \ref AUTIL_RAPIDJSON_BIGENDIAN.

    Default detection implemented with reference to
    \li https://gcc.gnu.org/onlinedocs/gcc-4.6.0/cpp/Common-Predefined-Macros.html
    \li http://www.boost.org/doc/libs/1_42_0/boost/detail/endian.hpp
*/
#ifndef AUTIL_RAPIDJSON_ENDIAN
// Detect with GCC 4.6's macro
#  ifdef __BYTE_ORDER__
#    if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#      define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_LITTLEENDIAN
#    elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#      define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_BIGENDIAN
#    else
#      error Unknown machine endianess detected. User needs to define AUTIL_RAPIDJSON_ENDIAN.
#    endif // __BYTE_ORDER__
// Detect with GLIBC's endian.h
#  elif defined(__GLIBC__)
#    include <endian.h>
#    if (__BYTE_ORDER == __LITTLE_ENDIAN)
#      define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_LITTLEENDIAN
#    elif (__BYTE_ORDER == __BIG_ENDIAN)
#      define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_BIGENDIAN
#    else
#      error Unknown machine endianess detected. User needs to define AUTIL_RAPIDJSON_ENDIAN.
#   endif // __GLIBC__
// Detect with _LITTLE_ENDIAN and _BIG_ENDIAN macro
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_LITTLEENDIAN
#  elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#    define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_BIGENDIAN
// Detect with architecture macros
#  elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) || defined(__hpux) || defined(__hppa) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#    define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_BIGENDIAN
#  elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) || defined(_M_IX86) || defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__bfin__)
#    define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_LITTLEENDIAN
#  elif defined(_MSC_VER) && defined(_M_ARM)
#    define AUTIL_RAPIDJSON_ENDIAN AUTIL_RAPIDJSON_LITTLEENDIAN
#  elif defined(AUTIL_RAPIDJSON_DOXYGEN_RUNNING)
#    define AUTIL_RAPIDJSON_ENDIAN
#  else
#    error Unknown machine endianess detected. User needs to define AUTIL_RAPIDJSON_ENDIAN.   
#  endif
#endif // AUTIL_RAPIDJSON_ENDIAN

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_64BIT

//! Whether using 64-bit architecture
#ifndef AUTIL_RAPIDJSON_64BIT
#if defined(__LP64__) || defined(_WIN64) || defined(__EMSCRIPTEN__)
#define AUTIL_RAPIDJSON_64BIT 1
#else
#define AUTIL_RAPIDJSON_64BIT 0
#endif
#endif // AUTIL_RAPIDJSON_64BIT

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_ALIGN

//! Data alignment of the machine.
/*! \ingroup AUTIL_RAPIDJSON_CONFIG
    \param x pointer to align

    Some machines require strict data alignment. Currently the default uses 4 bytes
    alignment. User can customize by defining the AUTIL_RAPIDJSON_ALIGN function macro.
*/
#ifndef AUTIL_RAPIDJSON_ALIGN
#if AUTIL_RAPIDJSON_64BIT == 1
#define AUTIL_RAPIDJSON_ALIGN(x) (((x) + static_cast<uint64_t>(7u)) & ~static_cast<uint64_t>(7u))
#else
#define AUTIL_RAPIDJSON_ALIGN(x) (((x) + 3u) & ~3u)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_UINT64_C2

//! Construct a 64-bit literal by a pair of 32-bit integer.
/*!
    64-bit literal with or without ULL suffix is prone to compiler warnings.
    UINT64_C() is C macro which cause compilation problems.
    Use this macro to define 64-bit constants by a pair of 32-bit integer.
*/
#ifndef AUTIL_RAPIDJSON_UINT64_C2
#define AUTIL_RAPIDJSON_UINT64_C2(high32, low32) ((static_cast<uint64_t>(high32) << 32) | static_cast<uint64_t>(low32))
#endif

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_SSE2/AUTIL_RAPIDJSON_SSE42/AUTIL_RAPIDJSON_SIMD

/*! \def AUTIL_RAPIDJSON_SIMD
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief Enable SSE2/SSE4.2 optimization.

    RapidJSON supports optimized implementations for some parsing operations
    based on the SSE2 or SSE4.2 SIMD extensions on modern Intel-compatible
    processors.

    To enable these optimizations, two different symbols can be defined;
    \code
    // Enable SSE2 optimization.
    #define AUTIL_RAPIDJSON_SSE2

    // Enable SSE4.2 optimization.
    #define AUTIL_RAPIDJSON_SSE42
    \endcode

    \c AUTIL_RAPIDJSON_SSE42 takes precedence, if both are defined.

    If any of these symbols is defined, RapidJSON defines the macro
    \c AUTIL_RAPIDJSON_SIMD to indicate the availability of the optimized code.
*/
#if defined(AUTIL_RAPIDJSON_SSE2) || defined(AUTIL_RAPIDJSON_SSE42) \
    || defined(AUTIL_RAPIDJSON_DOXYGEN_RUNNING)
#define AUTIL_RAPIDJSON_SIMD
#endif

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE

#ifndef AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE
/*! \def AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \brief User-provided \c SizeType definition.

    In order to avoid using 32-bit size types for indexing strings and arrays,
    define this preprocessor symbol and provide the type rapidjson::SizeType
    before including RapidJSON:
    \code
    #define AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE
    namespace rapidjson { typedef ::std::size_t SizeType; }
    #include "rapidjson/..."
    \endcode

    \see rapidjson::SizeType
*/
#ifdef AUTIL_RAPIDJSON_DOXYGEN_RUNNING
#define AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE
#endif
AUTIL_RAPIDJSON_NAMESPACE_BEGIN
//! Size type (for string lengths, array sizes, etc.)
/*! RapidJSON uses 32-bit array/string indices even on 64-bit platforms,
    instead of using \c size_t. Users may override the SizeType by defining
    \ref AUTIL_RAPIDJSON_NO_SIZETYPEDEFINE.
*/
typedef unsigned SizeType;
AUTIL_RAPIDJSON_NAMESPACE_END
#endif

// always import std::size_t to rapidjson namespace
AUTIL_RAPIDJSON_NAMESPACE_BEGIN
using std::size_t;
AUTIL_RAPIDJSON_NAMESPACE_END

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_ASSERT

//! Assertion.
/*! \ingroup AUTIL_RAPIDJSON_CONFIG
    By default, rapidjson uses C \c assert() for internal assertions.
    User can override it by defining AUTIL_RAPIDJSON_ASSERT(x) macro.

    \note Parsing errors are handled and can be customized by the
          \ref AUTIL_RAPIDJSON_ERRORS APIs.
*/
#ifndef AUTIL_RAPIDJSON_ASSERT
#include <cassert>
#define AUTIL_RAPIDJSON_ASSERT(x) assert(x)
#endif // AUTIL_RAPIDJSON_ASSERT

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_STATIC_ASSERT

// Adopt from boost
#ifndef AUTIL_RAPIDJSON_STATIC_ASSERT
#ifndef __clang__
//!@cond AUTIL_RAPIDJSON_HIDDEN_FROM_DOXYGEN
#endif
AUTIL_RAPIDJSON_NAMESPACE_BEGIN
template <bool x> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };
template<int x> struct StaticAssertTest {};
AUTIL_RAPIDJSON_NAMESPACE_END

#define AUTIL_RAPIDJSON_JOIN(X, Y) AUTIL_RAPIDJSON_DO_JOIN(X, Y)
#define AUTIL_RAPIDJSON_DO_JOIN(X, Y) AUTIL_RAPIDJSON_DO_JOIN2(X, Y)
#define AUTIL_RAPIDJSON_DO_JOIN2(X, Y) X##Y

#if defined(__GNUC__)
#define AUTIL_RAPIDJSON_STATIC_ASSERT_UNUSED_ATTRIBUTE __attribute__((unused))
#else
#define AUTIL_RAPIDJSON_STATIC_ASSERT_UNUSED_ATTRIBUTE 
#endif
#ifndef __clang__
//!@endcond
#endif

/*! \def AUTIL_RAPIDJSON_STATIC_ASSERT
    \brief (Internal) macro to check for conditions at compile-time
    \param x compile-time condition
    \hideinitializer
 */
#define AUTIL_RAPIDJSON_STATIC_ASSERT(x) \
    typedef ::AUTIL_RAPIDJSON_NAMESPACE::StaticAssertTest< \
      sizeof(::AUTIL_RAPIDJSON_NAMESPACE::STATIC_ASSERTION_FAILURE<bool(x) >)> \
    AUTIL_RAPIDJSON_JOIN(StaticAssertTypedef, __LINE__) AUTIL_RAPIDJSON_STATIC_ASSERT_UNUSED_ATTRIBUTE
#endif

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_LIKELY, AUTIL_RAPIDJSON_UNLIKELY

//! Compiler branching hint for expression with high probability to be true.
/*!
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \param x Boolean expression likely to be true.
*/
#ifndef AUTIL_RAPIDJSON_LIKELY
#if defined(__GNUC__) || defined(__clang__)
#define AUTIL_RAPIDJSON_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define AUTIL_RAPIDJSON_LIKELY(x) x
#endif
#endif

//! Compiler branching hint for expression with low probability to be true.
/*!
    \ingroup AUTIL_RAPIDJSON_CONFIG
    \param x Boolean expression unlikely to be true.
*/
#ifndef AUTIL_RAPIDJSON_UNLIKELY
#if defined(__GNUC__) || defined(__clang__)
#define AUTIL_RAPIDJSON_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define AUTIL_RAPIDJSON_UNLIKELY(x) x
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Helpers

//!@cond AUTIL_RAPIDJSON_HIDDEN_FROM_DOXYGEN

#define AUTIL_RAPIDJSON_MULTILINEMACRO_BEGIN do {  
#define AUTIL_RAPIDJSON_MULTILINEMACRO_END \
} while((void)0, 0)

// adopted from Boost
#define AUTIL_RAPIDJSON_VERSION_CODE(x,y,z) \
  (((x)*100000) + ((y)*100) + (z))

///////////////////////////////////////////////////////////////////////////////
// AUTIL_RAPIDJSON_DIAG_PUSH/POP, AUTIL_RAPIDJSON_DIAG_OFF

#if defined(__GNUC__)
#define AUTIL_RAPIDJSON_GNUC \
    AUTIL_RAPIDJSON_VERSION_CODE(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#endif

#if defined(__clang__) || (defined(AUTIL_RAPIDJSON_GNUC) && AUTIL_RAPIDJSON_GNUC >= AUTIL_RAPIDJSON_VERSION_CODE(4,2,0))

#define AUTIL_RAPIDJSON_PRAGMA(x) _Pragma(AUTIL_RAPIDJSON_STRINGIFY(x))
#define AUTIL_RAPIDJSON_DIAG_PRAGMA(x) AUTIL_RAPIDJSON_PRAGMA(GCC diagnostic x)
#define AUTIL_RAPIDJSON_DIAG_OFF(x) \
    AUTIL_RAPIDJSON_DIAG_PRAGMA(ignored AUTIL_RAPIDJSON_STRINGIFY(AUTIL_RAPIDJSON_JOIN(-W,x)))

// push/pop support in Clang and GCC>=4.6
#if defined(__clang__) || (defined(AUTIL_RAPIDJSON_GNUC) && AUTIL_RAPIDJSON_GNUC >= AUTIL_RAPIDJSON_VERSION_CODE(4,6,0))
#define AUTIL_RAPIDJSON_DIAG_PUSH AUTIL_RAPIDJSON_DIAG_PRAGMA(push)
#define AUTIL_RAPIDJSON_DIAG_POP  AUTIL_RAPIDJSON_DIAG_PRAGMA(pop)
#else // GCC >= 4.2, < 4.6
#define AUTIL_RAPIDJSON_DIAG_PUSH /* ignored */
#define AUTIL_RAPIDJSON_DIAG_POP /* ignored */
#endif

#elif defined(_MSC_VER)

// pragma (MSVC specific)
#define AUTIL_RAPIDJSON_PRAGMA(x) __pragma(x)
#define AUTIL_RAPIDJSON_DIAG_PRAGMA(x) AUTIL_RAPIDJSON_PRAGMA(warning(x))

#define AUTIL_RAPIDJSON_DIAG_OFF(x) AUTIL_RAPIDJSON_DIAG_PRAGMA(disable: x)
#define AUTIL_RAPIDJSON_DIAG_PUSH AUTIL_RAPIDJSON_DIAG_PRAGMA(push)
#define AUTIL_RAPIDJSON_DIAG_POP  AUTIL_RAPIDJSON_DIAG_PRAGMA(pop)

#else

#define AUTIL_RAPIDJSON_DIAG_OFF(x) /* ignored */
#define AUTIL_RAPIDJSON_DIAG_PUSH   /* ignored */
#define AUTIL_RAPIDJSON_DIAG_POP    /* ignored */

#endif // AUTIL_RAPIDJSON_DIAG_*

///////////////////////////////////////////////////////////////////////////////
// C++11 features

#ifndef AUTIL_RAPIDJSON_HAS_CXX11_RVALUE_REFS
#if defined(__clang__)
#define AUTIL_RAPIDJSON_HAS_CXX11_RVALUE_REFS __has_feature(cxx_rvalue_references) && \
    (defined(_LIBCPP_VERSION) || defined(__GLIBCXX__) && __GLIBCXX__ >= 20080306)
#elif (defined(AUTIL_RAPIDJSON_GNUC) && (AUTIL_RAPIDJSON_GNUC >= AUTIL_RAPIDJSON_VERSION_CODE(4,3,0)) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || \
      (defined(_MSC_VER) && _MSC_VER >= 1600)

#define AUTIL_RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#else
#define AUTIL_RAPIDJSON_HAS_CXX11_RVALUE_REFS 0
#endif
#endif // AUTIL_RAPIDJSON_HAS_CXX11_RVALUE_REFS

#ifndef AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT
#if defined(__clang__)
#define AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT __has_feature(cxx_noexcept)
#elif (defined(AUTIL_RAPIDJSON_GNUC) && (AUTIL_RAPIDJSON_GNUC >= AUTIL_RAPIDJSON_VERSION_CODE(4,6,0)) && defined(__GXX_EXPERIMENTAL_CXX0X__))
//    (defined(_MSC_VER) && _MSC_VER >= ????) // not yet supported
#define AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT 1
#else
#define AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT 0
#endif
#endif
#if AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT
#define AUTIL_RAPIDJSON_NOEXCEPT noexcept
#else
#define AUTIL_RAPIDJSON_NOEXCEPT /* noexcept */
#endif // AUTIL_RAPIDJSON_HAS_CXX11_NOEXCEPT

// no automatic detection, yet
#ifndef AUTIL_RAPIDJSON_HAS_CXX11_TYPETRAITS
#define AUTIL_RAPIDJSON_HAS_CXX11_TYPETRAITS 0
#endif

//!@endcond

///////////////////////////////////////////////////////////////////////////////
// new/delete

#ifndef AUTIL_RAPIDJSON_NEW
///! customization point for global \c new
#define AUTIL_RAPIDJSON_NEW(x) new x
#endif
#ifndef AUTIL_RAPIDJSON_DELETE
///! customization point for global \c delete
#define AUTIL_RAPIDJSON_DELETE(x) delete x
#endif

///////////////////////////////////////////////////////////////////////////////
// Allocators and Encodings

#include "allocators.h"
#include "encodings.h"

/*! \namespace rapidjson
    \brief main RapidJSON namespace
    \see AUTIL_RAPIDJSON_NAMESPACE
*/
AUTIL_RAPIDJSON_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
//  Stream

/*! \class rapidjson::Stream
    \brief Concept for reading and writing characters.

    For read-only stream, no need to implement PutBegin(), Put(), Flush() and PutEnd().

    For write-only stream, only need to implement Put() and Flush().

\code
concept Stream {
    typename Ch;    //!< Character type of the stream.

    //! Read the current character from stream without moving the read cursor.
    Ch Peek() const;

    //! Read the current character from stream and moving the read cursor to next character.
    Ch Take();

    //! Get the current read cursor.
    //! \return Number of characters read from start.
    size_t Tell();

    //! Begin writing operation at the current read pointer.
    //! \return The begin writer pointer.
    Ch* PutBegin();

    //! Write a character.
    void Put(Ch c);

    //! Flush the buffer.
    void Flush();

    //! End the writing operation.
    //! \param begin The begin write pointer returned by PutBegin().
    //! \return Number of characters written.
    size_t PutEnd(Ch* begin);
}
\endcode
*/

//! Provides additional information for stream.
/*!
    By using traits pattern, this type provides a default configuration for stream.
    For custom stream, this type can be specialized for other configuration.
    See TEST(Reader, CustomStringStream) in readertest.cpp for example.
*/
template<typename Stream>
struct StreamTraits {
    //! Whether to make local copy of stream for optimization during parsing.
    /*!
        By default, for safety, streams do not use local copy optimization.
        Stream that can be copied fast should specialize this, like StreamTraits<StringStream>.
    */
    enum { copyOptimization = 0 };
};

//! Reserve n characters for writing to a stream.
template<typename Stream>
inline void PutReserve(Stream& stream, size_t count) {
    (void)stream;
    (void)count;
}

//! Write character to a stream, presuming buffer is reserved.
template<typename Stream>
inline void PutUnsafe(Stream& stream, typename Stream::Ch c) {
    stream.Put(c);
}

//! Put N copies of a character to a stream.
template<typename Stream, typename Ch>
inline void PutN(Stream& stream, Ch c, size_t n) {
    PutReserve<Stream>(stream, n);
    for (size_t i = 0; i < n; i++)
        PutUnsafe(stream, c);
}

///////////////////////////////////////////////////////////////////////////////
// StringStream

//! Read-only string stream.
/*! \note implements Stream concept
*/
template <typename Encoding>
struct GenericStringStream {
    typedef typename Encoding::Ch Ch;

    GenericStringStream(const Ch *src) : src_(src), head_(src) {}

    Ch Peek() const { return *src_; }
    Ch Take() { return *src_++; }
    size_t Tell() const { return static_cast<size_t>(src_ - head_); }

    Ch* PutBegin() { AUTIL_RAPIDJSON_ASSERT(false); return 0; }
    void Put(Ch) { AUTIL_RAPIDJSON_ASSERT(false); }
    void Flush() { AUTIL_RAPIDJSON_ASSERT(false); }
    size_t PutEnd(Ch*) { AUTIL_RAPIDJSON_ASSERT(false); return 0; }

    const Ch* src_;     //!< Current read position.
    const Ch* head_;    //!< Original head of the string.
};

template <typename Encoding>
struct StreamTraits<GenericStringStream<Encoding> > {
    enum { copyOptimization = 1 };
};

//! String stream with UTF8 encoding.
typedef GenericStringStream<UTF8<> > StringStream;

///////////////////////////////////////////////////////////////////////////////
// InsituStringStream

//! A read-write string stream.
/*! This string stream is particularly designed for in-situ parsing.
    \note implements Stream concept
*/
template <typename Encoding>
struct GenericInsituStringStream {
    typedef typename Encoding::Ch Ch;

    GenericInsituStringStream(Ch *src) : src_(src), dst_(0), head_(src) {}

    // Read
    Ch Peek() { return *src_; }
    Ch Take() { return *src_++; }
    size_t Tell() { return static_cast<size_t>(src_ - head_); }

    // Write
    void Put(Ch c) { AUTIL_RAPIDJSON_ASSERT(dst_ != 0); *dst_++ = c; }

    Ch* PutBegin() { return dst_ = src_; }
    size_t PutEnd(Ch* begin) { return static_cast<size_t>(dst_ - begin); }
    void Flush() {}

    Ch* Push(size_t count) { Ch* begin = dst_; dst_ += count; return begin; }
    void Pop(size_t count) { dst_ -= count; }

    Ch* src_;
    Ch* dst_;
    Ch* head_;
};

template <typename Encoding>
struct StreamTraits<GenericInsituStringStream<Encoding> > {
    enum { copyOptimization = 1 };
};

//! Insitu string stream with UTF8 encoding.
typedef GenericInsituStringStream<UTF8<> > InsituStringStream;

///////////////////////////////////////////////////////////////////////////////
// Type

//! Type of JSON value
enum Type {
    kNullType = 0,      //!< null
    kFalseType = 1,     //!< false
    kTrueType = 2,      //!< true
    kObjectType = 3,    //!< object
    kArrayType = 4,     //!< array 
    kStringType = 5,    //!< string
    kNumberType = 6     //!< number
};

AUTIL_RAPIDJSON_NAMESPACE_END

#endif // AUTIL_RAPIDJSON_AUTIL_RAPIDJSON_H_
