#include "test_framework.h"
#include "mock_reader.h"
#include "../../src/monitor/thermal.h"

static void test_thermal_temp(void)
{
    thermal_zone_t zone = {0};
    int rc = thermal_read(mock_sysfs_read, 0, &zone);
    ASSERT_EQ_INT(rc, 0);
    ASSERT_EQ(zone.temp_milli_c, 45000L);
}

static void test_thermal_type(void)
{
    thermal_zone_t zone = {0};
    thermal_read(mock_sysfs_read, 0, &zone);
    ASSERT_STR_EQ(zone.type, "x86_pkg_temp");
}

static void test_thermal_zone_id(void)
{
    thermal_zone_t zone = {0};
    thermal_read(mock_sysfs_read, 0, &zone);
    ASSERT_EQ_INT(zone.zone_id, 0);
}

static void test_thermal_missing_zone(void)
{
    thermal_zone_t zone = {0};
    int rc = thermal_read(mock_sysfs_read, 99, &zone);
    ASSERT_EQ_INT(rc, -1);
}

int main(void)
{
    printf("=== test_thermal ===\n");
    RUN_TEST(test_thermal_temp);
    RUN_TEST(test_thermal_type);
    RUN_TEST(test_thermal_zone_id);
    RUN_TEST(test_thermal_missing_zone);
    TEST_SUMMARY();
    TEST_EXIT();
}
