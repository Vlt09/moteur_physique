/**@file    gfl_geom2d.h
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
  #ifndef _GFL_GEOM_2D_H
    #define _GFL_GEOM_2D_H
  /**@endcond                   */
    
    /******************************************************************
     * Coordonnees (x,y) -> point, vecteur
     * ----------------------------------------------------------------
     * Le type 'primaire' GFLcoord s'utilise lorsqu'on manipule des
     * coordonnées qui ne sont pas clairement identifiées comme <Point>
     * ou <Vecteur>
     ******************************************************************/
    typedef struct
    {
      double x,y;
    }
    GFLcoord, GFLpoint, GFLvector;
  
    /** PRIMITIVES GEOMETRIQUES ET FONCTIONS UTILES                  **/
              /** construit le point (x,y) **/
    GFLpoint  gfl_Point2d(double x, double y);
              /** construit le point Q=P+V **/
    GFLpoint  gfl_PplusV(GFLpoint P, GFLvector V);
              /** crée un point aléatoire dans la fenêtre (avec une marge de securite) **/
    GFLpoint  gfl_RandPoint(double margin);
  
              /** construit le vecteur (x,y) **/
    GFLvector gfl_Vector2d(double x, double y);
              /** construit le vecteur AB **/
    GFLvector gfl_Vector2p(GFLpoint A, GFLpoint B);
              /** crée un vecteur de direction aléatoire de norme definie  **/
    GFLvector gfl_RandVector(double norm);
  
              /** construit le vecteur U+V **/
    GFLvector gfl_AddVect(GFLvector U, GFLvector V);
              /** construit le vecteur U-V **/
    GFLvector gfl_SubVect(GFLvector U, GFLvector V);
  
              /** rotation d'angle "radians" sur le vecteur U **/
    GFLvector gfl_VectRotate(GFLvector U, double radians);
  
              /** mapping scalaire  => C*umap **/
    GFLcoord  gfl_mapscal2(GFLcoord C, double   umap);
              /** mapping vectoriel => C*umap **/
    GFLcoord  gfl_mapvect2(GFLcoord C, GFLcoord umap);
  
              /** renvoie une copie 'normee' de U (V=U/||U||) **/
    GFLvector gfl_NormalVector(GFLvector U);
              /** renvoie le vecteur norme AB/d(A,B)          **/
    GFLvector gfl_NormalVector2p(GFLpoint A, GFLpoint B);
              /** normalise le vecteur V et renvoie la norme  **/
    double    gfl_Normalize(GFLvector *V);
        /** calcule le produit scalaire U°V et AB°AC**/
    double    gfl_ProdScal(GFLvector U, GFLvector V);
    double    gfl_ProdScal3p(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /** produit vectoriel (U^V).z et (AB^AC).z  : en 2D c'est un scalaire !!!! **/
    double    gfl_ProdVect(GFLvector U, GFLvector V);
    double    gfl_ProdVect3p(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /** renvoie le milieu du segment (AB)       **/
    GFLpoint  gfl_Milieu(GFLpoint A, GFLpoint B);
              /** renvoie le barycentre du triangle (ABC) **/
    GFLpoint  gfl_Baryc3(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /** distance euclidienne au carre **/
    double    gfl_SqrDist(GFLpoint A,GFLpoint B);
              /** distance euclidienne **/
    double    gfl_Dist(GFLpoint A, GFLpoint B);
  
              /** norme euclidienne du vecteur V au carre (v.x)²+(v.y)² **/
    double    gfl_SqrNorm(GFLvector V);
              /** norme euclidienne du vecteur V                        **/
    double    gfl_Norm(GFLvector V);
              /** norme L2 du vecteur V  max(|v.x|,|v.y|)               **/
    double    gfl_NormL2(GFLvector V);
  
              /** retourne +1 si (ABC) orienté trigo+
                 0 .........alignés
                -1 .........orienté trigo- **/
    int       gfl_Orient(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /** calcule le cosinus de l'angle (U,V) -- produit scalaire  normé **/
    double    gfl_CosVect(GFLvector U, GFLvector V);
              /** calcule le   sinus de l'angle (U,V) -- produit vectoriel normé **/
    double    gfl_SinVect(GFLvector U, GFLvector V);

    /** Quelques versions en macros - conservées pour compatibilité avec anciennes versions *
     *  MAIS IL FAUT EVITER DE LES UTILISER                                                **/
    #define GFLmilieu(A,B)                      /***/((GFLpoint ){(0.5*((A).x+(B).x)),(0.5*((A).y+(B).y))})
    #define GFLdefvectorxy(x,y)                 /***/((GFLvector){(double)(x),(double)(y)})
    #define GFLdefvector(A,B)                   /***/((GFLvector){((B).x-(A).x),((B).y-(A).y)})
    #define GFLnormalvector2P(A,B)              /***/((GFLvector){((A).y-(B).y),((B).x-(A).x)})
    #define GFLnormalvector(V)                  /***/((GFLvector){(-(V).y),((V).x)})
    #define GFLaddvector(U,V)                   /***/((GFLvector){((U).x+(V).x),((U).y+(V).y)})
    #define GFLdifvector(U,V)                   /***/((GFLvector){((U).x-(V).x),((U).y-(V).y)})
    #define GFLmulvector(U,a)                   /***/((GFLvector){((double)(a))*(U).x,((double)(a))*(U).y})
    #define GFLpointplusvector(A,V)             /***/((GFLpoint ){((A).x+(V).x),((A).y+(V).y)})
    #define GFLsqrdist(A,B)                     /***/(SQR((B).x-(A).x)+SQR((B).y-(A).y))
    #define GFLdist(A,B)                        /***/(sqrt(GFLsqrdist((A),(B))))
    #define GFLsqrnorm(U)                       /***/(SQR((U).x)+SQR((U).y))
    #define GFLnorm(U)                          /***/(sqrt(GFLsqrnorm(U)))
    #define GFLprodscal(U,V)                    /***/(((U).x*(V).x)+((U).y*(V).y))
    #define GFLpntprodscal(A,B,C)               /***/((((B).x-(A).x)*((C).x-(A).x)) + (((B).y-(A).y)*((C).y-(A).y)))
    #define GFLprodvect(U,V)                    /***/( ((U).x*(V).y)-((U).y*(V).x))
    #define GFLprodvect3P(A,B,C)                /***/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x)))
    #define GFLorient(A,B,C)                    /***/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x))>0.?+1:-1)
    #define GFLorientv(u,v)                     /***/( ((u).x*(v).y>(u).y*(v).x)?+1:-1)
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!**/
