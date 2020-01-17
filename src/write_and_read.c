//
// Created by Ju-Jin Yoo on 2019/12/23.
//

#include <stdio.h>
#include <libserialport.h>
#include <stdlib.h>
#include <string.h>
#include "utils/serial_utils.h"

#define AT_CMD_GET_PHONE_NUMBER        "AT+CNUM\r"
#define MAX_BUF_LEN        (512)

int main() {
    struct sp_port *port;
    const char desired_port[] = "/dev/ttyS1";
    const int desired_baudrate = 115200;
    const unsigned int serial_timeout = 100;   /* milli second */

    char write_buff[MAX_BUF_LEN];
    int write_num = 0;
    char read_buff[MAX_BUF_LEN];
    int read_num = 0;

    /* 기기에서 사용 가능 serial 포트 확인 */
    print_available_ports_info();

    /* /dev/ttyS5를 통해서 구조체 struct sp_port 가져오기 */
    enum sp_return error = sp_get_port_by_name(desired_port, &port);
    if (error != SP_OK) {
        printf("Error finding serial device\n");
        return EXIT_FAILURE;
    }

    /* 읽기&쓰기 모드로 포트 오픈 */
    error = sp_open(port, SP_MODE_READ_WRITE);
    if (error == SP_OK) {
        /* baudrate 설정 */
        sp_set_baudrate(port, desired_baudrate);
    } else {
        printf("Error opening serial device\n");
        sp_free_port(port);

        return EXIT_FAILURE;
    }
    printf("Opened serial port : %s\n", sp_get_port_description(port));

//    strcpy(write_buff, "Command you want to send to serial device");
    strcpy(write_buff, AT_CMD_GET_PHONE_NUMBER);
    /* 종료 시그널(Ctrl + C)을 받기 전까지 계속해서 시리얼 통신 */
    /* blocking으로 쓰기 수행 */
    write_num = sp_blocking_write(port, write_buff, MAX_BUF_LEN, serial_timeout);
    if (write_num < 1)
        printf("Error write to serial device : %d\n", write_num);
    /* blocking으로 읽기 수행 */
    read_num = sp_blocking_read(port, read_buff, MAX_BUF_LEN, serial_timeout);
    /* print read_buff */
    if (read_num > 0)
        parse_serial(read_buff, read_num);

    sp_close(port);
    sp_free_port(port);

    return EXIT_SUCCESS;
}