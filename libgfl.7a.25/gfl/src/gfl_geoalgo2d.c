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

extern double gfl_ProjPointDroite(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint *P)
{
  double d=GFLsqrdist(A,B);
 *P=A;
  if (GFLISZERO(d)) return 0.;

  d = GFLpntprodscal(A,B,C)/d;
  P->x += d*(B.x-A.x);
  P->y += d*(B.y-A.y);
  return d;
}

extern double gfl_ProjPointTrajet(GFLpoint A, GFLvector v, GFLpoint C, GFLpoint *P)
{
  double d=gfl_Normalize(&v);
 *P=A;
  if (GFLISZERO(d)) return 0.;

  d = gfl_ProdScal(v,gfl_Vector2p(A,C));
  P->x += d*v.x;
  P->y += d*v.y;
  return d;
}

extern bool gfl_SegmentInterSegment(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint D, GFLpoint *I)
{
  GFLvector AB= {B.x-A.x,B.y-A.y};
  GFLvector CD= {D.x-C.x,D.y-C.y};
  double    ps= GFLprodvect(AB,CD);

  if (GFLISZERO(ps)) return false;

  GFLvector CA= {A.x-C.x,A.y-C.y};

  double t = GFLprodvect(AB,CA)/ps;
  if (t<0. || t>1.) return false;

  t = GFLprodvect(CD,CA)/ps;
  if (t<0. || t>1.) return false;

  I->x = A.x+t*AB.x;
  I->y = A.y+t*AB.y;
  return true;
}

/*! Intersection Segment/Cercle : (version 09/12/2006)
  * -1 : A et B confondus ou Inters. Dte/Cercle hors de [AB] ou pas d'intesection Dte/Cercle
  *  0 : [AB] entierement dans le cercle
  * +1 : [AB] tangent au Cercle
  * +2 : A dedans, B dehors
  * +3 : A dehors, B dedans
  * +4 : A et B dehors, 2 intersections
  ---------------------------------------------------------------------------------------!*/
extern int gfl_SegmentInterCercle(GFLpoint A, GFLpoint B, GFLpoint C, double r, GFLpoint *I1, GFLpoint *I2)
{
  bool    boolA,boolB;
  GFLpoint P;
  double  xab,yab,d,t,r2=r*r;

  boolA = (GFLsqrdist(A,C)<=r2); /* A est dans le cercle */
  boolB = (GFLsqrdist(B,C)<=r2); /* B est dans le cercle */

  if (boolA && boolB) { *I1=A; *I2=B; return 0; } /* pas d'intersection, A et B ne sont pas modifies */

  t=gfl_ProjPointDroite(A,B,C,&P);
  d=GFLsqrdist(C,P);

  if (!boolA) /* A est dehors */
  {
    if (!boolB)  /* B est dehors */
    {
      /*if (GFLISZERO(t)) return -1;*/      /* A et B confondus            */
      if (t<ZERO || t>1.) return -1;      /* Intersections hors de [A,B] */

      if (d>r2) return -1;              /* Pas d'intersection          */

      if (GFLISZERO(d-r2))                 /* segment tangent au cercle   */
      {
        *I1=*I2=P;
        return 1;
      }

      /* 2 intersections, symetriques par rapport a P */
      d=sqrt((r2-d)/GFLsqrdist(A,B));
      xab = d*(B.x-A.x);
      yab = d*(B.y-A.y);
      I1->x = P.x-xab; I1->y = P.y-yab;
      I2->x = P.x+xab; I2->y = P.y+yab;
      return 4;
    }
    /* B dedans : une intersection */
    d=sqrt((r2-d)/GFLsqrdist(A,B));
    xab = d*(B.x-A.x);
    yab = d*(B.y-A.y);
    I1->x = I2->x = P.x-xab;
    I1->y = I2->y = P.y-yab;
    return 3;
  }
  /* dernier cas : A dedans, B dehors */
  d=sqrt((r2-d)/GFLsqrdist(A,B));
  xab = d*(B.x-A.x);
  yab = d*(B.y-A.y);
  I1->x = I2->x = P.x+xab;
  I1->y = I2->y = P.y+yab;
  return 2;
}

/*! Intersection Cercle/Cercle (version 09/12/2006)
  * -2 : centres des cercles confondus
  * -1 : un cercle contient l'autre
  *  0 : pas d'intersection, cercles disjoints
  * +1 : deux intersections
  * +2 : cercles tangents
  -----------------------------------------------!*/
