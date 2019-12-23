//
// Created by Ju-Jin Yoo on 2019/12/23.
//

#ifndef LIBSERIALPORT_EXAMPLE_SERIAL_UTILS_H
#define LIBSERIALPORT_EXAMPLE_SERIAL_UTILS_H

/* 기기에서 사용 가능 serial 포트 확인 */
void list_ports();
void parse_serial(char *byte_buff, int byte_num);

#endif //LIBSERIALPORT_EXAMPLE_SERIAL_UTILS_H
