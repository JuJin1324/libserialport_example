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

## CMake
### 동적 라이브러리 링크
add_executable 실행파일 명이 `application.out`으로 가정

로컬 x86_64에서 실행시 
```cmake
target_link_libraries(application.out -L/usr/local/lib serialport)
```

* arm-linux 컴파일러를 사용하는 기기에서 해당 라이브러리를 동적으로 사용하기 위해서 로컬(/usr/local/arm-linux-libserialport/lib)에 있는 모든 파일을 해당 기기의 `/usr/lib`로 복사 / 붙여넣기
* arm-linux에서 실행시 
```cmake
target_link_libraries(application.out -L/usr/local/arm-linux-libserialport/lib serialport)
```

### [주의!]정적 라이브러리 링크 
<b>[주의!] log4c를 정적 라이브러리로 사용하게 되면 소스코드 공개가 의무화됨으로 소스코드 공개가 꺼려지면 동적 라이브러리 링크 사용을 추천</b>
* add_executable 실행파일 명이 `application.out`으로 가정
* arm-linux용 정적 라이브러리인 `libserialport.a`가 `/usr/local/arm-linux-libserialport/lib` 아래 있다고 가정
```cmake
target_link_libraries(application.out /usr/local/arm-linux-libserialport/lib/libserialport.a)
```

### 크로스 컴파일 선택
다음의 셋팅을 주석 처리 혹은 주석 풀기를 통해서 arm-linux 로 컴파일 할지 x86_64 로 컴파일할지 선택 
```cmake
set(ARM_LINUX_COMPILE ON)
```
