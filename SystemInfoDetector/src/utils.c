#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <direct.h>
#include <io.h>

// 调整头文件顺序：winsock2.h 必须在 windows.h 之前
#include <winsock2.h>
#include <windows.h>
#include <winreg.h>
#include <tlhelp32.h>  // 添加这个头文件，用于进程枚举
#include <pdh.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include "utils.h"

// 注意：这里的库链接顺序也需要调整
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
// ==================== 字符串操作 ====================

char* string_trim(char* str) {
    if (str == NULL) return NULL;

    char* end;

    // 去除前导空格
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    // 去除尾部空格
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

char* string_to_lower(char* str) {
    if (str == NULL) return NULL;

    for (char* p = str; *p; p++) {
        *p = tolower(*p);
    }

    return str;
}

char* string_to_upper(char* str) {
    if (str == NULL) return NULL;

    for (char* p = str; *p; p++) {
        *p = toupper(*p);
    }

    return str;
}

int string_contains(const char* str, const char* substr) {
    if (str == NULL || substr == NULL) return 0;
    return strstr(str, substr) != NULL;
}

char* string_replace(char* str, const char* old, const char* new_str) {
    static char buffer[4096];
    char* p;

    if (!str || !old || !new_str) return NULL;

    if (!(p = strstr(str, old))) return str;

    strncpy(buffer, str, p - str);
    buffer[p - str] = '\0';

    sprintf(buffer + (p - str), "%s%s", new_str, p + strlen(old));

    return buffer;
}

// ==================== 文件操作 ====================

long file_get_size(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int file_exists(const char* filename) {
    return _access(filename, 0) == 0;
}

int directory_exists(const char* path) {
    DWORD attrib = GetFileAttributesA(path);
    return (attrib != INVALID_FILE_ATTRIBUTES &&
            (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

int create_directory(const char* path) {
    return _mkdir(path) == 0;
}

char* file_read_all(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t read = fread(buffer, 1, size, file);
    buffer[read] = '\0';

    fclose(file);
    return buffer;
}

// ==================== 系统工具 ====================

void get_current_time(char* buffer, size_t buffer_size) {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void print_color(const char* color, const char* format, ...) {
    printf("%s", color);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(COLOR_RESET);
}

void print_header(const char* title) {
    printf("\n");
    print_color(COLOR_CYAN, "╔══════════════════════════════════════════════════════════╗\n");
    print_color(COLOR_CYAN, "║ %-60s ║\n", title);
    print_color(COLOR_CYAN, "╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_separator() {
    printf("------------------------------------------------------------\n");
}

void clear_screen() {
    system("cls");
}

void pause_execution() {
    printf("\n按任意键继续...");
    getchar();
}

int get_screen_width() {
    return GetSystemMetrics(SM_CXSCREEN);
}

int get_screen_height() {
    return GetSystemMetrics(SM_CYSCREEN);
}

// ==================== 格式转换 ====================

char* format_bytes(uint64_t bytes, char* buffer, size_t buffer_size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double value = (double)bytes;

    while (value >= 1024 && unit_index < 4) {
        value /= 1024;
        unit_index++;
    }

    snprintf(buffer, buffer_size, "%.2f %s", value, units[unit_index]);
    return buffer;
}

char* format_time_interval(unsigned long milliseconds, char* buffer, size_t buffer_size) {
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;

    if (days > 0) {
        snprintf(buffer, buffer_size, "%lu天 %lu小时 %lu分钟",
                 days, hours % 24, minutes % 60);
    } else if (hours > 0) {
        snprintf(buffer, buffer_size, "%lu小时 %lu分钟 %lu秒",
                 hours, minutes % 60, seconds % 60);
    } else if (minutes > 0) {
        snprintf(buffer, buffer_size, "%lu分钟 %lu秒",
                 minutes, seconds % 60);
    } else {
        snprintf(buffer, buffer_size, "%lu秒", seconds);
    }

    return buffer;
}

char* format_temperature(double celsius, char* buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "%.1f°C (%.1f°F)",
             celsius, celsius * 9/5 + 32);
    return buffer;
}

char* format_percentage(double value, char* buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "%.1f%%", value * 100);
    return buffer;
}

// ==================== 数学工具 ====================

double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

int round_to_int(double value) {
    return (int)(value + 0.5);
}

// ==================== 性能测量 ====================

void timer_start(PerformanceTimer* timer) {
    QueryPerformanceFrequency(&timer->frequency);
    QueryPerformanceCounter(&timer->start);
}

void timer_stop(PerformanceTimer* timer) {
    QueryPerformanceCounter(&timer->end);
}

double timer_get_elapsed_ms(const PerformanceTimer* timer) {
    return ((double)(timer->end.QuadPart - timer->start.QuadPart) * 1000.0) /
           (double)timer->frequency.QuadPart;
}

double timer_get_elapsed_seconds(const PerformanceTimer* timer) {
    return (double)(timer->end.QuadPart - timer->start.QuadPart) /
           (double)timer->frequency.QuadPart;
}

// ==================== 注册表操作 ====================

int registry_read_string(HKEY hive, const char* key_path,
                         const char* value_name, char* buffer, size_t buffer_size) {
    HKEY hKey;
    DWORD dwType, dwSize = buffer_size;

    if (RegOpenKeyExA(hive, key_path, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return 0;
    }

    int result = RegQueryValueExA(hKey, value_name, NULL, &dwType,
                                  (LPBYTE)buffer, &dwSize);

    RegCloseKey(hKey);

    return result == ERROR_SUCCESS && dwType == REG_SZ;
}

int registry_read_dword(HKEY hive, const char* key_path,
                        const char* value_name, DWORD* value) {
    HKEY hKey;
    DWORD dwType, dwSize = sizeof(DWORD);

    if (RegOpenKeyExA(hive, key_path, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return 0;
    }

    int result = RegQueryValueExA(hKey, value_name, NULL, &dwType,
                                  (LPBYTE)value, &dwSize);

    RegCloseKey(hKey);

    return result == ERROR_SUCCESS && dwType == REG_DWORD;
}

int registry_get_subkeys(HKEY hive, const char* key_path,
                         char** subkeys, int max_count) {
    HKEY hKey;
    int count = 0;

    if (RegOpenKeyExA(hive, key_path, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return 0;
    }

    char key_name[256];
    DWORD key_name_size;
    DWORD index = 0;

    while (count < max_count) {
        key_name_size = sizeof(key_name);
        if (RegEnumKeyExA(hKey, index, key_name, &key_name_size,
                          NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            subkeys[count] = _strdup(key_name);
            if (subkeys[count] == NULL) break;
            count++;
            index++;
        } else {
            break;
        }
    }

    RegCloseKey(hKey);
    return count;
}

// ==================== 进程和系统 ====================

int is_process_running(const char* process_name) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    int found = 0;
    if (Process32First(hSnapshot, &pe32)) {
        do {
            // 注意：pe32.szExeFile 是一个字符数组，不是指针
            if (_stricmp(pe32.szExeFile, process_name) == 0) {
                found = 1;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return found;
}

int get_process_count(const char* process_name) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    int count = 0;
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, process_name) == 0) {
                count++;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return count;
}

void get_system_uptime(char* buffer, size_t buffer_size) {
    DWORD uptime = GetTickCount();
    format_time_interval(uptime, buffer, buffer_size);
}

void get_windows_directory(char* buffer, size_t buffer_size) {
    GetWindowsDirectoryA(buffer, (UINT)buffer_size);
}

void get_system_directory(char* buffer, size_t buffer_size) {
    GetSystemDirectoryA(buffer, (UINT)buffer_size);
}

void get_temp_directory(char* buffer, size_t buffer_size) {
    GetTempPathA((DWORD)buffer_size, buffer);
}

// ==================== 硬件相关 ====================

int get_cpu_usage() {
    static PDH_HQUERY cpu_query = NULL;
    static PDH_HCOUNTER cpu_counter = NULL;

    if (cpu_query == NULL) {
        PdhOpenQuery(NULL, 0, &cpu_query);
        PdhAddCounterA(cpu_query, "\\Processor(_Total)\\% Processor Time", 0, &cpu_counter);
        PdhCollectQueryData(cpu_query);
        Sleep(1000);
    }

    PDH_FMT_COUNTERVALUE counter_value;
    PdhCollectQueryData(cpu_query);

    if (PdhGetFormattedCounterValue(cpu_counter, PDH_FMT_DOUBLE, NULL, &counter_value) == ERROR_SUCCESS) {
        return (int)counter_value.doubleValue;
    }

    return 0;
}

int get_memory_usage() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) {
        double usage = 1.0 - ((double)memStatus.ullAvailPhys / (double)memStatus.ullTotalPhys);
        return (int)(usage * 100);
    }

    return 0;
}

void get_disk_space_info(const char* drive,
                         uint64_t* total,
                         uint64_t* free,
                         uint64_t* used) {
    ULARGE_INTEGER free_bytes, total_bytes, total_free_bytes;

    if (GetDiskFreeSpaceExA(drive, &free_bytes, &total_bytes, &total_free_bytes)) {
        *total = total_bytes.QuadPart;
        *free = free_bytes.QuadPart;
        *used = total_bytes.QuadPart - free_bytes.QuadPart;
    } else {
        *total = *free = *used = 0;
    }
}

// ==================== 网络相关 ====================

void get_mac_address(char* buffer, size_t buffer_size) {
    IP_ADAPTER_INFO adapter_info[16];
    DWORD buffer_size2 = sizeof(adapter_info);

    if (GetAdaptersInfo(adapter_info, &buffer_size2) == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO adapter = adapter_info;
        if (adapter != NULL) {
            snprintf(buffer, buffer_size, "%02X:%02X:%02X:%02X:%02X:%02X",
                     adapter->Address[0], adapter->Address[1],
                     adapter->Address[2], adapter->Address[3],
                     adapter->Address[4], adapter->Address[5]);
            return;
        }
    }

    strncpy(buffer, "未知", buffer_size);
}

void get_ip_address(char* buffer, size_t buffer_size) {
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        strncpy(buffer, "未知", buffer_size);
        return;
    }

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    struct hostent* host = gethostbyname(hostname);
    if (host != NULL && host->h_addr_list[0] != NULL) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
        strncpy(buffer, inet_ntoa(addr), buffer_size);
    } else {
        strncpy(buffer, "未知", buffer_size);
    }

    WSACleanup();
}

void get_hostname(char* buffer, size_t buffer_size) {
    gethostname(buffer, (int)buffer_size);
}