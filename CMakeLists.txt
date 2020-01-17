cmake_minimum_required(VERSION 3.15)
project(libserialport_example C)

set(CMAKE_C_STANDARD 99)

include_directories(/usr/local/include src)

set(ARM_LINUX_COMPILE ON)       # 이 셋팅을 주석 처리 혹은 주석 풀기를 통해서 arm-linux 로 컴파일 할지 x86_64 로 컴파일할지 선택

## 크로스 컴파일(Cross Compile) 옵션 셋팅
if (${ARM_LINUX_COMPILE})
    message("BUILD ARCH : ARM-LINUX")

    ## arm-linux Cross Compile Options for macOS(Ubuntu dosen't need below options)
    set(ARM_LINUX_TOOLCHAIN_DIR /usr/local/arm-unknown-linux-gnueabi)
    set(CMAKE_C_COMPILER    ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-gcc)
    set(CMAKE_LINKER        ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ld)
    set(CMAKE_NM            ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-nm)
    set(CMAKE_OBJCOPY       ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objcopy)
    set(CMAKE_OBJDUMP       ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-objdump)
    set(CMAKE_RANLIB        ${ARM_LINUX_TOOLCHAIN_DIR}/bin/arm-unknown-linux-gnueabi-ranlib)
    include_directories(${ARM_LINUX_TOOLCHAIN_DIR}/arm-unknown-linux-gnueabi/sysroot/usr/include)

    ### ARM 컴파일용 log4c 라이브러리 디렉토리 설정
    set(SERIAL_PORT_LIB_DIR /usr/local/arm-linux-libserialport)

    ## macOS : warning: cannot find entry symbol arch_paths_first; 에러 대처
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
    set(CMAKE_C_LINK_FLAGS "")
    set(CMAKE_CXX_LINK_FLAGS "")
else () # LOCAL
    message("BUILD ARCH : i386(LOCAL)")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LOCAL__")
    set(CMAKE_C_COMPILER gcc)

    ### x86_64 컴파일용 log4c 라이브러리 디렉토리 설정
    set(SERIAL_PORT_LIB_DIR /usr/local)
endif ()
### 공통 설정
include_directories(${SERIAL_PORT_LIB_DIR}/include)

add_executable(nonblocking_read.out src/nonblocking_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(nonblocking_read.out -L${SERIAL_PORT_LIB_DIR}/lib serialport)

add_executable(blocking_read.out src/blocking_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(blocking_read.out -L${SERIAL_PORT_LIB_DIR}/lib serialport)

add_executable(write_and_read.out src/write_and_read.c src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(write_and_read.out -L${SERIAL_PORT_LIB_DIR}/lib serialport)

add_executable(serial_info.out src/utils/serial_utils.c src/utils/serial_utils.h)
target_link_libraries(serial_info.out -L${SERIAL_PORT_LIB_DIR}/lib serialport)
