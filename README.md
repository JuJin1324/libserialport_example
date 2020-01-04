# libserialport example
C언어 serial 통신 라이브러리 
* [공식 페이지](https://sigrok.org/wiki/Libserialport)
* 라이센스(license) 정보
    - LGPL(GNU Lesser General Public License)
    - 동적 링크 사용시 상업에 사용하는 소스 공개 의무 없음
    - 정적 링크 사용시 상업에 사용하는 소스 공개 의무 <b>있음</b>
    - [오픈소스SW 라이선스 LGPL 바로알기](https://openbee.kr/422)

## libserialport 설치
### macOS
* 다운 받을 디렉터리 : ~/Downloads
* 다운로드 : wget
```bash
$ cd ~/Downloads
$ wget http://sigrok.org/download/source/libserialport/libserialport-0.1.1.tar.gz
$ tar -xvf libserialport-0.1.1.tar.gz
$ cd libserialport-0.1.1
```
* macOS 전용 libserialport 라이브러리 생성 명령어
  - 라이브러리 생성할 위치 : `/usr/local`
```bash
$ ./configure 
$ sudo make clean
$ make
$ sudo make install
```

* arm-linux 전용 libserialport 라이브러리 생성 명령어 
 - 라이브러리 생성할 위치 : `/usr/local/arm-linux-libserialport`
```bash
### arm-linux 설치 관련 설정
$ mkdir /usr/local/arm-linux-libserialport
$ CC=arm-unknown-linux-gnueabi-gcc LD=arm-unknown-linux-gnueabi-ld CXX=arm-unknown-linux-gnueabi-c++ NM=arm-unknown-linux-gnueabi-nm OBJDUMP=arm-unknown-linux-gnueabi-objdump RANLIB=arm-unknown-linux-gnueabi-ranlib AR=arm-unknown-linux-gnueabi-ar ./configure --build=x86_64-apple-darwin`uname -r` --host=arm-linux --prefix=/usr/local/arm-linux-libserialport 

### 설치
$ sudo make clean
$ make
$ sudo make install
```

### Ubuntu(Linux)
* 다운로드 후 압축 풀기 및 디렉터리 이동
    - wget을 통한 다운로드
    - /home 디렉터리 아래에서 진행
```bash
$ cd /home
$ wget http://sigrok.org/download/source/libserialport/libserialport-0.1.1.tar.gz
$ tar -xvf libserialport-0.1.1.tar.gz
$ cd libserialport-0.1.1
```

* Ubuntu(Linux) 전용 libserialport 라이브러리 생성 명령어
```bash
### --prefix : 라이브러리 생성 디렉터리 셋팅
$ ./configure --prefix=/usr
$ sudo make clean
$ make
$ sudo make install
```

* arm-linux 전용 libserialport 라이브러리 생성 명령어 
* arm용 log4c 라이브러리를 /usr/libserialport-arm 아래 설치하겠다고 가정
armv7용 라이브러리를 설치할 디렉터리 생성 : $ mkdir /usr/arm-linux-libserialport
```bash
### 라이브러리 생성관련 셋팅 
$ CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-c++ ./configure --build=x86_64-linux-gnu --host=arm-linux --prefix=/usr/arm-linux-libserialport

$ sudo make clean
$ make
$ sudo make install
```
* host(arm-linux)와 Cross Toolchain의 host triplet(arm-unknown-linux)이 일치하지 않아서 CC/CXX/LD/NM/OBJDUMP/RANLIB/AR을 직접 지정해주었다. 우분투의 경우 host(arm-linux)와 Cross Toolchain의 host triplet(arm-linux)로 일치하기 때문에 CC를 제외하고는 따로 지정해줄 필요가 없다.

### libserialport의 configure 파라미터 정리
  - CC : log4c가 동작할 컴파일러 / 예시) `CC=arm-unknown-linux-gnueabi-gcc`
  - CXX : log4c를 CPP 호환성 컴파일러 / 예시) `CXX=arm-unknown-linux-gnueabi-c++`
  - LD : 링커 / 예시) `LD=arm-unknown-linux-gnueabi-ld` 
  - NM : 메타 정보 표시 / 예시) `NM=arm-unknown-linux-gnueabi-nm`
  - OBJDUMP : OBJDUMP / 예시) `OBJDUMP=arm-unknown-linux-gnueabi-objdump` 
  - RANLIB : RANLIB / 예시) `RANLIB=arm-unknown-linux-gnueabi-ranlib`
  - AR : AR / 예시) `AR=arm-unknown-linux-gnueabi-ar`
  - build : 로컬 PC의 커널 정보 / 예시) `--build=x86_64-apple-darwin19.0.0` 
  - host : 타겟 컴파일러의 호스트 정보 / 예시) `--host=arm-linux`
  - prefix : 생성될 라이브러리를 저장할 디렉터리 지정 / 예시) `--prefix=/usr/local/arm-linux-libserialport`
  - without-expat : 모름 / 예시) `--without-expat`

## CMake
### 동적 라이브러리 링크
* add_executable 실행파일 명이 `application.out`으로 가정
* 로컬 x86_64에서 실행시 `target_link_libraries(application.out -L/usr/local/lib serialport)`
* arm-linux 컴파일러를 사용하는 기기에서 해당 라이브러리를 동적으로 사용하기 위해서 로컬(/usr/local/arm-linux-libserialport/lib)에 있는 모든 파일을 해당 기기의 `/usr/lib`로 복사 / 붙여넣기
* arm-linux에서 실행시 `target_link_libraries(application.out -L/usr/local/arm-linux-libserialport/lib serialport)`

### [주의!]정적 라이브러리 링크 
* <b>[주의!] log4c를 정적 라이브러리로 사용하게 되면 소스코드 공개가 의무화됨으로 소스코드 공개가 꺼려지면 동적 라이브러리 링크 사용을 추천</b>
* add_executable 실행파일 명이 `application.out`으로 가정
* arm-linux용 정적 라이브러리인 `libserialport.a`가 `/usr/local/arm-linux-libserialport/lib` 아래 있다고 가정
* `target_link_libraries(application.out /usr/local/arm-linux-libserialport/lib/libserialport.a)`

### 크로스 컴파일 선택
* 다음의 셋팅을 주석 처리 혹은 주석 풀기를 통해서 arm-linux 로 컴파일 할지 x86_64 로 컴파일할지 선택 
    - `set(ARM_LINUX_COMPILE ON)`
