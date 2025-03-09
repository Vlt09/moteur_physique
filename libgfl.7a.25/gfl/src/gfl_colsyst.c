/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 7a - juillet 2024
  ================================================================!*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <gfl.h>

/******************************************************************************/
/*                          CONVERSION RGB -> GrayScale                       */
/******************************************************************************/
extern bool gfl_RGBtoGRAY(GFLpixmap** gray, GFLpixmap* rgb, bool AVG0_or_MAX1)
{  
  if (rgb==NULL || rgb->umap==NULL) gfl_error_("image d'entrée invalide <rgb>\n");
  if (rgb->layer!=3)                gfl_error_("image d'entrée nombre de plans (%d) invalide\n",rgb->layer);
  if (!(gfl_PixmapAlloc(gray, rgb->width, rgb->height, 1 , rgb->depth))) gfl_funcfail_("gfl_PixmapAlloc");

  uchar *s  = rgb->umap;
  uchar *d = (*gray)->umap;
  switch (AVG0_or_MAX1)
  {
    case false : 
      for (; s<rgb->uend; (s+=3,d++) )
      {
        ushort tmp=(*s+*(s+1)+*(s+2))/3;
        *d = (uchar)tmp;
      } 
      break;       
    case true : 
      for (; s<rgb->uend; (s+=3,d++) )
      {
        ushort tmp=MAX3((*s),(*(s+1)),(*(s+2)));
        *d = (uchar)tmp;
      } 
      break;       
  }
  return true;
}


/******************************************************************************/
/*                            CONVERSION RGB -> YIQ                           */
/******************************************************************************/

/**************************************/
/* RGB -> YIQ avec coefficients réels */
/**************************************/
extern bool gfl_RGBtoYIQ_dbl(GFLpixmap** dst, GFLpixmap* src)
{
  static double RGBYIQ[9] = { +0.299, +0.587, +0.114,
                              +0.596, -0.275, -0.321,
                              +0.212, -0.523, +0.311 };
  uint    color;
  double  min,max,delta;
  double *mat,*pmat;
  uchar  *ppix;

  if (!(mat=(double*)calloc(src->height*src->width,sizeof(double)))) gfl_allocfail_(src->width*src->height*sizeof(double));
  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_funcfail_("gfl_PixmapAlloc");

  for (color=0;color<(uint)src->layer; color++)
  {
    min =  100000.;
    max = -100000.;
    for ( (pmat=mat, ppix = src->umap); ppix < src->uend; (pmat++, ppix+=3) )
    {
      *pmat = RGBYIQ[color+0]*(*(ppix  )) +
              RGBYIQ[color+1]*(*(ppix+1)) +
              RGBYIQ[color+2]*(*(ppix+2));
       min = MIN(*pmat,min);
       max = MAX(*pmat,max);
     }
    delta=255/(max-min);

    for ( (pmat=mat, ppix = (*dst)->umap+color); ppix < (*dst)->uend; (pmat++, ppix+=3) )
     *ppix = (uchar)((*pmat-min)*delta);
  }
  free(mat);
  return true;
}

/**************************************/
/* YIQ -> RGB avec coefficients réels */
/**************************************/
extern bool gfl_YIQtoRGB_dbl(GFLpixmap** dst, GFLpixmap* src)
{
  static double YIQRGB[9] = {+1.000, +0.956, +0.621,
                             +1.000, -0.272, -0.647,
                             +1.000, -1.105, +1.702 };
  uint    color;
  double *mat,min,max,delta;
  double *pmat;
  uchar  *ppix;

  if (!(mat=(double*)calloc(src->height*src->width,sizeof(double)))) gfl_allocfail_(src->width*src->height*sizeof(double));
  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_funcfail_("gfl_PixmapAlloc");

  for (color=0;color<(uint)src->layer; color++)
  {
    min =  100000.;
    max = -100000.;
    for ( (pmat=mat, ppix = src->umap); ppix < src->uend; (pmat++, ppix+=3) )
    {
      *pmat = YIQRGB[color+0]*(*(ppix  )) +
              YIQRGB[color+1]*(*(ppix+1)) +
              YIQRGB[color+2]*(*(ppix+2));
       min = MIN(*pmat,min);
       max = MAX(*pmat,max);
     }
    delta=255/(max-min);

    for ( (pmat=mat, ppix = (*dst)->umap+color); ppix < (*dst)->uend; (pmat++, ppix+=3) )
     *ppix = (uchar)((*pmat-min)*delta);
  }
  free(mat);
  return true;
}

