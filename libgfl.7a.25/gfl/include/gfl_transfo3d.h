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
  #ifndef _GFL_TRANSFO_3D_H
    #define _GFL_TRANSFO_3D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
    #include <gfl_geom3d.h>
  
    /*! MATRICE EN COORDONNEES HOMOGENE
     *  type matrice en coordonnees homogenes
     *  ----------------------------------------------------------
     *  ATTENTION  : OpenGl gere ça directement en <double mat[16]
     *               et traite ça par colonne plutôt que par ligne
    !*/
    typedef struct { double m[16]; } GFLhmat;
    /*! | 00 04 08 | 12 |!*/
    /*! | 01 05 09 | 13 |!*/
    /*! | 02 06 10 | 14 |!*/
    /*! | 03 07 11 | 15 |!*/
  
     /*! 1° colonne!*/
    #define c00  0
    #define c10  1
    #define c20  2
    #define c30  3
    /*! 2° colonne!*/
    #define c01  4
    #define c11  5
    #define c21  6
    #define c31  7
    /*! 3° colonne!*/
    #define c02  8
    #define c12  9
    #define c22 10
    #define c32 11
    /*! 4° colonne!*/
    #define c03 12
    #define c13 13
    #define c23 14
    #define c33 15
  
  
    /*=****************************************!*/
    /*!  Identite                             !*/
    /*=****************************************!*/
    GFLhmat   gfl_Identity(void);
    /*=****************************************!*/
    /*!  MATRICES DE TRANSFORMATION STANDARD  !*/
    /*=****************************************!*/
    GFLhmat   gfl_Translation1v(GFLvector t);
    GFLhmat   gfl_Translation3d(double tx, double ty, double tz);
    GFLhmat   gfl_Homothetie1v(GFLvector h);
    GFLhmat   gfl_Homothetie3d(double hx, double hy, double hz);
    GFLhmat   gfl_RotationZ(double a_radians);
    GFLhmat   gfl_RotationY(double a_radians);
    GFLhmat   gfl_RotationX(double a_radians);
    /*=****************************************!*/
    /*! Fonctions utiltaires sur les matrices !*/
    /*=****************************************!*/
    /*! Produit Matrice(A)*Vecteur(V) -> Vecteur!*/
    GFLvector gfl_Mat_x_Vector(GFLhmat A, GFLvector V);
    /*! Produit Matrice(A)*Point(P) -> Point    !*/
    GFLpoint  gfl_Mat_x_Point(GFLhmat A, GFLpoint P);
    /*! version generique des 2 precedentes     !*/
    GFLcoord  gfl_Mat_x_Coord3d(GFLhmat A, double x, double y, double z, double delta);
    /*! Produit Matrice(A)*Matrice(B)->Matrice  !*/
    GFLhmat   gfl_Mat_x_Mat(GFLhmat A,GFLhmat B);
    /*! Affichage de la matrice!*/
    void gfl_Mat_print(GFLhmat A);
    /*! conversion (transposition) au format de amatrice OpenGl!*/
    GFLhmat gfl_FormatGLmat(GFLhmat src);
  
    bool gfl_Mat_Invert(GFLhmat M, GFLhmat *Mi);
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*============================================================================!*/
