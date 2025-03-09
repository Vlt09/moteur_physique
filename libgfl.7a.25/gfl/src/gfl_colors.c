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

/* pour compatibilité OpenGl - équivalent glColor4fv */
extern void     gfl_Color3fv(GFLcolor c) { glColor3f(c.r,c.g,c.b);     }
extern void     gfl_Color4fv(GFLcolor c) { glColor4f(c.r,c.g,c.b,c.a); }

/* conversion pour compatibilité libMLV/SDL */
extern GFLfcolor gfl_uColor_to_fColor(GFLucolor col)
{
  return (GFLfcolor){(float)(   ((col>>24)&0x000000FF)/255.),
                     (float)(   ((col>>16)&0x000000FF)/255.),
                     (float)(   ((col>> 8)&0x000000FF)/255.),
                     (float)(1.-((col    )&0x000000FF)/255.)};
}

extern GFLucolor gfl_fColor_to_uColor(GFLfcolor col)
{
  return (GFLucolor)((((uint)(col.r*255))<<24) |
                     (((uint)(col.g*255))<<16) |
                     (((uint)(col.b*255))<< 8) |
                     (((uint)((1.-col.a)*255))));
}

#ifdef _GFL3D_
  /* pour compatibilité OpenGl */
  extern void gfl_Materialfv(GLenum face,GLenum pname, GFLcolor col)
  {
    GLfloat GLCOL[4] = {col.r,col.g,col.b,col.a};
    glMaterialfv(face,pname,GLCOL);
  }
  
  /* pour compatibilité OpenGl */
  extern void gfl_Enable_Blend(void)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  }
  
  /* pour compatibilité OpenGl */
  extern void gfl_Material(GFLcolor col, float ambi, float diff, float spec, float shine, float alpha)
  {
    glEnable(GL_LIGHTING);
    GLfloat tmp[4];
    tmp[3]=alpha;
    tmp[0]=ambi*col.r;
    tmp[1]=ambi*col.g;
    tmp[2]=ambi*col.b;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,tmp);
    tmp[0]=diff*col.r;
    tmp[1]=diff*col.g;
    tmp[2]=diff*col.b;
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,tmp);
    tmp[0]=spec*col.r;
    tmp[1]=spec*col.g;
    tmp[2]=spec*col.b;
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,tmp);
    glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shine*128.);
  }
#endif

/* mapping Couleur */
extern GFLcolor gfl_mapscal4(GFLcolor C, float    umap) { return (GFLcolor){umap*C.r,umap*C.g,umap*C.b,umap*C.a}; }
extern GFLcolor gfl_mapvect4(GFLcolor C, GFLcolor umap) { return (GFLcolor){umap.r*C.r,umap.g*C.g,umap.b*C.b,umap.a*C.a}; }

extern GFLcolor gfl_Color_NegRGB(GFLcolor col)
{
  return (GFLcolor){(float)(1.-col.r),(float)(1.-col.g),(float)(1.-col.b),(float)col.a};
}

extern GFLcolor gfl_Color_NegRGBA(GFLcolor col)
{
  return (GFLcolor){(float)(1.-col.r),(float)(1.-col.g),(float)(1.-col.b),(float)(1.-col.a)};
}

extern double gfl_ColorDist(GFLcolor c1, GFLcolor c2)
{
  double dr=fabs(c1.r-c2.r),
         dg=fabs(c1.g-c2.g),
         db=fabs(c1.b-c2.b);
  return MAX3(dr,dg,db);
}


/* conversion RGBA -> HSVA */
extern GFLcolor gfl_rgba_hsva_4fv(GFLcolor col)
{
  float  y=col.r-col.g,
         c=col.g-col.b,
         m=col.b-col.r;

  /* teintes grises R=G=B */
  if (GFLISZERO(y) && GFLISZERO(c) && GFLISZERO(m))
    return (GFLcolor){0.,0.,col.r,col.a};

  float  min = MIN3(col.r,col.g,col.b);
  float  max = MAX3(col.r,col.g,col.b);
  float  amp = 1./(max-min);
  float  h;
       if (col.r==max) { h = fmod((c*amp),6); }
  else if (col.g==max) { h = m*amp + 1./3.;   }
  else                 { h = y*amp + 2./3.;   }

  return (GFLcolor){h,(max*amp),(max),(col.a)};
}

