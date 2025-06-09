# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.10)

# Get the directory above the source directory
get_filename_component(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/ ABSOLUTE)
get_filename_component(PROJECT_NAME_RAW ${PROJECT_ROOT_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME_CLEAN ${PROJECT_NAME_RAW})
project(${PROJECT_NAME_CLEAN})
# Set Clang and Clang++ as the C and C++ compilers
set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

# Avoid linking with MSVC runtime
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld -nostdlib++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=lld -nostdlib++")

# Ensure that the MSVC libraries aren't linked
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

# Explicitly set 32-bit flags for Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

# Set build type configurations
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths relative to the build directory
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(PROJECT_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# Set output directories for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)

# Find all .lib files in the lib directory
file(GLOB LIBRARY_FILES "${PROJECT_LIB_DIR}/*.lib")

# Find all header directories in the include directory
file(GLOB INCLUDE_SUBDIRS RELATIVE ${PROJECT_INCLUDE_DIR} ${PROJECT_INCLUDE_DIR}/*)

foreach(INCLUDE_SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${PROJECT_INCLUDE_DIR}/${INCLUDE_SUBDIR})
        include_directories(${PROJECT_INCLUDE_DIR}/${INCLUDE_SUBDIR})
    endif()
endforeach()

# Find C files in specific include subdirectories that need special handling
file(GLOB C_SOURCE_FILES 
    "${PROJECT_INCLUDE_DIR}/*/**.c"
    "${PROJECT_INCLUDE_DIR}/*/**.cpp"
)

# Find project source files
file(GLOB PROJECT_SOURCE_FILES 
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_FILES}
    ${C_SOURCE_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES})

link_directories(${PROJECT_LIB_DIR})

# Suppress warnings for C source files
set_source_files_properties(${C_SOURCE_FILES} 
    PROPERTIES COMPILE_FLAGS "-w"
)

# Set compiler-specific flags
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_DIR}/debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_DIR}/release)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()
