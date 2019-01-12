cmake_minimum_required(VERSION 3.9)

project(compiled_templates)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(LIB_SRC 
  lib/cpl_tmpl/interp/ast.cpp
  lib/cpl_tmpl/interp/ast.h
  lib/cpl_tmpl/interp/ops.cpp
  lib/cpl_tmpl/interp/ops.h
  lib/cpl_tmpl/interp/template.cpp

  lib/cpl_tmpl/lex.cpp
  lib/cpl_tmpl/lex.h
  lib/cpl_tmpl/parse.cpp
  lib/cpl_tmpl/parse.h

  lib/cpl_tmpl/str_utils.h
)


add_library(Cpl_tmpl ${LIB_SRC})
add_library(Cpl_tmpl::Cpl_tmpl ALIAS Cpl_tmpl)

target_include_directories(Cpl_tmpl PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(Cpl_tmpl PUBLIC cxx_std_17)

install(TARGETS Cpl_tmpl 
  EXPORT Cpl_tmplTargets
  ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR} )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/cpl_tmpl DESTINATION include)

install(EXPORT Cpl_tmplTargets
        FILE Cpl_tmplConfig.cmake
        NAMESPACE Cpl_tmpl::
        DESTINATION lib/cmake/Cpl_tmpl
      )

enable_testing()
add_subdirectory(tests)