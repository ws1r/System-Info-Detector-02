//
// Created by Administrator on 2026/1/5 0005.
//

#ifndef HARDWARE_INFO_H
#define HARDWARE_INFO_H

typedef struct {
    char cpu_name[128];
    int cpu_cores;
    char gpu_name[128];
    char gpu_vendor[64];
    char motherboard[128];
    char bios_version[64];
} HardwareInfo;

void get_hardware_info(HardwareInfo* info);
void print_hardware_info(const HardwareInfo* info);

#endif // HARDWARE_INFO_H