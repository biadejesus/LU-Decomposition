#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main() {
  int32_t min32 = INT32_MIN;
  int32_t max32 = INT32_MAX;
  int64_t min64 = INT64_MIN;
  int64_t max64 = INT64_MAX;
  assert(sizeof(INT32_MIN) == 4);
  assert(sizeof(INT32_MAX) == 4);
  assert(sizeof(INT64_MIN) == 8);
  assert(sizeof(INT64_MAX) == 8);
  printf("done.\n");
  return 0;
}

