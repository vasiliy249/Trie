cmake_minimum_required (VERSION 2.6)
project (Trie)

set(sources
  stdafx.cpp
  Trie.cpp
  main.cpp
)

set(headers
  Trie.h
  stdafx.h
)

include (CMakePCH.txt)
use_precompiled_header(sources stdafx.h stdafx.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS filesystem)                                                                            
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

link_directories(${Boost_LIBRARY_DIRS})

add_executable (Trie ${sources} ${headers})

target_link_libraries(Trie ${Boost_FILESYSTEM_LIBRARY})