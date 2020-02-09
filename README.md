# libserialport example
C언어 serial 통신 라이브러리 

[공식 페이지](https://sigrok.org/wiki/Libserialport)

라이센스(license) 정보
* LGPL(GNU Lesser General Public License)
* 동적 링크 사용시 상업에 사용하는 소스 공개 의무 없음
* 정적 링크 사용시 상업에 사용하는 소스 공개 의무 <b>있음</b>
* [오픈소스SW 라이선스 LGPL 바로알기](https://openbee.kr/422)

## 설치 : Microsoft Windows
<b>MSYS2 / MinGW 사용</b>   

* MSYS2/MinGW를 사용하여 CLion 환경 구축하기 : [참조 사이트](https://github.com/JuJin1324/CLion_stater#microsoft-windows)
* 라이브러리 설치를 위한 패키지 설치 : `pacman -S isl autoconf automake-wrapper libtool make`
* 라이브러리 생성 소스 다운로드를 위한 git 설치 : `pacman -S git`

### 라이브러리 생성
* --host : 산출될 라이브러리가 구동될 툴체인의 prefix(gcc 앞에 붙는 prefix)
* --prefix : 산출물을 담을 디렉터리 경로(사용자 지정)
```bash
$ git clone git://sigrok.org/libserialport
$ cd libserialport
$ ./autogen.sh
$ ./configure --host=x86_64-w64-mingw32 --prefix=$HOME/Documents/dev/external
$ make clean
$ make
$ make install
```

## 설치 : macOS / Ubuntu(Linux)
### 라이브러리 생성 
* --host : gcc 앞에 따로 prefix가 붙지 않음으로 사용 안함.
* --prefix : 산출물을 담을 디렉터리 경로(사용자 지정)
```bash
$ git clone git://sigrok.org/libserialport
$ cd libserialport
$ ./autogen.sh
$ ./configure --prefix=$HOME/Documents/dev/external
$ make clean
$ sudo make
$ sudo make install
```

## 실행 환경(Target OS)이 arm-linux인 라이브러리 생성
### 라이브러리 생성 
* --prefix : 산출물을 담을 디렉터리 경로(여기서는 ~/Documents/dev/external/ARM-Linux 아래 설치)
```bash
$ git clone git://sigrok.org/libserialport
$ cd libserialport
$ ./autogen.sh
```
macOS에서 생성하는 경우 : `CC=arm-unknown-linux-gnueabi-gcc CXX=arm-unknown-linux-gnueabi-c++ ./configure --host=arm-unknown-linux-gnueabi --prefix=$HOME/Documents/dev/external/ARM-Linux`

Ubuntu에서 생성하는 경우 : `CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-c++ ./configure --host=arm-linux --prefix=$HOME/Documents/dev/external/ARM-Linux`

Windows(MinGW)에서 생성하는 경우 : `CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-c++ ./configure --host=arm-linux-gnueabihf --prefix=$HOME/Documents/dev/external/ARM-Linux`

```bash
$ make clean
$ sudo make
$ sudo make install
```

## CMake : 동적 라이브러리 링크 
add_executable 실행파일명 예시 : `application`   
* ${CMAKE_SOURCE_DIR} : 현재 프로젝트 디렉터리
* ${CMAKE_HOST_SYSTEM_NAME} : 현재 CMake가 동작하는 운영체제 명(예시 : Darwin, Linux, Windows), (Darwin = macOS)
* target_link_libraries()는 add_executable() 아래에 위치 시킨다.

```cmake
set(EXTERNAL_LIB_ROOT ${CMAKE_SOURCE_DIR}/external/${CMAKE_HOST_SYSTEM_NAME})
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_LIB_ROOT}/include)
set(EXTERNAL_LIB_DIR ${EXTERNAL_LIB_ROOT}/lib)

include_directories(${EXTERNAL_INCLUDE_DIR})

find_library(
            SERIALPORT
            NAMES serialport
            PATHS ${EXTERNAL_LIB_DIR}
            REQUIRED)

## target_link_libraries()는 add_executable() 아래에 위치 시킨다.
target_link_libraries(application ${SERIALPORT})
```

### Windows에서 동적 라이브러리 사용시 주의
Windows에서 동적 라이브러리를 통해 링크할 경우 external/Windows/bin 아래 <b>libserialport-0.dll</b> 파일이 실행 파일과 같은 곳에 위치하도록 복붙 필요함.

### ARM-Linux 기기에서 동적 라이브러리 사용시 주의
external/ARM-Linux/lib 아래 3개의 파일(<b>libserialport.so, libserialport.so.0, libserialport.so.0.1.0</b>)을 
해당 기기의 `/usr/lib`로 복붙 필요함.

## CMake 정적 라이브러리 링크 : Windows
<b>[주의!] libserialport를 정적 라이브러리로 사용하여 개발된 상용 프로그램 배포시 소스코드 공개가 의무화됨으로 소스코드 공개가 꺼려지면 동적 라이브러리 링크 사용을 추천</b>   
add_executable 실행파일명 예시 : `application`   
* ${CMAKE_SOURCE_DIR} : 현재 프로젝트 디렉터리
* ${CMAKE_HOST_SYSTEM_NAME} : 현재 CMake가 동작하는 운영체제 명(예시 : Darwin, Linux, Windows), (Darwin = macOS)
* target_link_libraries()는 add_executable() 아래에 위치 시킨다.
* Windows의 경우 libserialport를 정적라이브러리로 사용시에 libsetupapi 라이브러리와 함께 사용해야함.

```cmake
set(EXTERNAL_LIB_ROOT ${CMAKE_SOURCE_DIR}/external/${CMAKE_HOST_SYSTEM_NAME})
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_LIB_ROOT}/include)
set(EXTERNAL_LIB_DIR ${EXTERNAL_LIB_ROOT}/lib)

include_directories(${EXTERNAL_INCLUDE_DIR})

find_library(
        SETUPAPI
        NAMES setupapi
        PATHS /usr/lib
)
find_library(
        SERIALPORT
        NAMES libserialport.a
        PATHS ${EXTERNAL_LIB_DIR}
        REQUIRED)

## target_link_libraries()는 add_executable() 아래에 위치 시킨다.
target_link_libraries(application ${SETUPAPI} ${SERIALPORT})
```

## CMake 정적 라이브러리 링크 : macOS / Linux
macOS / Linux의 경우에도 다른 라이브러리(Windows에서 추가했던 libsetupapi)를 추가해야하지만 동적 라이브러리 사용시에 
Windows 처럼 .dll 파일을 실행파일과 같은 위치에 놓을 필요가 없기 때문에 굳이 정적 라이브러리를 사용하지 않고 동적 라이브러리를 사용하였다...
