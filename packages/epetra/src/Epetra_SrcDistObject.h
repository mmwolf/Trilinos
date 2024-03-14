/*
//@HEADER
// ************************************************************************
//
//               Epetra: Linear Algebra Services Package
//                 Copyright 2011 Sandia Corporation
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
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef EPETRA_SRCDISTOBJECT_H
#define EPETRA_SRCDISTOBJECT_H

#include "Epetra_ConfigDefs.h"

class Epetra_BlockMap;


//! Epetra_SrcDistObject: A class for supporting flexible source distributed objects for import/export operations.

/*! The Epetra_SrcDistObject is a base class for all Epetra distributed global objects that are potential
    source objects for the general Epetra_DistObject class.  It provides a way to send a very general distributed
    object as the potential source object for an import or export object.  For example, it is possible to pass
    an Epetra_RowMatrix object as the source object for an import/export where the target is an Epetra_CrsMatrix, or
    an Epetra_CrsGraph (where the RowMatrix values will be ignored).

*/

//==========================================================================
class EPETRA_LIB_DLL_EXPORT Epetra_SrcDistObject {

  public:
    //! @name Destructor
  //@{
  //! Epetra_SrcDistObject destructor.
  virtual ~Epetra_SrcDistObject() {};
  //@}


  //! @name Attribute accessor methods
  //@{
  //! Returns a reference to the Epetra_BlockMap for this object.
  virtual const Epetra_BlockMap & Map() const = 0;
};

#endif /* EPETRA_SRCDISTOBJECT_H */

#if defined(Epetra_SHOW_DEPRECATED_WARNINGS)
#ifdef __GNUC__
#warning "The Epetra package is deprecated"
#endif
#endif

