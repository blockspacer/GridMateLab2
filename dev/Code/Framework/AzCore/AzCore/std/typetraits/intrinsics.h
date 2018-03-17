/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#ifndef AZSTD_TYPE_TRAITS_INTRINSICS_INCLUDED
#define AZSTD_TYPE_TRAITS_INTRINSICS_INCLUDED

#include <AzCore/std/typetraits/config.h>

//
// Helper macros for builtin compiler support.
// If your compiler has builtin support for any of the following
// traits concepts, then redefine the appropriate macros to pick
// up on the compiler support:
//
// (these should largely ignore cv-qualifiers)
// AZSTD_IS_UNION(T) should evaluate to true if T is a union type
// AZSTD_IS_POD(T) should evaluate to true if T is a POD type
// AZSTD_IS_EMPTY(T) should evaluate to true if T is an empty struct or union
// AZSTD_HAS_TRIVIAL_CONSTRUCTOR(T) should evaluate to true if "T x;" has no effect
// AZSTD_HAS_TRIVIAL_COPY(T) should evaluate to true if T(t) <==> memcpy
// AZSTD_HAS_TRIVIAL_ASSIGN(T) should evaluate to true if t = u <==> memcpy
// AZSTD_HAS_TRIVIAL_DESTRUCTOR(T) should evaluate to true if ~T() has no effect
// AZSTD_HAS_NOTHROW_CONSTRUCTOR(T) should evaluate to true if "T x;" can not throw
// AZSTD_HAS_NOTHROW_COPY(T) should evaluate to true if T(t) can not throw
// AZSTD_HAS_NOTHROW_ASSIGN(T) should evaluate to true if t = u can not throw
// AZSTD_HAS_VIRTUAL_DESTRUCTOR(T) should evaluate to true T has a virtual destructor
//
// The following can also be defined: when detected our implementation is greatly simplified.
// Note that unlike the macros above these do not have default definitions, so we can use
// #ifdef MACRONAME to detect when these are available.
//
// AZSTD_IS_ABSTRACT(T) true if T is an abstract type
// AZSTD_IS_BASE_OF(T,U) true if T is a base class of U
// AZSTD_IS_CLASS(T) true if T is a class type
// AZSTD_IS_CONVERTIBLE(T,U) true if T is convertible to U
// AZSTD_IS_ENUM(T) true is T is an enum
// AZSTD_IS_POLYMORPHIC(T) true if T is a polymorphic type
#if defined(AZ_COMPILER_MWERKS) && (__MSL_CPP__ >= 0x8000)
// Metrowerks compiler is acquiring intrinsic type traits support
// post version 8.  We hook into the published interface to pick up
// user defined specializations as well as compiler intrinsics as
// and when they become available:
#   include <msl_utility>
#   define AZSTD_IS_UNION(T) Metrowerks::is_union<T>::value
// failes in our unitest
//#   define AZSTD_IS_POD(T) Metrowerks::is_POD<T>::value
#   define AZSTD_HAS_TRIVIAL_CONSTRUCTOR(T) Metrowerks::has_trivial_default_ctor<T>::value
#   define AZSTD_HAS_TRIVIAL_COPY(T) Metrowerks::has_trivial_copy_ctor<T>::value
#   define AZSTD_HAS_TRIVIAL_ASSIGN(T) Metrowerks::has_trivial_assignment<T>::value
#   define AZSTD_HAS_TRIVIAL_DESTRUCTOR(T) Metrowerks::has_trivial_dtor<T>::value
// because of POD issues we can't define it
//#   define AZSTD_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(AZ_COMPILER_MSVC) && defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140050215)
#   include <AzCore/std/typetraits/is_same.h>

