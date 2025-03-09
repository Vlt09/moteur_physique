/**@file    gfl.h
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

#ifndef _LIB_GFL_
  #define _LIB_GFL_
/**@endcond                   */

  /* les lib. standards /usr/lib */
  #include <stdio.h>
  #include <stdarg.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <ctype.h>
  #include <sched.h>
  #include <sys/stat.h>
  #include <time.h>
  #include <math.h>
  #ifndef WIN32
    #include <sys/times.h>
  #else
    #include <times.h>
  #endif
  #include <sys/time.h>

  /* FreeGlut /usr/lib
   * si nécessaire, installer freeglut (VERSION 3-dev) :
   * $> sudo apt-get install freeglut3-dev
   **/
  #include <GL/freeglut.h>

  /* les libs. locales         */
  /* les basiques              */
  #include <gfl_mtrack.h>      /* traceur d'allocation / garbage collector  */
  #include <gfl_error.h>       /* fonctions/macro erreurs/warning           */
  #include <gfl_types.h>       /* types primaires et macros diverses        */
  #include <gfl_tools.h>       /* utilitaires divers                        */
  #include <gfl_colors.h>      /* gestions des couleurs                     */
  #include <gfl_control.h>     /* gestion des boutons, scroll, souris...    */
  #include <gfl_window.h>      /* routines et fonctions de base             */

  /* les "peripheriques"       */
  #include <gfl_pixmap.h>      /* images au format PNM                      */
  #include <gfl_colsyst.h>     /* systèmes colorimétriques RGB|HSV|YcbCr... */
  #include <gfl_capture.h>     /* routines de capture d'ecran image/video   */

  /* les "haut niveau" 2D      */
  #ifdef _GFL2D_
    #include <gfl_geom2d.h>    /* primitives et opérateurs géométriques     */
    #include <gfl_draw2d.h>    /* routines de tracé de primitives           */
    #include <gfl_transfo2d.h> /* transfo. en coordonnees homogenes 4x4     */
  #endif
 
  /* les "haut niveau" 3D      */
  #ifdef _GFL3D_
    #include <gfl_camlight3d.h>/* gestion de la caméra et de la lumière     */
    #include <gfl_transfo3d.h> /* transfo. en coordonnees homogenes 4x4     */
    #include <gfl_particule3d.h> /*     */
  #endif
  
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
