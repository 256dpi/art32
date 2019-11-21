#include <art32/vector.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

a32_vector_t a32_vector_new(size_t length) {
  // create vector
  a32_vector_t vector = {
      .len = length,
      .values = calloc(sizeof(double), length),
  };

  return vector;
}

a32_vector_t a32_vector_view(a32_vector_t vec, size_t offset, size_t length) {
  // create vector view
  a32_vector_t vector = {.len = length, .values = vec.values + offset};

  return vector;
}

void a32_vector_free(a32_vector_t vec) {
  // free data
  free(vec.values);
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
  double m = a32_vector_mag(vec);

  // skip if zero
  if (m == 0) {
    return;
  }

  // normalize values
  for (size_t i = 0; i < vec.len; i++) {
    vec.values[i] = vec.values[i] / m;
  }
}

void a32_vector_print(a32_vector_t vec) {
  printf("[");

  for (size_t i = 0; i < vec.len; i++) {
    printf("%+.3f ", vec.values[i]);
  }

  printf("]\n");
}
