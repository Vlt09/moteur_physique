/**@file    g3x.h
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

#ifndef _G3X_PART_H
  #define _G3X_PART_H
/**@endcond                   */

  #include <g3x.h>

  /**---------------------------------------------------------------*
   * le type "particule" :
   * - une position p
   * - une vitesse dont on gère séparément
   *   - la direction u (de norme 1)
   *   - l'amplitude  d
   *---------------------------------------------------------------**/
  typedef struct
  {
    G3Xpoint  p; // position
    G3Xvector u; // direction (doit TOUJOURS etre de norme 1)
    double    d; // la norme associée
  }
  G3Xpart;

/**-------------------------------------------------------------------------- *
 *  constructeur 1 : un point et une vitesse (décomposée en direction/norme)
 *---------------------------------------------------------------------------**/
  G3Xpart   g3x_Part_pv(G3Xpoint pos, G3Xvector vit);

/**-------------------------------------------------------------------------- *
 *  constructeur 2 : un point et une direction et une norme
 *---------------------------------------------------------------------------**/
  G3Xpart   g3x_Part_pud(G3Xpoint pos, G3Xvector dir, double norm);

/**-------------------------------------------------------------------------- *
 *  constructeur 3 : un bi-point [AB] => A, direction(AB), distance(AB)
 *---------------------------------------------------------------------------**/
  G3Xpart   g3x_Part_2p(G3Xpoint A, G3Xpoint B);

/**-------------------------------------------------------------------------- *
 *  vecteur vitesse "réel" v=d*u
 *---------------------------------------------------------------------------**/
  G3Xvector g3x_PartFullVect(G3Xpart *a);

/**-------------------------------------------------------------------------- *
 *  position de l'extremité du vecteur p+d*u
 *---------------------------------------------------------------------------**/
  G3Xpoint  g3x_PartNextPos(G3Xpart *a);

/**-------------------------------------------------------------------------- *
 *  déplacement de la particule p <- p+d*u
 *  équivalent à a->p = g3x_PartNextPos(a);
 *---------------------------------------------------------------------------**/
  void      g3x_PartMove(G3Xpart *a);

/**-------------------------------------------------------------------------- *
 *  rotation d'angles "rad" (donc en radians...) du vecteur directeur u
 *---------------------------------------------------------------------------**/
  void      g3x_PartRotate(G3Xpart *a, G3Xcoord radxyz);


/**========================================================================= *
 *  DES FONCTIONALITES DE PLUS HAUT NIVEAU
 * ========================================================================= **/
/**-------------------------------------------------------------------------- *
 *  garde la particule dans un volume (cinétique inverse)
 *  la "box", parallele aux axes, est definie par ses valeurs min
 *  (DLB Down/Left/Back) et max (URF Up/Right/Front)
 *---------------------------------------------------------------------------**/
  void      g3x_PartBoxBounce(G3Xpart *a, G3Xcoord DLB, G3Xcoord URF);
  void      g3x_PartBoxCross(G3Xpart *a, G3Xcoord DLB, G3Xcoord URF);
/**-------------------------------------------------------------------------- *
 *  intersection des trajectoire et récupération du point de collision
 *---------------------------------------------------------------------------**/
  bool      g3x_PartInterPart(G3Xpart *a, G3Xpart *b, G3Xpoint *I);

/**-------------------------------------------------------------------------- *
 *  collision/déviation de 2 particules dans un rayon 'dist'
 *  concrètement, il faudra associer un 'rayon' à chaque particule et
 *  on utilisera dist = (ray_a+ray_b)
 *---------------------------------------------------------------------------**/
  bool      g3x_PartCollPart(G3Xpart *a, G3Xpart *b, double ray);

/**-------------------------------------------------------------------------- *
 *  Collision/déviation Particule/Sphere
 *---------------------------------------------------------------------------**/
  bool      g3x_PartCollSphere(G3Xpart *a, G3Xpoint C, double r);
  bool      g3x_PartBypassSphere(G3Xpart *a, G3Xpoint C, double r, double coeff);

/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
  bool      g3x_PartPursuit(G3Xpart *a, G3Xpart *b, double alf, double dist);

/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
  bool      g3x_PartPosTrack(G3Xpart *a, G3Xpoint tar, double alf, double dist);
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!**/
