#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 16 * 1024 * 1024
#define IN_SIZES                                                      \
    { 12, 64, 128, 256, 1024, 16 * 1024, 1024 * 1024, 16 * 1024 * 1024, 1}

#define PRINTHASH(n)                                                  \
    printf("    { ");                                                 \
    for (int j = 0; j < n; ++j) {                                     \
        printf("0x%02x, ", out[j]);                                   \
    }                                                                 \
    printf("}\n");

int siphash(const uint8_t *in, const size_t inlen, const uint8_t *k,
            uint8_t *out, const size_t outlen);
int halfsiphash(const uint8_t *in, const size_t inlen, const uint8_t *k,
                uint8_t *out, const size_t outlen);

int run_hash(uint8_t *indata, const int insize, uint8_t *k, uint8_t *out) {
    clock_t clk_pre, clk_post;

    // do the bench
    clk_pre = clock();
    siphash(indata, insize, k, out, 8);
    clk_post = clock();
    return clk_post - clk_pre;
}

int main() {
    static uint8_t in[MAX_SIZE] = {
        0}; // static, to allocate mem from heap, not stack
    uint8_t out[8], k[16];
    int i, j;
    const int sizes[] = IN_SIZES;

    // Create a key
    for (i = 0; i < 16; i++)
        k[i] = i;

    // read random data from 'random.data' into `in`.
    FILE *rnd_src = fopen("random.data", "rb");
    if (rnd_src == NULL) {
        perror("random.data");
        return (1);
    }
    size_t num_read = fread(in, 1, MAX_SIZE, rnd_src);
    if (num_read < MAX_SIZE) {
        fprintf(stderr, "read error\n");
        return (1);
    }
    fclose(rnd_src);

    // Zero the 1st data byte. This way we can verify results.
    in[0] = 0;

    printf("CLOCKS_PER_SEC: %li\n", CLOCKS_PER_SEC);
    for (i = 0; i < sizeof(sizes) / sizeof(*sizes); i++) {

        printf("INPUT SIZE    : %i\nCLOCKS: ", sizes[i]);

        for (j = 0; j < 10; j++) {
            printf(" %i", run_hash(in, sizes[i], k, out));
        }
        printf("\n");
    }
    // The last hash is displayed, just to verify. Should give
    // for message being 0x00 (one byte): 
    //    { 0xfd, 0x67, 0xdc, 0x93, 0xc5, 0x39, 0xf8, 0x74, }
    PRINTHASH(8);
}
