#include <art32/vector.h>

#include <stdlib.h>

a32_vector_t a32_vector_new(size_t length) {
  // create vector
  a32_vector_t vector = {
      .len = length,
      .data = (float *)calloc(sizeof(float), length),
  };

  return vector;
}

void a32_vector_free(a32_vector_t vector) {
  // free data
  free(vector.data);
}
