#include <stddef.h>

int LLVMFuzzerRunDriver(int *argc, char ***argv, int (*test_one_input)(const unsigned char *data, size_t size));
int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size);

int main(int argc, char **argv)
{
    return LLVMFuzzerRunDriver(&argc, &argv, LLVMFuzzerTestOneInput);
}
