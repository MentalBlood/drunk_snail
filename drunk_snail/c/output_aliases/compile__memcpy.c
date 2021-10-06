#define compile__memcpy(src_start, src_end) {\
    if ((result_end - result) + (src_end - src_start) + 2 >= buffer_size) {\
        if (!depth)\
            free(result);\
        return NULL;\
    }\
    memcpy(result_end, src_start, sizeof(char) * (src_end - src_start));\
    result_end += src_end - src_start;\
}