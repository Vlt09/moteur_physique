/**@file    gfl_polygon.h
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
#ifed _GFL2D_
  #ifndef _GFL_POLYGON_H
    #define _GFL_POLYGON_H
  /**@endcond                   */
  
    #include <gfl_geom.h>
  
    typedef struct _cell
    {
      GFLpoint s;
      double   o; // ouverture de l'angle
      struct _cell *prev,*next;
    } GFLvertex, *GFLpolygon;
  
    void       gfl_SetPolygonStockSize(uint n);
    void       gfl_FreeStockPolygon(void); /* usage interne - NE PAS UTILISER */
    GFLvertex* gfl_Vertex_XY(double x, double y);
    GFLvertex* gfl_Vertex(GFLpoint P);
    void       gfl_InsereSommet(GFLpolygon *Pol, GFLpoint P);
    void       gfl_InsereVertex(GFLpolygon *Pol, GFLvertex *newv_rtx);
    GFLvertex* gfl_InsereApres(GFLvertex* V, GFLpoint P);
    GFLvertex* gfl_InsereVertexApres(GFLvertex* prev_vrtx, GFLvertex *new_vrtx);
    void       gfl_SetPolygonOpen(GFLpolygon pol);
    GFLvertex* gfl_DelVertex(GFLvertex** V);
    int        gfl_CleanPolygon(GFLpolygon Pol);
    void       gfl_PrintPolygon(GFLpolygon Pol, FILE* output);
    void       gfl_ScanPolygon(GFLpolygon* Pol, FILE* input);
    void       gfl_RegulPolygon(GFLpolygon* Pol, uint deg, GFLpoint C, double r, double alpha);
    void       gfl_TabPolygon(GFLpolygon* Pol, GFLpoint S[], uint n, bool closed);
    int        gfl_IsConvex(GFLpolygon Pol);
    void       gfl_DrawPolygon(GFLpolygon Pol, GFLcolor col, uint w);
    void       gfl_DrawSplinePolygon(GFLpolygon Pol);
    void       gfl_DrawFilledPolygon(GFLpolygon Pol, GFLcolor col);
    void       gfl_FreePolygon(GFLpolygon* Pol);
  
    bool       gfl_PointDansPolygon(GFLpoint P, GFLpolygon Pol);
    bool       gfl_VertexDansPolygon(GFLvertex *v, GFLpolygon Pol, int orient, GFLvertex** som);
    bool       gfl_PointDansPolygon2(GFLpoint P, GFLpolygon Pol, GFLvertex** som);
    GFLvertex* gfl_PointDansPolygon3(GFLpoint P, GFLpolygon Pol);
  
  /*! Intersection de polygones                                     !*/
    GFLpolygon gfl_PolygonInterPolygon(GFLpolygon Pol1, GFLpolygon Pol2);
  
  /*! Clipping d'un segment sur un polygone                         !*/
    bool      gfl_Clipping(GFLpoint *A, GFLpoint *B, GFLpolygon Pol);
    int       gfl_Clipping2(GFLpoint* A, GFLpoint* B, GFLpolygon Pol);
  
  /*! Clipping d'un polygone sur un cercle :
    *  -1 : les 2 sont disjoints - par d'untersection, pas d'inclusion
    *   0 : le polygone est inclus dans le cercle
    *  +1 : le cercle est inclus dans le polygone
    * n>1 : le polygone intersection a n sommets, certains pouvant
            appartenir au polyg. initial.
                                                                    !*/
    int gfl_CercleClipPolyg(GFLpoint C, double r, GFLpolygon Poly, GFLpolygon *Clip);
  
    /*! Enveloppe convexe (polygone) sur un ensemble de points !*/
    bool gfl_ConvexHull(GFLpoint *point, int nbpt, GFLpolygon *EC, int *EClen);
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
