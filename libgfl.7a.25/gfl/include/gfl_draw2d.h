/**@file    gfl_draw.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#endif

#ifdef _GFL2D_
  #ifndef _GFL_DRAW2D_H
    #define _GFL_DRAW2D_H
  /**@endcond                   */
  
    /* Fonctions de dessin de base */
    #include <gfl_colors.h>
    #include <gfl_geom2d.h>
  
    /* fixe la couleur de trace courante */
    void gfl_SetColor(GFLcolor col);
  
    /* fixe la taille du point courante */
    void gfl_SetPointSize(double size);
  
    /*!  POINTS SIMPLES                                                           !*/
    /* trace un point en (x,y) avec la couleur et taille courantes                 */
    void gfl_PlotDft(double x, double y);
  
    /* trace un point en (x,y) avec la couleur et taille donnees                   */
    /* <color> et <width> deviennent les valeurs courantes                         */
    void gfl_Plot(double x, double y, GFLcolor color, int width);
    void gfl_DrawPoint(GFLpoint P, GFLcolor color, int width);
  
    /*!  LIGNES SIMPLES                                                           !*/
    /* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille courantes        */
    void gfl_LineDft(double xa, double ya, double xb, double yb);
  
    /* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille doonees          */
    /* <color> et <width> deviennent les valeurs courantes                         */
    void gfl_Line(double xa, double ya, double xb, double yb,GFLcolor color, int width);
    void gfl_DrawLine(GFLpoint A, GFLpoint B, GFLcolor color, int width);
  
    /* trace une ligne (xa,ya)->(xb,yb) en interpollant les couleurs de cola a colb */
    /* <colb> et <width> deviennent les valeurs courantes                           */
    void gfl_Line_2col(double xa, double ya, GFLcolor cola, double xb, double yb, GFLcolor colb, int width);
    void gfl_DrawLine_2col(GFLpoint A, GFLcolor colA, GFLpoint B, GFLcolor colB, int width);
  
    /*! CERCLES/DISQUE                                                             !*/
    /* Centre (xc,yc), rayon r, couleur, épaisseur du trait                         */
    void gfl_Circle(double xc, double yc, double r, GFLcolor col, int width);
    void gfl_DrawCircle(GFLpoint C, double r, GFLcolor col, int width);
  
    void gfl_FillCircle(double xc, double yc, double r, GFLcolor col);
    void gfl_DrawFillCircle(GFLpoint C, double r, GFLcolor col);
  
    /* tranches et secteurs ('camenberts') */
    void gfl_FillCircleSlice(double xc, double yc, GFLcolor colC, double xa, double ya, GFLcolor colA, double deg);
    void gfl_FillCircleSector(double xc, double yc, double xa, double ya, double xb, double yb, GFLcolor col);
  
    /*!  ELLIPSES                                                                                     !*/
    /* Centre (xc,yc), rayons (rx,ry), orientation degrot (en degres), couleur, epaisseur du trait.... */
    void gfl_Ellipse(double xc, double yc, double rx, double ry, double degrot, GFLcolor col, int width);
    void gfl_FillEllipse(double xc, double yc, double rx, double ry, double degrot, GFLcolor col);
    /* Secteur d'ellipse : angle du secteur degsec (en degres) */
    void gfl_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, GFLcolor col, int width);
    void gfl_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, GFLcolor col);
  
    /*! TRIANGLES                                                                     !*/
    void gfl_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, GFLcolor col, int width);
    void gfl_DrawTriangle(GFLpoint A, GFLpoint B, GFLpoint C, GFLcolor color, int width);
  
    void gfl_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, GFLcolor col);
    void gfl_DrawFillTriangle(GFLpoint A, GFLpoint B, GFLpoint C, GFLcolor color);
  
    /* couleurs interpollées sur les sommets                                           */
    void gfl_FillTriangle_3col(double xa, double ya, GFLcolor cola,
                               double xb, double yb, GFLcolor colb,
                               double xc, double yc, GFLcolor colc);
    void gfl_DrawFillTriangle_3col(GFLpoint A, GFLcolor colA,
                                   GFLpoint B, GFLcolor colB,
                                   GFLpoint C, GFLcolor colC);
  
    /*! RECTANGLE PARALLELE AUX AXES                                                  !*/
    /* coin inf. gauche (xd,yd) Coin sup. droit (xu,yu)                                */
    void gfl_Rectangle(double xdl, double ydl, double xur, double yur, GFLcolor col, int width);
    void gfl_DrawRectangle(GFLpoint DL, GFLpoint UR, GFLcolor color, int width);
    void gfl_FillRectangle(double xdl, double ydl, double xur, double yur, GFLcolor col);
    void gfl_DrawFillRectangle(GFLpoint DL, GFLpoint UR, GFLcolor color);
  
  
    /*! QUADRILATERES QUELCONQUES (ABCD)                                              !*/
    void gfl_Quad(double xa, double ya, double xb, double yb,
                  double xc, double yc, double xd, double yd, GFLcolor col, int width);
    void gfl_DrawQuad(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint D, GFLcolor color, int width);
    void gfl_FillQuad(double xa, double ya, double xb, double yb,
                      double xc, double yc, double xd, double yd, GFLcolor col);
    void gfl_DrawFillQuad(GFLpoint A, GFLpoint B, GFLpoint C, GFLpoint D, GFLcolor color);
  
    /* couleurs interpollées sur les sommets                                           */
    void gfl_FillQuad_4col(double xa, double ya,GFLcolor cola,
                           double xb, double yb,GFLcolor colb,
                           double xc, double yc,GFLcolor colc,
                           double xd, double yd,GFLcolor cold);
    void gfl_DrawFillQuad_4col(GFLpoint A,GFLcolor cola,
                               GFLpoint B,GFLcolor colb,
                               GFLpoint C,GFLcolor colc,
                               GFLpoint D,GFLcolor cold);
  
    /*! "spot" : tache de couleur <col> dégradée vers le transparent                  !*/
    void gfl_Spot(double xc, double yc, double r, GFLcolor col);
  
    /* Quelques versions en macros - conservées pour compatibilité avec anciennes versions */
    /* MAIS IL FAUT EVITER DE LES UTILISER                                                 */
    #define GFLdefpoint(x,y)                    /**/((GFLpoint){(double)(x),(double)(y)})
    #define GFLdrawpoint(P,col,size)            /**/gfl_Plot((P).x,(P).y,(col),(size));
    #define GFLdrawline(A,B,col,size)           /**/gfl_Line(((A).x),((A).y),((B).x),((B).y),(col),(size))
    #define GFLdrawline2col(A,colA,B,colB,size) /**/gfl_Line_2col(((A).x),((A).y),(colA),((B).x),((B).y),(colB),(size))
    #define GFLdrawvect(A,U,col,size)           /**/gfl_Line(((A).x),((A).y),((A).x+(U).x),((A).y+(U).y),(col),(size))
    #define GFLdrawcircle(C,r,col,size)         /**/gfl_Circle((C).x,(C).y,(r),(col),(size))
    #define GFLdrawfillcircle(C,r,col)          /**/gfl_FillCircle((C).x,(C).y,(r),(col))
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
