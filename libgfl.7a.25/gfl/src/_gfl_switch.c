/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/
/**@brief simple on/off switch associated to boolean gloabl <flag> (green=true / red=false)   */
typedef struct
{
  bool *flag;            /**@brief pointer to a global boolean var         */
  int  num;              /**@brief the button number                       */
  int  x,y;              /**@brief button coordinates on the display       */
  int  len;              /**@brief button label length                     */
  char name[_GFL_NAMESIZE_+1]; /**@brief the button name                         */
  char info[_GFL_INFOSIZE_+1]; /**@brief text for information pannel (key '?')   */
} GFLswitch;

#define _SWITCHWIDTH_ 15
static GFLswitch *_SWITCH_=NULL,*TheSwitch=NULL;
static int       switchnum=0;

static int _switch_id_on_=0,_switch_id_off_=0,
           _info_id_on_  =0,_info_id_off_  =0;


/*=  Attribue un numero et un texte au switch  =*/
extern bool gfl_CreateSwitch(const char *name, bool *flag, const char *info)
{
  if (!_SWITCH_)
    if (!(_SWITCH_=(GFLswitch*)calloc(_GFL_BLOCSIZE_,sizeof(GFLswitch)))) return false;

  GFLswitch *swt = _SWITCH_+switchnum;

  strncpy(swt->name,name,_GFL_NAMESIZE_);
  if (info) strncpy(swt->info,info,127);
  swt->len = 0;
  swt->len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,swt->len+32);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  swt->num    = switchnum;
  swt->flag   = flag;
  swt->x      = Xbutpos;
  swt->y      = Ybutpos; Ybutpos+=22;
  switchnum++;
  return true;
}

/*=  Renvoie le numero du switch selectionne =*/
extern int gfl_GetSwitch(void) { return TheSwitch?(int)TheSwitch->num:-1; }


/*= Si un switch est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheSwitch>   =*/
static __inline__ bool gfl_SelectSwitch(int x, int y)
{
  GFLswitch *swt=_SWITCH_;
  while (swt<_SWITCH_+switchnum)
  {
    if (abs(swt->x-x)<2*Xbutw && abs(swt->y-y)<10)
    {
      pushmode=GLUT_DOWN;
     *swt->flag=!*swt->flag;
      return true;
    }
    swt++;
  }
  return false;
}

/*= libere les switch        =*/
/* obsolète -> garbage collector
static __inline__ void gfl_FreeSwitches()
{
  if (_SWITCH_)
  {
    free(_SWITCH_);
    TheSwitch=_SWITCH_=NULL;
  }
}*/

static __inline__ void gfl_InitSwitchOn()
{
  int   r  = _SWITCHWIDTH_;
  int   rr = r*r;
  float irr= 1./rr;
  int   x,y,yy;
  float d;

  _switch_id_on_ = glGenLists(1);
  glNewList(_switch_id_on_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_POINTS);
    for (y=-r; y<=+r; y++)
    {
      yy=y*y;
      for (x=-r; x<=+r; x++)
      {
        if ((d=(float)(x*x+yy))>rr) continue;
        d=(1.-d*irr);
        d=d*d;
        glColor4f(0.,d,0,d);
        glVertex2i(x,y);
      }
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glEndList();
}

static __inline__ void gfl_InitSwitchOff()
{
  int   r  = _SWITCHWIDTH_;
  int   rr = r*r;
  float irr= 1./rr;
  int   x,y,yy;
  float d;

  _switch_id_off_ = glGenLists(1);
  glNewList(_switch_id_off_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_POINTS);
    for (y=-r; y<=+r; y++)
    {
      yy=y*y;
      for (x=-r; x<=+r; x++)
      {
        if ((d=(float)(x*x+yy))>rr) continue;
        d=(1.-d*irr);
        d=d*d;
        glColor4f(d,0.,0,d);
        glVertex2i(x,y);
      }
    }
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glEnd();
    glEndList();
}

static __inline__ void gfl_DrawSwitches(void)
{
  for (GFLswitch  *swt=_SWITCH_; swt<_SWITCH_+switchnum; swt++)
  {
    glPushMatrix();
      glTranslatef(+_SWITCHWIDTH_,curheight-swt->y,0.);
      glCallList(*swt->flag?_switch_id_on_:_switch_id_off_);
      glColor4f(GFLk.r,GFLk.g,GFLk.b,1.-GFLk.a);
      glRasterPos2i(_SWITCHWIDTH_,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)swt->name);
    glPopMatrix();
  }
}

static void gfl_InitCornSwitchOn()
{
  _info_id_on_ = glGenLists(1);
  glNewList(_info_id_on_, GL_COMPILE);
    int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
    glColor4f(GFLk.r,GFLwc.g,GFLwc.b,1.-GFLwc.a);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'?');
    glColor4f(GFLw.r,GFLw.g,GFLw.b,1.-GFLw.a);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'?');
  glEndList();
}

static void gfl_InitCornSwitchOff()
{
  _info_id_off_ = glGenLists(1);
  glNewList(_info_id_off_, GL_COMPILE);
    int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
    GFLcolor col = GFLwb;
    col.a = 1.-col.a;
    gfl_Color4fv(col);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
    col = GFLk;
    col.a = 1.-col.a;
    gfl_Color4fv(col);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glEndList();
}

static bool _info_switch_=false;
static void gfl_DrawInfoSwitch(void)
{
  glPushMatrix();
    glTranslatef(Ydialwidth/2,Xdialheight/2,0.);
    glCallList(_info_switch_?_info_id_on_:_info_id_off_);
  glPopMatrix();
}
