#pragma once 

#include "basetypes.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

namespace base {


void *map_anon_memory(unsigned long size, bool mlocked, const char *operation, bool zero = false);



}; //namespace base

