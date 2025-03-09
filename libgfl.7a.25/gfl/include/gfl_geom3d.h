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
  #ifndef _GFL_GEOM_3D_H
    #define _GFL_GEOM_3D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
  
    /******************************************************************
     * Coordonnees XYZ - Attention OpenGl gere ca en <double xyz[3]>
     * ----------------------------------------------------------------
     * Le type 'primaire' GFLcoord s'utilise lorsqu'on manipule des
     * coordonnées qui ne sont pas clairement identifiées comme <Point>
     * ou <Vecteur>
     ******************************************************************/
    typedef struct
    {
      double x,y,z;
    }
    GFLcoord, GFLpoint, GFLvector;
  
    /* A VOIR : introduire un type "Coord. Sphérique" (r,theta,phi)
     * typedef struct
     * {
     *   double r,t,f;
     * }
     * A voir en vis à vis avec <gfl_quaternions> et <gfl_particules>
     * => bcp de choses proches
     */
  
              /****************************************************************/
              /*! PRIMITIVES GEOMETRIQUES ET FONCTIONS UTILES                !*/
              /****************************************************************/
              /*! construit le point (x,y,z)                                 !*/
    GFLpoint  gfl_Point3d(double x, double y, double z);
              /*! construit le point Q=P+V                                   !*/
    GFLpoint  gfl_PplusV(GFLpoint P, GFLvector V);
  
              /*! construit le vecteur (x,y,z)!*/
    GFLvector gfl_Vector3d(double x, double y, double z);
              /*! construit le vecteur AB!*/
    GFLvector gfl_Vector2p(GFLpoint A, GFLpoint B);
              /*! crée un vecteur de direction, aléatoire de norme < maxnorm !*/
    GFLvector gfl_RandVector(double maxnorm);
  
              /*! construit le vecteur U+V!*/
    GFLvector gfl_AddVect(GFLvector U, GFLvector V);
              /*! construit le vecteur U-V!*/
    GFLvector gfl_SubVect(GFLvector U, GFLvector V);
  
              /*! renvoie le milieu du segment (AB)      !*/
    GFLpoint  gfl_Milieu(GFLpoint A, GFLpoint B);
              /*! renvoie le barycentre du triangle (ABC)!*/
    GFLpoint  gfl_Baryc3(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /*! mapping scalaire  => C*umap!*/
    GFLcoord  gfl_mapscal3(GFLcoord C, double   umap);
              /*! mapping vectoriel => C*umap!*/
    GFLcoord  gfl_mapvect3(GFLcoord C, GFLcoord umap);
  
    /* conventions de nommage à revoir */
  
              /*! calcule le produit scalaire U°V, AB°U=U°AB et AB°AC*/
    double    gfl_ProdScal(GFLvector U, GFLvector V);
    double    gfl_ProdScal3p(GFLpoint A, GFLpoint B, GFLpoint C);
    double    gfl_ProdScal2p1v(GFLpoint A, GFLpoint B, GFLvector U);
  
              /*! produit vectoriel (U^V), (AB^V), (U^AC) et (AB^AC)!*/
    GFLvector  gfl_ProdVect(GFLvector U, GFLvector V);
    GFLvector  gfl_ProdVect2p1v(GFLpoint A, GFLpoint B, GFLvector V);
    GFLvector  gfl_ProdVect1v2p(GFLvector U,GFLpoint A, GFLpoint C);
    GFLvector  gfl_ProdVect3p(GFLpoint A, GFLpoint B, GFLpoint C);
  
              /*! normalise le vecteur!*/
    GFLvector gfl_NormalVector(GFLvector U);
    GFLvector gfl_NormalVector2p(GFLpoint A, GFLpoint B);
              /*! normalise le vecteur V et renvoie la norme!*/
    double    gfl_Normalize(GFLvector *V);
  
              /*! distance euclidienne au carre!*/
    double    gfl_SqrDist(GFLpoint A,GFLpoint B);
              /*! distance euclidienne!*/
    double    gfl_Dist(GFLpoint A, GFLpoint B);
  
              /*! norme euclidienne au carre (v.x)²+(v.y)²+(v.z)²      !*/
    double    gfl_SqrNorm(GFLvector V);
              /*! norme euclidienne du vecteur V                       !*/
    double    gfl_Norm(GFLvector V);
              /*! norme L2 du vecteur V  max(|v.x|,|v.y|,|v.z|)        !*/
    double    gfl_NormL2(GFLvector V);
  
  
    /**---------------------------------------------------------------*
     *  pour compatibilité OpenGl
     * ---------------------------------------------------------------*/
    void gfl_Vertex3dv(GFLpoint  p); /*! Equivalent glVertex3dv(double p[3])!*/
    void gfl_Normal3dv(GFLvector v); /*! Equivalent glNormal3dv(double v[3])!*/
  
    /** Quelques versions en macros - conservées pour compatibilité avec anciennes versions *
     *  MAIS IL FAUT EVITER DE LES UTILISER                                                **/
    #define GFLmilieu(A,B)           /**/((GFLpoint ){(0.5*((A).x+(B).x)),(0.5*((A).y+(B).y)),,(0.5*((A).z+(B).z))})
    #define GFLdefvectorxy(x,y)      /**/((GFLvector){(double)(x),(double)(y),(double)(z)})
    #define GFLdefvector(A,B)        /**/((GFLvector){((B).x-(A).x),((B).y-(A).y),((B).z-(A).z)})
    #define GFLaddvector(U,V)        /**/((GFLvector){((U).x+(V).x),((U).y+(V).y),((U).z+(V).z)})
    #define GFLdifvector(U,V)        /**/((GFLvector){((U).x-(V).x),((U).y-(V).y),((U).z-(V).z)})
    #define GFLmulvector(U,a)        /**/((GFLvector){((double)(a))*(U).x,((double)(a))*(U).y,((double)(a))*(U).z})
    #define GFLpointplusvector(A,V)  /**/((GFLpoint ){((A).x+(V).x),((A).y+(V).y),((A).z+(V).z)})
    #define GFLsqrdist(A,B)          /**/(SQR((B).x-(A).x)+SQR((B).y-(A).y)+SQR((B).z-(A).z))
    #define GFLdist(A,B)             /**/(sqrt(GFLsqrdist((A),(B))))
    #define GFLsqrnorm(U)            /**/(SQR((U).x)+SQR((U).y)+SQR((U).z))
    #define GFLnorm(U)               /**/(sqrt(GFLsqrnorm(U)))
    #define GFLprodscal(U,V)         /**/(((U).x*(V).x)+((U).y*(V).y)+((U).z*(V).z))
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
