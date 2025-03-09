#ifdef __cplusplus
  extern "C" {
#endif

  #ifndef _GFL_COLSYST_H
    #define _GFL_COLSYST_H
  
    #define GFL_RGB_ 0
    #define GFL_HSV_ 1
    #define GFL_YIQ_ 2
    #define GFL_YCC_ 3
  
    #include <gfl_types.h>
  
    /******************************************************************************/
    /*                          CONVERSION RGB -> GrayScale                       */
    /*  AVG0_or_MAX1 : False -> moyenne(R,G,B) / True -> max(R,G,B)               */
    /******************************************************************************/
    bool gfl_RGBtoGRAY(GFLpixmap** gray, GFLpixmap* rgb, bool AVG0_or_MAX1);
  
    /******************************************************************************/
    /*                         CONVERSION RGB <-> HSV                             */
    /******************************************************************************/    
    bool gfl_RGBtoHSV(GFLpixmap** dst, GFLpixmap* src);
    bool gfl_HSVtoRGB(GFLpixmap** dst, GFLpixmap* src);
  
    bool gfl_RGBtoVHS(GFLpixmap** dst, GFLpixmap* src, bool QUANT, uchar SCUT);
  
    /******************************************************************************/
    /*                         CONVERSION RGB <-> TSL                             */
    /******************************************************************************/
    bool gfl_RGBtoTSL(GFLpixmap** dst, GFLpixmap* src);
  
    /******************************************************************************/
    /*                         CONVERSION RGB -> YIQ                              */
    /******************************************************************************/
    bool gfl_RGBtoYIQ_dbl(GFLpixmap** dst, GFLpixmap* src);
    bool gfl_YIQtoRGB_dbl(GFLpixmap** dst, GFLpixmap* src);
  
    bool gfl_RGBtoYIQ_int(GFLpixmap** dst, GFLpixmap* src);
    bool gfl_YIQtoRGB_int(GFLpixmap** dst, GFLpixmap* src);
  
    /******************************************************************************/
    /*       CONVERSION RGB -> YCbCr  -- Specifications du format TIFF-6.0        */
    /******************************************************************************/
    /*************/
    /* NORME 601 */
    /*************/
    #define  R601 0.299
    #define  G601 0.587
    #define  B601 0.114
  
    bool gfl_RGBtoYCbCr_601(GFLpixmap** dst, GFLpixmap* src);
    bool gfl_YCbCrtoRGB_601(GFLpixmap** dst, GFLpixmap* src);
  
    /*************/
    /* NORME 709 */
    /*************/
    #define  R709 0.2125
    #define  G709 0.0721
    #define  B709 0.7154
  
    bool gfl_RGBtoYCbCr_709(GFLpixmap** dst, GFLpixmap* src);
    bool gfl_YCbCrtoRGB_709(GFLpixmap** dst, GFLpixmap* src);
  
  #endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/

