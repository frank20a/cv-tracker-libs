include(FindPackageHandleStandardArgs)

if( EXISTS "$ENV{Spinnaker_ROOT_DIR}" )
  file( TO_CMAKE_PATH "$ENV{Spinnaker_ROOT_DIR}" Spinnaker_ROOT_DIR )
  set( Spinnaker_ROOT_DIR "${Spinnaker_ROOT_DIR}" CACHE PATH "Prefix for Spinnaker installation." )
elseif(UNIX AND EXISTS "/opt/spinnaker")
  set( Spinnaker_ROOT_DIR "/opt/spinnaker" )
else()
  message(FATAL_ERROR "Spinnaker_ROOT_DIR not set")
endif()

find_path(Spinnaker_INCLUDE_DIRS
  NAMES Spinnaker.h
  HINTS
  ${Spinnaker_ROOT_DIR}/include
)

if(WIN32)
  if (CMAKE_GENERATOR MATCHES "(Win64|IA64)")
    set(LIBDIR_POSTFIX "64")
  endif()

  find_library(Spinnaker_LIBRARIES
    NAMES Spinnaker_v140
    HINTS
    ${Spinnaker_ROOT_DIR}/lib${LIBDIR_POSTFIX}/vs2015
  )
elseif(UNIX AND NOT APPLE)
  find_library(Spinnaker_LIBRARIES
    NAMES Spinnaker
    HINTS
    ${Spinnaker_ROOT_DIR}/lib
  )
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

set(Spinnaker_INCLUDE_DIRS ${Spinnaker_INCLUDE_DIRS})
set(Spinnaker_LIBRARIES ${Spinnaker_LIBRARIES})

find_package_handle_standard_args(Spinnaker
  FOUND_VAR Spinnaker_FOUND
  REQUIRED_VARS Spinnaker_INCLUDE_DIRS Spinnaker_LIBRARIES)


if(Spinnaker_FOUND AND NOT TARGET Spinnaker::Spinnaker)
  add_library(Spinnaker::Spinnaker UNKNOWN IMPORTED)
  set_target_properties(Spinnaker::Spinnaker PROPERTIES
    IMPORTED_LOCATION                 "${Spinnaker_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES     "${Spinnaker_INCLUDE_DIRS}"
    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX")
  
  message(STATUS "Found Spinnaker incl. dir: ${Spinnaker_INCLUDE_DIRS}")
  message(STATUS "Found Spinnaker lib: ${Spinnaker_LIBRARIES}")
else()
  message(STATUS "Spinnaker not found (includ: ${Spinnaker_INCLUDE_DIRS}, lib: ${Spinnaker_LIBRARIES})")
endif()