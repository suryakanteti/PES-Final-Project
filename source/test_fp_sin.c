/*
 * test_fp_sin.c
 *
 *  Created on: Apr 17, 2022
 *      Author: Surya Kanteti
 *
 *  Code provided by Howdy Pierce to test implemented sine function.
 */

#include "test_fp_sin.h"
#include <math.h>
#include "fp_trig.h"
#include <stdio.h>
#include <assert.h>

/*
 * Test sine function
 *
 * Tests the implemented sine function (present in fp_trig module)
 * against the standard library function and prints the max error and
 * sum of squares of errors.
 *
 * @input None
 * @return None
 *
 */
void test_sin()
{
  double act_sin;
  double exp_sin;

  double err;
  double sum_sq = 0;
  double max_err = 0;

  for (int i=-TWO_PI; i <= TWO_PI; i++)
  {
    exp_sin = sin( (double)i / TRIG_SCALE_FACTOR) * TRIG_SCALE_FACTOR;
    act_sin = fp_sin(i);

    err = act_sin - exp_sin;
    if (err < 0)
      err = -err;

    if (err > max_err)
      max_err = err;
    sum_sq += err*err;
  }

  assert(max_err < 2.0);
  assert(sum_sq < 12000);
}
