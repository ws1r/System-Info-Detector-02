//
// Created by Administrator on 2026/1/5 0005.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sysinfoapi.h>
#include <lm.h>  // 用于网络用户信息
#include "system_info.h"

// 链接库
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#ifdef _WIN32
#pragma comment(lib, "netapi32.lib")
#endif

void get_system_info(SystemInfo* info) {
    if (!info) return;

    // 操作系统信息
    OSVERSIONINFOEX osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        sprintf(info->os_version, "Windows %lu.%lu",
                osvi.dwMajorVersion, osvi.dwMinorVersion);
    } else {
        strcpy(info->os_version, "Unknown");
    }

    // 检查是否为64位系统
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        strcpy(info->architecture, "x64");
    } else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        strcpy(info->architecture, "x86");
    } else {
        strcpy(info->architecture, "Unknown");
    }

    // 计算机名
    DWORD size = sizeof(info->computer_name);
    GetComputerNameA(info->computer_name, &size);

    // 用户名
    size = sizeof(info->username);
    GetUserNameA(info->username, &size);

    // 内存信息
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        info->total_ram = (unsigned long)(memStatus.ullTotalPhys / (1024 * 1024));
        info->available_ram = (unsigned long)(memStatus.ullAvailPhys / (1024 * 1024));
    } else {
        info->total_ram = 0;
        info->available_ram = 0;
    }

    strcpy(info->os_name, "Microsoft Windows");
}

void print_system_info(const SystemInfo* info) {
    if (!info) return;

    printf("========== 系统信息 ==========\n");
    printf("操作系统: %s\n", info->os_name);
    printf("系统版本: %s\n", info->os_version);
    printf("系统架构: %s\n", info->architecture);
    printf("计算机名: %s\n", info->computer_name);
    printf("用户名: %s\n", info->username);
    printf("总内存: %lu MB\n", info->total_ram);
    printf("可用内存: %lu MB\n", info->available_ram);
    printf("==============================\n\n");
}
