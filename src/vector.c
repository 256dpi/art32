#include <art32/vector.h>

#include <stdio.h>
#include <stdlib.h>

a32_vector_t a32_vector_new(size_t length) {
  // create vector
  a32_vector_t vector = {
      .len = length,
      .data = (float *)calloc(sizeof(float), length),
  };

  return vector;
}

void a32_vector_free(a32_vector_t vec) {
  // free data
  free(vec.data);
}

float a32_vector_max(a32_vector_t vec) {
  // set max to first element
  float max = vec.data[0];

  // find the biggest element
  for (size_t i = 0; i < vec.len; i++) {
    if (vec.data[i] > max) {
      max = vec.data[i];
    }
  }

  return max;
}

float a32_vector_min(a32_vector_t vec) {
  // set min to first element
  float min = vec.data[0];

  // find the smallest element
  for (size_t i = 0; i < vec.len; i++) {
    if (vec.data[i] < min) {
      min = vec.data[i];
    }
  }

  return min;
}

void a32_vector_print(a32_vector_t vec) {
  printf("[");

  for (size_t i = 0; i < vec.len; i++) {
    printf("%+.3f ", vec.data[i]);
  }

  printf("]\n");
}
