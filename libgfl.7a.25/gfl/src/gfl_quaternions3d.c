/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

extern void gfl_QuatIdentity(GFLquat A)
{
  memset(&A,0,sizeof(GFLquat));
  A.r = 1.0;
}

extern GFLquat gfl_QuatSet(double r, GFLvector v)
{
  return (GFLquat){r,v};
}

extern GFLquat gfl_QuatSet4(double r, double x, double y, double z)
{
  return (GFLquat){r,(GFLvector){x,y,z}};
}

extern GFLquat gfl_QuatAdd(GFLquat A, GFLquat B)
{
  return (GFLquat){A.r+B.r,gfl_AddVect(A.v,B.v)};
}

extern GFLquat gfl_QuatProd(GFLquat A, GFLquat B)
{
  GFLquat C;
  C.r = A.r*B.r - GFLprodscal(A.v,B.v);
  GFLvector w;
  GFLprodvect(w,A.v,B.v);
  C.v.x = A.r*B.v.x + B.r*A.v.x + w.x;
  C.v.y = A.r*B.v.y + B.r*A.v.y + w.y;
  C.v.z = A.r*B.v.z + B.r*A.v.z + w.z;
  return C;
}

extern GFLquat gfl_QuatConj(GFLquat A)
{
  A.v.x=-A.v.x;
  A.v.y=-A.v.y;
  A.v.z=-A.v.z;
  return A;
}

extern double gfl_QuatSqrNorm(GFLquat A)
{
  return SQR(A.r)+GFLsqrnorm(A.v);
}

extern double gfl_QuatNorm(GFLquat A)
{
  return sqrt(SQR(A.r)+GFLsqrnorm(A.v));
}

extern GFLquat gfl_QuatNormalize(GFLquat A)
{
  double  n=gfl_QuatSqrNorm(A);
  if (GFLISZERO(n)) { memset(&A,0,sizeof(GFLquat)); return A; }
  n = 1./sqrt(n);
  A.r   *=n;
  A.v=gfl_mapscal3(A.v,n);
  return A;
}

extern GFLquat gfl_QuatScalMap(GFLquat A, double a)
{
  A.r*=a;
  A.v=gfl_mapscal3(A.v,a);
  return A;
}


extern GFLquat gfl_QuatInv(GFLquat A)
{
  double n = gfl_QuatNorm(A);
  if (GFLISZERO(n)) { memset(&A,0,sizeof(GFLquat)); return A; }
  A.v.x=-A.v.x;
  A.v.y=-A.v.y;
  A.v.z=-A.v.z;
  gfl_QuatScalMap(A,1./n);
  return A;
}


extern void gfl_QuatToHmat(GFLquat A, GFLhmat M)
{
  M[a00]= A.r;   M[a01]=-A.v.x; M[a02]=-A.v.y; M[a03]=-A.v.z;
  M[a10]=+A.v.x; M[a11]= A.r;   M[a12]=-A.v.z; M[a13]=+A.v.y;
  M[a20]=+A.v.y; M[a21]=+A.v.z; M[a22]= A.r;   M[a23]=-A.v.x;
  M[a30]=+A.v.z; M[a33]=-A.v.y; M[a32]=+A.v.x; M[a33]= A.r;
}

extern void gfl_QuatPrint(GFLquat A)
{
  fprintf(stderr,"Quaternion : [%f|(%f,%f,%f)] ||Q||=%lf\t\r",A.r,A.v.x,A.v.y,A.v.z,gfl_QuatNorm(A));
}


extern void gfl_QuatRot(GFLquat Qrot, GFLcoord src, GFLcoord dest)
{
  GFLquat Qsrc = gfl_QuatSet(0,src);
  GFLquat Qdst;

  Qdst  = gfl_QuatProd(Qrot,Qsrc);
  Qrot.v= gfl_mapscal3(Qrot.v,-1.);
  Qdst  = gfl_QuatProd(Qdst,Qrot);
  dest  = Qdst.v;
}

extern void gfl_AxeRadRot(GFLvector v, double rad, GFLcoord src, GFLcoord dest)
{
  GFLvector u=v;
  gfl_Normalize(&u);/* direction normee */
  double c=cos(0.5*rad);
  double s=sin(0.5*rad);
  u=gfl_mapscal3(u,s);
  GFLquat Qrot = gfl_QuatSet(c,u);
  GFLquat Qsrc = gfl_QuatSet(0,src);
  GFLquat Qdst;

  Qdst = gfl_QuatProd(Qrot,Qsrc);
  u=gfl_mapscal3(u,-1.);
  Qrot = gfl_QuatSet(c,u);
  Qdst = gfl_QuatProd(Qdst,Qrot);
  dest = Qdst.v;
}



/* Quaternion d'alignement du vecteur v sur le vecteur cible */
extern GFLquat gfl_QuatAlign(GFLvector v, GFLvector const cible)
{
  GFLvector v1,c1,w;
  GFLquat   Q;

  /* c1 : cible normee */
  c1=cible;
  gfl_Normalize(&c1);

  v1=v;
  gfl_Normalize(&v1);
  /* v1 transforme en (v1+c1)/2 pour avoir l'angle moitie */
  /* angle(v1,c1)=0.5*angle(v,c) et v1 reste norme        */
  v1=gfl_AddVect(v1,c1);
  gfl_Normalize(&v1);

  /* direction de rotation : v1^c1 de norme sin(a) */
  GFLprodvect(w,v1,c1);

  /* et cos(a)=v1*c1 */
  return gfl_QuatSet(GFLprodscal(v1,c1),w);
}


#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
