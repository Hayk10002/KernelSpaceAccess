#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <algorithm>
#include <random>
#include <format>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#endif

#if defined(_WIN32) && defined(_MSC_VER)    
// ----------------------------------
// Windows SEH version
// ----------------------------------
std::optional<unsigned char> val_at_address(unsigned char* addr) {
    __try {
        return *addr;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return std::nullopt;
    }
}
#else
// ----------------------------------
// POSIX signal-safe version
// ----------------------------------
sigjmp_buf jump_buffer;

void signal_handler(int sig) {
    siglongjmp(jump_buffer, 1);
}

void install_signal_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    sigaction(SIGSEGV, &sa, nullptr);
}

std::optional<unsigned char> val_at_address(unsigned char* addr) {
    install_signal_handler();

    if (sigsetjmp(jump_buffer, 1) == 0) return *addr;
    else                                return std::nullopt;
}
#endif

#define REP_1(e) e
#define REP_2(e) REP_1(e), REP_1(e)
#define REP_4(e) REP_2(e), REP_2(e)
#define REP_8(e) REP_4(e), REP_4(e)
#define REP_16(e) REP_8(e), REP_8(e)
#define REP_32(e) REP_16(e), REP_16(e)
#define REP_64(e) REP_32(e), REP_32(e)
#define REP_96(e) REP_64(e), REP_32(e)
#define REP_98(e) REP_96(e), REP_2(e)

int generateRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

int global_init = 10;
int global_init2[98] = { REP_98((int)0x0102030405060708) };
int global_init3 = (int)0x0807060504030201;

int global_non_init[300];

int main() {

    int local = 20;
    int local2[98] = { REP_98((int)0x090a0b0c0d0e0f10) };
    int local3 = (int)0x100f0e0d0c0b0a09;

    std::generate(global_non_init, global_non_init + 300, []() {
        static int i = 0;
        return i++;
    });

    unsigned char *local_start = (unsigned char*)&(local2[generateRandomNumber(0, 97)]), *local_end = local_start;
    unsigned char *global_init_start = (unsigned char*)&(global_init2[generateRandomNumber(0, 97)]), *global_init_end = global_init_start;
    unsigned char *global_non_init_start = (unsigned char*)&(global_non_init[generateRandomNumber(0, 299)]), *global_non_init_end = global_non_init_start;

    while (auto val = val_at_address((unsigned char*)local_start))           local_start--;
    while (auto val = val_at_address((unsigned char*)local_end))             local_end++;
    while (auto val = val_at_address((unsigned char*)global_init_start))     global_init_start--;
    while (auto val = val_at_address((unsigned char*)global_init_end))       global_init_end++;
    while (auto val = val_at_address((unsigned char*)global_non_init_start)) global_non_init_start--;
    while (auto val = val_at_address((unsigned char*)global_non_init_end))   global_non_init_end++;

    std::cout << std::format("local start estimated:            {}\n" ,(void*)++local_start);
    std::cout << std::format("local end estimated:              {}\n" ,(void*)--local_end);
    std::cout << std::format("global init start estimated:      {}\n" ,(void*)++global_init_start);
    std::cout << std::format("global init end estimated:        {}\n" ,(void*)--global_init_end);
    std::cout << std::format("global non-init start estimated:  {}\n" ,(void*)++global_non_init_start);
    std::cout << std::format("global non-init end estimated:    {}\n" ,(void*)--global_non_init_end);
    
    return 0;
}
