namespace bstd {
    void _putc(char);
    void _puts(const char*);
    char sprintf(void(*putc)(char), void(*puts)(const char*), const char* fmt, ...);
    
}