/****************************************/
/* RGB -> YIQ avec coefficients entiers */
/****************************************/
static inline void Converse(double *Vin, double  *M, double *Vout)
{
  Vout[0] = M[0]*Vin[0] + M[1]*Vin[1] + M[2]*Vin[2];
  Vout[1] = M[3]*Vin[0] + M[4]*Vin[1] + M[5]*Vin[2];
  Vout[2] = M[6]*Vin[0] + M[7]*Vin[1] + M[8]*Vin[2];
}

extern bool gfl_RGBtoYIQ_int(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double Vin[3],Vout[3];
  static double RGBYIQ[9] = {+0.299, +0.587, +0.114,
                             +0.596, -0.275, -0.321,
                             +0.212, -0.523, +0.311 };
  static double  m[3] = {+0.000,-151.980000,-133.365000 };
  static double  p[3] = {+1.000,+  0.838926,+  0.956783 };

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_funcfail_("gfl_PixmapAlloc");

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    Vin[0] = (double)(*s++);
    Vin[1] = (double)(*s++);
    Vin[2] = (double)(*s++);

    Converse(Vin,RGBYIQ,Vout);

    *d++ = CAST_UCHAR((Vout[0]-m[0])*p[0]);
    *d++ = CAST_UCHAR((Vout[1]-m[1])*p[1]);
    *d++ = CAST_UCHAR((Vout[2]-m[2])*p[2]);
  }
  return true;
}

/****************************************/
/* YIQ -> RGB avec coefficients entiers */
/****************************************/
extern bool gfl_YIQtoRGB_int(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double Vin[3],Vout[3];
  static double YIQRGB[9] = {+1.000, +0.956, +0.621,
                             +1.000, -0.272, -0.647,
                             +1.000, -1.105, +1.702 };

  static double m[3] = {+200.279000,-118.616000,+72.650000 };
  static double p[3] = {+  0.999741,+  0.987985,+ 0.973524 };

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_funcfail_("gfl_PixmapAlloc");

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    Vin[0] = (double)(*s++);
    Vin[1] = (double)(*s++);
    Vin[2] = (double)(*s++);

    Converse(Vin,YIQRGB,Vout);

    *d++ = CAST_UCHAR((Vout[0]-m[0])*p[0]);
    *d++ = CAST_UCHAR((Vout[1]-m[1])*p[1]);
    *d++ = CAST_UCHAR((Vout[2]-m[2])*p[2]);
  }
  return true;
}

/******************************************************************************/
/*       CONVERSION RGB >-> YCbCr  -- Specifications du format TIFF-6.0       */
/******************************************************************************/
/*************/
/* NORME 601 */
/*************/
extern bool gfl_RGBtoYCbCr_601(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double R,G,B;
  double Y,Cb,Cr;
  if (!(gfl_PixmapAlloc(dst,src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);
  
  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    R  = (double)(*s++);
    G  = (double)(*s++);
    B  = (double)(*s++);
    Y  = R601*R + G601*G + B601*B;
    /*minY = MIN(minY,Y); maxY = MAX(maxY,Y); */
    Cb = 128.+(B-Y)/(2.-2.*B601);
    /*minC = MIN(minC,Cb); maxC = MAX(maxC,Cb); */
    Cr = 128.+(R-Y)/(2.-2.*R601);
    /*minC = MIN(minC,Cr); maxC = MAX(maxC,Cr); */
    *d++ = CAST_UCHAR(Y);
    *d++ = CAST_UCHAR(Cb);
    *d++ = CAST_UCHAR(Cr);
  }
  return true;
}
/*------------------------------------------------------------*/
extern bool gfl_YCbCrtoRGB_601(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double R,G,B;
  double Y,Cb,Cr;
 
  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
      Y  = (double)(*s++);
      Cb = (double)(*s++)-128;
      Cr = (double)(*s++)-128;
      R  = Y + Cr*(2.-2.*R601 );
      B  = Y + Cb*(2.-2.*B601);
      G  = (Y-B601*B-R601*R)/G601;
      *d++ = CAST_UCHAR(R);
      *d++ = CAST_UCHAR(G);
      *d++ = CAST_UCHAR(B);
    }
  return true;
}

