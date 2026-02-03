namespace bstd {
    extern void _putc(char);
    extern void _puts(const char*);
    extern char sprintf(void(*putc)(char), void(*puts)(const char*), const char* fmt, ...);
    
}
