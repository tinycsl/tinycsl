# 设置你的项目源文件
set(SOURCES
    src/ss_array.c 
    src/ss_slice.c 
    src/ss_hashmap.c 
    src/ss_obtree.c 
    src/ss_string_utils.c 
    src/ss_string.c 
    src/ss_alloc.c 
    src/ss_list.c
    src/ss_bigbitset.c
    src/ss_bitarray.c
)


include_directories(${CMAKE_BINARY_DIR}/include/tcsl)

# add_library(tcsl SHARED ${SOURCES})
add_library(tcsl STATIC ${SOURCES})

add_executable(test_tcsl ${SOURCES} 
    tests/main.c 
    tests/ss_list_test.c
    tests/ss_obtree_test.c
    tests/ss_bitarray_test.c
    tests/ss_array_test.c
    tests/ss_string_test.c
    tests/ss_string_utils_test.c
    tests/ss_alloc_test.c
    tests/ss_hashmap_test.c
    tests/ss_bigbitset_test.c
    tests/ss_compare_test.c
    tests/ss_hash_test.c
)

target_link_libraries(${PROJECT_NAME} m)


# 生成头文件
file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")
file(COPY ${HEADER_FILES} DESTINATION ${CMAKE_BINARY_DIR}/include/tcsl/)


# 设置编译器选项（如果需要）
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)


if(WIN32)
  add_definitions(-D_WIN32)
endif()

if(WIN64)
  add_definitions(-D_WIN64)
endif()

if (UNIX AND NOT WIN32)
    # 启用 AddressSanitizer 和调试符号
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

    # 链接时也需要加上 AddressSanitizer
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address ")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address ")

    # # 链接时也需要加上 AddressSanitizer
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -static-libasan -static-libubsan")
    # set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address -static-libasan -static-libubsan")
endif()