#   define AZSTD_IS_UNION(T) __is_union(T)
#   define AZSTD_IS_POD(T) (__is_pod(T) && __has_trivial_constructor(T))
#   define AZSTD_IS_EMPTY(T) __is_empty(T)
#   define AZSTD_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   define AZSTD_HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
#   define AZSTD_HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#   define AZSTD_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   define AZSTD_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   define AZSTD_HAS_NOTHROW_COPY(T) __has_nothrow_copy(T)
#   define AZSTD_HAS_NOTHROW_ASSIGN(T) __has_nothrow_assign(T)
#   define AZSTD_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define AZSTD_IS_ABSTRACT(T) __is_abstract(T)
#   define AZSTD_IS_BASE_OF(T, U) (__is_base_of(T, U) && !is_same<T, U>::value)
#   define AZSTD_IS_CLASS(T) __is_class(T)
//  This one doesn't quite always do the right thing:
#   define AZSTD_IS_CONVERTIBLE(_From, _To) __is_convertible_to(_From, _To)
#   define AZSTD_IS_ENUM(T) __is_enum(T)
//  This one doesn't quite always do the right thing:
//  #   define AZSTD_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   define AZSTD_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(AZ_COMPILER_CLANG) || (defined(AZ_COMPILER_GCC) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3))))
#   include <AzCore/std/typetraits/is_same.h>
#   include <AzCore/std/typetraits/is_reference.h>
#   include <AzCore/std/typetraits/is_volatile.h>

#   define AZSTD_IS_UNION(T) __is_union(T)
#   define AZSTD_IS_POD(T) __is_pod(T)
#   define AZSTD_IS_EMPTY(T) __is_empty(T)
#   define AZSTD_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
//  This is claiming that unique_ptr is trivially copyable, which is incorrect.
//  #   define AZSTD_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T) && !is_reference<T>::value)
#   define AZSTD_HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#   define AZSTD_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   define AZSTD_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   define AZSTD_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T) && !is_volatile<T>::value && !is_reference<T>::value)
#   define AZSTD_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T) && !is_volatile<T>::value)
#   define AZSTD_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define AZSTD_IS_ABSTRACT(T) __is_abstract(T)
#   define AZSTD_IS_BASE_OF(T, U) (__is_base_of(T, U) && !is_same<T, U>::value)
#   define AZSTD_IS_CLASS(T) __is_class(T)
#   define AZSTD_IS_ENUM(T) __is_enum(T)
#   define AZSTD_IS_POLYMORPHIC(T) __is_polymorphic(T)

#   define AZSTD_HAS_TYPE_TRAITS_INTRINSICS
#endif

#ifndef AZSTD_IS_UNION
#   define AZSTD_IS_UNION(T) false
#endif

#ifndef AZSTD_IS_POD
#   define AZSTD_IS_POD(T) false
#endif

#ifndef AZSTD_IS_EMPTY
#   define AZSTD_IS_EMPTY(T) false
#endif

#ifndef AZSTD_HAS_TRIVIAL_CONSTRUCTOR
#   define AZSTD_HAS_TRIVIAL_CONSTRUCTOR(T) false
#endif

#ifndef AZSTD_HAS_TRIVIAL_COPY
#   define AZSTD_HAS_TRIVIAL_COPY(T) false
#endif

#ifndef AZSTD_HAS_TRIVIAL_ASSIGN
#   define AZSTD_HAS_TRIVIAL_ASSIGN(T) false
#endif

#ifndef AZSTD_HAS_TRIVIAL_DESTRUCTOR
#   define AZSTD_HAS_TRIVIAL_DESTRUCTOR(T) false
#endif

#ifndef AZSTD_HAS_NOTHROW_CONSTRUCTOR
#   define AZSTD_HAS_NOTHROW_CONSTRUCTOR(T) false
#endif

#ifndef AZSTD_HAS_NOTHROW_COPY
#   define AZSTD_HAS_NOTHROW_COPY(T) false
#endif

#ifndef AZSTD_HAS_NOTHROW_ASSIGN
#   define AZSTD_HAS_NOTHROW_ASSIGN(T) false
#endif

#ifndef AZSTD_HAS_VIRTUAL_DESTRUCTOR
#   define AZSTD_HAS_VIRTUAL_DESTRUCTOR(T) false
#endif

#endif // AZSTD_TYPE_TRAITS_INTRINSICS_INCLUDED
#pragma once
