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

#ifdef _GFL3D_
  #ifndef _GFL_PART_3D_H
    #define _GFL_PART_3D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
    #include <gfl_geom3d.h>
  
    /**---------------------------------------------------------------*
     * le type "particule" :
     * - une position p
     * - une vitesse dont on gère séparément
     *   - la direction u (de norme 1)
     *   - l'amplitude  d
     *---------------------------------------------------------------**/
    typedef struct
    {
      GFLpoint  p; // position
      GFLvector u; // direction (doit TOUJOURS etre de norme 1)
      double    d; // la norme associée
    }
    GFLpart;
  
  /**-------------------------------------------------------------------------- *
   *  constructeur 1 : un point et une vitesse (décomposée en direction/norme)
   *---------------------------------------------------------------------------**/
    GFLpart   gfl_Part_pv(GFLpoint pos, GFLvector vit);
  
  /**-------------------------------------------------------------------------- *
   *  constructeur 2 : un point et une direction et une norme
   *---------------------------------------------------------------------------**/
    GFLpart   gfl_Part_pud(GFLpoint pos, GFLvector dir, double norm);
  
  /**-------------------------------------------------------------------------- *
   *  constructeur 3 : un bi-point [AB] => A, direction(AB), distance(AB)
   *---------------------------------------------------------------------------**/
    GFLpart   gfl_Part_2p(GFLpoint A, GFLpoint B);
  
  /**-------------------------------------------------------------------------- *
   *  vecteur vitesse "réel" v=d*u
   *---------------------------------------------------------------------------**/
    GFLvector gfl_PartFullVect(GFLpart *a);
  
  /**-------------------------------------------------------------------------- *
   *  position de l'extremité du vecteur p+d*u
   *---------------------------------------------------------------------------**/
    GFLpoint  gfl_PartNextPos(GFLpart *a);
  
  /**-------------------------------------------------------------------------- *
   *  déplacement de la particule p <- p+d*u
   *  équivalent à a->p = gfl_PartNextPos(a);
   *---------------------------------------------------------------------------**/
    void      gfl_PartMove(GFLpart *a);
  
  /**-------------------------------------------------------------------------- *
   *  rotation d'angles "rad" (donc en radians...) du vecteur directeur u
   *---------------------------------------------------------------------------**/
    void      gfl_PartRotate(GFLpart *a, GFLcoord radxyz);
  
  
  /**========================================================================= *
   *  DES FONCTIONALITES DE PLUS HAUT NIVEAU
   * ========================================================================= **/
  /**-------------------------------------------------------------------------- *
   *  garde la particule dans un volume (cinétique inverse)
   *  la "box", parallele aux axes, est definie par ses valeurs min
   *  (DLB Down/Left/Back) et max (URF Up/Right/Front)
   *---------------------------------------------------------------------------**/
    void      gfl_PartBoxBounce(GFLpart *a, GFLcoord DLB, GFLcoord URF);
    void      gfl_PartBoxCross (GFLpart *a, GFLcoord DLB, GFLcoord URF);
  /**-------------------------------------------------------------------------- *
   *  intersection des trajectoire et récupération du point de collision
   *---------------------------------------------------------------------------**/
    bool      gfl_PartInterPart(GFLpart *a, GFLpart *b, GFLpoint *I);
  
  /**-------------------------------------------------------------------------- *
   *  collision/déviation de 2 particules dans un rayon 'dist'
   *  concrètement, il faudra associer un 'rayon' à chaque particule et
   *  on utilisera dist = (ray_a+ray_b)
   *---------------------------------------------------------------------------**/
    bool      gfl_PartCollPart(GFLpart *a, GFLpart *b, double ray);
  
  /**-------------------------------------------------------------------------- *
   *  Collision/déviation Particule/Sphere
   *---------------------------------------------------------------------------**/
    bool      gfl_PartCollSphere(GFLpart *a, GFLpoint C, double r);
    bool      gfl_PartBypassSphere(GFLpart *a, GFLpoint C, double r, double coeff);
  
  /**-------------------------------------------------------------------------- *
   *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
   *  si dist(a,b)<dist : pas de poursuite
   *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
   *---------------------------------------------------------------------------**/
    bool      gfl_PartPursuit(GFLpart *a, GFLpart *b, double alf, double dist);
  
  /**-------------------------------------------------------------------------- *
   *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
   *  si dist(a,b)<dist : pas de poursuite
   *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
   *---------------------------------------------------------------------------**/
    bool      gfl_PartPosTrack(GFLpart *a, GFLpoint tar, double alf, double dist);
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!**/
