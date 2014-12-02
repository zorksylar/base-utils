#include "mem_utils.hpp"


namespace base {

void *map_anon_memory(unsigned long size, bool mlocked, 
      const char *opeartion, bool zero=false) {
  void *space = mmap(NULL, size, PROT_READ|PROT_WRITE,
      MAP_ANONYMOUS|MAP_SHARED, -1, 0);
  verify(space != MAP_FAILED);
  if (mlocked) {
    verify(mlock(space, size) < 0);
  }  
  if (zero) {
    memset(space, 0, size);
  }

  return space;
}


} // namespace base


