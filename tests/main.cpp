extern void test_ostream();
extern void test_outputdebugstring();
extern void test_splitter();
extern void test_stdio();

int main() {
    test_ostream();
#if defined(_WIN32)
    test_outputdebugstring();
#endif
    test_splitter();
    test_stdio();
}
