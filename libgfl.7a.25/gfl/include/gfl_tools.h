/**@file    gfl_tools.h
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

#ifndef _GFL_TOOLS_H_
  #define _GFL_TOOLS_H_
/**@endcond                   */

  #include <gfl_types.h>

  /* horloge */
  typedef struct
  {
    uint hh; /* heure    [0,23]  */
    uint mm; /* minutes  [0,59]  */
    uint ss; /* secondes [0,59]  */
    uint ds; /* dizieme  de sec. */
    uint cs; /* centieme de sec. */
    uint ms; /* millieme de sec. */
  } GFLclock;

  /* calcule le temps "processeur" (cumul des threads) */
  /* retour au format hh:mm:ss:cs:ms                   */
  char*  gfl_ProcTimer(void);
  /* chrono "temps réel"                               */
  char*  gfl_Chrono(void);
  /* récupère le chrono courant                        */
  void   gfl_GetTime(GFLclock* clock);

  /*!****************************************************************************!*/
  /*! générateurs aléatoires                                                     !*/
  /*! DANS CETTE VERSION, C'EST LA GRAINE PAR DEFAUT                             !*/
  /*! => à l'utilisateur de faire appel à rand(seed)                             !*/
  /*!****************************************************************************!*/
  /* intervalle [root*(1.-percent), root*(1.+percent)] */
  double gfl_Rand_Percent(double root, double percent);
  /* intervalle [root-delta, root+delta]               */
  double gfl_Rand_Delta(double root, double delta);
  /* intervalle [min, max]                             */
  double gfl_Rand_MinMax(double min, double max);

  /*!****************************************************************************!*/
  /*! FONCTIONS TRIGO USUELLES                                                   !*/
  /*! redéfinition des fonctions <cos> et <sin> basée sur des LUT à 720 valeurs  !*/
  /*! -> un peu moins précis (0.5°) mais beaucoup plus rapide                    !*/ 
  /*!****************************************************************************!*/
             /** appelée automatiquement : NE PAS UTILISER **/
  //bool      gfl_InitTrigonometric(void);

  double    gfl_Idxcos(int    idx); /** angle : valeur d'indice ramenée dans [0,720] (1/2 degré) **/
  double    gfl_Idxsin(int    idx);
  double    gfl_Radcos(double rad); /** angle : valeur en radian ramenée dans [0,720] **/
  double    gfl_Radsin(double rad);
  double    gfl_Degcos(double deg); /** angle : valeur en degré ramenée dans [0,720] **/
  double    gfl_Degsin(double deg);

#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
