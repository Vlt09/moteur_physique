/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 540


/* -----------------------------------------------------------------------
 * VARIABLES GLOBALES
 * ----------------------------------------------------------------------- */

/* des points (cf. <gfl_geom.h>)    */
static GFLpoint A,B,C,D,E;
static GFLpoint  mpos;     // la position de la souris

/* parametres pour l'animation */
static double   xa1,ya1,xa2,ya2,pasxa1,pasya1,pasxa2,pasya2;

/* des parametres controlables pour les scrollbars */
static double   rA,rE,cE,bkg;
/* une couleur  (cf. <gfl_color.h>) */
static GFLcolor col;

/* des flags booleen pour les boutons 'switch' */
static bool _DISKFLAG_  =false;
static bool _SQUAREFLAG_=false;
static bool _TEXTFLAG_  =false;

/* des numéros de boutons exclusif - ça DOIT être des constantes */
#define _BUT1_ 0
#define _BUT2_ 1

/* -----------------------------------------------------------------------
 * FONCTIONS D'INTERACTIONS ASSOCIEES A DES TOUCHES, BOUTONS
 * ----------------------------------------------------------------------- */
/*! une fonction associée a une touche clavier 'normale' : 's' [cf void ctrl(void)] !*/
static void toggle_square(void) { _SQUAREFLAG_=!_SQUAREFLAG_; }

/*! une fonction associée à une touche clavier 'spéciale' : <F1> [cf void ctrl(void)] !*/
/*! -> affiche l'état sur <stderr> !*/
static void toggle_panscan(void)
{
  static bool flag = true;
  gfl_SetPanScanAndZoom((flag=!flag));  /* (cf. <gfl_window.h>) */
  fprintf(stderr,"PANSCAN %sLOCKED\e[0;0m                                     \r",
                  flag?"\e[1;34mUN":"\e[1;31m");
}

/*! un fonction associée a un bouton 'popup' : !*/
/*! remise aux positions initiales             !*/
static void reset(void)
{
  xa1=0.5*gfl_GetXMin(); ya1=0.5*gfl_GetYMin();
  xa2=0.5*gfl_GetXMax(); ya2=0.5*gfl_GetYMax();
  A.x=xa1; A.y=ya1;
  B.x=xa2; B.y=ya1;
  C.x=xa1; C.y=ya2;
  D.x=xa2; D.y=ya2;
  E.x=0.0; E.y=0.0;
}

/*! la fonction d'initialisation !*/
static void init(void)
{
  reset();
  rA=0.2;
  rE=0.2;
  cE=0.5;
  bkg=1.;
  col=(GFLcolor){0.,0.,1.};
  pasxa1=pasxa2=(xa2-xa1)*0.004;
  pasya1=pasya2=(ya2-ya1)*0.004;

  fprintf(stderr,"(%lf,%lf)->(%lf,%lf)\n",gfl_GetXMin(),gfl_GetYMin(),gfl_GetXMax(),gfl_GetYMax());
}

