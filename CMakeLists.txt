# Thanks to this web page for the template for this cmake file
## https://heejune.me/2016/08/17/build-your-own-clang-example-outside-of-the-llvm-source-tree/
# Invoke cmake with -DCMAKE_PREFIX_PATH=/path/to/llvm/CMAKE_ROOT

cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (Examples)


############################################################
#   base
############################################################

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

############################################################
#   setting Project informations
############################################################

set(LIBRARY_LIST clangFrontend  clangSerialization clangDriver clangParse clangRewriteFrontend clangStaticAnalyzerFrontend clangSema)
set(LIBRARY_LIST ${LIBRARY_LIST} clangAnalysis clangEdit clangAST clangLex clangBasic clangTooling
      clangRewrite clangASTMatchers clangToolingCore)
set(COMPONENT_LIST mcparser bitreader support mc option)

############################################################
#   generate makefiles
############################################################

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
if(LLVM_BUILD_MAIN_SRC_DIR)
  include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
  include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)

add_executable(simple_print_ast simple_print_ast.cpp)
add_executable(print_local_function_decls print_local_function_decls.cpp)
add_executable(trivial_function_name_rewriter trivial_function_name_rewriter.cpp)
add_executable(replace_for_pattern replace_for_pattern.cpp)

foreach(exec_name
            simple_print_ast print_local_function_decls trivial_function_name_rewriter
            replace_for_pattern)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    foreach(link_lib IN LISTS LIBRARY_LIST)
      target_link_libraries(${exec_name} optimized ${link_lib})
      target_link_libraries(${exec_name} debug     ${link_lib}d)
    endforeach()
  else()
    target_link_libraries(${exec_name} ${LIBRARY_LIST})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-unused-parameter -fno-strict-aliasing -fno-exceptions -fno-rtti")
    #set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  endif()

  #llvm_map_components_to_libnames(llvm_libs ${COMPONENT_LIST})
  #target_link_libraries(${exec_name} ${llvm_libs})
  target_link_libraries(${exec_name}
    LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
    LLVMX86AsmPrinter # MC, Support, X86Utils
    LLVMX86Info # MC, Support, Target
    LLVMX86Utils # Core, Support
    LLVMipo
    LLVMScalarOpts
    LLVMInstCombine
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMOption # Support
    LLVMMCParser # MC, Support
    LLVMMC # Object, Support
    LLVMObject # BitReader, Core, Support
    LLVMBitReader # Core, Support
    LLVMCore # Support
    LLVMSupport
  )
endforeach()

message(STATUS "User selected librarys = ${LIBRARY_LIST}")
message(STATUS "User selected components = ${COMPONENT_LIST}")
message(STATUS "    = ${llvm_libs}")