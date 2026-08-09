#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <art32/vector.h>

a32_vector_t a32_vector_new(size_t length) {
  // reject a length whose byte count is not representable
  if (length > SIZE_MAX / sizeof(double)) {
    return a32_vector_use(NULL, length);
  }

  // allocate values
  double* values = calloc(length, sizeof(double));

  // create vector
  a32_vector_t vec = {
      .length = length,
      .values = values,
  };

  return vec;
}

void a32_vector_free(a32_vector_t vec) {
  // free data
  free(vec.values);
}

a32_vector_t a32_vector_use(const double* values, size_t length) {
  // create vector
  a32_vector_t vec = {
      .length = length,
      .values = (double*)values,
  };

  return vec;
}

void a32_vector_copy(a32_vector_t dst, a32_vector_t src) {
  // assert shape
  assert(dst.length == src.length);

  // copy values
  memcpy(dst.values, src.values, sizeof(double) * src.length);
}

a32_vector_t a32_vector_view(a32_vector_t vec, size_t offset, size_t length) {
  // assert shape (the addition cannot overflow for in range offsets)
  assert(offset <= vec.length);
  assert(length <= vec.length - offset);

  // create vector view
  a32_vector_t view = {.length = length, .values = vec.values + offset};

  return view;
}

double a32_vector_max(a32_vector_t vec) {
  // handle empty vector
  if (vec.length == 0) {
    return 0;
  }

  // set max to first element
  double max = vec.values[0];

  // find the biggest element
  for (size_t i = 1; i < vec.length; i++) {
    if (vec.values[i] > max) {
      max = vec.values[i];
    }
  }

  return max;
}

double a32_vector_min(a32_vector_t vec) {
  // handle empty vector
  if (vec.length == 0) {
    return 0;
  }

  // set min to first element
  double min = vec.values[0];

  // find the smallest element
  for (size_t i = 1; i < vec.length; i++) {
    if (vec.values[i] < min) {
      min = vec.values[i];
    }
  }

  return min;
}

double a32_vector_mag(a32_vector_t vec) {
  // prepare sum
  double sum = 0;

  // add products
  for (size_t i = 0; i < vec.length; i++) {
    sum += (vec.values[i] * vec.values[i]);
  }

  return sqrt(sum);
}

void a32_vector_norm(a32_vector_t vec) {
  // get magnitude
  double mag = a32_vector_mag(vec);

  // skip if zero
  if (mag == 0) {
    return;
  }

  // normalize values
  for (size_t i = 0; i < vec.length; i++) {
    vec.values[i] = vec.values[i] / mag;
  }
}

void a32_vector_multiply(a32_vector_t vec1, a32_vector_t vec2) {
  // assert shape
  assert(vec1.length == vec2.length);

  // multiply
  for (size_t e = 0; e < vec1.length; e++) {
    vec1.values[e] *= vec2.values[e];
  }
}

void a32_vector_print(a32_vector_t vec) {
  // print layout
  printf("[");
  for (size_t i = 0; i < vec.length; i++) {
    printf("%+.3f%s", vec.values[i], i + 1 < vec.length ? " " : "");
  }
  printf("]\n");
}
