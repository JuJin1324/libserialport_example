cmake_minimum_required(VERSION 3.15)
project(libserialport_example C)

set(CMAKE_C_STANDARD 99)

################### 사용자 지정 ####################
## Build 하여 산출된 응용프로그램이 실행될 운영체제 선택
## 주석 시 Host OS / 주석 해제 시 ARM-Linux
#set(BUILD_FOR_ARM_LINUX ON)

#################### Macro ######################
## ARM-Linux 툴체인 셋팅
## @Param
## ARM_LINUX_TOOLCHAIN_DIR : 툴체인 위치, 예시) /usr/local/arm-linux-gnueabi
macro(set_arm_linux_toolchain ARM_LINUX_TOOLCHAIN_DIR)
    ## arm-linux Cross Compile Options for macOS(Ubuntu dosen't need below options)
    set(CMAKE_C_COMPILER ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-gcc)
    set(CMAKE_LINKER ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ld)
    set(CMAKE_NM ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-nm)
    set(CMAKE_OBJCOPY ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objcopy)
    set(CMAKE_OBJDUMP ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objdump)
    set(CMAKE_RANLIB ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ranlib)
    include_directories(${ARM_LINUX_TOOLCHAIN_DIR}/arm-unknown-linux-gnueabi/sysroot/usr/include)

    if (APPLE)
        ## macOS - warning: cannot find entry symbol arch_paths_first; 에러 대처
        set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
        set(CMAKE_C_LINK_FLAGS "")
        set(CMAKE_CXX_LINK_FLAGS "")
    endif ()
endmacro()

### external 디렉터리 아래 CMake가 자동 인식하는 OS 이름으로 된 디렉터리 명에 따라서
### 해당 OS 에서 사용할 라이브러리를 선택해주는 매크로
set(EXTERNAL_LIB_ROOT ${CMAKE_SOURCE_DIR}/external/${CMAKE_HOST_SYSTEM_NAME})
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_LIB_ROOT}/include)
set(EXTERNAL_LIB_DIR ${EXTERNAL_LIB_ROOT}/lib)

macro(set_external_library_static)
    if (WIN32)
        find_library(
                SETUPAPI
                NAMES setupapi
                PATHS /usr/lib
        )
    else ()
        set(SETUPAPI "")
    endif ()
    find_library(
            SERIALPORT
            NAMES libserialport.a
            PATHS ${EXTERNAL_LIB_DIR}
            REQUIRED)
endmacro()

macro(set_external_library_dynamic)
    set(SETUPAPI "")
    find_library(
            SERIALPORT
            NAMES serialport
            PATHS ${EXTERNAL_LIB_DIR}
            REQUIRED)
endmacro()

macro(set_external_library_dynamic_arm_linux)
    set(SETUPAPI "")
    find_library(
            SERIALPORT
            NAMES serialport
            PATHS ${CMAKE_SOURCE_DIR}/external/ARM-Linux/lib
            REQUIRED)
endmacro()

##########################################
## CLion이 구동되는 운영체제에 따른 셋팅
if (APPLE)
    ### Darwin
    message("BUILD HOST OS : ${CMAKE_HOST_SYSTEM_NAME}(macOS)")
    set_external_library_dynamic()
elseif (WIN32)
    ### Microsoft Windows
    message("BUILD HOST OS : ${CMAKE_HOST_SYSTEM_NAME}")
    set_external_library_static()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexec-charset=CP949")         # 윈도우에서 CLion을 통해 생성한 .exe 파일 실행 시 한글 깨짐 현상 방지
else ()
    ### Linux
    message("BUILD HOST OS : ${CMAKE_HOST_SYSTEM_NAME}")
    set_external_library_dynamic()
endif ()

##########################################
## Build 하여 산출된 응용프로그램이 실행될 운영체제에 따른 셋팅 : ARM-Linux / Host OS 중 택
if (${BUILD_FOR_ARM_LINUX})
    message("BUILD TARGET OS : ARM-LINUX")
    set_arm_linux_toolchain(/usr/local/arm-unknown-linux-gnueabi)
    set_external_library_dynamic_arm_linux()

else () # Host OS
    message("BUILD TARGET OS : ${CMAKE_HOST_SYSTEM_NAME}")
endif ()

##########################################
## Application Build 셋팅
include_directories(
        ${EXTERNAL_INCLUDE_DIR}
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
