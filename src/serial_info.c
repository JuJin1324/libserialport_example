//
// Created by jujin on 2020-02-08.
//

#include <stdlib.h>

#include <utils/serial_utils.h>

int main() {

    /* 기기에서 사용 가능 serial 포트 확인 */
    print_available_ports_info();

    return EXIT_SUCCESS;
}