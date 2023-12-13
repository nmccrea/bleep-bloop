/**
 * @brief This file implements the test cases for the `Integer` API.
 */

#include "fsl_debug_console.h"
#include "integer.h"

/**
 * @brief Performs a single test of the `Integer_SquareRoot` function.
 *
 * @param x The value for which to take the square root.
 * @param expected_result The expected result of the square root operation.
 *
 * @returns 0 if the test passed, 1 otherwise.
 */
int _IntegerTest_PerformTest(int32_t x, int32_t expected_result)
{
  int32_t result = Integer_SquareRoot(x);
  int pass = (result == expected_result);
  PRINTF("`Integer_SquareRoot(%d)` returned %d, expected %d - %s\r\n",
         x,
         result,
         expected_result,
         pass ? "PASS" : "FAIL");

  return !pass;
}

int IntegerTest()
{
  int total_failures = 0;
  PRINTF("Running `Integer` API tests...\r\n");
  total_failures += _IntegerTest_PerformTest(-1, -1);
  total_failures += _IntegerTest_PerformTest(-1000, -1);
  total_failures += _IntegerTest_PerformTest(0, 0);
  total_failures += _IntegerTest_PerformTest(1, 1);
  total_failures += _IntegerTest_PerformTest(2, 1);
  total_failures += _IntegerTest_PerformTest(3, 1);
  total_failures += _IntegerTest_PerformTest(4, 2);
  total_failures += _IntegerTest_PerformTest(5, 2);
  total_failures += _IntegerTest_PerformTest(6, 2);
  total_failures += _IntegerTest_PerformTest(7, 2);
  total_failures += _IntegerTest_PerformTest(8, 2);
  total_failures += _IntegerTest_PerformTest(9, 3);
  total_failures += _IntegerTest_PerformTest(10, 3);
  total_failures += _IntegerTest_PerformTest(11, 3);
  total_failures += _IntegerTest_PerformTest(12, 3);
  total_failures += _IntegerTest_PerformTest(13, 3);
  total_failures += _IntegerTest_PerformTest(14, 3);
  total_failures += _IntegerTest_PerformTest(15, 3);
  total_failures += _IntegerTest_PerformTest(16, 4);
  total_failures += _IntegerTest_PerformTest(100, 10);
  total_failures += _IntegerTest_PerformTest(1000, 31);
  total_failures += _IntegerTest_PerformTest(297242, 545);
  total_failures += _IntegerTest_PerformTest(245924492, 15681);

  PRINTF("`Integer` API tests complete with %d failures\r\n", total_failures);
  return total_failures;
}