#include "test_framework.h"
#include "mock_reader.h"
#include "../../src/monitor/cpuidle.h"

static void test_cpuidle_state_name(void)
{
    cpuidle_info_t info = {0};
    int rc = cpuidle_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ_INT(rc, 0);
    ASSERT_EQ_INT(info.num_states, 2);
    ASSERT_STR_EQ(info.states[0].name, "C1");
    ASSERT_STR_EQ(info.states[1].name, "C6");
}

static void test_cpuidle_usage(void)
{
    cpuidle_info_t info = {0};
    cpuidle_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ(info.states[0].usage, 12345L);
}

static void test_cpuidle_time(void)
{
    cpuidle_info_t info = {0};
    cpuidle_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ(info.states[0].time_us, 9876543L);
}

static void test_cpuidle_disabled(void)
{
    cpuidle_info_t info = {0};
    cpuidle_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ_INT(info.states[0].disabled, 0);
}

static void test_cpuidle_missing_cpu(void)
{
    cpuidle_info_t info = {0};
    int rc = cpuidle_read(mock_sysfs_read, 99, &info);
    ASSERT_EQ_INT(rc, -1);
}

int main(void)
{
    printf("=== test_cpuidle ===\n");
    RUN_TEST(test_cpuidle_state_name);
    RUN_TEST(test_cpuidle_usage);
    RUN_TEST(test_cpuidle_time);
    RUN_TEST(test_cpuidle_disabled);
    RUN_TEST(test_cpuidle_missing_cpu);
    TEST_SUMMARY();
    TEST_EXIT();
}
