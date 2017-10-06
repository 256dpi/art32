#ifndef A32_STRCONV_H
#define A32_STRCONV_H

/**
 * Returns a string containing the value with 4 decimals.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v - The number.
 * @return Pointer to string.
 */
const char* a32_d2str(double num);

/**
 * Returns a string containing the value.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v - The number.
 * @return Pointer to string.
 */
const char* a32_l2str(long num);

/**
 * Returns the number from the string.
 *
 * @param str - The string.
 * @return The number.
 */
double a32_str2d(const char* str);

/**
 * Returns the number from the string.
 *
 * @param str - The string.
 * @return The number.
 */
long a32_str2l(const char* str);

/**
 * Returns the number from the string.
 *
 * @param str - The string.
 * @return The number.
 */
int a32_str2i(const char* str);

#endif  // A32_STRCONV_H
