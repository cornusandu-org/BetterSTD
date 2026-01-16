#include <cstdarg>
#include <unistd.h>
#include <cstring>

/* write one char */
static void putc(char c) {
    write(1, &c, 1);
}

/* write string */
static void puts(const char* s) {
    write(1, s, strlen(s));
}

/* int to string (base 10 or 16) */
static void itoa(int value, char* buf, int base) {
    char tmp[32];
    int i = 0;
    unsigned v = value;

    if (base == 10 && value < 0) {
        v = -value;
    }

    do {
        tmp[i++] = "0123456789abcdef"[v % base];
        v /= base;
    } while (v);

    int pos = 0;
    if (base == 10 && value < 0)
        buf[pos++] = '-';

    while (i--)
        buf[pos++] = tmp[i];

    buf[pos] = '\0';
}

/* minimal printf */
int my_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char buf[32];

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            putc(*fmt);
            continue;
        }

        ++fmt; // skip %

        switch (*fmt) {
            case 'd': {
                int v = va_arg(ap, int);
                itoa(v, buf, 10);
                puts(buf);
                break;
            }
            case 'x': {
                int v = va_arg(ap, int);
                itoa(v, buf, 16);
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
                char _m2 = *(fmt + 2);
                fmt += 3;
                if (_m1 == '8' && _m2 == '6') {  // %m86  for 32-bit memory addresses  (WILL TRUNCATE IF NEEDED)

                } else if (_m1 == '6' && _m2 == '4') {  // %m64  for 64-bit memory addresses

                }
            }
            case '%':
                putc('%');
                break;
            default:
                putc('%');
                putc(*fmt);
                break;
        }
    }

    va_end(ap);
    return 0;
}
