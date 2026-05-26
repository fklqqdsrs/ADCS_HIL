#pragma once

/* Keep Eigen policy in a central file and include this before any Eigen header. */
#ifndef EIGEN_MPL2_ONLY
#define EIGEN_MPL2_ONLY
#endif

#ifndef EIGEN_RUNTIME_NO_MALLOC
#define EIGEN_RUNTIME_NO_MALLOC
#endif

/* For release flight builds you may define ADCS_EIGEN_DISABLE_DEBUG_CHECKS.
   During SIL/HIL, leave Eigen assertions enabled so runtime malloc checks can catch mistakes. */
#if defined(ADCS_EIGEN_DISABLE_DEBUG_CHECKS) && !defined(EIGEN_NO_DEBUG)
#define EIGEN_NO_DEBUG
#endif

/* Project policy: flight build shall not use exceptions or RTTI. Enforce via build flags. */
#if defined(__cpp_exceptions)
#error "Disable C++ exceptions for flight build: use -fno-exceptions"
#endif

#if defined(__GXX_RTTI)
#error "Disable RTTI for flight build: use -fno-rtti"
#endif
