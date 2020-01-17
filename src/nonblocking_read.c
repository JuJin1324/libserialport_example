#include <stdio.h>
#include <signal.h>  /* for using signal() */
#include <unistd.h>  /* for sleep function */
#include <libserialport.h>
#include <stdlib.h>
#include "utils/serial_utils.h"

#define MILLI_SECOND        (1000)
#define MAX_BUF_LEN         (512)

volatile sig_atomic_t stop_thread;   /* Ctrl+C 스탑 시그널 시 쓰레드 종료를 위한 변수 */

void sig_handler() {
    stop_thread = 1;
    printf("App exited.\n");
}

int main() {
    struct sp_port *port;
    const char desired_port[] = "/dev/ttyS1";
    const int desired_baud_rate = 19200;

    char byte_buff[MAX_BUF_LEN];
    int byte_num = 0;
    int bytes_waiting;

    /*
     * 종료 시그널(Ctrl + C) 처리 변수 및 함수
     * sig_handler 함수에서 stop_thread 변수를 1로 만들어서 아래의 while 문 종료시킬 예정
     */
    stop_thread = 0;
    signal(SIGINT, (void *) sig_handler);

    /* 기기에서 사용 가능 serial 포트 확인 */
    print_available_ports_info();

    /* /dev/ttyS5를 통해서 구조체 struct sp_port 가져오기 */
    enum sp_return error = sp_get_port_by_name(desired_port, &port);
    if (error != SP_OK) {
        printf("Error finding serial device\n");
        return EXIT_FAILURE;
    }

    /* 읽기모드로 포트 오픈 */
    printf("Opening port '%s' \n", desired_port);
    error = sp_open(port, SP_MODE_READ);
    if (error == SP_OK) {
        /* baudrate 설정 */
        sp_set_baudrate(port, desired_baud_rate);
    } else {
        printf("Error opening serial device\n");
        sp_free_port(port);

        return EXIT_FAILURE;
    }

    /* 종료 시그널(Ctrl + C)을 받기 전까지 계속해서 시리얼 통신 */
    while (!stop_thread) {
        usleep(100 * MILLI_SECOND);
        bytes_waiting = sp_input_waiting(port);
        if (bytes_waiting > 0) {
            printf("Bytes waiting %i\n", bytes_waiting);
            /* nonblocking으로 읽기 수행 */
            byte_num = sp_nonblocking_read(port, byte_buff, MAX_BUF_LEN);

            /* print byte_buff */
            parse_serial(byte_buff, byte_num);
        }
        fflush(stdout);
    }
    sp_close(port);
    sp_free_port(port);

    return EXIT_SUCCESS;
}