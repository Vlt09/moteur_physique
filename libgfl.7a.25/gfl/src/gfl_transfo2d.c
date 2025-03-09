/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

extern GFLhmat gfl_Identity(void)
{
  GFLhmat Id;
  memset(Id.m,0,9*sizeof(double));
  Id.m[c00] = Id.m[c11] = Id.m[c22] = 1.0;
  return Id;
}

extern GFLhmat gfl_Translation1v(GFLvector t)
{
  GFLhmat A=gfl_Identity();
  A.m[c02] = t.x;
  A.m[c12] = t.y;
  return A;
}

extern GFLhmat gfl_Translation2d(double tx, double ty)
{
  GFLhmat A=gfl_Identity();
  A.m[c02] = tx;
  A.m[c12] = ty;
  return A;
}

extern GFLhmat gfl_Homothetie1v(GFLvector h)
{
  GFLhmat A=gfl_Identity();
  A.m[c00] = h.x;
  A.m[c11] = h.y;
  return A;
}

extern GFLhmat gfl_Homothetie2d(double hx, double hy)
{
  GFLhmat A=gfl_Identity();
  A.m[c00] = hx;
  A.m[c11] = hy;
  return A;
}

extern GFLhmat gfl_Rotation(double a_radians)
{
  GFLhmat A=gfl_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[c00] = +c; A.m[c01] = -s;
  A.m[c10] = +s; A.m[c11] = +c;
  return A;
}

/** version (en degrés) utilisant les tables (LUT)
  *  de cos/sin prédéfinie -- précision 1/2 degré
  *  moins précis, mais plus rapide         **/
extern GFLhmat gfl_Rotation_LUT(double a_degre)
{
  GFLhmat A=gfl_Identity();
  double c=gfl_Degcos(a_degre), s=gfl_Degsin(a_degre);
  A.m[c00] = +c; A.m[c01] = -s;
  A.m[c10] = +s; A.m[c11] = +c;
  return A;
}

extern GFLvector gfl_Mat_x_Vector(GFLhmat A, GFLvector V)
{
  GFLvector U;
  U.x = A.m[c00]*V.x + A.m[c01]*V.y;
  U.y = A.m[c10]*V.x + A.m[c11]*V.y;
  return U;
}

extern GFLpoint gfl_Mat_x_Point(GFLhmat A, GFLpoint P)
{
  GFLpoint M;
  M.x = A.m[c00]*P.x + A.m[c01]*P.y + A.m[c02];
  M.y = A.m[c10]*P.x + A.m[c11]*P.y + A.m[c12];
  return M;
}

extern GFLcoord gfl_Mat_x_Coord2d(GFLhmat A, double x, double y, double delta)
{
  GFLcoord M;
  M.x = A.m[c00]*x + A.m[c01]*y + A.m[c02]*delta;
  M.y = A.m[c10]*x + A.m[c11]*y + A.m[c12]*delta;
  return M;
}

extern GFLhmat gfl_Mat_x_Mat(GFLhmat A,GFLhmat B)
{
  GFLhmat D;
  /* 1ere colonne */
  D.m[c00] = A.m[c00]*B.m[c00] + A.m[c01]*B.m[c10] + A.m[c02]*B.m[c20];
  D.m[c10] = A.m[c10]*B.m[c00] + A.m[c11]*B.m[c10] + A.m[c12]*B.m[c20];
  D.m[c20] = A.m[c20]*B.m[c00] + A.m[c21]*B.m[c10] + A.m[c22]*B.m[c20];
  /* 2eme colonne */
  D.m[c01] = A.m[c00]*B.m[c01] + A.m[c01]*B.m[c11] + A.m[c02]*B.m[c21];
  D.m[c11] = A.m[c10]*B.m[c01] + A.m[c11]*B.m[c11] + A.m[c12]*B.m[c21];
  D.m[c21] = A.m[c20]*B.m[c01] + A.m[c21]*B.m[c11] + A.m[c22]*B.m[c21];
  /* 3eme colonne */
  D.m[c02] = A.m[c00]*B.m[c02] + A.m[c01]*B.m[c12] + A.m[c02]*B.m[c22];
  D.m[c12] = A.m[c10]*B.m[c02] + A.m[c11]*B.m[c12] + A.m[c12]*B.m[c22];
  D.m[c22] = A.m[c20]*B.m[c02] + A.m[c21]*B.m[c12] + A.m[c22]*B.m[c22];
  return D;
}

/* Creation de la matrice 3x3 de rotation autour du GFLpoint C, d'angle alpha */
extern GFLhmat gfl_RotationC(GFLpoint C, float alpha)
{
  return gfl_Mat_x_Mat(gfl_Translation2d(C.x,C.y),gfl_Mat_x_Mat(gfl_Rotation(alpha),gfl_Translation2d(-C.x,-C.y)));
}

extern GFLhmat gfl_RotationCXY(double Cx, double Cy, float alpha)
{
  return gfl_Mat_x_Mat(gfl_Translation2d(Cx,Cy),
                       gfl_Mat_x_Mat(gfl_Rotation(alpha),gfl_Translation2d(-Cx,-Cy)));
}


extern void gfl_Mat_print(GFLhmat A)
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c00],A.m[c01],A.m[c02]);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c10],A.m[c11],A.m[c12]);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c20],A.m[c21],A.m[c22]);
  fprintf(stderr,"\n");
}

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/

