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

/*==============================================================================================================*/

static __inline__ bool gfl_IsInWindow(double x, double y)
{
  return (x>=gfl_GetXMin() && x<=gfl_GetXMax() && y>=gfl_GetYMin() && y<=gfl_GetYMax());
}

extern void gfl_SetPointSize(double size) { glPointSize(size); }
extern void gfl_SetColor(GFLcolor col) { glColor4d(col.r,col.g,col.b,col.a); }

extern void gfl_PlotDft(double x, double y)
{
  glBegin(GL_POINTS);
    glVertex2d(x,y);
  glEnd();
}

extern void gfl_Plot(double x, double y, GFLcolor color, int width)
{
  if (!gfl_IsInWindow(x,y)) return;
  glPointSize((GLfloat)width);
  GFLcolor4fv(color);
  glBegin(GL_POINTS);
    glVertex2d(x,y);
  glEnd();
  glPointSize(1.);
}

extern void gfl_DrawPoint(GFLpoint P, GFLcolor color, int width) { gfl_Plot(P.x, P.y,color,width); }

/*==============================================================================================================*/
extern void gfl_LineDft(double xa, double ya, double xb, double yb)
{
  glBegin(GL_LINES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
  glEnd();
}

extern void gfl_Line(double xa, double ya, double xb, double yb, GFLcolor col, int width)
{
  glLineWidth((GLfloat)width);
  GFLcolor4fv(col);
  glBegin(GL_LINES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
  glEnd();
  glLineWidth(1.);
}

extern void gfl_DrawLine(GFLpoint A, GFLpoint B, GFLcolor color, int width)
{
  gfl_Line(A.x, A.y, B.x, B.y, color, width);
}


extern void gfl_Line_2col(double xa, double ya, GFLcolor cola, double xb, double yb, GFLcolor colb, int width)
{
  glLineWidth((GLfloat)width);
  glBegin(GL_LINES);
    GFLcolor4fv(cola);
    glVertex2d(xa,ya);
    GFLcolor4fv(colb);
    glVertex2d(xb,yb);
  glEnd();
  glLineWidth(1.);
}

extern void gfl_DrawLine_2col(GFLpoint A, GFLcolor colA, GFLpoint B, GFLcolor colB, int width)
{
  gfl_Line_2col(A.x, A.y, colA, B.x, B.y, colB, width);
}

/*!============================================================================================================!*/
/*! Cercles, Ellipses, secteurs ....                                                                           !*/
/*!============================================================================================================!*/
extern void gfl_Circle(double xc, double yc, double r, GFLcolor col, int width)
{
//  if (!gfl_IsInWindow(xc,yc)) return;
  if (GFLISZERO(r)) return;
  double x,y;
  GFLcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_POLYGON);
    int i,n=(int)((gfl_GetXPixSize()*720)/r);
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      x=xc+r*gfl_Idxcos(i);
      y=yc+r*gfl_Idxsin(i);
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void gfl_DrawCircle(GFLpoint C, double r, GFLcolor col, int width)
{
  gfl_Circle(C.x,C.y,r,col,width);
}

extern void gfl_FillCircle(double xc, double yc, double r, GFLcolor col)
{
  double x,y;

  //if (!gfl_IsInWindow(xc,yc)) return;
  if (GFLISZERO(r)) return;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glBegin(GL_POLYGON);
    int i,n=(int)((gfl_GetXPixSize()*720)/r);
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      x=xc+r*gfl_Idxcos(i);
      y=yc+r*gfl_Idxsin(i);
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_DrawFillCircle(GFLpoint C, double r, GFLcolor col)
{
  gfl_FillCircle(C.x,C.y,r,col);
}


extern void gfl_Ellipse(double xc, double yc, double rx, double ry, double degrot, GFLcolor col, int width)
{
  if (GFLISZERO(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  GFLcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_LINE_LOOP);
    double x,y,xa,ya;
    double ca=gfl_Idxcos((int)(2*degrot));
    double sa=gfl_Idxsin((int)(2*degrot));
    int    i,n=(int)((gfl_GetXPixSize()*1440)/(rx+ry));
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      xa=rx*gfl_Idxcos(i);
      ya=ry*gfl_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void gfl_FillEllipse(double xc, double yc, double rx, double ry, double degrot, GFLcolor col)
{
  if (GFLISZERO(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glBegin(GL_POLYGON);
    double x,y,xa,ya;
    double ca=gfl_Idxcos((int)(2*degrot));
    double sa=gfl_Idxsin((int)(2*degrot));
    int    i,n=(int)((gfl_GetXPixSize()*1440)/(rx+ry));
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      xa=rx*gfl_Idxcos(i);
      ya=ry*gfl_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, GFLcolor col, int width)
{
  if (GFLISZERO(rx*ry)) return;
  GFLcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_LINE_LOOP);
    double x,y,xa,ya;
    double ca=gfl_Idxcos((int)(2*degrot));
    double sa=gfl_Idxsin((int)(2*degrot));
    int    i,n=(int)((gfl_GetXPixSize()*1440)/(rx+ry));
    int    deb=720-2*degsec;
    n = CLIP(1,n,144);
    glVertex2d(xc,yc);
    for (i=deb;i<=720;i+=n)
    {
      xa=rx*gfl_Idxcos(i);
      ya=ry*gfl_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void gfl_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, GFLcolor col)
{
  if (GFLISZERO(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glBegin(GL_POLYGON);
    double x,y,xa,ya;
    double ca=gfl_Idxcos((int)(2*degrot));
    double sa=gfl_Idxsin((int)(2*degrot));
    int    i,n=(int)((gfl_GetXPixSize()*1440)/(rx+ry));
    int    deb=720-2*degsec;
    n = CLIP(1,n,144);
    glVertex2d(xc,yc);
    for (i=deb;i<=720;i+=n)
    {
      xa=rx*gfl_Idxcos(i);
      ya=ry*gfl_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_FillCircleSlice(double xc, double yc, GFLcolor colC, double xa, double ya, GFLcolor colA, double deg)
{
  double r,d,a,x,y;

  r=sqrt(SQR(xc-xa)+SQR(yc-ya));
  if (GFLISZERO(r)) return;

  a = acos((xa-xc)/r);
  a*=(asin((ya-yc)/r)<0.?-1.:+1.);

  deg*=DEGTORAD;
  d=a-0.5*deg;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glBegin(GL_POLYGON);
    GFLcolor4fv(colC);
    glVertex2d(xc,yc);
    GFLcolor4fv(colA);
    while (d<a+0.5*deg)
    {
      x=xc+r*gfl_Radcos(d);
      y=yc+r*gfl_Radsin(d);
      glVertex2d(x,y);
      d+=deg*0.1;
    }
    x=xc+r*gfl_Radcos(a+0.5*deg);
    y=yc+r*gfl_Radsin(a+0.5*deg);
    glVertex2d(x,y);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_FillCircleSector(double xc, double yc, double xa, double ya, double xb, double yb, GFLcolor col)
{
  double r,d,a,b,x,y;

  r=sqrt(SQR(xc-xa)+SQR(yc-ya));
  if (GFLISZERO(r)) return;
  a = acos((xa-xc)/r);
  a*=(asin((ya-yc)/r)<0.?-1.:+1.);

  b = acos((xb-xc)/r);
  b*=(asin((yb-yc)/r)<0.?-1.:+1.);
  if (b<a) {d=a; a=b; b=d;}
  d=a;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_POLYGON);
    GFLcolor4fv(col);
    while (d<b)
    {
      x=xc+r*gfl_Radcos(d);
      y=yc+r*gfl_Radsin(d);
      glVertex2d(x,y);
      d+=(b-a)*0.1;
    }
    x=xc+r*gfl_Radcos(b);
    y=yc+r*gfl_Radsin(b);
    glVertex2d(x,y);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}


/*!============================================================================================================!*/
/*! Triangles                                                                                                  !*/
/*!============================================================================================================!*/
extern void gfl_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, GFLcolor col, int width)
{
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  GFLcolor4fv(col);
  glBegin(GL_TRIANGLES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
  glEnd();
  glLineWidth(1.);
}

extern void gfl_DrawTriangle(GFLpoint A, GFLpoint B, GFLpoint C, GFLcolor col, int width)
{
  gfl_Triangle(A.x,A.y,B.x,B.y,C.x,C.y,col,width);
}


extern void gfl_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, GFLcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glBegin(GL_TRIANGLES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_DrawFillTriangle(GFLpoint A, GFLpoint B, GFLpoint C, GFLcolor col)
{
  gfl_FillTriangle(A.x,A.y,B.x,B.y,C.x,C.y,col);
}


extern void gfl_FillTriangle_3col(double xa, double ya, GFLcolor cola,
                                  double xb, double yb, GFLcolor colb,
                                  double xc, double yc, GFLcolor colc)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
    GFLcolor4fv(cola); glVertex2d(xa,ya);
    GFLcolor4fv(colb); glVertex2d(xb,yb);
    GFLcolor4fv(colc); glVertex2d(xc,yc);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_DrawFillTriangle_3col(GFLpoint A, GFLcolor colA, GFLpoint B, GFLcolor colB, GFLpoint C, GFLcolor colC)
{
  gfl_FillTriangle_3col(A.x,A.y,colA,B.x,B.y,colB,C.x,C.y,colC);
}

/*!============================================================================================================!*/
/*! Rectangles parallèles aux axes                                                                             !*/
/*!============================================================================================================!*/
extern void gfl_Rectangle(double xd, double yd, double xu, double yu, GFLcolor col, int width)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  GFLcolor4fv(col);
  glLineWidth((GLfloat)width);
  glRectd(xd,yd,xu,yu);
  glLineWidth(1.);
}

extern void gfl_DrawRectangle(GFLpoint DL, GFLpoint UR, GFLcolor color, int width)
{
  gfl_Rectangle(DL.x,DL.y,UR.x,UR.y,color,width);
}

extern void gfl_FillRectangle(double xd, double yd, double xu, double yu, GFLcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glRectd(xd,yd,xu,yu);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_DrawFillRectangle(GFLpoint DL, GFLpoint UR, GFLcolor color)
{
  gfl_FillRectangle(DL.x,DL.y,UR.x,UR.y,color);
}


/*!============================================================================================================!*/
/*! Quadrilatères quelconques                                                                                  !*/
/*!============================================================================================================!*/
extern void gfl_Quad(double xa, double ya,
                     double xb, double yb,
                     double xc, double yc,
                     double xd, double yd, GFLcolor col, int width)
{
  GFLcolor4fv(col);
  glLineWidth((GLfloat)width);
  glBegin(GL_QUADS);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
    glVertex2d(xd,yd);
  glEnd();
}

extern void gfl_FillQuad(double xa, double ya,
                         double xb, double yb,
                         double xc, double yc,
                         double xd, double yd,GFLcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(col);
  glBegin(GL_QUADS);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
    glVertex2d(xd,yd);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void gfl_FillQuad_4col(double xa, double ya,GFLcolor cola,
                              double xb, double yb,GFLcolor colb,
                              double xc, double yc,GFLcolor colc,
                              double xd, double yd,GFLcolor cold)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_QUADS);
    GFLcolor4fv(cola); glVertex2d(xa,ya);
    GFLcolor4fv(colb); glVertex2d(xb,yb);
    GFLcolor4fv(colc); glVertex2d(xc,yc);
    GFLcolor4fv(cold); glVertex2d(xd,yd);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}



/*!============================================================================================================!*/
/*! "spots" : coûte TRES cher !!!!  => A EVITER                                                                !*/
/*!============================================================================================================!*/
#define SPOTPOINTSIZE 2
extern void gfl_Spot(double xc, double yc, double r, GFLcolor col)
{
  int i,j,rix,riy;
  double x,y,yy,rr;
  float  d;
    glPointSize(SPOTPOINTSIZE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_POINTS);
  rr=r*r;
  riy=(int)(r/gfl_GetYPixSize());
  rix=(int)(r/gfl_GetXPixSize());
  for (j=-riy; j<=+riy; j++)
  {
    y=j*SPOTPOINTSIZE*gfl_GetYPixSize();
    yy=y*y;
    for (i=-rix; i<=+rix; i++)
    {
       x=i*SPOTPOINTSIZE*gfl_GetXPixSize();
       if ((d=(float)(x*x+yy))>rr) continue;
       d=pow((1.-d/rr),0.333);
       glColor4f(d*col.r,d*col.g,d*col.b,d*(1.-col.a));
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glPointSize(1.);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
}

extern void gfl_SpotBar(double xa, double ya, double xb, double yb, double r, GFLcolor col)
{

  double x,y,yy,rr;
  float  d;
  glBegin(GL_LINES);
  rr=r*r;
  for (y=-r; y<=+r; y+=gfl_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=gfl_GetXPixSize())
    {
       if ((d=(double)(x*x+yy))>rr) continue;
       d=(1.-d/rr);
       glColor4f(d*col.r,d*col.g,d*col.b,d*col.a);
       glVertex2d(xa+x,ya+y);
       glVertex2d(xb+x,yb+y);
    }
  }
  glEnd();
}

extern void gfl_SpotMap(double xc, double yc, double r, GFLcolor col, float (*umap)(float))
{
  double x,y,yy,rr;
  float  d;

  glPointSize(SPOTPOINTSIZE);
  glBegin(GL_POINTS);
  rr=r*r;
  for (y=-r; y<=+r; y+=SPOTPOINTSIZE*gfl_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=SPOTPOINTSIZE*gfl_GetXPixSize())
    {
       if ((d=(float)(x*x+yy))>rr) continue;
       d=umap(1.-d/rr);
       glColor4f(col.r,col.g,col.b,d*col.a);
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glPointSize(1.);
}

extern void gfl_Spotf2(double xc, double yc, double r, GFLcolor col, int blend_mode)
{
  double x,y,yy,rr;
  float  d;

  switch (blend_mode)
  {
    case 1 : glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA); break;

    case 2 : glBlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_COLOR); break;
    case 3 : glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_COLOR); break;

    case 4 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_COLOR); break;
    case 5 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_COLOR); break;

    case 6 : glBlendFunc(GL_ONE_MINUS_SRC_COLOR,GL_DST_ALPHA); break;
    case 7 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_ALPHA); break;

    case 8 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); break;

  }
  glBegin(GL_POINTS);
  rr=r*r;
  for (y=-r; y<=+r; y+=gfl_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=gfl_GetXPixSize())
    {
       if ((d=(double)(x*x+yy))>rr) continue;
       d=(1.-d/rr);
       d=d*d;
       glColor4d(d*col.r,d*col.g,d*col.b,d*col.a);
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
}

#ifdef __cplusplus
  }
#endif
