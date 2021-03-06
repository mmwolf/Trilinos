/*
//@HEADER
// ************************************************************************
//
//               KokkosKernels: Linear Algebra and Graph Kernels
//                 Copyright 2016 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Siva Rajamanickam (srajama@sandia.gov)
//
// ************************************************************************
//@HEADER
*/





#ifndef __KOKKOSKERNELS_TRSM_SERIAL_IMPL_HPP__
#define __KOKKOSKERNELS_TRSM_SERIAL_IMPL_HPP__


/// \author Kyungjoo Kim (kyukim@sandia.gov)

#include "KokkosKernels_Util.hpp"

namespace KokkosKernels {

  namespace Serial {

    template<typename ArgDiag>
    struct Trsm<Side::Left,Uplo::Lower,Trans::NoTranspose,ArgDiag,Algo::Trsm::Unblocked> {
      template<typename ScalarType,
               typename AViewType,
               typename BViewType>
      KOKKOS_INLINE_FUNCTION
      static int
      invoke(const ScalarType alpha,
             const AViewType A,
             /**/  BViewType B) {

        typedef typename BViewType::value_type value_type;
        
        if (alpha == 0) {
          Util::set(B, 0);
        } else {
          if (alpha != 1)
            Util::scale(B, alpha);
          
          // B (m x n), A(m x m)
          const int
            m = B.dimension(0),
            n = B.dimension(1);
          
          for (int p=0;p<m;++p) {
            value_type
              *__restrict__ alpha11 = &A(p, p),
              *__restrict__ a21     = alpha11 + A.stride_0(),
              *__restrict__ b1t     = &B(p, 0),
              *__restrict__ B2      = b1t + B.stride_0();
            
            const int
              iend = m-p-1,
              jend = n;
            
            if (!ArgDiag::use_unit_diag) {
              value_type
                *__restrict__ sb1t = b1t;
              for (int j=0;j<jend;++j,sb1t+=B.stride_1())
                *sb1t /= *alpha11;
            }
            
            for (int i=0;i<iend;++i,a21+=A.stride_0(),B2+=B.stride_0()) {
              value_type
                *__restrict__ sb1t = b1t,
                *__restrict__ sB2  = B2;
              for (int j=0;j<jend;++j,sb1t+=B.stride_1(),sB2+=B.stride_1()) 
                *sB2 -= *a21 * *sb1t;
            }
          }
        }
        
        return 0;
      }
    };

    template<typename ArgDiag>
    struct Trsm<Side::Right,Uplo::Upper,Trans::NoTranspose,ArgDiag,Algo::Trsm::Unblocked> {
      
      template<typename ScalarType,
               typename AViewType,
               typename BViewType>
      KOKKOS_INLINE_FUNCTION
      static int
      invoke(const ScalarType alpha,
             const AViewType A,
             /**/  BViewType B) {

        typedef typename BViewType::value_type value_type;        

        if (alpha == 0) {
          Util::set(B, 0);
        } else {
          if (alpha != 1)
            Util::scale(B, alpha);
          
          // B (m x n), A(m x m)
          const int
            m = B.dimension(0),
            n = B.dimension(1);
          
          for (int p=0;p<n;++p) {
            value_type
              *__restrict__ alpha11 = &A(p, p),
              *__restrict__ a12t    = alpha11 + A.stride_1(),
              *__restrict__ b1      = &B(0, p),
              *__restrict__ B2      = b1 + B.stride_1();
            
            const int
              iend = m,
              jend = n-p-1;
            
            if (!ArgDiag::use_unit_diag) {
              value_type
                *__restrict__ sb1 = b1;
              for (int i=0;i<iend;++i,sb1+=B.stride_0())
                *sb1 /= *alpha11;
            }
            
            for (int i=0;i<iend;++i,b1+=B.stride_0(),B2+=B.stride_0()) {
              value_type
                *__restrict__ sa12t = a12t,
                *__restrict__ sB2   = B2;
              for (int j=0;j<jend;++j,sa12t+=A.stride_1(),sB2+=B.stride_1())
                *sB2 -= *sa12t * *b1;
            }
          }
        }
        
        return 0;
      }
    };

  }


}

#endif
