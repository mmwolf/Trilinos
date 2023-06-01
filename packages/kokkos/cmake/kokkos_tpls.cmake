KOKKOS_CFG_DEPENDS(TPLS OPTIONS)
KOKKOS_CFG_DEPENDS(TPLS DEVICES)
KOKKOS_CFG_DEPENDS(TPLS COMPILER_ID)

FUNCTION(KOKKOS_TPL_OPTION PKG DEFAULT)
  CMAKE_PARSE_ARGUMENTS(PARSED
    ""
    "TRIBITS"
    ""
    ${ARGN})

  IF (PARSED_TRIBITS)
    #this is also a TPL option you can activate with Tribits
    IF (NOT "${TPL_ENABLE_${PARSED_TRIBITS}}" STREQUAL "")
      #Tribits brought its own default that should take precedence
      SET(DEFAULT ${TPL_ENABLE_${PARSED_TRIBITS}})
    ENDIF()
  ENDIF()

  KOKKOS_ENABLE_OPTION(${PKG} ${DEFAULT} "Whether to enable the ${PKG} library")
  KOKKOS_OPTION(${PKG}_DIR "" PATH "Location of ${PKG} library")
  SET(KOKKOS_ENABLE_${PKG} ${KOKKOS_ENABLE_${PKG}} PARENT_SCOPE)
  SET(KOKKOS_${PKG}_DIR  ${KOKKOS_${PKG}_DIR} PARENT_SCOPE)

  IF (KOKKOS_HAS_TRILINOS
    AND KOKKOS_ENABLE_${PKG}
    AND NOT PARSED_TRIBITS)
    #this TPL was enabled, but it is not valid to use inside of TriBITS
    MESSAGE(FATAL_ERROR "Enabled TPL ${PKG} inside TriBITS build, "
           "but this can only be enabled in a standalone build")
  ENDIF()
ENDFUNCTION()

KOKKOS_TPL_OPTION(HWLOC   Off TRIBITS HWLOC)
KOKKOS_TPL_OPTION(LIBNUMA Off)
KOKKOS_TPL_OPTION(MEMKIND Off)
IF(KOKKOS_ENABLE_MEMKIND)
  SET(KOKKOS_ENABLE_HBWSPACE ON)
ENDIF()
KOKKOS_TPL_OPTION(CUDA    ${Kokkos_ENABLE_CUDA} TRIBITS CUDA)
KOKKOS_TPL_OPTION(LIBRT   Off)
IF(KOKKOS_ENABLE_HIP AND NOT KOKKOS_CXX_COMPILER_ID STREQUAL HIPCC AND NOT
    KOKKOS_HAS_TRILINOS)
  SET(ROCM_DEFAULT ON)
ELSE()
  SET(ROCM_DEFAULT OFF)
ENDIF()
KOKKOS_TPL_OPTION(ROCM    ${ROCM_DEFAULT})

IF (WIN32)
  SET(LIBDL_DEFAULT Off)
ELSE()
  SET(LIBDL_DEFAULT On)
ENDIF()
KOKKOS_TPL_OPTION(LIBDL ${LIBDL_DEFAULT} TRIBITS DLlib)

IF(Trilinos_ENABLE_Kokkos AND TPL_ENABLE_HPX)
SET(HPX_DEFAULT ON)
ELSE()
SET(HPX_DEFAULT OFF)
ENDIF()
KOKKOS_TPL_OPTION(HPX ${HPX_DEFAULT})

KOKKOS_TPL_OPTION(THREADS ${Kokkos_ENABLE_THREADS} TRIBITS Pthread)

IF(Trilinos_ENABLE_Kokkos AND TPL_ENABLE_quadmath)
  SET(LIBQUADMATH_DEFAULT ON)
ELSE()
  SET(LIBQUADMATH_DEFAULT OFF)
ENDIF()
KOKKOS_TPL_OPTION(LIBQUADMATH ${LIBQUADMATH_DEFAULT} TRIBITS quadmath)

#Make sure we use our local FindKokkosCuda.cmake
KOKKOS_IMPORT_TPL(HPX INTERFACE)
IF (NOT KOKKOS_ENABLE_COMPILE_AS_CMAKE_LANGUAGE)
  KOKKOS_IMPORT_TPL(CUDA INTERFACE)
ENDIF()
KOKKOS_IMPORT_TPL(HWLOC)
KOKKOS_IMPORT_TPL(LIBNUMA)
KOKKOS_IMPORT_TPL(LIBRT)
KOKKOS_IMPORT_TPL(LIBDL)
KOKKOS_IMPORT_TPL(MEMKIND)
IF (NOT WIN32)
  KOKKOS_IMPORT_TPL(THREADS INTERFACE)
ENDIF()
IF (NOT KOKKOS_ENABLE_COMPILE_AS_CMAKE_LANGUAGE)
  KOKKOS_IMPORT_TPL(ROCM INTERFACE)
ENDIF()
KOKKOS_IMPORT_TPL(LIBQUADMATH)

IF (Kokkos_ENABLE_IMPL_DESUL_ATOMICS AND Kokkos_ENABLE_DESUL_ATOMICS_EXTERNAL)
  find_package(desul REQUIRED COMPONENTS atomics)
  KOKKOS_EXPORT_CMAKE_TPL(desul REQUIRED COMPONENTS atomics)
ENDIF()

if (Kokkos_ENABLE_IMPL_MDSPAN AND Kokkos_ENABLE_MDSPAN_EXTERNAL)
  find_package(mdspan REQUIRED)
  KOKKOS_EXPORT_CMAKE_TPL(mdspan REQUIRED)
endif()

IF (Kokkos_ENABLE_OPENMP)
  find_package(OpenMP REQUIRED)
  # FIXME_TRILINOS Trilinos doesn't allow for Kokkos to use find_dependency
  # so we just append the flags here instead of linking with the OpenMP target.
  IF(KOKKOS_HAS_TRILINOS)
    COMPILER_SPECIFIC_FLAGS(DEFAULT ${OpenMP_CXX_FLAGS})
  ELSE()
    KOKKOS_EXPORT_CMAKE_TPL(OpenMP REQUIRED)
  ENDIF()
ENDIF()

#Convert list to newlines (which CMake doesn't always like in cache variables)
STRING(REPLACE ";" "\n" KOKKOS_TPL_EXPORT_TEMP "${KOKKOS_TPL_EXPORTS}")
#Convert to a regular variable
UNSET(KOKKOS_TPL_EXPORTS CACHE)
SET(KOKKOS_TPL_EXPORTS ${KOKKOS_TPL_EXPORT_TEMP})
IF (KOKKOS_ENABLE_MEMKIND)
   SET(KOKKOS_ENABLE_HBWSPACE)
   LIST(APPEND KOKKOS_MEMSPACE_LIST HBWSpace)
ENDIF()
