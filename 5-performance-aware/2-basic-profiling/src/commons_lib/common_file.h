
static FILE* Open(long long unsigned pairCount, const char* label, const char *extension) {
    char temp[256];
    sprintf(temp, "data_%llu_%s.%s", pairCount, label, extension);
    FILE* result = fopen(temp, "wb");
    if (!result) {
        fprintf(stderr, "Unable to open \"%s\" for writing.\n", temp);
    }
    return result;
}
