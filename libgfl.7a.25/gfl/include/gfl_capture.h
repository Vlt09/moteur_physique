/**@file    gfl_capture.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _GFL_CAPTURE_
  #define _GFL_CAPTURE_
/**@endcond                   */

  #include <gfl_types.h>

  /*! CES FONCTIONS SONT ACTIVEES AUTOMATIQUEMENT !*/
  /*! il n'y a donc aucune raison de les utiliser !*/
  void gfl_SetFrameRate(int frame_per_sec);
  int  gfl_GetFrameRate(void);
  void gfl_SetBitRate(int bit_rate);
  int  gfl_GetBitRate(void);
  void gfl_SetPid(int force_pid);
  int  gfl_GetPid(void);
  void gfl_SetMaxImage(int);
  int  gfl_GetMaxImage(void);
  bool gfl_PlugCapture(const char *basename, int downleftx, int downlefty, int uprightx, int uprighty);
  void gfl_UnplugCapture(void);
  bool gfl_Snapshot(const char *format, const char *basename, int w, int h);
  bool gfl_FilmFrame(void);
  bool gfl_RealTimeFrame(void);
  bool gfl_MakeMpeg(void);
  bool gfl_MakeAvi(void);
  bool gfl_MakeMpeg4(void);
  bool gfl_MakeFlv(void);
  bool gfl_FFmpeg(void);

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
