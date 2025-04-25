#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#endif

constexpr size_t STEP = 64;

#if defined(_WIN32) && defined(_MSC_VER)
// ----------------------------------
// Windows SEH version
// ----------------------------------
void scan_memory_windows(uintptr_t start, uintptr_t end) {
    std::cout << "Scanning memory (Windows SEH) from 0x" << std::hex << start
              << " to 0x" << end << "...\n";

    for (uintptr_t addr = start; addr < end; addr += STEP) {
        __try {
            volatile unsigned char val = *reinterpret_cast<unsigned char*>(addr);
            std::cout << "0x" << std::hex << addr << ": " << std::dec << (int)val << "\n";
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            std::cout << "Access violation at 0x" << std::hex << addr << "\n";
        }
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

void scan_memory_posix(uintptr_t start, uintptr_t end) {
    install_signal_handler();
    std::cout << "Scanning memory (POSIX) from 0x" << std::hex << start
              << " to 0x" << end << "...\n";

    for (uintptr_t addr = start; addr < end; addr += STEP) {
        if (sigsetjmp(jump_buffer, 1) == 0) {
            volatile unsigned char val = *reinterpret_cast<unsigned char*>(addr);
            std::cout << "0x" << std::hex << addr << ": " << std::dec << (int)val << "\n";
        } else {
            std::cout << "Access violation at 0x" << std::hex << addr << "\n";
        }
    }
}
#endif

int global_var = 42;

int main() {
    int stack_var = 0;
    void* heap_ptr = malloc(1);
    uintptr_t stack_addr = reinterpret_cast<uintptr_t>(&stack_var);
    uintptr_t heap_addr = reinterpret_cast<uintptr_t>(heap_ptr);
    free(heap_ptr);

    uintptr_t start = std::min(stack_addr, heap_addr);
    uintptr_t end   = std::max(stack_addr, heap_addr);

#if defined(_WIN32)
    scan_memory_windows(start, end);
#else
    scan_memory_posix(start, end);
#endif

    return 0;
}
