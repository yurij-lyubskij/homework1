#include "general.h"
#include "types.h"

// Возвращает позицию вставки элемента на отрезке [first, last).
int find_insertion_point(const void *arr, int size, const void *element,
                         int (*compare)(const void *arr, const void *element,
                                        int number)) {
  if (size == 0) {
    return 0;
  }
  int first = 0;
  int last = size;  // Элемент в last не учитывается.
  while (first < last) {
    int mid = (first + last) / 2;
    if (compare(arr, element, mid) < 0) {
      first = mid + 1;
    } else {
      // В случае равенства arr[mid] останется справа.
      last = mid;
    }
  }
  return first;
}

void *grow(void *buffer, int *current_capacity, size_t element_size) {
  int init_size = 4;
  int newBufferSize =
      (init_size < *current_capacity * 2) ? *current_capacity * 2 : init_size;
  void *newBuffer = realloc(buffer, newBufferSize * element_size);
  if (newBuffer != NULL) {
    *current_capacity = newBufferSize;
    return newBuffer;
  }
  return NULL;
}

int add_if_not_exists(void **buffer, int *capacity, int *size, void *element,
                      size_t element_size,
                      int (*compare)(const void *arr, const void *element,
                                     int number),
                      void (*setter)(void **buffer, int pos, void *element),
                      void *(*getter)(void **buffer, int pos)) {
  if (*size == *capacity) {
    (*buffer) = grow((void *)(*buffer), capacity, element_size);
  }
  if (!(*size < *capacity && (*buffer) != NULL)) {
    return -1;
  }
  int pos =
      find_insertion_point((const void *)(*buffer), *size, element, compare);
  if (!(pos >= 0 && pos <= *size)) {
    return -1;
  }
  if (pos < *size) {
    if (compare((const void *)(*buffer), element, pos) == 0) {
      return pos;
    }
    memmove(getter(buffer, pos + 1), getter(buffer, pos),
            ((*size) - pos) * element_size);  // проверку возвр знач добавить
  }
  setter(buffer, pos, element);
  (*size)++;
  return pos;
}
