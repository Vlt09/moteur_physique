/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ===================================================================!*/

/*====================================================================*/
/*=              trace des axes (spécifique 2D)                      =*/
/*====================================================================*/
extern void gfl_Axes(void)
{
  glutSetWindow(drawwin);
  gfl_Color4fv(GFLwb);
  glLineWidth(1.);
  glBegin(GL_LINES);
    glVertex2d(Xwmin,0);
    glVertex2d(Xwmax,0);
    glVertex2d(0,Ywmin);
    glVertex2d(0,Ywmax);
  glEnd();
//  if (!_GFL_SHOW_COORD_) return;
  GFLcolor col=(GFLcolor){0.25,0.25,0.25,0.75};
  gfl_SetFontAttributes('s',0,'l');
  gfl_Print(Xwmin,0,col,"%.2f",Xwmin);
  gfl_SetFontAttributes('s',0,'r');
  gfl_Print(Xwmax,0,col,"%.2f",Xwmax);
}

/*====================================================================*/
extern void gfl_Grid(GFLcolor col1, GFLcolor col2)
{
  glutSetWindow(drawwin);
  double x,y;
  double xstep=0.05*(Xwmax-Xwmin),
	 ystep=0.05*(Ywmax-Ywmin);
  gfl_Color4fv(col2);
  glLineWidth(1.);
  glBegin(GL_LINES);
    for (x=+xstep;x>Xwmin;x-=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
    for (x=-xstep;x<Xwmax;x+=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
    for (y=+ystep;y<Ywmax;y+=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
    for (y=-ystep;y>Ywmin;y-=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
  gfl_Color4fv(col1);
  glVertex2d(Xwmin,0);
  glVertex2d(Xwmax,0);
  glVertex2d(0,Ywmin);
  glVertex2d(0,Ywmax);
  glEnd();
  GFLcolor col=(GFLcolor){0.25,0.25,0.25,0.75};
  gfl_SetFontAttributes('s',0,'l');
  gfl_Print(Xwmin,0,col,"%.2f",Xwmin);
  gfl_SetFontAttributes('s',0,'r');
  gfl_Print(Xwmax,0,col,"%.2f",Xwmax);
}

/*====================================================================*/
extern void gfl_Grad(GFLcolor col_axe, double step, GFLcolor col_grad)
{
  glutSetWindow(drawwin);
  double x,y;
  double xstep=step,
	       ystep=step;
  gfl_Color4fv(col_grad);
  glLineWidth(1.);
  glBegin(GL_LINES);
    for (x=+xstep;x>Xwmin;x-=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
    for (x=-xstep;x<Xwmax;x+=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
    for (y=+ystep;y<Ywmax;y+=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
    for (y=-ystep;y>Ywmin;y-=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
  glLineWidth(2.);
  gfl_Color4fv(col_axe);
  glVertex2d(Xwmin,0);
  glVertex2d(Xwmax,0);
  glVertex2d(0,Ywmin);
  glVertex2d(0,Ywmax);
  glEnd();
  GFLcolor col=(GFLcolor){0.,0.,0.,0.};
  double   voffset = +6*Xpixsize;
  gfl_SetFontAttributes('s',0,'l');
  gfl_Print(Xwmin,0+voffset,col,"%+.2f",Xwmin);
  gfl_SetFontAttributes('s',0,'r');
  gfl_Print(Xwmax,0+voffset,col,"%+.2f",Xwmax);
  gfl_SetFontAttributes('s',0,'c');
  gfl_Print(0,Ywmin+voffset,col,"%+.2f",Ywmin);
  gfl_Print(0,Ywmax-voffset,col,"%+.2f",Ywmax);
}

/*====================================================================*/
extern void gfl_MultiGrad(GFLcolor col_axe, int nb_level, ...)
{
  glutSetWindow(drawwin);
  va_list ap;
  va_start(ap,nb_level);
  double   step[8];
  GFLcolor col[8];
  int      i;

  double   x,y,xstep,ystep;

  for (i=0;i<nb_level;i++)
  {
    step[i] = va_arg(ap,double   );
    col[i]  = va_arg(ap,GFLcolor);
  }

  double e = 1.;
  for (i=nb_level-1;i>=0;i--)
  {
    xstep=step[i];
    ystep=step[i];
    if (xstep<5*Xpixsize || ystep<5*Ypixsize) continue;
    gfl_Color4fv(col[i]);
    glLineWidth(e);
    e*=1.5;
    glBegin(GL_LINES);
      for (x=+xstep;x>Xwmin;x-=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
      for (x=-xstep;x<Xwmax;x+=xstep) { glVertex2d(x,Ywmin); glVertex2d(x,Ywmax); }
      for (y=+ystep;y<Ywmax;y+=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
      for (y=-ystep;y>Ywmin;y-=ystep) { glVertex2d(Xwmin,y); glVertex2d(Xwmax,y); }
    glEnd();
  }
  gfl_Color4fv(col_axe);
  glLineWidth(2.);
  glBegin(GL_LINES);
    glVertex2d(Xwmin,0);
    glVertex2d(Xwmax,0);
    glVertex2d(0,Ywmin);
    glVertex2d(0,Ywmax);
  glEnd();
}

/*====================================================================*/
extern void gfl_AutoGrad(GFLcolor col_axe, GFLcolor col_grad)
{
  glutSetWindow(drawwin);
  double scale = 1;
  double step = (Xwmax-Xwmin)/2.;
  
  while (step<1. ) { scale*=10; step*=10.; }
  while (step>10.) { scale/=10; step/=10.; }

  step  = 1./scale;

  gfl_MultiGrad(GFLk,2,step*1.,GFLo_c,step*0.1,GFLwc_c);
  GFLcolor col=(GFLcolor){0.,0.,0.,0.};
  double   voffset = +6*Xpixsize;
  gfl_SetFontAttributes('s','b','l');
  gfl_Print(Xwmin,0+voffset,col,"%+.2f",Xwmin);
  gfl_SetFontAttributes('s','b','r');
  gfl_Print(Xwmax,0+voffset,col,"%+.2f",Xwmax);
  gfl_SetFontAttributes('s','b','c');
  gfl_Print(0,Ywmin+voffset,col,"%+.2f",Ywmin);
  gfl_Print(0,Ywmax-1.5*voffset,col,"%+.2f",Ywmax);
}


