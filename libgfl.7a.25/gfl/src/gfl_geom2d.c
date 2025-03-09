/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  =================================================================== */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

/*====================================================================*/
/*         points & vecteurs - constructeurs/opérateurs               */
/*====================================================================*/
extern GFLpoint gfl_Point2d(double x, double y) { return (GFLpoint){x,y}; }

/* (x,y) -> V(x,y) */
extern GFLvector gfl_Vector2d(double x, double y)  { return (GFLvector){x,y}; }
/* (A,B) -> AB */
extern GFLvector gfl_Vector2p(GFLpoint A, GFLpoint B) { return (GFLvector){(B.x-A.x),(B.y-A.y)}; }

/* (U,V) -> U+V */
extern GFLvector gfl_AddVect(GFLvector U, GFLvector V) { return (GFLvector){(U.x+V.x),(U.y+V.y)}; }
/* (U,V) -> U-V */
extern GFLvector gfl_SubVect(GFLvector U, GFLvector V) { return (GFLvector){(U.x-V.x),(U.y-V.y)}; }

/* mapping Point/Vecteur */
extern GFLcoord gfl_mapscal2(GFLcoord C, double   umap) { return (GFLcoord){umap*C.x,umap*C.y}; }
extern GFLcoord gfl_mapvect2(GFLcoord C, GFLcoord umap) { return (GFLcoord){umap.x*C.x,umap.y*C.y}; }

/* crée un point aléatoire dans la fenêtre */
extern GFLpoint gfl_RandPoint(double margin)
{
  return (GFLpoint){ gfl_Rand_MinMax(gfl_GetXMin()+margin,gfl_GetXMax()-margin) ,
                     gfl_Rand_MinMax(gfl_GetYMin()+margin,gfl_GetYMax()-margin) };
}

/* crée un vecteur de direction, aléatoire de norme définie  */
extern GFLvector gfl_RandVector(double norm)
{
  GFLvector V=(GFLvector){ gfl_Rand_MinMax(-1.,+1.),
                           gfl_Rand_MinMax(-1.,+1.) };
  gfl_Normalize(&V);
  return gfl_mapscal2(V,norm);
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
  double norm = SQR(V->x)+SQR(V->y);

  if (GFLISZERO(norm)) { V->x=V->y=0.; return 0.; }

  norm = sqrt(norm);
  V->x/=norm;
  V->y/=norm;

  return norm;
}

// applique une rotation d'angle "radians" à la direction de U
extern GFLvector gfl_VectRotate(GFLvector U, double radians)
{
  U = gfl_Mat_x_Vector(gfl_Rotation(radians),U);
  return U;
}

/* produit scalaire  : cree le scalaire U°V */
extern double gfl_ProdScal(GFLvector U, GFLvector V) { return (U.x*V.x + U.y*V.y); }
/* produit scalaire : cree le scalaire AB°AC */
extern double gfl_ProdScal3p(GFLpoint A, GFLpoint B, GFLpoint C) { return gfl_ProdScal(gfl_Vector2p(A,B),gfl_Vector2p(A,C)); }

/* produit vectoriel (U^V).z   : en 2D c'est un scalaire !!!! */
extern double gfl_ProdVect(GFLvector U, GFLvector V) { return (U.x*V.y - U.y*V.x); }
/* produit vectoriel (AB^AC).z : en 2D c'est un scalaire !!!! */
extern double gfl_ProdVect3p(GFLpoint A, GFLpoint B, GFLpoint C) { return gfl_ProdVect(gfl_Vector2p(A,B),gfl_Vector2p(A,C)); }

/* renvoie le milieu du segment (AB) */
extern GFLpoint gfl_Milieu(GFLpoint A, GFLpoint B) { return (GFLpoint){(A.x+B.x)*0.5, (A.y+B.y)*0.5 }; }
/* renvoie le barycentre du triangle (ABC) */
extern GFLpoint gfl_Baryc3(GFLpoint A, GFLpoint B, GFLpoint C) { return (GFLpoint){(A.x+B.x+C.x)/3., (A.y+B.y+C.y)/3. }; }

/* distance euclidienne au carre */
extern double gfl_SqrDist(GFLpoint A,GFLpoint B) { return (SQR(B.x-A.x)+SQR(B.y-A.y)); }
/* distance euclidienne */
extern double gfl_Dist(GFLpoint A, GFLpoint B) { return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y))); }

/* norme du vecteur V au carre */
extern double gfl_SqrNorm(GFLvector V) { return (SQR(V.x)+SQR(V.y)); }
/* norme du vecteur V  */
extern double gfl_Norm(GFLvector V)   { return sqrt(SQR(V.x)+SQR(V.y)); }

/* norme L2 du vecteur V  */
extern double gfl_NormL2(GFLvector V) { return MAX(fabs(V.x),fabs(V.y)); }

/* retourne +1 si (ABC) orienté trigo+
             0 .........alignés
            -1 .........orienté trigo- */
extern int gfl_Orient(GFLpoint A, GFLpoint B, GFLpoint C)
{
  double o = gfl_ProdVect3p(A,B,C);
  if (GFLISZERO(o)) return 0;
  return (o>0.? +1.:-1.);
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
