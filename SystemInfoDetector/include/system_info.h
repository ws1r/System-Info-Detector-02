//
// Created by Administrator on 2026/1/5 0005.
//

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

typedef struct {
    char os_name[64];
    char os_version[64];
    char architecture[16];
    char computer_name[64];
    char username[64];
    unsigned long total_ram;      // MB
    unsigned long available_ram;  // MB
} SystemInfo;

void get_system_info(SystemInfo* info);
void print_system_info(const SystemInfo* info);
const char* get_windows_version();

#endif // SYSTEM_INFO_H
