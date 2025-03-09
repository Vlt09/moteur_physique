/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdarg.h>
#include <gfl_types.h>

static bool _GFL_VERB_ = false;

extern bool* gfl_verb_(void) { return &_GFL_VERB_; }

extern void gfl_p_verb_(const char *format, ...)
{
  if (!_GFL_VERB_) return;
  va_list ap;
  va_start(ap, format);
  vfprintf(stdout, format, ap);
  va_end(ap);
  fprintf(stdout,"\e[0;0;0m");
}

extern void gfl_p_err_(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  fprintf(stderr,"\e[31m(error)\e[0;0;0m ");
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stdout,"\e[0;0;0m");
}

extern void gfl_p_warn_(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  fprintf(stderr,"\e[34m(warning)\e[0;0;0m ");
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stdout,"\e[0;0;0m");
}

#ifdef __cplusplus
  }
#endif