/*! la fonction de contrôle (cf. <gfl_ctrl.h>) !*/
static void ctrl(void)
{
  /* 5 points définis comme "controlables" à la souris (clic>drag&drop) (cf. <gfl_ctrl.h>) */
  gfl_SetControlPoint(&A,10);
  gfl_SetControlPoint(&B,10);
  gfl_SetControlPoint(&C,10);
  gfl_SetControlPoint(&D,10);
  gfl_SetControlPoint(&E,10);

  /* les boutons 'switch' (on/off) - associes a des flag booleen (cf. <gfl_ctrl.h>) */
  gfl_CreateSwitch("D.",&_DISKFLAG_  ,"affiche/masque le disque de centre A");
  gfl_CreateSwitch("S.",&_SQUAREFLAG_,"affiche/masque le 'quad' <ABDC>     ");

  /* les boutons selecteurs exclusifs => a utiliser avec un switch/case (cf. <gfl_ctrl.h>) */
  gfl_CreateButton("spot","affiche/masque le 'spot' de centre E      "); // -> _BUT1_
  gfl_CreateButton("tri.","affiche/masque le triangle tricolore <BCD>"); // -> _BUT2_

  /* un bouton "popup" : exécute une action 1 fois à chaque clic (cf. <gfl_ctrl.h>) */
  gfl_CreatePopUp("reset",reset,"reset positions");

  /* les scrollbars : 1 horiz. / 2 vertic., tous attaché à des var. réelles (cf. <gfl_ctrl.h>) */
  gfl_CreateScrollh_d("rA" ,&rA,0.1,1.0,"rayon du disque de centre A      ");

  int id; /* pour recuperer le n° du scrollbar .... */
  id=gfl_CreateScrollv_d("rE" ,&rE,0.1,1.0,"rayon du 'spot' de centre E      ");
  /* ... pour changer sa couleur de fond du scrollbar */
  gfl_SetScrollColor(id,GFLrb_c);

  id=gfl_CreateScrollv_d("col",&cE,0.0,1.0,"couleur du spot 0.:bleu->1.:rouge");
  gfl_SetScrollColor(id,GFLgb_c);

  /* une action attachee a une touce clavier 'normale' (cf. <gfl_ctrl.h>)  */
  gfl_SetKeyAction('s',toggle_square,"affiche/masque le 'quad' <ABDC>");
  /* une action attachee a une touce clavier 'spéciale' (cf. <gfl_ctrl.h>)  */
  gfl_SetSpecialKeyAction(SKEY_F1,toggle_panscan,"bloque/débloque le zoom et le panscan");

  gfl_CreateSwitch("T.",&_TEXTFLAG_,"affiche/masque les textes");
}

/*! la fonction de gestion des événements (clavier/souris) !*/
static void evts(void)
{
  /* la souris :
   * ATTENTION : gfl_GetClic() renvoie NULL si pas de clic
   * donc ne SURTOUT PAS utiliser directement (clic->x,clic->y) ==> Seg.Fault !!!
   * => TOUJOURS dans un  if ( (clic=gfl_GetClic())) ...
   * => gfl_GetMousePosition() : position courante, indep. du clic
   */
  GFLpoint *clic=NULL;// si clic gauche -> récup. position

  if (!(clic=gfl_GetClic())) return;
  mpos=*gfl_GetMousePosition();
}