/*************/
/* NORME 709 */
/*************/
extern bool gfl_RGBtoYCbCr_709(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double R,G,B;
  double Y,Cb,Cr;

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    R = (double)(*s++);
    G = (double)(*s++);
    B = (double)(*s++);
    Y = R709*R + G709*G + B709*B;
    Cb= (B-Y)/(2-2*B709);
    Cr= (R-Y)/(2-2*R709);
    *d++ = CAST_UCHAR(Y);
    *d++ = CAST_UCHAR(Cb+128);
    *d++ = CAST_UCHAR(Cr+128);
  }
  return true;
}
/*------------------------------------------------------------*/
extern bool gfl_YCbCrtoRGB_709(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  double R,G,B;
  double Y,Cb,Cr;

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
      Y  = (double)(*s++);
      Cb = (double)(*s++)-128;
      Cr = (double)(*s++)-128;
      R  = Y + Cr*(2.-2.*R709 );
      B  = Y + Cb*(2.-2.*B709);
      G  = (Y-B709*B-R709*R)/G709;
      *d++ = CAST_UCHAR(R);
      *d++ = CAST_UCHAR(G);
      *d++ = CAST_UCHAR(B);
    }
  return true;
}


/******************************************************************************/
/*                         CONVERSION RGB <-> HSV                             */
/******************************************************************************/

/**************/
/* RGB -> HSV */
/**************/

static inline void RGBtoHSV_float(float R, float G, float B, float* H, float* S, float* V)
{
  float max = MAX3(R,G,B);
  float amp = max-MIN3(R,G,B);
  uchar flag = ((max==R)?0:(max==G?1:2));

  *V = max;
  if (IS_ZERO(amp)) /* teintes grises R=G=B */
  {
    *S= 0.;
    *H= 0.;
     return;
  }

 *S = amp/max;
  switch (flag)
  {
    case 0 : *H =    (G-B)/amp; break;
    case 1 : *H = 2.+(B-R)/amp; break;
    case 2 : *H = 4.+(R-G)/amp; break;
  }
 *H *= 60.;
  if (*H<0) *H += 360.;
}

extern bool gfl_RGBtoHSV(GFLpixmap** dst, GFLpixmap* src)
{
  float  R,G,B,H,S,V;
  static float NBGtoDEG = 0.711111111111111111111; /* 256./360. */
  uchar *s,*d;

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    R = (*s)*RGB_to_01; s++;
    G = (*s)*RGB_to_01; s++;
    B = (*s)*RGB_to_01; s++;

    RGBtoHSV_float(R,G,B,&H,&S,&V);

   *d = CAST_UCHAR(H*NBGtoDEG); d++;
   *d = CAST_UCHAR(S*255.5);    d++;
   *d = CAST_UCHAR(V*255.5);    d++;
  }
  return true;
}

/**************/
/* HSV -> RGB */
/**************/
static inline void HSVtoRGB_float(float H, float S, float V, float* R, float* G, float* B)
{
  float f,p,q,t;
  int   i;

  if (S<=ZERO) /* pas de couleur */
  {
    *R=*G=*B=V;
    return;
  }

  H /= 60.;
  i = (int)H;
  f = H-i;
  p = V*(1.-S);
  q = V*(1.-S*f);
  t = V*(1.-S*(1.-f));
  switch(i)
  {
    case 0 : *R=V; *G=t; *B=p; break;
    case 1 : *R=q; *G=V; *B=p; break;
    case 2 : *R=p; *G=V; *B=t; break;
    case 3 : *R=p; *G=q; *B=V; break;
    case 4 : *R=t; *G=p; *B=V; break;
    case 5 : *R=V; *G=p; *B=q; break;
  }
}

extern bool gfl_HSVtoRGB(GFLpixmap** dst, GFLpixmap* src)
{
  uchar *s,*d;
  float  R=0,G=0,B=0,H=0,S=0,V=0;

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth)))  gfl_allocfail_(src->width*src->height*src->layer);

  s  = src->umap;
  d = (*dst)->umap;
  while (s < src->uend)
  {
    H = (*s)*RGB_to_01*360.; s++;
    S = (*s)*RGB_to_01;      s++;
    V = (*s)*RGB_to_01;      s++;
    HSVtoRGB_float(H,S,V,&R,&G,&B);

    *d = CAST_UCHAR(R*255.5); d++;
    *d = CAST_UCHAR(G*255.5); d++;
    *d = CAST_UCHAR(B*255.5); d++;
  }
  return true;
}


