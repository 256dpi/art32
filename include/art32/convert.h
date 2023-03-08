#ifndef A32_CONVERT_H
#define A32_CONVERT_H

/**
 * Trim leading and trailing whitespace of a string.
 *
 * Note: The function will modify the passed string.
 *
 * @param str The source string.
 * @return The trimmed string
 */
char* a32_trim(char* str);

/**
 * Returns a string containing the value.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v The number.
 * @return Pointer to string.
 */
const char* a32_i2str(int32_t num);

/**
 * Returns a string containing the value.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v The number.
 * @return Pointer to string.
 */
const char* a32_l2str(int64_t num);

/**
 * Returns a string containing the value with up to 6 decimals.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v The number.
 * @return Pointer to string.
 */
const char* a32_f2str(float num);

/**
 * Returns a string containing the value with up to 6 decimals.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v The number.
 * @return Pointer to string.
 */
const char* a32_d2str(double num);

/**
 * Returns the number from the string.
 *
 * @param str The string.
 * @return The number.
 */
int32_t a32_str2i(const char* str);

/**
 * Returns the number from the string.
 *
 * @param str The string.
 * @return The number.
 */
int64_t a32_str2l(const char* str);

/**
 * Returns the number from the string.
 *
 * @param str The string.
 * @return The number.
 */
float a32_str2f(const char* str);

/**
 * Returns the number from the string.
 *
 * @param str The string.
 * @return The number.
 */
double a32_str2d(const char* str);

#endif  // A32_CONVERT_H