/* conversion HSVA -> RGBA */
extern GFLcolor gfl_hsva_rgba_4fv(GFLcolor col)
{
  float  h=6.*col.r, s=col.g, v=col.b, a=col.a;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (GFLcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (GFLcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (GFLcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (GFLcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (GFLcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (GFLcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent */
extern GFLcolor gfl_hsva_rgba_4f(float h, float s, float v, float a)
{
  h*=6;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (GFLcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (GFLcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (GFLcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (GFLcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (GFLcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (GFLcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent : teinte pure */
extern GFLcolor gfl_h110_rgba_1f(float h)
{
  h*=6;
  float  x = 1.-fabs(fmod(h,2)-1.);
  switch ((int)h)
  {                         // R  G  B  A
    case 0 : return (GFLcolor){1, x, 0, 0}; // rouge...jaune
    case 1 : return (GFLcolor){x, 1, 0, 0}; // jaune...vert
    case 2 : return (GFLcolor){0, 1, x, 0}; // vert....cyan
    case 3 : return (GFLcolor){0, x, 1, 0}; // cyan....bleu
    case 4 : return (GFLcolor){x, 0, 1, 0}; // bleu....magenta
    default: return (GFLcolor){1, 0, x, 0}; // magenta.rouge
  }
}


/*====================================================================*/
/*                  generateurs de cartes de couleurs                 */
/*====================================================================*/
extern void gfl_ColorMapHSV360(GFLcolor *umap, int n)
{
  int       i;
  float     h,x;
  GFLcolor *c=umap;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = 1.-fabs(fmod(h,2)-1.);
    switch ((int)h)
    {                       // R  G  B  A
      case 0 : *c = (GFLcolor){1, x, 0, 0}; break; // rouge...jaune
      case 1 : *c = (GFLcolor){x, 1, 0, 0}; break; // jaune...vert
      case 2 : *c = (GFLcolor){0, 1, x, 0}; break; // vert....cyan
      case 3 : *c = (GFLcolor){0, x, 1, 0}; break; // cyan....bleu
      case 4 : *c = (GFLcolor){x, 0, 1, 0}; break; // bleu....magenta
      default: *c = (GFLcolor){1, 0, x, 0}; break; // magenta.rouge
    }
    c++;
  }
}

extern void gfl_ColorMapHSV360_va(GFLcolor *umap, int n, float v, float a)
{
  int    i;
  float  h, x;
  GFLcolor *c=umap;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (GFLcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (GFLcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (GFLcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (GFLcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (GFLcolor){  x, 0.0, v  , a}; break;
      case 5  : *c = (GFLcolor){v  , 0.0, v-x, a}; break;
      default : *c = (GFLcolor){v  , x  , 0.0, a}; break;
    }
    c++;
  }
}

extern void gfl_ColorMapHSV360_sa(GFLcolor *umap, int n, float s, float a)
{
  int    i;
  float  h, x;
  GFLcolor *c=umap;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = s*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (GFLcolor){    s   ,    x   ,(1.0f-s), a}; break;
      case 1  : *c = (GFLcolor){   s-x  ,    s   ,(1.0f-s), a}; break;
      case 2  : *c = (GFLcolor){(1.0f-s),    s   ,    x   , a}; break;
      case 3  : *c = (GFLcolor){(1.0f-s),   s-x  ,    s   , a}; break;
      case 4  : *c = (GFLcolor){    x   ,(1.0f-s),    s   , a}; break;
      default : *c = (GFLcolor){    s   ,(1.0f-s),   s-x  , a}; break;
    }
    c++;
  }
}

extern void gfl_ColorMapHSV360_interv(GFLcolor *umap, int n, float min, float max)
{
  int    i;
  float  h,x,v=1.,a=0.;
  GFLcolor *c=umap;

  for (i=0;i<n;i++)
  {
    h=6.*(min+ (i/(n*(max-min))));
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (GFLcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (GFLcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (GFLcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (GFLcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (GFLcolor){  x, 0.0, v  , a}; break;
      default : *c = (GFLcolor){v  , 0.0, v-x, a}; break;
    }
    c++;
  }
}

extern void gfl_ColorMapHSV360_modulate(GFLcolor *umap, int n, float (*mod_func)(float))
{
  static long double DEGtoNG = 1./60.;
  long double h;
  int i,u;
  float x;
  GFLcolor *c=umap;

  for (i=0;i<n;i++)
  {
    h=((360.*i)/n)*DEGtoNG;
    u = (int)h;
    h-= u;
    x = (float)(h*h); /* x dans [0,1] */
    x = mod_func(x);
    switch(u)
    {
      case 0  : *c = (GFLcolor){1.0  ,   x  , 0.0  , 0.0}; break;
      case 1  : *c = (GFLcolor){1.0-x, 1.0  , 0.0  , 0.0}; break;
      case 2  : *c = (GFLcolor){0.0  , 1.0  ,   x  , 0.0}; break;
      case 3  : *c = (GFLcolor){0.0  , 1.0-x, 1.0  , 0.0}; break;
      case 4  : *c = (GFLcolor){  x  , 0.0  , 1.0  , 0.0}; break;
      default : *c = (GFLcolor){1.0  , 0.0  , 1.0-x, 0.0}; break;
    }
    c++;
  }
}


extern void gfl_SetIndexColor(GFLcolor* umap, int idx, GFLcolor col)
{
  *(umap+idx)=col;
}

#ifdef __cplusplus
  }
#endif
