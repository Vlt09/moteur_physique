/**@file    gfl_geoalgo<.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#endif

#ifdef _GFL2D_
  #ifndef _GFL_GEOALGO_2D_H
    #define _GFL_GEOALGO_2D_H
  /**@endcond                   */
  
    #include <gfl_geom2d.h>
  
    /** determine la porjection P du point C sur la dte (AB) -- retour : t tq AP=t.AB *
     * t<0. P est 'avant A', 0.<t<1. P est sur [A,B], 1.<t P est 'après B'           **/
    double    gfl_ProjPointDroite(GFLpoint A, GFLpoint  B, GFLpoint C, GFLpoint *P);
  
    /** même chose, sur une trajectoire (A,v) renvoie d tel que P = A+d.v             *
     *  ATTENTION : v n'est pas forcement de norme 1 => à l'utilisateur de gerer ca  **/
    double    gfl_ProjPointTrajet(GFLpoint A, GFLvector v, GFLpoint C, GFLpoint *P);
  
    /** inteserction de 2 droites -- retour:VRAI/FAUX, intersection dans I **/
    bool      gfl_DroiteInterDroite(GFLpoint A, GFLvector AB, GFLpoint C, GFLvector CD, GFLpoint *I);
  
    /** intersection des segments [AB] et [CD] - comme les dtes, mais inters. sur les 2 segments **/
    bool      gfl_SegmentInterSegment(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint D, GFLpoint *I);
  
  /** Intersection Segment/Cercle :
    *  0 : [AB] entierement dans le cercle
    * -1 : A et B confondus ou Inters. Dte/Cercle hors de [AB] ou pas d'intesection Dte/Cercle
    * +1 : [AB] tangent au Cercle
    * +2 : A dedans, B dehors
    * +3 : A dehors, B dedans
    * +4 : A et B dehors, 2 intersections
    ---------------------------------------------------------------------------------------**/
    int       gfl_SegmentInterCercle(GFLpoint A, GFLpoint B, GFLpoint C, double r, GFLpoint *I1, GFLpoint *I2);
  
  /** Intersection Cercle/Cercle
    * -2 : centres des cercles confondus
    * -1 : un cercle contient l'autre
    *  0 : pas d'intersection, cercles disjoints
    * +1 : deux intersections
    * +2 : cercles tangents
    -----------------------------------------------**/
    int       gfl_CercleInterCercle(GFLpoint C1, double r1, GFLpoint C2, double r2, GFLpoint *I1, GFLpoint *I2);
  
    /** Cercles inscrit et circonscrit au triangle ABC **/
    void      gfl_CercleInscrit(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint *CCI, double *rci);
    void      gfl_CercleCirconscrit(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint *CCC, double *rcc);
  
    /** clipping d'un segment [AB] sur un rectangle parallele auyx axes (DownLeft,UpRight..) -- retour:VRAI/FAUX  **/
    bool      gfl_CohenSutherland(GFLpoint *A, GFLpoint *B, GFLpoint DL, GFLpoint UL, GFLpoint UR, GFLpoint DR);
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
