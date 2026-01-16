#include <cstdarg>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include <stdlib.h>
#include <vector>
#include "bstd/re_printf.hpp"
#include "bstd/memory.hpp"

using namespace bstd::mem;

/* write one char */
static void _putc(char c) {
    write(1, &c, 1);
}

/* write string */
static void _puts(const char* s) {
    write(1, s, strlen(s));
}

/* int to string (base 10 or 16) */
static void c_itoa(intptr_t value, char* buf, int base) {
    char tmp[32];
    intptr_t i = 0;
    unsigned v = value;

    if (base == 10 && value < 0) {
        v = -value;
    }

    do {
        tmp[i++] = "0123456789abcdef"[v % base];
        v /= base;
    } while (v);

    intptr_t pos = 0;
    if (base == 10 && value < 0)
        buf[pos++] = '-';

    while (i--)
        buf[pos++] = tmp[i];

    buf[pos] = '\0';
}

/* minimal printf */
namespace bstd {
char core_printf(void(*putc)(char), void(*puts)(const char*), const char* fmt, va_list ap) {
    char buf[256];

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            putc(*fmt);
            continue;
        }

        ++fmt; // skip %

        switch (*fmt) {
            case 'd': {
                int v = va_arg(ap, int);
                c_itoa(v, buf, 10);
                puts(buf);
                break;
            }
            case 'x': {
                int v = va_arg(ap, int);
                c_itoa(v, buf, 16);
                puts(buf);
                break;
            }
            case 's': {
                const char* s = va_arg(ap, const char*);
                puts(s ? s : "(null)");
                break;
            }
            case 'c': {
                char c = (char)va_arg(ap, int);
                putc(c);
                break;
            }
            case 'm': {
                char _m1 = *(fmt + 1);
                if (_m1 == '\0') return 1;
                char _m2 = *(fmt + 2);
                if (_m2 == '\0') return 1;
                fmt += 2;
                if (_m1 == '8' && _m2 == '6') {  // %m86  for 32-bit memory addresses  (WILL TRUNCATE IF NEEDED)
                    // usage: printf("%m86", sizeof(address), address)
                    uint32_t size = va_arg(ap, uint32_t);
                    uint32_t address;
                    if (size == 4) {
                        address = va_arg(ap, uint32_t);
                    } else {
                        address = static_cast<uint32_t>(va_arg(ap, uint64_t));
                    }
                    buf[0] = '0';
                    buf[1] = 'x';
                    c_itoa(address, buf+2, 16);

                } else if (_m1 == '6' && _m2 == '4') {  // %m64  for 64-bit memory addresses
                    // usage: printf("%m64", address)

                    uint64_t address = va_arg(ap, uint64_t);

                    buf[0] = '0';
                    buf[1] = 'x';
                    c_itoa(address, buf+2, 16);
                } else {
                    mempcpy(buf, "0x????????????", strlen("0x????????????"));
                }
                puts(buf);
                break;
            }
            case '%':
                putc('%');
                break;
            default:
                putc('%');
                putc(*fmt);
                break;
        }
    
        memset(buf, 0, sizeof(buf));
    }

    return 0;
}

char printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char r = core_printf(::_putc, ::_puts, fmt, ap);
    va_end(ap);
    return r;
}


// APPARENTLY C++ (GNU) DOES NOT SUPPORT NESTED FUNCTIONS THROUGH TRAMPOLINES
//char sprintf(char* buf, const char* fmt, ...) {
//    va_list ap;
//    va_start(ap, fmt);
//    size_t counter = 0;
//    void writecbuf(char a) {
//        buf[counter] = a;
//        buf[counter+1] = 0;
//        counter++;
//    }
//    void writesbuf(char* a) {
//        memcpy(buf+counter, a, strlen(a) + '\0');
//        counter++;
//    }
//    char r = core_printf(writecbuf, writesbuf, fmt, ap);
//    va_end(ap);
//    return r;
//}

}
