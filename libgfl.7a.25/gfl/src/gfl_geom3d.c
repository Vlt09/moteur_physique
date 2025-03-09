/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6.0 - Apr.2020
  =====================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

/*====================================================================*/
/* compatibilité OpenGl                                               */
/*====================================================================*/
/* Equivalent glVertex3dv(double p[3]) */
extern void gfl_Vertex3dv(GFLpoint p) { glVertex3d(p.x,p.y,p.z); };

/* Equivalent glNormal3dv(double v[3]) */
extern void gfl_Normal3dv(GFLvector v) { glNormal3d(v.x,v.y,v.z); };


/*====================================================================*/
/*         points & vecteurs - constructeurs/opérateurs               */
/*====================================================================*/
extern GFLpoint gfl_Point3d(double x, double y, double z)
{
  return (GFLpoint){x,y,z};
}

extern GFLpoint gfl_PplusV(GFLpoint P, GFLvector V)
{
  return (GFLpoint){P.x+V.x,P.y+V.y,P.z+V.z};
}
/* (A,B) -> AB */
extern GFLvector gfl_Vector2p(GFLpoint A, GFLpoint B)
{
  return (GFLvector){(B.x-A.x),(B.y-A.y),(B.z-A.z)};
}

/* (x,y,z) -> V(x,y,z) */
extern GFLvector gfl_Vector3d(double x, double y, double z)
{
  return (GFLvector){x,y,z};
}

/* (U,V) -> U+V */
extern GFLvector gfl_AddVect(GFLvector U, GFLvector V)
{
  return (GFLvector){(U.x+V.x),(U.y+V.y),(U.z+V.z)};
}
/* (U,V) -> U-V */
extern GFLvector gfl_SubVect(GFLvector U, GFLvector V)
{
  return (GFLvector){(U.x-V.x),(U.y-V.y),(U.z-V.z)};
}

/* mapping scalaire Point/Vecteur */
extern GFLcoord gfl_mapscal3(GFLcoord C, double   umap)
{
  return (GFLcoord){umap*C.x,umap*C.y,umap*C.z};
}

/* mapping vectoriel Point/Vecteur */
extern GFLcoord gfl_mapvect3(GFLcoord C, GFLcoord umap)
{
  return (GFLcoord){umap.x*C.x,umap.y*C.y,umap.z*C.z};
}


/* crée un vecteur de direction, aléatoire de norme < n  */
extern GFLvector gfl_RandVector(double maxnorm)
{
  return (GFLvector){ gfl_Rand_MinMax(-maxnorm,+maxnorm),
                      gfl_Rand_MinMax(-maxnorm,+maxnorm),
                      gfl_Rand_MinMax(-maxnorm,+maxnorm) };
}

extern GFLvector gfl_NormalVector(GFLvector V)
{
  gfl_Normalize(&V);
  return V;
}

extern GFLvector gfl_NormalVector2p(GFLpoint A, GFLpoint B)
{
  GFLvector V=gfl_Vector2p(A,B);
  gfl_Normalize(&V);
  return V;
}

/* normalise le vecteur V et renvoie la norme */
extern double gfl_Normalize(GFLvector *V)
{
  double norm = SQR(V->x)+SQR(V->y)+SQR(V->z);
  if (norm<1.E-20) norm=1.;
  else norm = sqrt(norm);
  V->x/=norm;
  V->y/=norm;
  V->z/=norm;
  return norm;
}

/* produit scalaire  : cree le scalaire U°V */
extern double gfl_ProdScal(GFLvector U, GFLvector V)
{
  return (U.x*V.x+U.y*V.y+U.z*V.z);
}

/* produit scalaire : cree le scalaire AB°U */
extern double gfl_ProdScal2p1v(GFLpoint A, GFLpoint B, GFLvector U)
{
  return ((B.x-A.x)*U.x+(B.y-A.y)*U.y+(B.x-A.x)*U.z);
}

/* produit scalaire : cree le scalaire AB°AC */
extern double gfl_ProdScal3p(GFLpoint A, GFLpoint B, GFLpoint C)
{
  return ((B.x-A.x)*(C.x-A.x)+(B.y-A.y)*(C.y-A.y)+(B.x-A.x)*(C.z-A.z));
}

/* produit vectoriel (U^V) */
extern GFLvector gfl_ProdVect(GFLvector U, GFLvector V)
{
  return (GFLvector){(U.y*V.z-U.z*V.y),(U.z*V.x-U.x*V.z),(U.x*V.y-U.y*V.x)};
}

/* produit vectoriel (AB^AC) */
extern GFLvector gfl_ProdVect3p(GFLpoint A, GFLpoint B, GFLpoint C)
{
  return gfl_ProdVect(gfl_Vector2p(A,B),gfl_Vector2p(A,C));
}

/* produit vectoriel (AB^U) */
extern GFLvector gfl_ProdVect2p1v(GFLpoint A, GFLpoint B, GFLvector U)
{
  return gfl_ProdVect(gfl_Vector2p(A,B),U);
}

/* produit vectoriel (U^AB) */
extern GFLvector gfl_ProdVect1v2p(GFLvector U, GFLpoint A, GFLpoint B)
{
  return gfl_ProdVect(U,gfl_Vector2p(A,B));
}

/* renvoie le milieu du segment (AB) */
extern GFLpoint gfl_Milieu(GFLpoint A, GFLpoint B)
{
  return (GFLpoint){(A.x+B.x)*0.5, (A.y+B.y)*0.5, (A.z+B.z)*0.5 };
}

/* renvoie le barycentre du triangle (ABC) */
extern GFLpoint gfl_Baryc3(GFLpoint A, GFLpoint B, GFLpoint C)
{
  return (GFLpoint){(A.x+B.x+C.x)/3., (A.y+B.y+C.y)/3., (A.z+B.z+C.z)/3. };
}

/* distance euclidienne au carre */
extern double gfl_SqrDist(GFLpoint A,GFLpoint B)
{
  return (SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z));
}

/* distance euclidienne */
extern double gfl_Dist(GFLpoint A, GFLpoint B)
{
  return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z)));
}

/* norme du vecteur V au carre */
extern double gfl_SqrNorm(GFLvector V)
{
  return (SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme du vecteur V  */
extern double gfl_Norm(GFLvector V)
{
  return sqrt(SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme L2 du vecteur V  */
extern double gfl_NormL2(GFLvector V)
{
  return MAX3(fabs(V.x),fabs(V.y),fabs(V.z));
}

/* renvoie le cosinus de l'angle UV (produit scalaire normé) */
extern double gfl_CosVect(GFLvector U, GFLvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.x + U.x*V.y)/sqrt(nunv);
}

/* renvoie le sinus de l'angle UV (produit vectoriel normé) */
extern double gfl_SinVect(GFLvector U, GFLvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.y - U.y*V.x)/sqrt(nunv);
}

#ifdef __cplusplus
  }
#endif
