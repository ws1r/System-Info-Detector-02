#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

// 先声明Windows.h但不包含，避免重复包含问题
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

// 颜色定义（Windows控制台）
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

// 文件大小转换
#define KB 1024
#define MB (1024 * KB)
#define GB (1024 * MB)
#define TB (1024 * GB)

// 内存管理
#define SAFE_FREE(ptr) \
    do { \
        if (ptr != NULL) { \
            free(ptr); \
            ptr = NULL; \
        } \
    } while(0)

// 错误处理
#define CHECK_NULL(ptr, msg) \
    if (ptr == NULL) { \
        fprintf(stderr, "错误: %s (文件: %s, 行: %d)\n", msg, __FILE__, __LINE__); \
        return; \
    }

#define CHECK_NULL_RETURN(ptr, msg, ret_val) \
    if (ptr == NULL) { \
        fprintf(stderr, "错误: %s (文件: %s, 行: %d)\n", msg, __FILE__, __LINE__); \
        return ret_val; \
    }

// 数据类型别名
typedef unsigned char byte;
typedef int bool;
#define TRUE 1
#define FALSE 0

// 字符串操作
char* string_trim(char* str);
char* string_to_lower(char* str);
char* string_to_upper(char* str);
int string_contains(const char* str, const char* substr);
char* string_replace(char* str, const char* old, const char* new_str);

// 文件操作
long file_get_size(const char* filename);
int file_exists(const char* filename);
int directory_exists(const char* path);
int create_directory(const char* path);
char* file_read_all(const char* filename);

// 系统工具
void get_current_time(char* buffer, size_t buffer_size);
void print_color(const char* color, const char* format, ...);
void print_header(const char* title);
void print_separator();
void clear_screen();
void pause_execution();
int get_screen_width();
int get_screen_height();

// 格式转换
char* format_bytes(uint64_t bytes, char* buffer, size_t buffer_size);
char* format_time_interval(unsigned long milliseconds, char* buffer, size_t buffer_size);
char* format_temperature(double celsius, char* buffer, size_t buffer_size);
char* format_percentage(double value, char* buffer, size_t buffer_size);

// 数学工具
double clamp(double value, double min, double max);
int random_int(int min, int max);
double random_double(double min, double max);
int round_to_int(double value);

// 性能测量
typedef struct {
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    LARGE_INTEGER frequency;
} PerformanceTimer;

void timer_start(PerformanceTimer* timer);
void timer_stop(PerformanceTimer* timer);
double timer_get_elapsed_ms(const PerformanceTimer* timer);
double timer_get_elapsed_seconds(const PerformanceTimer* timer);

// 注册表操作
int registry_read_string(HKEY hive, const char* key_path,
                         const char* value_name, char* buffer, size_t buffer_size);
int registry_read_dword(HKEY hive, const char* key_path,
                        const char* value_name, DWORD* value);
int registry_get_subkeys(HKEY hive, const char* key_path,
                         char** subkeys, int max_count);

// 进程和系统
int is_process_running(const char* process_name);
int get_process_count(const char* process_name);
void get_system_uptime(char* buffer, size_t buffer_size);
void get_windows_directory(char* buffer, size_t buffer_size);
void get_system_directory(char* buffer, size_t buffer_size);
void get_temp_directory(char* buffer, size_t buffer_size);

// 硬件相关
int get_cpu_usage();
int get_memory_usage();
void get_disk_space_info(const char* drive,
                         uint64_t* total,
                         uint64_t* free,
                         uint64_t* used);

// 网络相关
void get_mac_address(char* buffer, size_t buffer_size);
void get_ip_address(char* buffer, size_t buffer_size);
void get_hostname(char* buffer, size_t buffer_size);

#endif // UTILS_H