/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code exemple pour prototypage avec libgfl.7e                  =!*/
/*!=================================================================!*/

#include <gfl.h>

/* tailles initiales de la fenêtre graphique (en pixels)     */
#define WWIDTH  720 
#define WHEIGHT 720

static void hsv_circle(void)
{
  double x,y,r,step=2./WWIDTH;
  double h,s,v;
  GFLcolor rgb;

  for (y=-1;y<=+1;y+=step)
    for (x=-1;x<=+1;x+=step)
    {
      r = x*x+y*y;
      if (r>1.)
      {
        v = (y+1.)/2.;
        rgb=(GFLcolor){v,v,v,0.};
      }
      else
      {
        v = 1.;
        s = sqrt(r);
        if (y==0.) h = (x<0.?0.5:0.0);
        else       h = (2.*atan(-y/(-x+s))+PI)/(2.*PI);
        rgb = gfl_hsva_rgba_4f(h,s,v,0.);
      }
      gfl_Plot(x,y,rgb,1);
    }
}

/*! fonction d'initialisation !*/
static void init(void)
{
  gfl_SetBkGdCol(0.); /* couleur de fond : noir */
}

/*! fonction de dessin        !*/
static void draw(void)
{
  hsv_circle();
}

/*!***************************!*/
/*! fonction principale       !*/
/*!***************************!*/
int main(int argc, char **argv)
{
  gfl_InitWindow(*argv,WWIDTH,WHEIGHT);
  gfl_SetCenteredDrawZone(0.,0.,2.,0.);

  gfl_SetInitFunction(init); /*! fonction d'initialisation !*/
  gfl_SetDrawFunction(draw); /*! fonction de dessin        !*/

  return gfl_MainStart();
}
