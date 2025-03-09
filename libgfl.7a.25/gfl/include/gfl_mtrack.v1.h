#ifndef _GFL_MTRACK_H_
  #define _GFL_MTRACK_H_

  #include <stdlib.h>
  #include <gfl_types.h>

  #define malloc(size)       gfl_mtmalloc(size, __FILE__,__func__,__LINE__)
  #define calloc(nmemb,size) gfl_mtcalloc(nmemb,size, __FILE__,__func__,__LINE__)
  #define realloc(ptr,size)  gfl_mtrealloc(ptr,size, __FILE__,__func__,__LINE__)
  #define free(ptr)          gfl_mtfree  (ptr,__FILE__,__func__,__LINE__)

  void* gfl_mtmalloc(size_t , char*, const char*, int);
  void* gfl_mtcalloc(size_t, size_t , char*, const char*, int);
  void* gfl_mtrealloc(void *, size_t , char*, const char*, int);
  void  gfl_mtfree  (void* , char*, const char*, int);

  void gfl_SetMTVerbose(bool flag);
  bool gfl_GetMTVerbose(void);

#endif
