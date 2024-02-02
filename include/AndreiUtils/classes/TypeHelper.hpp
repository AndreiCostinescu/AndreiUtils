//
// Created by Andrei on 08.11.22.
//

#ifndef ANDREIUTILS_TYPEHELPER_HPP
#define ANDREIUTILS_TYPEHELPER_HPP

#define CreateHelperTypes(Type) \
typedef Type * Type##_P; \
typedef Type & Type##_R; \
typedef Type && Type##_U; \
typedef Type const Type##_C; \
typedef Type volatile Type##_V; \
\
typedef Type * & Type##_PR; \
typedef Type const * & Type##_CPR; \
typedef Type * const & Type##_PCR; \
typedef Type const * const & Type##_CPCR; \
\
typedef Type const * Type##_CP; \
typedef Type const & Type##_CR; \
typedef Type const && Type##_CU; \
\
typedef Type * const Type##_PC; \
\
typedef Type const * const Type##_CPC; \
\
typedef Type volatile * Type##_VP; \
typedef Type volatile & Type##_VR; \
typedef Type volatile && Type##_VU; \
typedef Type volatile const Type##_VC; \
\
typedef Type volatile * & Type##_VPR; \
typedef Type volatile const * & Type##_VCPR; \
typedef Type volatile * const & Type##_VPCR; \
typedef Type volatile const * const & Type##_VCPCR; \
\
typedef Type volatile const * Type##_VCP; \
typedef Type volatile const & Type##_VCR; \
typedef Type volatile const && Type##_VCU; \
\
typedef Type volatile * const Type##_VPC; \
\
typedef Type volatile const * const Type##_VCPC;

template<typename T>
using C = T const;

template<typename T>
using P = T *;

template<typename T>
using R = T &;

template<typename T>
using U = T &&;

template<typename T>
using V = T volatile;

template<typename T>
using CP = T const *;

template<typename T>
using CR = T const &;

template<typename T>
using CU = T const &&;

template<typename T>
using PC = T *const;

template<typename T>
using PR = T *&;

template<typename T>
using PU = T *&&;

template<typename T>
using VC = T volatile const;

template<typename T>
using VP = T volatile *;

template<typename T>
using VR = T volatile &;

template<typename T>
using VU = T volatile &&;

template<typename T>
using CPC = T const *const;

template<typename T>
using CPR = T const *&;

template<typename T>
using CPU = T const *&&;

template<typename T>
using PCR = T *const &;

template<typename T>
using PCU = T *const &&;

template<typename T>
using VCP = T volatile const *;

template<typename T>
using VCR = T volatile const &;

template<typename T>
using VCU = T volatile const &&;

template<typename T>
using VPC = T volatile *const;

template<typename T>
using VPR = T volatile *&;

template<typename T>
using VPU = T volatile *&&;

template<typename T>
using CPCR = T const *const &;

template<typename T>
using CPCU = T const *const &&;

template<typename T>
using VCPC = T volatile const *const;

template<typename T>
using VCPR = T volatile const *&;

template<typename T>
using VCPU = T volatile const *&&;

template<typename T>
using VPCR = T volatile *const &;

template<typename T>
using VPCU = T volatile *const &&;

template<typename T>
using VCPCR = T volatile const *const &;

template<typename T>
using VCPCU = T volatile const *const &&;

#endif //ANDREIUTILS_TYPEHELPER_HPP
