
#include <unity.h>
#include <vector>
#include "SampleBuffer.hpp"
#include "native/CopyOutput.hpp"

#ifdef UNIT_TEST

extern void hexdump(const char *buf, int buflen);

void test_fresh_buffer() {
    SampleBuffer<uint8_t, 1240> buffer;

    TEST_ASSERT_EQUAL(buffer.size, 1240);
    TEST_ASSERT_FALSE(buffer.full());
    TEST_ASSERT_FALSE(buffer.buffering());

}

void test_over_feed() {
    SampleBuffer<uint8_t, 20> buffer;

    for (int i = 0; i < 50; i++) {
      buffer.feed(i);
    }

    TEST_ASSERT_EQUAL(buffer.size, 20);
    TEST_ASSERT_FALSE(buffer.full());
    TEST_ASSERT_FALSE(buffer.buffering());
}



#define test_with_params(SZ, LEAD, SWEEP, NUM, ...) \
    SampleBuffer<uint8_t, SZ> buffer;                 \
\
    for (int i = 0; i < LEAD; i++) {                  \
      buffer.feed(i+10);                              \
    }                                                 \
    TEST_ASSERT_FALSE(buffer.full());                 \
    TEST_ASSERT_FALSE(buffer.buffering());            \
\
    buffer.sweep(SWEEP);                              \
    for (int i = 0; i < SWEEP-1; i++) {               \
      buffer.feed(i+100);                             \
    }                                                 \
    TEST_ASSERT_FALSE(buffer.full());                 \
    TEST_ASSERT_TRUE(buffer.buffering());             \
\
    buffer.feed(127);                                 \
    TEST_ASSERT_TRUE(buffer.full());                  \
    TEST_ASSERT_FALSE(buffer.buffering());            \
\
    CopyOutput output;                                \
    buffer.write(output);                             \
    TEST_ASSERT_EQUAL( output.len, NUM);              \
    TEST_ASSERT_EQUAL( output.captured.size(), NUM);  \
    char expected[NUM] = { __VA_ARGS__};              \
    TEST_ASSERT_EQUAL_INT8_ARRAY( expected, output.captured.data(), NUM)

void test_immediate_sweep() {
    test_with_params(20, 0, 10, 10,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 127);
}

void test_sweep_with_lead() {
    test_with_params(20, 5, 5, 10,
        10, 11, 12, 13, 14, 100, 101, 102, 103, 127);
}
void test_sweep_that_wraps_with_lead() {
    test_with_params(20, 15, 10, 20,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 100, 101, 102, 103, 104, 105, 106, 107, 108, 127 );
}

void test_sweep_with_lead_that_wraps() {
    test_with_params(20, 25, 10, 20,
        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 100, 101, 102, 103, 104, 105, 106, 107, 108, 127 );
}

void test_sweep_at_boundary_with_lead() {
    test_with_params(20, 20, 10, 20,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 100, 101, 102, 103, 104, 105, 106, 107, 108, 127 );
}

void test_sweep_with_lead_at_boundary() {
    test_with_params(20, 30, 10, 20,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 100, 101, 102, 103, 104, 105, 106, 107, 108, 127 );
}

void test_sweep_full_buffer() {
    test_with_params(20, 10, 20, 20,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 127 );
}

void test_sweep_full_buffer_at_boundary() {
    test_with_params(20, 20, 20, 20,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 127 );
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_fresh_buffer);
    RUN_TEST(test_over_feed);
    RUN_TEST(test_immediate_sweep);
    RUN_TEST(test_sweep_with_lead);
    RUN_TEST(test_sweep_that_wraps_with_lead);
    RUN_TEST(test_sweep_with_lead_that_wraps);
    RUN_TEST(test_sweep_at_boundary_with_lead);
    RUN_TEST(test_sweep_with_lead_at_boundary);
    RUN_TEST(test_sweep_full_buffer);
    RUN_TEST(test_sweep_full_buffer_at_boundary);

    UNITY_END();
}

#endif
