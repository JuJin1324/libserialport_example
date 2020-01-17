//
// Created by Ju-Jin Yoo on 2019/12/23.
//

#include <stdio.h>
#include <libserialport.h>

void print_port_transport(enum sp_transport port_transport) {
    printf("Port type : ");

    if (port_transport == SP_TRANSPORT_NATIVE) {
        printf("Native platform serial port.\n");
    } else if (port_transport == SP_TRANSPORT_USB) {
        printf("USB serial port adapter.\n");
    } else {
        printf("Bluetooth serial port adapter.\n");
    }
}

void parse_serial(char *byte_buff, int byte_num) {
    for (int i = 0; i < byte_num; i++) {
        printf("%c", byte_buff[i]);
    }
    printf("\n");
}

/* 기기에서 사용 가능 serial 포트 확인 */
void print_available_ports_info() {
    int i;
    struct sp_port **ports;
    char *serial_number;
    enum sp_transport port_transport;

    enum sp_return error = sp_list_ports(&ports);
    if (error == SP_OK) {
        printf("====== Print List of Available Ports ======\n");
        printf("----------------------------\n");
        for (i = 0; ports[i]; i++) {
            serial_number = sp_get_port_usb_serial(ports[i]);
            port_transport = sp_get_port_transport(ports[i]);

            printf("Found port    : %s\n", sp_get_port_name(ports[i]));
            printf("Serial number : %s\n", serial_number);
            print_port_transport(port_transport);
            printf("----------------------------\n");
        }
        printf("Last error message : %s\n", sp_last_error_message());
        sp_free_port_list(ports);
    } else {
        printf("No serial devices detected\n");
    }
}