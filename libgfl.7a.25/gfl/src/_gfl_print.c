/* ===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - 2022/2023
  ============================================================= */


/*=================================================*/
/*=           fonctions d'ecriture                =*/
/*=================================================*/
typedef struct
{
  const    uchar *str;
  double   x,y;
  GFLcolor col;
  int      len;
} GFLtext;

static char  _print_posit_='l';
static char  _print_style_='m';
static void* _print_font_ =  GLUT_BITMAP_9_BY_15;
static int   _v_offset_=0;
static int   _f_height_;

extern void gfl_SetFontAttributes(char size, char style, char pos)
{
  if (pos) _print_posit_ = pos;
  switch (size)
  {
    case 's' : _print_font_=GLUT_BITMAP_HELVETICA_10;   _v_offset_= 3; _f_height_= 5; break;
    case 'm' : _print_font_=GLUT_BITMAP_HELVETICA_12;   _v_offset_= 4; _f_height_= 6; break;
    case 'l' : _print_font_=GLUT_BITMAP_HELVETICA_18;   _v_offset_= 6; _f_height_= 9; break;
    case 'L' : _print_font_=GLUT_BITMAP_TIMES_ROMAN_24; _v_offset_= 8; _f_height_=12; break;
    case  0  : break;
  }
  switch (style)
  {
    case 'n' :
    case 'b' : _print_style_=style; break;
    case 'B' : _print_style_=style; break;
    case  0  : break;
  }
}

#ifdef _GFL2D_
  static __inline__ void gfl_TextPrint(GFLtext T)
  {
    switch (_print_posit_)
    {
      case 'c' : T.x -= 0.5*Xpixsize*T.len; break;
      case 'r' : T.x -= Xpixsize*(T.len+4); break;
      default  :                            break;
    }
    T.y-=_v_offset_*Ypixsize;
  
    gfl_Color4fv(T.col);
    glRasterPos2d(T.x,T.y);
    glutBitmapString(_print_font_,T.str);
  
    if (_print_style_!='b' && _print_style_!='B') return;
    glRasterPos2d(T.x+Xpixsize,T.y);
    glutBitmapString(_print_font_,T.str);
  
    if (_print_style_!='B') return;
    glRasterPos2d(T.x+2*Xpixsize,T.y);
    glutBitmapString(_print_font_,T.str);
  }
  
  /*====================================================================*/
  extern void gfl_Print(double x, double y, GFLcolor col, const char *format, ...)
  {
    va_list ap;
    char   msg[1024];
    const uchar *str=(uchar*)msg;
    va_start(ap, format);
    vsprintf(msg, format, ap);
    va_end(ap);
    GFLtext TEXT=(GFLtext){str,x,y,col,glutBitmapLength(_print_font_,str)};
    gfl_TextPrint(TEXT);
  }
#endif

/*=== TEXTE STATIQUE ===*/
static __inline__ void gfl_StaticTextPrint(GFLtext T)
{
  switch (_print_posit_)
  {
    case 'c' : T.x-=0.5*T.len; break;
    case 'r' : T.x-=(T.len+4); break;
    default  :                 break;
  }
  T.y-=_v_offset_;

  gfl_Color4fv(T.col);
  glRasterPos2d(GFLpixtowinX1(T.x),GFLpixtowinY1(T.y));
  glutBitmapString(_print_font_,T.str);

  if (_print_style_!='b' && _print_style_!='B') return;
  glRasterPos2d(GFLpixtowinX1(T.x+1),GFLpixtowinY1(T.y));
  glutBitmapString(_print_font_,T.str);

  if (_print_style_!='B') return;
  glRasterPos2d(GFLpixtowinX1(T.x+2),GFLpixtowinY1(T.y));
  glutBitmapString(_print_font_,T.str);
}


/*====================================================================*/
extern void gfl_StaticPrint(int x, int y, GFLcolor col, const char *format, ...)
{
  va_list ap;
  char    msg[1024];
  const uchar *str=(uchar*)msg;
  va_start(ap, format);
  vsprintf(msg, format, ap);
  va_end(ap);
  GFLtext TEXT=(GFLtext){str,(double)x,(double)y,col,glutBitmapLength(_print_font_,str)};
  gfl_StaticTextPrint(TEXT);
}

/*!
 * affichage texte dans case, plus ou moins pré-formaté
 !*/
extern void gfl_StaticTextBox(int x, int y, int margin, GFLcolor txtcol, GFLcolor bkgcol, int borderw, const char *format, ...)
{
  va_list ap;
  char    msg[1024];
  const uchar *str=(uchar*)msg;
  va_start(ap, format);
  vsprintf(msg, format, ap);
  va_end(ap);

  GFLtext TEXT=(GFLtext){str,(double)x,(double)y,txtcol,glutBitmapLength(_print_font_,str)};

  x-=margin;
  double xsize = TEXT.len+2*margin;
  double ysize = (1      +  margin)*_v_offset_;
  double dxsize = xsize*Xpixsize;
  double dysize = ysize*Ypixsize;

  double xoffset=0.;
  switch (_print_posit_)
  {
    case 'l' : xoffset= 0.;         break;
    case 'c' : xoffset= dxsize*0.5; break;
    case 'r' : xoffset= dxsize;     break;
  }

  double dx=Xwmin+x*Xpixsize-xoffset;
  double dy=Ywmin+y*Ypixsize-dysize/2.;
  
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  GFLcolor4fv(bkgcol);
  glRectd(dx,dy,dx+dxsize,dy+dysize);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  GFLcolor4fv(txtcol);
  glLineWidth((GLfloat)borderw);
  glRectd(dx,dy,dx+dxsize,dy+dysize);
  glLineWidth(1.);
  
  gfl_StaticTextPrint(TEXT);
}






/*====================================================================*/
/* variante ancienne version - obsolète                               */
extern void gfl_Write(const char* msg, int x, int y, GFLcolor col)
{
  const char  *c;
  #ifdef _GFL3D_
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(x,y);
  #endif
  #ifdef _GFL2D_
    glRasterPos2d(GFLpixtowinX1(x),GFLpixtowinY1(y));
  #endif
  gfl_Color4fv(col);
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(_print_font_,*c);
  #ifdef _GFL3D_
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
  #endif
}
