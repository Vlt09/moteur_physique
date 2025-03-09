/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/
typedef struct
{
  bool on;  /* flag actif/inactif             */
  int  num; /* numero attribue au bouton      */
  int  x,y; /* coordonnees du bouton          */
  int  len; /* taille du nom                  */
  char name[_GFL_NAMESIZE_+1]; /* nom               */
  char info[_GFL_INFOSIZE_+1]; /* info associée     */
} GFLbut;

static GFLbut *_BUTTON_=NULL,*TheButton=NULL;
static int      butnum=0;

static int _button_id_on_=0,_button_id_off_=0;

/*=  Attribue un numero et un texte au bouton  =*/
extern int gfl_CreateButton(const char *name, const char *info)
{
  if (!_BUTTON_)
    if (!(_BUTTON_=(GFLbut*)calloc(_GFL_BLOCSIZE_,sizeof(GFLbut)))) return false;

  GFLbut *but = _BUTTON_+butnum;

  strncpy(but->name,name,_GFL_NAMESIZE_);
  if (info) strncpy(but->info,info,127);
  but->len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,but->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  but->num  = butnum++;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=22;
  but->on   = false;
  return true;
}

/*= libere les bouttons        =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreeButtons()
{
  if (_BUTTON_)
  {
    free(_BUTTON_);
    TheButton=_BUTTON_=NULL;
  }
}*/


/*=  Renvoie le numero du bouton selectionne =*/
extern int gfl_GetButton(void) { return TheButton?(int)TheButton->num:-1; }

/*=  Debranche tous les boutons =*/
extern void gfl_ReleaseButton(void)
{
  if (!TheButton) return;
  TheButton->on=false;
  TheButton=NULL;
}


/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool gfl_SelectButton(int x, int y)
{
  GFLbut *but=_BUTTON_;
  while (but<_BUTTON_+butnum)
  {
    if (abs(but->x-x)<2*Xbutw && abs(but->y-y)<10)
    {
      pushmode=GLUT_DOWN;
      if (TheButton!=NULL) {                TheButton->on=false; }
      if (TheButton!=but ) { TheButton=but; TheButton->on=true;  }
      else TheButton=NULL;
      return true;
    }
    but++;
  }
  return false;
}

static __inline__ void gfl_InitButOn()
{
  _button_id_on_ = glGenLists(1);
  glNewList(_button_id_on_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      gfl_Color4fv(GFLy_b);
      glVertex2i(0,0);
      gfl_Color4fv(GFLg_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}

static __inline__ void gfl_InitButOff()
{
  _button_id_off_ = glGenLists(1);
  glNewList(_button_id_off_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      gfl_Color4fv(GFLo_b);
      glVertex2i(0,0);
      gfl_Color4fv(GFLr_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEndList();
}

/*=  dessinne tous les boutons  =*/
static __inline__ void gfl_DrawButtons(void)
{
  GFLbut  *but=_BUTTON_;
  GFLcolor col;
  while (but<_BUTTON_+butnum)
  {
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-but->y,0.);
      glCallList(but->on?_button_id_on_:_button_id_off_);
      glRasterPos2i(12,-4);
      col   = GFLk;
      col.a = 1.-col.a;
      gfl_Color4fv(col);
      glRasterPos2i(-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)but->name);
      glRasterPos2i(1-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)but->name);
    glPopMatrix();
    but++;
  }
}
