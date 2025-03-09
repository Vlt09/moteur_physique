/**@file    gfl_tools.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Dec.2024 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

  #ifndef _GFL_ERROR_H_
    #define _GFL_ERROR_H_
  /**@endcond                   */
  
    void    gfl_p_err_   (const char *format, ...);
    #define gfl_error_   (fprintf(stderr,"\e[31m>> %-12s \e[0;0m",__func__), gfl_p_err_)
    void    gfl_p_warn_  (const char *format, ...);
    #define gfl_warning_ (fprintf(stderr,"\e[32m>> %-12s \e[0;0m",__func__), gfl_p_warn_)
  
    #define gfl_fopenfail_(filename,rwa) { gfl_error_("unable to open <%s> ('%s')\n",filename,rwa); return 0; }
    #define gfl_allocfail_(size)         { gfl_error_("allocation error (%zu)\n",(size_t)(size));   return 0; }
    #define gfl_funcfail_(fctname)       { gfl_error_("function <%s> failed\n",fctname);            return 0; }
    #define gfl_callfail_(callname)      { gfl_error_("fcall <%s> failed\n",callname);              return 0; }
  
  #endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
