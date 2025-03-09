/**===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie GFL - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================**/

/*====================================================================*/
/*=                               LA CAMERA                          =*/
/*====================================================================*/
static GFLpoint  cam_default_pos={5.,5.,5.};
static GFLpoint  cam_default_tar={0.,0.,0.};

static GLdouble  theta_cam_init=+0.25*PI,
                 phi_cam_init  =+0.25*PI,
                 dist_cam_init = 5.;

static GFLcamera _main_cam_;

extern GFLcamera* gfl_GetCamera(void) { return &_main_cam_; }
extern double     gfl_GetCameraDist(void) { return _main_cam_.dist; }

extern void gfl_SetCameraTracking(GFLpoint *position, GFLpoint *target)
{
  if (position) _main_cam_.pos=position;
  if (target  ) _main_cam_.tar=target;
}

extern void gfl_SetCameraSpheric(double theta, double phi, double dist)
{
  /* matrices de la caméra      */
  _main_cam_.Md = gfl_Identity();
  _main_cam_.Mi = gfl_Identity();
  _main_cam_.Md = gfl_Mat_x_Mat(gfl_RotationZ(PI/2),_main_cam_.Md);
  _main_cam_.Mi = gfl_Mat_x_Mat(_main_cam_.Mi,gfl_RotationZ(-PI/2));
  _main_cam_.Md = gfl_Mat_x_Mat(gfl_RotationY(phi  ),_main_cam_.Md);
  _main_cam_.Mi = gfl_Mat_x_Mat(_main_cam_.Mi,gfl_RotationY(-phi  ));
  _main_cam_.Md = gfl_Mat_x_Mat(gfl_RotationZ(theta),_main_cam_.Md);
  _main_cam_.Mi = gfl_Mat_x_Mat(_main_cam_.Mi,gfl_RotationZ(-theta));
  _main_cam_.Md = gfl_Mat_x_Mat(gfl_Translation3d(0.,0.,dist),_main_cam_.Md);
  _main_cam_.Mi = gfl_Mat_x_Mat(_main_cam_.Mi,gfl_Translation3d(0.,0.,-dist));
  /* paramètres courants        */
  _main_cam_.theta= theta;
  _main_cam_.phi  = phi;
  _main_cam_.dist = dist;
  /* mise à jour de la position */
  _main_cam_.pos->x = _main_cam_.tar->x + dist*cos(theta)*cos(phi);
  _main_cam_.pos->y = _main_cam_.tar->y + dist*sin(theta)*cos(phi);
  _main_cam_.pos->z = _main_cam_.tar->z + dist*sin(phi);
  _main_cam_.upv.z  = cos(phi)>0.?1.0:-1.0;
}

extern void gfl_SetCameraCartesian(GFLpoint position, GFLpoint target)
{
 *_main_cam_.pos   = position;
 *_main_cam_.tar   = target;
  _main_cam_.dist  = gfl_Norm(position);
  _main_cam_.phi   = (GFLISZERO(_main_cam_.dist  ) ? 0.     : asin(position.z/_main_cam_.dist   ));
  _main_cam_.theta = (GFLISZERO(position.y) ? 0.5*PI : atan(position.x/position.y));
  gfl_SetCameraSpheric(_main_cam_.theta, _main_cam_.phi, _main_cam_.dist);
}

static void gfl_PanScan(GFLcamera *cam, double dx, double dy)
{
  GFLhmat M=cam->Mi;
  M = gfl_Mat_x_Mat(gfl_Translation3d(-dx,0.,-dy),M);
  M = gfl_Mat_x_Mat(cam->Md,M);
  *cam->pos = gfl_Mat_x_Point(M,*cam->pos);
  *cam->tar = gfl_Mat_x_Point(M,*cam->tar);
}

extern void gfl_SetCameraTheta(double   theta) { _main_cam_.theta=theta; }
extern void gfl_SetCameraPhi  (double   phi  ) { _main_cam_.phi=phi;}
extern void gfl_SetCameraDist (double   dist ) { _main_cam_.dist=dist; }
extern void gfl_SetCameraTar  (GFLpoint tar  ) { *(_main_cam_.tar)=tar; }

/* parametres d'ouverture de la camera 3D OpenGl (cf. gluLookAt() */
extern void gfl_SetPerspective(float near, float far, float open)
{
  _main_cam_.near = near;
  _main_cam_.far  = far;
  _main_cam_.open = open;
}

/*====================================================================*/
/*=                               LA LUMIERE                         =*/
/*====================================================================*/
//static GFLpoint  spot_default_pos={5.,5.,5.};

static GLdouble  theta_spot_init=+0.25*PI,
                 phi_spot_init  =+0.25*PI,
                 dist_spot_init = 5.;

static GFLlight  _main_spot_;

extern GFLlight* gfl_GetLight(void) { return &_main_spot_; }

extern void gfl_SetLightSpheric(double theta, double phi, double dist)
{
  _main_spot_.theta=theta;
  _main_spot_.phi  =phi;
  _main_spot_.dist =dist;
  _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
  _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
  _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
}

extern void gfl_SetLightCartesian(GFLpoint position)
{
  _main_spot_.pos   = position;
  _main_spot_.dist  = gfl_Norm(_main_spot_.pos);
  _main_spot_.phi   = (GFLISZERO(_main_spot_.dist ) ? 0.     : asin(_main_spot_.pos.z/_main_spot_.dist   ));
  _main_spot_.theta = (GFLISZERO(_main_spot_.pos.y) ? 0.5*PI : atan(_main_spot_.pos.x/_main_spot_.pos.y ));
  gfl_SetLightSpheric(_main_spot_.theta, _main_spot_.phi, _main_spot_.dist);
}

extern void gfl_SetSpotSteps(GLfloat dx, GLfloat dy, GLfloat dz)
{
  if (dx!=0.) _main_spot_.dx=dx;
  if (dy!=0.) _main_spot_.dy=dy;
  if (dz!=0.) _main_spot_.dz=dz;
}

extern void gfl_SetLightAmbient  (float r, float g, float b){ _main_spot_.ambi=(GFLcolor){r,g,b,0.}; }
extern void gfl_SetLightDiffuse  (float r, float g, float b){ _main_spot_.diff=(GFLcolor){r,g,b,0.}; }
extern void gfl_SetLightSpecular (float r, float g, float b){ _main_spot_.spec=(GFLcolor){r,g,b,0.}; }
extern void gfl_SetLightPosition (float x, float y, float z){ _main_spot_.pos =(GFLpoint){x,y,z}   ; }
