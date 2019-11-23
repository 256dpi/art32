#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <art32/vector.h>

a32_vector_t a32_vector_new(size_t length) {
  // allocate values
  double* values = calloc(sizeof(double), length);

  // create vector
  a32_vector_t vec = {
      .len = length,
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
      .len = length,
      .values = (double*)values,
  };

  return vec;
}

void a32_vector_copy(a32_vector_t dst, a32_vector_t src) {
  // assert shape
  assert(dst.len == src.len);

  // copy values
  memcpy(dst.values, src.values, sizeof(double) * src.len);
}

a32_vector_t a32_vector_view(a32_vector_t vec, size_t offset, size_t length) {
  // create vector view
  a32_vector_t out = {.len = length, .values = vec.values + offset};

  return out;
}

double a32_vector_max(a32_vector_t vec) {
  // set max to first element
  double max = vec.values[0];

  // find the biggest element
  for (size_t i = 0; i < vec.len; i++) {
    if (vec.values[i] > max) {
      max = vec.values[i];
    }
  }

  return max;
}

double a32_vector_min(a32_vector_t vec) {
  // set min to first element
  double min = vec.values[0];

  // find the smallest element
  for (size_t i = 0; i < vec.len; i++) {
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
  for (size_t i = 0; i < vec.len; i++) {
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
  for (size_t i = 0; i < vec.len; i++) {
    vec.values[i] = vec.values[i] / mag;
  }
}

void a32_vector_multiply(a32_vector_t vec1, a32_vector_t vec2) {
  // assert shape
  assert(vec1.len == vec2.len);

  // multiply
  for (size_t e = 0; e < vec1.len; e++) {
    vec1.values[e] *= vec2.values[e];
  }
}

void a32_vector_print(a32_vector_t vec) {
  // print layout
  printf("[");
  for (size_t i = 0; i < vec.len; i++) {
    printf("%+.3f%s", vec.values[i], i + 1 < vec.len ? " " : "");
  }
  printf("]\n");
}
