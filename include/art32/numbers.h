#ifndef A32_NUMBERS_H
#define A32_NUMBERS_H

/**
 * Constrain number to be withing min and max.
 *
 * @param num - The number.
 * @param min - The lower range.
 * @param max - The upper range.
 * @return The constrained value.
 */
double a32_constrain_d(double num, double min, double max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num - The number.
 * @param min - The lower range.
 * @param max - The upper range.
 * @return The constrained value.
 */
float a32_constrain_f(float num, float min, float max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num - The number.
 * @param min - The lower range.
 * @param max - The upper range.
 * @return The constrained value.
 */
long a32_constrain_l(long num, long min, long max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num - The number.
 * @param min - The lower range.
 * @param max - The upper range.
 * @return The constrained value.
 */
int a32_constrain_i(int num, int min, int max);

/**
 * Map number from one range to another.
 *
 * @param num - The number.
 * @param in_min - The input lower range.
 * @param in_max - The input upper range.
 * @param out_min - The output lower range.
 * @param out_max - The output upper range.
 * @return The mapped value.
 */
double a32_map_d(double num, double in_min, double in_max, double out_min, double out_max);

/**
 * Map number from one range to another.
 *
 * @param num - The number.
 * @param in_min - The input lower range.
 * @param in_max - The input upper range.
 * @param out_min - The output lower range.
 * @param out_max - The output upper range.
 * @return The mapped value.
 */
double a32_map_f(float num, float in_min, float in_max, float out_min, float out_max);

/**
 * Map number from one range to another.
 *
 * @param num - The number.
 * @param in_min - The input lower range.
 * @param in_max - The input upper range.
 * @param out_min - The output lower range.
 * @param out_max - The output upper range.
 * @return The mapped value.
 */
long a32_map_l(long num, long in_min, long in_max, long out_min, long out_max);

/**
 * Map number from one range to another.
 *
 * @param num - The number.
 * @param in_min - The input lower range.
 * @param in_max - The input upper range.
 * @param out_min - The output lower range.
 * @param out_max - The output upper range.
 * @return The mapped value.
 */
int a32_map_i(int num, int in_min, int in_max, int out_min, int out_max);

#endif  // A32_NUMBERS_H