/*! la fonction de dessin (cf <gfl_draw.h>) !*/
static void draw()
{
  /* affichages liés aux 'switch' */
  if (_SQUAREFLAG_) gfl_FillQuad(A.x,A.y,B.x,B.y,D.x,D.y,C.x,C.y,GFLya_b);  /* tracé de formes diverses... */
  if (_DISKFLAG_)   gfl_FillCircle(A.x,A.y,rA,GFLca_b);                     /* ... cf. <gfl_draw.h>        */
  if (_TEXTFLAG_)                                                           /* exemples texte              */
  {
    gfl_SetFontAttributes('L','B','c'); gfl_StaticTextBox(150, 30,3,GFLr,GFLo_c ,2,"size : 'L', style : 'B', pos : 'c'");
    gfl_SetFontAttributes('l','b', 0 ); gfl_StaticTextBox(150, 70,3,GFLb,GFLwc_a,1,"size : 'l', style : 'b', pos : 'c'");
    gfl_SetFontAttributes('m','n','l'); gfl_StaticTextBox(150,100,3,GFLw,GFLb   ,0,"size : 'm', style : 'n', pos : 'l'");
    gfl_SetFontAttributes('s','b','r'); gfl_StaticTextBox(150,130,3,GFLk,GFLwc_a,1,"size : 's', style : 'b', pos : 'r'");
  }
  /* affichages liés aux boutons selecteurs */
  switch (gfl_GetButton())
  {
    case _BUT1_ : col.r=cE; col.g=1.-cE; gfl_Spot(E.x,E.y,rE,col);                       break;
    case _BUT2_ : gfl_FillTriangle_3col(B.x,B.y,GFLr_b ,C.x,C.y,GFLg_b ,D.x,D.y,GFLb_b); break;
  }

  /* des affichages divers */
  gfl_Line(A.x,A.y,E.x,E.y,GFLr,2); /* trace une ligne entre A et E */
  gfl_DrawLine(B,E,GFLg,1);         /* variante                     */

  /* trace des points et affiche leurs coord. à côté*/
  gfl_SetFontAttributes('m','n','r'); /* MEDIUM|NORMAL|RIGHT */
  gfl_Plot(A.x,A.y,GFLr,4);  gfl_Print(A.x,A.y,GFLk,"  A:%+.2lf,%+.2lf",A.x,A.y);
  /* variante */
  gfl_DrawPoint(B,GFLr,4);   gfl_Print(B.x,B.y,GFLk,"  B:%+.2lf,%+.2lf",B.x,B.y);

  gfl_SetFontAttributes(0,0,'l');     /* ------|------|LEFT  */
  gfl_Plot(C.x,C.y,GFLr,4); gfl_Print(C.x,C.y,GFLk,"  C:%+.2lf,%+.2lf",C.x,C.y);
  gfl_Plot(D.x,D.y,GFLr,4); gfl_Print(D.x,D.y,GFLk,"  D:%+.2lf,%+.2lf",D.x,D.y);
  gfl_Plot(E.x,E.y,GFLr,4); gfl_Print(E.x,E.y,GFLk,"  E:%+.2lf,%+.2lf",E.x,E.y);

  /* manip. texte - cf. <gfl_window.h> */
  gfl_SetFontAttributes('L','B','l'); /* LARGE|BOLD|LEFT */
  gfl_StaticPrint(gfl_GetPixWidth()/2,gfl_GetPixHeight()/2,GFLwc_a,"TEXTE FIXE");

}

/*! la fonction d'animation - run/stop : <SPACEBAR> !*/
/*! modifie les données globales entre 2 affichages !*/
static void anim(void)
{
  /* avancement des parametres */
  xa1+=pasxa1; ya1+=pasya1;
  xa2-=pasxa2; ya2-=pasya2;
  /* change de direction sur les bords de la fenetre */
  if (xa1>gfl_GetXMax() || xa1<gfl_GetXMin()) pasxa1=-pasxa1;
  if (xa2>gfl_GetXMax() || xa2<gfl_GetXMin()) pasxa2=-pasxa2;
  if (ya1>gfl_GetYMax() || ya1<gfl_GetYMin()) pasya1=-pasya1;
  if (ya2>gfl_GetYMax() || ya2<gfl_GetYMin()) pasya2=-pasya2;
  /* mise a jour des 4 points (A,B,C,D) : 3 façons de faire */
  /*1*/A = (GFLpoint){xa1,ya1};
  /*2*/B.x=xa1; B.y=ya2;
  /*3*/C = gfl_Point2d(xa2,ya1);
       D = (GFLpoint){xa2,ya2};
}

/*! la fonction de sortie (passée à <atexit>) !*/
/*! ici, rien à faire...                      !*/
static void quit()
{
  fprintf(stdout,"\n--bye--\n");
}

/***************************************************************************/
/** La fonction principale : NE CHANGE JAMAIS (ou presque)                **/
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels)  */
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles */
  gfl_SetCenteredDrawZone(0.,0.,5.,0.);

  /* 3°) association fonctions/handlers                   */
  gfl_SetInitFunction(init); /* fonction d'initialisation */
  gfl_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  gfl_SetEvtsFunction(evts); /* fonction d'événements     */
  gfl_SetDrawFunction(draw); /* fonction de dessin        */
  gfl_SetAnimFunction(anim); /* fonction d'animation      */
  gfl_SetExitFunction(quit); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale                */
  return gfl_MainStart();
  /* RIEN APRES CA                                        */
  fprintf(stdout,"ce texte ne s'affiche pas !!! ");
}
