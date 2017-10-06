#ifndef STRCONV_H
#define STRCONV_H

/**
 * Returns a string containing the value with 4 decimals.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v - The number.
 * @return Pointer to string.
 */
const char* a32_dtostr(double num);

/**
 * Returns a string containing the value.
 *
 * Note: The returned pointer is altered on the next call.
 *
 * @param v - The number.
 * @return Pointer to string.
 */
const char* a32_ltostr(long num);

#endif  // STRCONV_H
