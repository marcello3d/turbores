#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/turbores.h"

#define DONT_CHECK -1

typedef struct TestCase {
    const char *name;
    const char *packet_path;
    const char *reference_path;
    uint32_t bit_depth;
    uint32_t visible_width;
    uint32_t visible_height;
    uint32_t coded_width;
    uint32_t coded_height;
    uint32_t pixel_format;
    int32_t aspect_ratio_num;
    int32_t aspect_ratio_den;
    int32_t color_primaries;
    int32_t color_transfer;
    int32_t color_matrix;
    uint32_t scan_type;
    size_t frame_data_size;
} TestCase;

static const TestCase test_cases[] = {
    {
        .name = "Full HD 422 frame",
        .packet_path = "tests/public/buck-bunny.prores",
        .reference_path = "build/test-references/buck-bunny.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = 1,
        .aspect_ratio_den = 1,
        .color_primaries = 1,
        .color_transfer = 1,
        .color_matrix = 1,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
    {
        .name = "1904-wide frame",
        .packet_path = "tests/public/buck-bunny-1904.prores",
        .reference_path = "build/test-references/buck-bunny-1904.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 2 * 2,
    },
    {
        .name = "444 frame",
        .packet_path = "tests/public/buck-bunny-444.prores",
        .reference_path = "build/test-references/buck-bunny-444.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 3 * 2,
    },
    {
        .name = "Transparent frame",
        .packet_path = "tests/public/transparent.prores",
        .reference_path = "build/test-references/transparent.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444AP10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = 2,
        .color_transfer = 2,
        .color_matrix = 2,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 4 * 2,
    },
    {
        .name = "12-bit transparent frame",
        .packet_path = "tests/public/4444-12bit.prores",
        .reference_path = "build/test-references/4444-12bit.framedata",
        .bit_depth = 12,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444AP12,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 4 * 2,
    },
    {
        .name = "Interlaced frame",
        .packet_path = "tests/public/interlaced-buck-bunny.prores",
        .reference_path = "build/test-references/interlaced-buck-bunny.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_INTERLACED_TOP_FIELD_FIRST,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
    {
        .name = "HDR 422 frame",
        .packet_path = "tests/public/hdr-422.prores",
        .reference_path = "build/test-references/hdr-422.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = 1,
        .aspect_ratio_den = 1,
        .color_primaries = 9,
        .color_transfer = 18,
        .color_matrix = 9,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
};

#define CHECK_EQ(actual, expected, what) \
    do { \
        long long check_actual = (long long) (actual); \
        long long check_expected = (long long) (expected); \
        if (check_actual != check_expected) { \
            fprintf( \
                stderr, \
                "FAILED: %s (threads %u): %s was %lld, expected %lld\n", \
                test_case->name, threads, what, check_actual, check_expected \
            ); \
            return 1; \
        } \
    } while (0)

static uint8_t *readFile(const char *path, size_t *size) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open %s\n", path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    *size = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *data = malloc(*size);
    if (fread(data, 1, *size, file) != *size) {
        fprintf(stderr, "Failed to read %s\n", path);
        exit(1);
    }

    fclose(file);
    return data;
}

static int runCase(const TestCase *test_case, uint32_t threads) {
    size_t packet_size;
    uint8_t *packet = readFile(test_case->packet_path, &packet_size);

    size_t reference_size;
    uint8_t *reference = readFile(test_case->reference_path, &reference_size);

    TurboresDecoder *decoder = turbores_decoder_create(threads, test_case->bit_depth, TURBORES_ALL_PIXEL_FORMATS);
    TurboresFrame *frame = turbores_frame_create();

    int32_t result = turbores_decode(decoder, frame, packet, packet_size);
    if (result != 0) {
        const uint8_t *message = turbores_decoder_error_message_ptr(decoder);
        fprintf(
            stderr,
            "FAILED: %s (threads %u): turbores_decode returned %d: %.*s\n",
            test_case->name,
            threads,
            result,
            (int) turbores_decoder_error_message_size(decoder),
            message ? (const char *) message : ""
        );
        return 1;
    }

    CHECK_EQ(turbores_frame_visible_width(frame), test_case->visible_width, "visible width");
    CHECK_EQ(turbores_frame_visible_height(frame), test_case->visible_height, "visible height");
    CHECK_EQ(turbores_frame_coded_width(frame), test_case->coded_width, "coded width");
    CHECK_EQ(turbores_frame_coded_height(frame), test_case->coded_height, "coded height");
    CHECK_EQ(turbores_frame_pixel_format(frame), test_case->pixel_format, "pixel format");
    CHECK_EQ(turbores_frame_original_pixel_format(frame), test_case->pixel_format, "original pixel format");
    CHECK_EQ(turbores_frame_scan_type(frame), test_case->scan_type, "scan type");

    if (test_case->aspect_ratio_num != DONT_CHECK) {
        CHECK_EQ(turbores_frame_aspect_ratio_num(frame), test_case->aspect_ratio_num, "aspect ratio numerator");
        CHECK_EQ(turbores_frame_aspect_ratio_den(frame), test_case->aspect_ratio_den, "aspect ratio denominator");
    }
    if (test_case->color_primaries != DONT_CHECK) {
        CHECK_EQ(turbores_frame_color_primaries(frame), test_case->color_primaries, "color primaries");
        CHECK_EQ(turbores_frame_color_transfer(frame), test_case->color_transfer, "color transfer");
        CHECK_EQ(turbores_frame_color_matrix(frame), test_case->color_matrix, "color matrix");
    }

    CHECK_EQ(turbores_frame_data_size(frame), test_case->frame_data_size, "frame data size");
    CHECK_EQ(reference_size, test_case->frame_data_size, "reference size");

    if (memcmp(turbores_frame_data_ptr(frame), reference, reference_size) != 0) {
        fprintf(stderr, "FAILED: %s (threads %u): frame data differs from reference\n", test_case->name, threads);
        return 1;
    }

    turbores_frame_destroy(frame);
    turbores_decoder_destroy(decoder);
    free(packet);
    free(reference);

    printf("ok - %s (threads %u)\n", test_case->name, threads);
    return 0;
}

typedef struct FieldWrite {
    size_t offset;
    size_t size;
    uint32_t value;
} FieldWrite;

static uint32_t readBigEndian(const uint8_t *data, size_t size) {
    uint32_t value = 0;
    for (size_t i = 0; i < size; i++) {
        value = (value << 8) | data[i];
    }
    return value;
}

static void writeBigEndian(uint8_t *data, size_t size, uint32_t value) {
    for (size_t i = 0; i < size; i++) {
        data[i] = (uint8_t) (value >> (8 * (size - i - 1)));
    }
}

static int messageContains(TurboresDecoder *decoder, const char *needle) {
    const char *message = (const char *) turbores_decoder_error_message_ptr(decoder);
    size_t message_size = turbores_decoder_error_message_size(decoder);
    size_t needle_size = strlen(needle);
    if (!message) {
        return 0;
    }
    for (size_t i = 0; i + needle_size <= message_size; i++) {
        if (memcmp(message + i, needle, needle_size) == 0) {
            return 1;
        }
    }
    return 0;
}

static int runMalformedHeaders(void) {
    const uint32_t threads = 1;
    const TestCase *base_case = &test_cases[0];
    size_t packet_size;
    uint8_t *original = readFile(base_case->packet_path, &packet_size);
    uint8_t *packet = malloc(packet_size);

    const uint32_t frame_size = readBigEndian(original, 4);
    const uint32_t header_size = readBigEndian(original + 8, 2);
    const size_t picture = 8 + header_size;
    const uint32_t picture_header_size = original[picture] >> 3;

    const struct {
        const char *name;
        FieldWrite writes[2];
        int32_t expected_result;
        const char *expected_message;
    } cases[] = {
        { "Zero frame size", { { 0, 4, 0 } }, TURBORES_ERROR_INVALID_DATA, "smaller than the fixed frame header" },
        { "Frame size before reader position", { { 0, 4, 2 } }, TURBORES_ERROR_INVALID_DATA, "smaller than the fixed frame header" },
        { "Short frame size", { { 0, 4, 27 } }, TURBORES_ERROR_INVALID_DATA, "smaller than the fixed frame header" },
        { "Frame size past packet", { { 0, 4, (uint32_t) packet_size + 1 } }, TURBORES_ERROR_INVALID_DATA, "smaller than the frame size" },
        { "Short frame header", { { 8, 2, 19 } }, TURBORES_ERROR_INVALID_DATA, "Frame header size too small" },
        {
            "Frame header past frame",
            { { 0, 4, (uint32_t) picture }, { 8, 2, header_size + 1 } },
            TURBORES_ERROR_INVALID_DATA,
            "Frame header extends past the end of the frame",
        },
        { "Zero frame width", { { 16, 2, 0 } }, TURBORES_ERROR_INVALID_DATA, "must be non-zero" },
        { "Zero frame height", { { 18, 2, 0 } }, TURBORES_ERROR_INVALID_DATA, "must be non-zero" },
        { "Zero picture header", { { picture, 1, 0 } }, TURBORES_ERROR_INVALID_DATA, "Picture header size too small" },
        { "Short picture header", { { picture, 1, 7 << 3 } }, TURBORES_ERROR_INVALID_DATA, "Picture header size too small" },
        { "Zero picture size", { { picture + 1, 4, 0 } }, TURBORES_ERROR_INVALID_DATA, "Picture header extends past" },
        {
            "Picture size shorter than picture header",
            { { picture + 1, 4, picture_header_size - 1 } },
            TURBORES_ERROR_INVALID_DATA,
            "Picture header extends past",
        },
        {
            "Picture size past frame",
            { { picture + 1, 4, frame_size - (uint32_t) picture + 1 } },
            TURBORES_ERROR_INVALID_DATA,
            "smaller than the picture data size",
        },
    };

    TurboresDecoder *decoder = turbores_decoder_create(threads, base_case->bit_depth, TURBORES_ALL_PIXEL_FORMATS);
    TurboresFrame *frame = turbores_frame_create();
    if (!decoder || !frame) {
        fprintf(stderr, "FAILED: Malformed headers: could not create decoder or frame\n");
        exit(1);
    }

    int failures = 0;
    int32_t control_result = turbores_decode(decoder, frame, original, packet_size);
    if (control_result != 0) {
        fprintf(stderr, "FAILED: Malformed headers: unmodified packet returned %d\n", control_result);
        failures++;
    }

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        memcpy(packet, original, packet_size);
        for (size_t w = 0; w < sizeof(cases[i].writes) / sizeof(cases[i].writes[0]); w++) {
            const FieldWrite *write = &cases[i].writes[w];
            if (write->size > 0) {
                writeBigEndian(packet + write->offset, write->size, write->value);
            }
        }

        int32_t result = turbores_decode(decoder, frame, packet, packet_size);
        if (result != cases[i].expected_result || !messageContains(decoder, cases[i].expected_message)) {
            fprintf(
                stderr,
                "FAILED: %s: returned %d (expected %d): %.*s\n",
                cases[i].name,
                result,
                cases[i].expected_result,
                (int) turbores_decoder_error_message_size(decoder),
                (const char *) turbores_decoder_error_message_ptr(decoder)
            );
            failures++;
        }
    }

    for (size_t size = 0; size < picture + 8; size++) {
        if (turbores_decode(decoder, frame, original, size) >= 0) {
            fprintf(stderr, "FAILED: Packet truncated to %zu bytes was accepted\n", size);
            failures++;
        }
    }

    turbores_frame_destroy(frame);
    turbores_decoder_destroy(decoder);
    free(packet);
    free(original);
    if (failures == 0) {
        printf("ok - Malformed and truncated headers\n");
    }
    return failures;
}

int main(void) {
    const uint32_t thread_configs[] = { 0, 4 };
    int failures = 0;

    for (size_t t = 0; t < sizeof(thread_configs) / sizeof(thread_configs[0]); t++) {
        for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
            failures += runCase(&test_cases[i], thread_configs[t]);
        }
    }

    failures += runMalformedHeaders();

    if (failures > 0) {
        fprintf(stderr, "%d test(s) failed\n", failures);
        return 1;
    }

    printf("\nAll tests passed.\n");
    return 0;
}
