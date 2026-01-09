//
// Created by Administrator on 2026/1/5 0005.
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "system_info.h"
#include "hardware_info.h"

// 显示程序标题
void display_title() {
    printf("\n");
    printf("========================================\n");
    printf("     System Information Detector v1.0   \n");
    printf("     (类似Everest的简化版本)           \n");
    printf("========================================\n");
    printf("\n");
}

// 显示系统时间
void display_system_time() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    printf("检测时间: %04d-%02d-%02d %02d:%02d:%02d\n\n",
           time.wYear, time.wMonth, time.wDay,
           time.wHour, time.wMinute, time.wSecond);
}

// 显示磁盘信息
void display_disk_info() {
    ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;

    printf("========== 磁盘信息 ==========\n");

    if (GetDiskFreeSpaceExA("C:", &freeBytes, &totalBytes, &totalFreeBytes)) {
        printf("C盘总空间: %.2f GB\n",
               totalBytes.QuadPart / (1024.0 * 1024 * 1024));
        printf("C盘可用空间: %.2f GB\n",
               freeBytes.QuadPart / (1024.0 * 1024 * 1024));
        printf("C盘使用率: %.1f%%\n",
               (1 - (double)freeBytes.QuadPart / totalBytes.QuadPart) * 100);
    } else {
        printf("无法获取磁盘信息\n");
    }

    printf("==============================\n\n");
}

// 显示屏幕分辨率
void display_screen_resolution() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    printf("========== 显示信息 ==========\n");
    printf("屏幕分辨率: %dx%d\n", width, height);
    printf("色彩深度: %d位\n", GetDeviceCaps(GetDC(NULL), BITSPIXEL));
    printf("==============================\n\n");
}

int main() {
    // 设置控制台编码为UTF-8
    SetConsoleOutputCP(65001);

    display_title();
    display_system_time();

    // 获取并显示系统信息
    SystemInfo sysInfo;
    get_system_info(&sysInfo);
    print_system_info(&sysInfo);

    // 获取并显示硬件信息
    HardwareInfo hwInfo;
    get_hardware_info(&hwInfo);
    print_hardware_info(&hwInfo);

    // 显示磁盘信息
    display_disk_info();

    // 显示屏幕信息
    display_screen_resolution();

    // 系统运行时间
    DWORD uptime = GetTickCount();
    printf("========== 系统运行时间 ==========\n");
    printf("系统已运行: %lu 小时 %lu 分钟\n",
           uptime / 3600000, (uptime % 3600000) / 60000);
    printf("==================================\n\n");

    printf("按任意键退出...\n");
    getchar();

    return 0;
}