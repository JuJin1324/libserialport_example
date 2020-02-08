cmake_minimum_required(VERSION 3.15)
project(libserialport_example C)

set(CMAKE_C_STANDARD 99)

#set(ARM_LINUX_COMPILE ON)       # 이 셋팅을 주석 처리 혹은 주석 풀기를 통해서 arm-linux 로 컴파일 할지 x86_64 로 컴파일할지 선택

message("Host OS : ${CMAKE_HOST_SYSTEM_NAME}")
## 크로스 컴파일(Cross Compile) 옵션 셋팅
if (${ARM_LINUX_COMPILE})
    message("Target OS : ARM-LINUX")

    ## arm-linux Cross Compile Options for Darwin(Ubuntu dosen't need below options)
    set(ARM_LINUX_TOOLCHAIN_DIR /usr/local/arm-unknown-linux-gnueabi)
    set(CMAKE_C_COMPILER    ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-gcc)
    set(CMAKE_LINKER        ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ld)
    set(CMAKE_NM            ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-nm)
    set(CMAKE_OBJCOPY       ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objcopy)
    set(CMAKE_OBJDUMP       ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objdump)
    set(CMAKE_RANLIB        ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ranlib)
    include_directories(${ARM_LINUX_TOOLCHAIN_DIR}/arm-unknown-linux-gnueabi/sysroot/usr/include)

    ## macOS - warning: cannot find entry symbol arch_paths_first; 에러 대처
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
    set(CMAKE_C_LINK_FLAGS "")
    set(CMAKE_CXX_LINK_FLAGS "")
else () # LOCAL
    message("Target OS : ${CMAKE_HOST_SYSTEM_NAME}")
endif ()

if (WIN32)
    set(CMAKE_CXX_FLAGS "-fexec-charset=CP949")         # 윈도우에서 CLion을 통해 생성한 .exe 파일 실행 시 한글 깨짐 현상 방지
endif()
set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/${CMAKE_HOST_SYSTEM_NAME})
find_library(
        SETUPAPI
        NAMES setupapi
        PATHS /usr/lib
        REQUIRED)
find_library(
        SERIALPORT
        NAMES libserialport.a
        PATHS ${EXTERNAL_LIB_DIR}/lib
        REQUIRED)
include_directories(
        ${EXTERNAL_LIB_DIR}/include
        src
)

add_executable(nonblocking_read src/nonblocking_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(nonblocking_read ${SETUPAPI} ${SERIALPORT})

add_executable(blocking_read src/blocking_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(blocking_read ${SETUPAPI} ${SERIALPORT})

add_executable(write_and_read src/write_and_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(write_and_read ${SETUPAPI} ${SERIALPORT})

add_executable(serial_info src/utils/serial_utils.c src/utils/serial_utils.h src/serial_info.c)
target_link_libraries(serial_info ${SETUPAPI} ${SERIALPORT})