/*!***********************************!*/
/*! RGB -> Coord. TSL                 !*/
/*! Juin 2008                         !*/
/*!***********************************!*/
extern bool gfl_RGBtoVHS(GFLpixmap** dst, GFLpixmap* src, bool QUANT, uchar SCUT)
{
  uchar *p,*d;
  int    i=0.;
  double r=0.,g=0.,b=0.,h=0.,u=0.,s=0.,v=0.;
  double min=0.,med=0.,max=0.;

  if (!(gfl_PixmapAlloc(dst, src->width, src->height, src->layer, src->depth))) gfl_allocfail_(src->width*src->height*src->layer);

  p=src->umap;
  d=(*dst)->umap;
  while (p < src->uend)
  {
    /* si c'est du gris, rien de plus */
    if (abs(*p-*(p+1))==0 &&
        abs(*p-*(p+2))==0  ) { *d=*p; d+=3; p+=3; continue; }

    r=(double)(*p*RGB_to_01); p++;
    g=(double)(*p*RGB_to_01); p++;
    b=(double)(*p*RGB_to_01); p++;

    v = r+g+b; /* valeur */
    s = 4.*(r*r+g*g+b*b-r*g-g*b-b*r); /* saturation : s²=4*(r²+g²+b²-rg-gb-br) */

    max = MAX(r,MAX(g,b));
    min = MIN(r,MIN(g,b));
         if (max==b) { if (min==r) { med=g; i=0; } else { med=r; i=1; }}
    else if (max==r) { if (min==g) { med=b; i=2; } else { med=g; i=3; }}
    else if (max==g) { if (min==b) { med=r; i=4; } else { med=b; i=5; }}

    if (v<1.)
    {
      h = (max-med)/(max+med-2.*min);
      /*! pour normaliser s² dans [0,1], il faudrait encore diviser par v²,  !*/
      /*! mais pour les teintes sombre, on ne le fait pas : résultat, la     !*/
      /*! valeur de s² est normalisée dans [0,v²] : elle est d'autant plus   !*/
      /*! faible que v est faible                                            !*/
      /*! Interprétation : pour les teintes très sombre (v~0.) saturation et !*/
      /*! teinte portent peu d'information.                                  !*/
      s/=(3.*h*h+1.);
    }
    else if (v>2.)
    {
        h = (med-min)/(2.*max-med-min);
      /*! Idem pour v~3. (teintes très claire) !*/
       s/=(3.*h*h+1.);
      h = 1.-h;    /* inversion du triangle !!! */
    }
    else
    {
      if ((med-min)>=(v-1.)*(max-min))
      {
        h = (max-med)/(max+med-2.*min);
        /*! dans les 2 cas intermédiares, il vaut mieux étalloner s² dans [0,1] !*/
        s/=v*v*(3.*h*h+1.);
        h= ((v==2.)?(0.):(h*v/(2.-v)));
        h=0.5*h;   /* calibration dans [0.0,0.5] */
      }
      else
      {
         h = (med-min)/(2.*max-med-min);
        u=3.-v;
        s/=u*u*(3.*h*h+1.);
        h= ((u==2.)?(0.):(h*u/(2.-u)));
        h=1.-h*0.5; /* inversion ET calibration dans [0.5,1.0] */
      }
    }
    /* inversion du sens de parcours pour les zones impaires */
    /* -> évite les rutures de teintes dans les dégradés     */
    if (i%2) h=1.-h;



    /* si QUANT : saturation au carré, quantifiée par v */
    /*            -> valeurs faibles pour v extrêmes    */
    /*    sinon : calibrée dans [0,1] quel que soit  v  */
    s = (QUANT ? 85.0*v*(3.-v)*s : 255.0*sqrt(s));

    if (s<=SCUT) s=h=0.;
    else h = 1.+42.5*(i+h); /* teinte                        */
    v =  85.0*v;            /* value : v-> (v/3)*255         */

    /*! On peut aussi choisir de code s² plutot que s et laiser au décodeur !*/
    /*! le soin de calculer la rac.carrée.                                  !*/
    /*! Ca peut être utile en association à un QSNU sur s ... à voir        !*/
     *d++= CAST_UCHAR(v);
     *d++= CAST_UCHAR(s);
     *d++= CAST_UCHAR(h);
  }
  return true;
}

#ifdef __cplusplus
  }
#endif
