/**@file    gfl.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifdef _GFL3D_
  #ifndef _GFL_QUATERNION_3D_H
    #define _GFL_QUATERNION_3D_H
  /**@endcond                   */
  
    #include <gfl_types.h>
  
    /*! type quaternion (reel/vecteur)!*/
    typedef struct
    {
      double    r;
      GFLvector v;
    } GFLquat;
  
    void    gfl_QuatIdentity(GFLquat A);
    GFLquat gfl_QuatSet(double r, GFLvector v);
    GFLquat gfl_QuatSet4(double r, double x, double y, double z);
    GFLquat gfl_QuatAdd(GFLquat A, GFLquat B);
    GFLquat gfl_QuatProd(GFLquat A, GFLquat B);
    GFLquat gfl_QuatConj(GFLquat A);
    double  gfl_QuatSqrNorm(GFLquat A);
    double  gfl_QuatNorm(GFLquat A);
    GFLquat gfl_QuatNormalize(GFLquat A);
    GFLquat gfl_QuatScalMap(GFLquat A, double a);
    GFLquat gfl_QuatInv(GFLquat A);
    void    gfl_QuatToHmat(GFLquat A, GFLhmat M);
    void    gfl_QuatPrint(GFLquat A);
    void    gfl_QuatRot(GFLquat Qrot, GFLcoord src, GFLcoord dest);
    void    gfl_AxeRadRot(GFLvector v, double rad, GFLcoord src, GFLcoord dest);
    GFLquat gfl_QuatAlign(GFLvector v, GFLvector const cible);
  #endif
#endif

#ifdef __cplusplus
  }
#endif
/*============================================================================!*/
