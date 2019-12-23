//
// Created by Ju-Jin Yoo on 2019/12/23.
//

#include <stdio.h>
#include <libserialport.h>
#include "serial_utils.h"

/* 기기에서 사용 가능 serial 포트 확인 */
void list_ports() {
    int i;
    struct sp_port **ports;

    enum sp_return error = sp_list_ports(&ports);
    if (error == SP_OK) {
        for (i = 0; ports[i]; i++) {
            printf("Found port: '%s'\n", sp_get_port_name(ports[i]));
        }
        sp_free_port_list(ports);
    } else {
        printf("No serial devices detected\n");
    }
    printf("\n");
}

void parse_serial(char *byte_buff, int byte_num) {
    for (int i = 0; i < byte_num; i++) {
        printf("%c", byte_buff[i]);
    }
    printf("\n");
}
