/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  960 
#define WHEIGHT 960

static GFLpoint  P;

/*! fonction d'initialisation !*/
static void init(void)
{
  gfl_SetBkGdCol(0.); /* couleur de fond : noir */
  P = (GFLpoint){0.,0.};
  gfl_SetFontAttributes('l','b','l'); 
}

/*! fonction de contrôle      !*/
static void ctrl(void)
{
  gfl_SetControlPoint(&P,10);
}

/*! fonction de dessin        !*/
static void draw(void)
{

  GFLvector u = (GFLvector)P;
  if (gfl_SqrNorm(u)>1.) gfl_Normalize(&u);
  P = (GFLpoint)u;

  /* saturation : distance courante au centre */
  double s = gfl_Normalize(&u);
  /* teinte : angle courant par rapport au vecteur [0,0]->[1,0] (rouge) */
  double h = PI - acos(gfl_ProdScal((GFLvector){1.,0.},u));
         h*= (gfl_ProdVect((GFLvector){1.,0.},u)<0.?+1.:-1.);
         h = 0.5*(1+h/PI);

  GFLcolor RGB = gfl_hsva_rgba_4f(h,s,1.,0.);
  GFLcolor NEG = gfl_Color_NegRGB(RGB);

  gfl_FillRectangle(-1.,-1.,1.,1,NEG);
  gfl_FillCircle(0.,0.,1.,RGB);

  gfl_Axes();

  gfl_Line(P.x,P.y,P.x,0.,NEG,1);
  gfl_Line(P.x,P.y,0.,P.y,NEG,1);
  gfl_Line(0.,0.,u.x,u.y,NEG,1);
  gfl_Plot(P.x,P.y,GFLk,8);
  gfl_Print(P.x,P.y,GFLk," (%.1lf,%.1lf)",h,s);
  gfl_Circle(0.,0.,1.,NEG,1.);

  gfl_StaticPrint(10,35,GFLw,"HEX=#%X|%X|%X",(int)rint(255*RGB.r),(int)rint(255*RGB.g),(int)rint(255*RGB.b));
  gfl_StaticPrint(10,15,GFLw,"DEC=%03d|%03d|%03d",(int)rint(255*RGB.r),(int)rint(255*RGB.g),(int)rint(255*RGB.b));
 
  gfl_StaticPrint(gfl_GetPixWidth()-200,35,GFLw,"HEX=#%X|%X|%X",(int)rint(255*NEG.r),(int)rint(255*NEG.g),(int)rint(255*NEG.b));
  gfl_StaticPrint(gfl_GetPixWidth()-200,15,GFLw,"DEC=%03d|%03d|%03d",(int)rint(255*NEG.r),(int)rint(255*NEG.g),(int)rint(255*NEG.b));

  fprintf(stderr,"dist = %lf\t\r",gfl_ColorDist(RGB,NEG));
}


/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  gfl_SetCenteredDrawZone(0.,0.,2.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetCtrlFunction(ctrl); /*! fonction de controle      !*/
  gfl_SetEvtsFunction(NULL); /*! fonction d'événements     !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/
  gfl_SetAnimFunction(NULL); /*! fonction d'animation      !*/

  return gfl_MainStart();
}
