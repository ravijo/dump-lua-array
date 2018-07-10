#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// read complete binary file
float* readFile(const char *file_name)
{
    printf("reading file %s\n", file_name);
    FILE* f = fopen(file_name, "rb");
    assert(f);

    fseek(f, 0, SEEK_END);
    long bytes = ftell(f);
    fseek(f, 0, SEEK_SET);

    float* data = (float*)malloc(bytes);
    fread(data, bytes, 1, f);
    assert(fclose(f) == 0);

    printf("size= %li\n", bytes);

    return data;
}

int main(int argc, char** argv)
{   
    char *file_name = "out.bin";
    float *data = readFile(file_name);

    printf("%f %f\n", data[0], data[1]);
    return 0;
}
