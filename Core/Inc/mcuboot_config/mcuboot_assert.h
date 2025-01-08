#pragma once

#include <inttypes.h>

void mcuboot_assert_handler(const char *file, int line);

#define assert(exp) \
  do {                                                  \
    if (!(exp)) {                                       \
      mcuboot_assert_handler(__FILE__, __LINE__);       \
    }                                                   \
  } while (0)