extern int gfl_CercleInterCercle(GFLpoint C1, double r1, GFLpoint C2, double r2, GFLpoint *I1, GFLpoint *I2)
{
  double    d=GFLdist(C1,C2),t;
  GFLpoint   P;
  GFLvector  u,v;

  /* cercles confondus */
  if (GFLISZERO(d)) return -2;

  /* pas d'intersection  : dist. centres > r1+r2 */
  if (d>r1+r2) return 0;

  /* vecteur C1C2 norme */
  u.x=(C2.x-C1.x)/d;
  u.y=(C2.y-C1.y)/d;

  /* cercles tangents : dist. centres = r1+r2 */
  if (GFLISZERO(r1+r2-d))
  {
    I1->x=I2->x=C1.x+r1*u.x;
    I1->y=I2->y=C1.y+r1*u.y;
    return 2;
  }

  /* d'apres Pythagore */
  t=0.5*(d-(r1*r1-r2*r2)/d);

  if (t*t>r2*r2) return -1;

  P.x=C2.x-t*u.x;
  P.y=C2.y-t*u.y;

  v.x = +u.y;
  v.y = -u.x;

  t = sqrt(r2*r2-t*t);
  I1->x=P.x+t*v.x;
  I1->y=P.y+t*v.y;
  I2->x=P.x-t*v.x;
  I2->y=P.y-t*v.y;
  return 1;
}

extern bool gfl_DroiteInterDroite(GFLpoint A, GFLvector u, GFLpoint B, GFLvector v, GFLpoint *I)
{
  GFLvector AB = {B.x-A.x,B.y-A.y};
  GFLvector N  = {-u.y,+u.x};
  double   t  = GFLprodscal(N,v);
  if (GFLISZERO(t)) return false;
  /* CI=tCD -> N.(CA+AI)=tN.CD -> t=(N.CA)/(N.CD) */
  t = GFLprodscal(N,AB)/t;
  I->x = (B.x-t*v.x);
  I->y = (B.y-t*v.y);
  return true;
}

extern void gfl_CercleCirconscrit(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint *CCC, double *rcc)
{
  gfl_DroiteInterDroite((GFLpoint ){0.5*(A.x+B.x),0.5*(A.y+B.y)},
                        (GFLvector){    (A.y-B.y),    (B.x-A.x)},
                        (GFLpoint ){0.5*(B.x+C.x),0.5*(B.y+C.y)},
                        (GFLvector){    (B.y-C.y),    (C.x-B.x)},CCC);
  *rcc=GFLdist(A,*CCC);
}

extern void gfl_CercleInscrit(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint *CCI, double *rci)
{
  GFLpoint D,E;
  double  d1,d2,d3;
  double  r1,r2;

  d1 = GFLdist(A,B); if (GFLISZERO(d1)) return;
  d2 = GFLdist(B,C); if (GFLISZERO(d2)) return;
  d3 = GFLdist(C,A); if (GFLISZERO(d3)) return;

  r1 = d1/d3;
  r2 = d2/d1;

  D.x = A.x+(B.x-A.x)+(C.x-A.x)*r1;
  D.y = A.y+(B.y-A.y)+(C.y-A.y)*r1;
  E.x = B.x+(C.x-B.x)+(A.x-B.x)*r2;
  E.y = B.y+(C.y-B.y)+(A.y-B.y)*r2;

  gfl_SegmentInterSegment(A,D,B,E,CCI);
  gfl_ProjPointDroite(A,B,*CCI,&D);
  *rci=GFLdist(*CCI,D);
}

