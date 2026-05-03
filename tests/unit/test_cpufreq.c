#include "test_framework.h"
#include "mock_reader.h"
#include "../../src/monitor/cpufreq.h"

/*
 * Unit tests for cpufreq_read().
 * All sysfs I/O is redirected through mock_sysfs_read() — no real /sys needed.
 */

static void test_cpufreq_cur_freq(void)
{
    cpufreq_info_t info = {0};
    int rc = cpufreq_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ_INT(rc, 0);
    ASSERT_EQ(info.cur_freq_khz, 2400000L);
}

static void test_cpufreq_min_max_freq(void)
{
    cpufreq_info_t info = {0};
    cpufreq_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ(info.min_freq_khz, 400000L);
    ASSERT_EQ(info.max_freq_khz, 3600000L);
}

static void test_cpufreq_governor(void)
{
    cpufreq_info_t info = {0};
    cpufreq_read(mock_sysfs_read, 0, &info);
    ASSERT_STR_EQ(info.governor, "powersave");
}

static void test_cpufreq_available_governors(void)
{
    cpufreq_info_t info = {0};
    cpufreq_read(mock_sysfs_read, 0, &info);
    ASSERT_EQ_INT(info.num_governors, 2);
    ASSERT_STR_EQ(info.available_governors[0], "performance");
    ASSERT_STR_EQ(info.available_governors[1], "powersave");
}

static void test_cpufreq_missing_cpu(void)
{
    cpufreq_info_t info = {0};
    /* cpu99 does not exist in mock_sysfs, should return error */
    int rc = cpufreq_read(mock_sysfs_read, 99, &info);
    ASSERT_EQ_INT(rc, -1);
}

int main(void)
{
    printf("=== test_cpufreq ===\n");
    RUN_TEST(test_cpufreq_cur_freq);
    RUN_TEST(test_cpufreq_min_max_freq);
    RUN_TEST(test_cpufreq_governor);
    RUN_TEST(test_cpufreq_available_governors);
    RUN_TEST(test_cpufreq_missing_cpu);
    TEST_SUMMARY();
    TEST_EXIT();
}
