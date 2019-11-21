#include <art32/vector.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

a32_vector_t a32_vector_new(size_t length) {
  // create vector
  a32_vector_t vector = {
      .len = length,
      .data = calloc(sizeof(double), length),
  };

  return vector;
}

a32_vector_t a32_vector_view(a32_vector_t vec, size_t offset, size_t length) {
  // create vector view
  a32_vector_t vector = {.len = length, .data = vec.data + offset};

  return vector;
}

void a32_vector_free(a32_vector_t vec) {
  // free data
  free(vec.data);
}

double a32_vector_max(a32_vector_t vec) {
  // set max to first element
  double max = vec.data[0];

  // find the biggest element
  for (size_t i = 0; i < vec.len; i++) {
    if (vec.data[i] > max) {
      max = vec.data[i];
    }
  }

  return max;
}

double a32_vector_min(a32_vector_t vec) {
  // set min to first element
  double min = vec.data[0];

  // find the smallest element
  for (size_t i = 0; i < vec.len; i++) {
    if (vec.data[i] < min) {
      min = vec.data[i];
    }
  }

  return min;
}

double a32_vector_mag(a32_vector_t vec) {
  // prepare sum
  double sum = 0;

  // add products
  for (size_t i = 0; i < vec.len; i++) {
    sum += (vec.data[i] * vec.data[i]);
  }

  return sqrt(sum);
}

void a32_vector_norm(a32_vector_t vec) {
  double m = a32_vector_mag(vec);
  if (m > 0) {
    for (size_t i = 0; i < vec.len; i++) {
      vec.data[i] = vec.data[i] / m;
    }
  }
}

void a32_vector_print(a32_vector_t vec) {
  printf("[");

  for (size_t i = 0; i < vec.len; i++) {
    printf("%+.3f ", vec.data[i]);
  }

  printf("]\n");
}