extern bool gfl_CohenSutherland(GFLpoint *A, GFLpoint *B, GFLpoint DL, GFLpoint UL, GFLpoint UR, GFLpoint DR)
{
  double xmin=DL.x, xmax=UR.x;
  double ymin=DL.y, ymax=UR.y;
  uchar  codeA=0,codeB=0;
  GFLpoint P,Q;

  /* Les codes associes aux 2 points */
  codeA = ((A->x<xmin)<<3)|((A->x>xmax)<<2)|((A->y>ymax)<<1)|(A->y<ymin);
  codeB = ((B->x<xmin)<<3)|((B->x>xmax)<<2)|((B->y>ymax)<<1)|(B->y<ymin);

/* Pour verification
  char   strcode[8];
  sprintf(strcode,"(%d%d%d%d)",(codeA>>3)&1,(codeA>>2)&1,(codeA>>1)&1,codeA&1);
  gfl_Write(strcode,A->x+0.6,A->y,GFLb);
  sprintf(strcode,"(%d%d%d%d)",(codeB>>3)&1,(codeB>>2)&1,(codeB>>1)&1,codeB&1);
  gfl_Write(strcode,B->x+0.6,B->y,GFLb);
*/
  /*********************/
  /* Les 2 blocs suivants sont equivalents et symetriques, */
  /* (A dedans et B dehors) puis (A dehors et B dedans)    */
  /* C'est juste 2 facons differentes de l'ecrire          */
  if (codeA==0)
  { /* on traite tout de suite le cas ou le segment est entierement dedans        */
    if (codeB==0) return true;
    /* Les 4 possibilites d'intersection : le test sur codeB n'est pas reellement */
    /* indipensable mais permet d'eviter le calcul d'intersection s'il est faux   */
    if (!((codeB&1) && gfl_SegmentInterSegment(*A,*B,DL,DR,&P)))
      if (!((codeB&2) && gfl_SegmentInterSegment(*A,*B,UL,UR,&P)))
        if (!((codeB&4) && gfl_SegmentInterSegment(*A,*B,DR,UR,&P)))
          if (!((codeB&8) && gfl_SegmentInterSegment(*A,*B,DL,UL,&P)))
            return false;
   *B=P;
    return true;
  }
  if (codeB==0) /* B dedans, A dehors */
  {
    if ( ((codeA&1) && gfl_SegmentInterSegment(*A,*B,DL,DR,&P)) ||
         ((codeA&2) && gfl_SegmentInterSegment(*A,*B,UL,UR,&P)) ||
         ((codeA&4) && gfl_SegmentInterSegment(*A,*B,DR,UR,&P)) ||
         ((codeA&8) && gfl_SegmentInterSegment(*A,*B,DL,UL,&P)) )
    { *A=P; return true; }
  }

  /* derniers cas :  A et B dehors */

  /* si codeA et code B ont au moins 1 bit en commun le segment est entierement dehors */
  if (codeA & codeB) return false;

  /* Dans tous les cas restants, il y a 0 ou 2 intersections */
  switch (codeA|codeB)
  { /* encore quelques cas particuliers facilement exploitables */
    case 3 /* 0001 | 0010 */:
      /* on est sûr que ca coupe les 2 bords horizontaux */
      gfl_SegmentInterSegment(*A,*B,DL,DR,&P);
      gfl_SegmentInterSegment(*A,*B,UL,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 12 /* 1000 | 0100 */:
      /* on est sûr que ca coupe les 2 bords verticaux */
      gfl_SegmentInterSegment(*A,*B,DR,UR,&P);
      gfl_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    case 5 /* 0001 | 0100 */:
      if (!gfl_SegmentInterSegment(*A,*B,DL,DR,&P)) return false;
      gfl_SegmentInterSegment(*A,*B,DR,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 9 /* 0001 | 1000 */:
      if (!gfl_SegmentInterSegment(*A,*B,DL,DR,&P)) return false;
      gfl_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    case 6 /* 0010 | 0100 */:
      if (!gfl_SegmentInterSegment(*A,*B,UL,UR,&P)) return false;
      gfl_SegmentInterSegment(*A,*B,DR,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 10 /* 0010 | 1000 */:
      if (!gfl_SegmentInterSegment(*A,*B,UL,UR,&P)) return false;
      gfl_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    default :
      /* VERSION H.Petiot - 24/05/2022 */
      /* Cette astuce s'appuie sur la lazyness du || : si on a un truc au premier if, c'est qu'il a check toutes les intersections avant celle qui a renvoyé true.
      Donc inutile de les re-check, d'où le fait de faire les check dans le sens horaire puis dans le sens anti-horaire. De plus une fois un true trouvé le || n'execute pas les autres donc on evite le surplus de calcul */
      if (gfl_SegmentInterSegment(*A, *B, UL, UR, &Q)|| // Upper edge
          gfl_SegmentInterSegment(*A, *B, UR, DR, &Q)|| // Right edge
          gfl_SegmentInterSegment(*A, *B, DR, DL, &Q)|| // Bottom edge
          gfl_SegmentInterSegment(*A, *B, DL, UL, &Q) ) // Left edge
        {
          if (gfl_SegmentInterSegment(*A, *B, DL, UL, &P)||  // Left edge
              gfl_SegmentInterSegment(*A, *B, DR, DL, &P)||  // Bottom edge
              gfl_SegmentInterSegment(*A, *B, UR, DR, &P)||  // Right edge
              gfl_SegmentInterSegment(*A, *B, UL, UR, &P) )  // Uper edge
           { // En théorie, on tombe dans ce cas seulement si on a une edge coupée par une autre.
             *A = P;
             *B = Q;
             return true;
           }
        }
      return false;

      /*
      if (gfl_SegmentInterSegment(*A,*B,UR,UL,&P))
      {
        if (!gfl_SegmentInterSegment(*A,*B,DR,UR,&Q))
          if (!gfl_SegmentInterSegment(*A,*B,DL,UL,&Q))
            gfl_SegmentInterSegment(*A,*B,DL,DR,&Q);
        *A = P;
        *B = Q;
        return true;
      }
      if (gfl_SegmentInterSegment(*A,*B,DL,DR,&P))
      {
        if (!gfl_SegmentInterSegment(*A,*B,DR,UR,&Q))
          gfl_SegmentInterSegment(*A,*B,DL,UL,&Q);
        *A = P;
        *B = Q;
         return true;
      }
      return false;
      */
  }
}

#ifdef __cplusplus
  }
#endif
