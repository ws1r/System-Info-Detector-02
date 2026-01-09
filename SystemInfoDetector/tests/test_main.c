//
// Created by Administrator on 2026/1/5 0005.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../include/system_info.h"
#include "../include/hardware_info.h"
#include "../include/utils.h"

// 测试系统信息获取
void test_system_info() {
    printf("=== 测试系统信息获取 ===\n");

    SystemInfo sys_info;
    get_system_info(&sys_info);

    printf("操作系统: %s\n", sys_info.os_name);
    printf("版本: %s\n", sys_info.os_version);
    printf("架构: %s\n", sys_info.architecture);
    printf("计算机名: %s\n", sys_info.computer_name);
    printf("用户名: %s\n", sys_info.username);
    printf("总内存: %lu MB\n", sys_info.total_ram);
    printf("可用内存: %lu MB\n", sys_info.available_ram);

    // 测试格式化显示
    char formatted[64];
    printf("格式化总内存: %s\n",
           format_bytes(sys_info.total_ram * MB, formatted, sizeof(formatted)));
    printf("格式化可用内存: %s\n",
           format_bytes(sys_info.available_ram * MB, formatted, sizeof(formatted)));

    printf("测试完成\n\n");
}

// 测试硬件信息获取
void test_hardware_info() {
    printf("=== 测试硬件信息获取 ===\n");

    HardwareInfo hw_info;
    get_hardware_info(&hw_info);

    printf("CPU: %s\n", hw_info.cpu_name);
    printf("核心数: %d\n", hw_info.cpu_cores);
    printf("主板: %s\n", hw_info.motherboard);
    printf("BIOS: %s\n", hw_info.bios_version);
    printf("GPU: %s\n", hw_info.gpu_name);
    printf("GPU厂商: %s\n", hw_info.gpu_vendor);

    printf("测试完成\n\n");
}

// 测试工具函数
void test_utils_functions() {
    printf("=== 测试工具函数 ===\n");

    // 测试字符串操作
    char test_str[] = "   Hello World!   ";
    printf("原始字符串: '%s'\n", test_str);
    printf("去除空格: '%s'\n", string_trim(test_str));
    printf("转小写: '%s'\n", string_to_lower(test_str));
    printf("转大写: '%s'\n", string_to_upper(test_str));

    // 测试格式转换
    char buffer[256];
    printf("\n格式转换测试:\n");
    printf("1GB = %s\n", format_bytes(1 * GB, buffer, sizeof(buffer)));
    printf("500MB = %s\n", format_bytes(500 * MB, buffer, sizeof(buffer)));

    // 测试时间格式化
    printf("1天2小时3分钟 = %s\n",
           format_time_interval((24*3600 + 2*3600 + 3*60) * 1000, buffer, sizeof(buffer)));

    // 测试百分比格式化
    printf("0.753 = %s\n", format_percentage(0.753, buffer, sizeof(buffer)));

    printf("测试完成\n\n");
}

// 测试文件操作
void test_file_operations() {
    printf("=== 测试文件操作 ===\n");

    const char* test_file = "test_file.txt";
    const char* test_content = "这是测试文件内容\n第二行\n第三行";

    // 创建测试文件
    FILE* file = fopen(test_file, "w");
    if (file) {
        fputs(test_content, file);
        fclose(file);
        printf("测试文件创建成功\n");
    }

    // 检查文件是否存在
    printf("文件是否存在: %s\n", file_exists(test_file) ? "是" : "否");

    // 获取文件大小
    long size = file_get_size(test_file);
    char formatted[64];
    printf("文件大小: %ld 字节 (%s)\n", size, format_bytes(size, formatted, sizeof(formatted)));

    // 读取文件内容
    char* content = file_read_all(test_file);
    if (content) {
        printf("文件内容:\n%s\n", content);
        free(content);
    }

    // 删除测试文件
    remove(test_file);
    printf("测试完成\n\n");
}

