//
// Created by Administrator on 2026/1/5 0005.
//

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winreg.h>  // 注册表操作
#include "hardware_info.h"

// 链接库
#pragma comment(lib, "advapi32.lib")

// 获取CPU信息
static void get_cpu_info(HardwareInfo* info) {
    HKEY hKey;
    DWORD dwType, dwSize = sizeof(info->cpu_name);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL,
                             &dwType, (LPBYTE)info->cpu_name, &dwSize) == ERROR_SUCCESS) {
            // 清理CPU名称中的多余空格
            char* p = info->cpu_name;
            while (*p == ' ') p++;
            if (p != info->cpu_name) {
                strcpy(info->cpu_name, p);
            }
        } else {
            strcpy(info->cpu_name, "Unknown CPU");
        }

        RegCloseKey(hKey);
    } else {
        strcpy(info->cpu_name, "Unknown CPU");
    }

    // 获取CPU核心数
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    info->cpu_cores = sysInfo.dwNumberOfProcessors;
}

// 获取BIOS信息
static void get_bios_info(HardwareInfo* info) {
    HKEY hKey;
    DWORD dwType, dwSize = sizeof(info->bios_version);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\BIOS",
                      0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExA(hKey, "BIOSVersion", NULL,
                             &dwType, (LPBYTE)info->bios_version, &dwSize) != ERROR_SUCCESS) {
            strcpy(info->bios_version, "Unknown");
        }

        RegCloseKey(hKey);
    } else {
        strcpy(info->bios_version, "Unknown");
    }
}

void get_hardware_info(HardwareInfo* info) {
    if (!info) return;

    get_cpu_info(info);
    get_bios_info(info);

    // GPU信息（简化版本，实际需要更复杂的检测）
    strcpy(info->gpu_name, "Generic GPU");
    strcpy(info->gpu_vendor, "Microsoft");

    // 主板信息
    strcpy(info->motherboard, "Generic Motherboard");
}

void print_hardware_info(const HardwareInfo* info) {
    if (!info) return;

    printf("========== 硬件信息 ==========\n");
    printf("CPU: %s\n", info->cpu_name);
    printf("CPU核心数: %d\n", info->cpu_cores);
    printf("GPU: %s\n", info->gpu_name);
    printf("GPU厂商: %s\n", info->gpu_vendor);
    printf("主板: %s\n", info->motherboard);
    printf("BIOS版本: %s\n", info->bios_version);
    printf("==============================\n\n");
}
