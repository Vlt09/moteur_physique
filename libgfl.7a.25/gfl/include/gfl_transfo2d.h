/**@file    gfl_transfo.h
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

#ifdef _GFL2D_
  #ifndef _GFL_TRANSFO2D_H
    #define _GFL_TRANSFO2D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
    #include <gfl_geom2d.h>
  
    /*! MATRICE EN COORDONNEES HOMOGENE
     *  type matrice en coordonnees homogenes
     *  ATTENTION  : OpenGl gere ça directement en <double mat[9]
     *               et traite ça par colonne plutôt que par ligne
    !*/
    typedef struct { double m[9]; } GFLhmat;
    /* | 00 03 | 06 | */
    /* | 01 04 | 07 | */
    /* | 02 05 | 08 | */
  
     /* 1° colonne */
    #define c00  0
    #define c10  1
    #define c20  2
    /* 2° colonne */
    #define c01  3
    #define c11  4
    #define c21  5
    /* 3° colonne */
    #define c02  6
    #define c12  7
    #define c22  8
  
    /*!****************************************!*/
    /*!   Identite                             !*/
    /*!****************************************!*/
    GFLhmat   gfl_Identity(void);
    /*!****************************************!*/
    /*!   MATRICES DE TRANSFORMATION STANDARD  !*/
    /*!****************************************!*/
    GFLhmat   gfl_Translation1v(GFLvector t);
    GFLhmat   gfl_Translation2d(double tx, double ty);
    GFLhmat   gfl_Homothetie1v(GFLvector h);
    GFLhmat   gfl_Homothetie2d(double hx, double hy);
    GFLhmat   gfl_Rotation(double a_radians);
    /** version (en degrés) utilisant les tables (LUT)
     *  de cos/sin prédéfinie -- précision 1/2 degré
     *  moins précis, mais plus rapide         **/
    GFLhmat   gfl_Rotation_LUT(double a_degre);
  
    /*!****************************************!*/
    /*!  Fonctions utiltaires sur les matrices !*/
    /*!****************************************!*/
    /* Produit Matrice(A)*Vecteur(V) -> Vecteur */
    GFLvector gfl_Mat_x_Vector(GFLhmat A, GFLvector V);
    /* Produit Matrice(A)*Point(P) -> Point     */
    GFLpoint  gfl_Mat_x_Point(GFLhmat A, GFLpoint P);
    /* version generique des 2 precedentes      */
    GFLcoord  gfl_Mat_x_Coord2d(GFLhmat A, double x, double y, double delta);
    /* Produit Matrice(A)*Matrice(B)->Matrice   */
    GFLhmat   gfl_Mat_x_Mat(GFLhmat A,GFLhmat B);
    /* Affichage de la matrice */
    void      gfl_Mat_print(GFLhmat A);
    /* Creation de la matrice 3x3 de rotation autour du GFLpoint C, d'angle alpha */
    GFLhmat   gfl_RotationC(GFLpoint C, float alpha);
    GFLhmat   gfl_RotationCXY(double Cx, double Cy, float alpha) ;
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