// 测试系统信息
void test_system_utils() {
    printf("=== 测试系统工具函数 ===\n");

    char buffer[256];

    // 测试系统运行时间
    get_system_uptime(buffer, sizeof(buffer));
    printf("系统运行时间: %s\n", buffer);

    // 测试目录获取
    get_windows_directory(buffer, sizeof(buffer));
    printf("Windows目录: %s\n", buffer);

    get_system_directory(buffer, sizeof(buffer));
    printf("系统目录: %s\n", buffer);

    get_temp_directory(buffer, sizeof(buffer));
    printf("临时目录: %s\n", buffer);

    // 测试网络信息
    get_mac_address(buffer, sizeof(buffer));
    printf("MAC地址: %s\n", buffer);

    get_ip_address(buffer, sizeof(buffer));
    printf("IP地址: %s\n", buffer);

    get_hostname(buffer, sizeof(buffer));
    printf("主机名: %s\n", buffer);

    printf("测试完成\n\n");
}

// 测试磁盘信息
void test_disk_info() {
    printf("=== 测试磁盘信息 ===\n");

    uint64_t total, free, used;
    char buffer1[64], buffer2[64], buffer3[64];

    get_disk_space_info("C:", &total, &free, &used);

    printf("C盘信息:\n");
    printf("总空间: %s\n", format_bytes(total, buffer1, sizeof(buffer1)));
    printf("可用空间: %s\n", format_bytes(free, buffer2, sizeof(buffer2)));
    printf("已用空间: %s\n", format_bytes(used, buffer3, sizeof(buffer3)));

    double usage = (double)used / total * 100;
    printf("使用率: %.1f%%\n", usage);

    printf("测试完成\n\n");
}

// 测试性能计时器
void test_performance_timer() {
    printf("=== 测试性能计时器 ===\n");

    PerformanceTimer timer;
    timer_start(&timer);

    // 模拟一些工作
    Sleep(150);  // 睡眠150毫秒

    timer_stop(&timer);

    printf("耗时: %.2f 毫秒\n", timer_get_elapsed_ms(&timer));
    printf("耗时: %.3f 秒\n", timer_get_elapsed_seconds(&timer));

    printf("测试完成\n\n");
}

// 测试颜色输出
void test_color_output() {
    printf("=== 测试颜色输出 ===\n");

    print_color(COLOR_RED, "这是红色文本\n");
    print_color(COLOR_GREEN, "这是绿色文本\n");
    print_color(COLOR_YELLOW, "这是黄色文本\n");
    print_color(COLOR_BLUE, "这是蓝色文本\n");
    print_color(COLOR_MAGENTA, "这是洋红色文本\n");
    print_color(COLOR_CYAN, "这是青色文本\n");
    print_color(COLOR_WHITE, "这是白色文本\n");

    print_header("这是一个标题");

    printf("测试完成\n\n");
}

// 测试进程检查
void test_process_check() {
    printf("=== 测试进程检查 ===\n");

    const char* processes[] = {"explorer.exe", "notepad.exe", "test_process.exe"};

    for (int i = 0; i < 3; i++) {
        const char* proc = processes[i];
        int running = is_process_running(proc);
        int count = get_process_count(proc);

        printf("进程 '%s':\n", proc);
        printf("  是否运行: %s\n", running ? "是" : "否");
        printf("  运行数量: %d\n", count);
    }

    printf("测试完成\n\n");
}

// 主测试函数
int main(int argc, char* argv[]) {
    // 设置控制台编码为UTF-8
    SetConsoleOutputCP(65001);

    printf("========================================\n");
    printf("     系统信息检测工具 - 测试套件       \n");
    printf("========================================\n\n");

    // 运行所有测试
    test_system_info();
    test_hardware_info();
    test_utils_functions();
    test_file_operations();
    test_system_utils();
    test_disk_info();
    test_performance_timer();
    test_color_output();
    test_process_check();

    printf("========================================\n");
    printf("           所有测试完成！              \n");
    printf("========================================\n");

    printf("\n按任意键退出...");
    getchar();

    return 0;
}