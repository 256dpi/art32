#ifndef A32_NUMBERS_H
#define A32_NUMBERS_H

#include <stdint.h>

/**
 * Constrain number to be withing min and max.
 *
 * @param num The number.
 * @param min The lower range.
 * @param max The upper range.
 * @return The constrained value.
 */
int32_t a32_constrain_i(int32_t num, int32_t min, int32_t max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num The number.
 * @param min The lower range.
 * @param max The upper range.
 * @return The constrained value.
 */
int64_t a32_constrain_l(int64_t num, int64_t min, int64_t max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num The number.
 * @param min The lower range.
 * @param max The upper range.
 * @return The constrained value.
 */
float a32_constrain_f(float num, float min, float max);

/**
 * Constrain number to be withing min and max.
 *
 * @param num The number.
 * @param min The lower range.
 * @param max The upper range.
 * @return The constrained value.
 */
double a32_constrain_d(double num, double min, double max);

/**
 * Map number from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped value.
 */
int32_t a32_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

/**
 * Map number from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped value.
 */
int64_t a32_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max);

/**
 * Map number from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped value.
 */
float a32_map_f(float num, float in_min, float in_max, float out_min, float out_max);

/**
 * Map number from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped value.
 */
double a32_map_d(double num, double in_min, double in_max, double out_min, double out_max);

/**
 * Map number safely from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped and constrained value.
 */
int32_t a32_safe_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

/**
 * Map number safely from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped and constrained value.
 */
int64_t a32_safe_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max);

/**
 * Map number safely from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped and constrained value.
 */
float a32_safe_map_f(float num, float in_min, float in_max, float out_min, float out_max);

/**
 * Map number safely from one range to another.
 *
 * @param num The number.
 * @param in_min The input lower range.
 * @param in_max The input upper range.
 * @param out_min The output lower range.
 * @param out_max The output upper range.
 * @return The mapped and constrained value.
 */
double a32_safe_map_d(double num, double in_min, double in_max, double out_min, double out_max);

#endif  // A32_NUMBERS_H
