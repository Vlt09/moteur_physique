/**@file    gfl_control.h
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

#ifndef _GFL_CONTOL_H
  #define _GFL_CONTOL_H
/**@endcond                   */

  #ifdef _GFL2D_
    #include <gfl_geom2d.h>
  #endif
  
  #ifdef _GFL3D_
    #include <gfl_geom3d.h>
  #endif

  /*========================================================================*/
  /*                               SOURIS                                   */
  /*========================================================================*/

  // les bouttons (molette active == MIDDLE)
  #define MOUSE_LEFT        GLUT_LEFT_BUTTON
  #define MOUSE_RIGHT       GLUT_RIGHT_BUTTON  // vérouillé : menu déroulant
  #define MOUSE_MIDDLE      GLUT_MIDDLE_BUTTON // vérouillé : panscan
  // les états
  #define MOUSE_PRESSED     GLUT_UP
  #define MOUSE_RELEASED    GLUT_DOWN
  // directions scroll molette
  #define WHEEL_SCROLL_UP   3   // vérouillé  : zoom +
  #define WHEEL_SCROLL_DOWN 4   // vérouillé  : zoom -

  /**@brief This function binds an action to the movements fo the mouse.
   * @param action: a function pointer expecting a GFLpoint as parameter containing the current mouse location.*/
  void      gfl_SetMouseMoveAction(void (*action)(GFLpoint _CLIC_POSITION_));

  /**@brief returns the current mouse location.
   * @warning There is actualy a small margin of error between exact mouse location and returned value.
   * Thus it is ill advised to atttempt pixel-exact click locations.                  */
  GFLpoint*  gfl_GetMousePosition(void);

  /**@brief  This function returns a boolean stating if the mouse is within the drawing window.
   * @param  margin: the margin of error of the calculation (as pixel precision
   *         gets harder for humans with higher screen resolutions)
   * @retval true - the mouse is in the window according to the margin
   * @retval false - the mouse isn't in the window                  */
  bool      gfl_MouseInWindow(double margin);

  /**@brief This function returns the coordinate at witch the last clic happened.
   * @return the address of the GFLpoint structure holding the coordinates
   * @retval NULL - no clic happened since last call or software start.                  */
  GFLpoint* gfl_GetClic(void);

  GFLpoint *gfl_GetClicPixPos(int *x, int *y);
  /*========================================================================*/
  /*                             KEYS (action)                              */
  /*========================================================================*/
  #define  KEY_ESCAPE 27        // vérouillée : sortie du programme
  #define  KEY_SPACE  32        // vérouillée : lance/arrête l'animation
  #define  KEY_ENTER  13
  #define  KEY_TAB     9
  #define  KEY_BACK    8

  /**@brief This function binds a key to a spécific action and stores the printable inforamtion about the action.
   * @warning as stated in GFLkey these key are not available to binding
   * @param key: a single character that should be obtainable with a keyboard only.
   * @param action: an function pointer to the deired behavious should the key be pressed.
   * @param info: a string detailing the action briefly (less than 127 character available.)
   * @return true if the key and action could be bound, false otherwise.          */
  bool      gfl_SetKeyAction(char key, void (*action)(void), const char *info);


  // touches "spéciales"
  /*---- pavé fléché -----*/
  #define  SKEY_LEFT   GLUT_ARROW_LEFT
  #define  SKEY_RIGHT  GLUT_ARROW_RIGHT
  #define  SKEY_UP     GLUT_ARROW_UP
  #define  SKEY_DOWN   GLUT_ARROW_DOWN
  #define  SKEY_PGUP   GLUT_ARROW_PAGE_UP
  #define  SKEY_PGDOWN GLUT_ARROW_PAGE_DOWN

  /*---- touche de fonction F1 à F12 selon les codes GLUT -----*/
  #define  SKEY_F1     GLUT_KEY_F1 // normalement c'est le code 112, mais en GLUT c'est 1
  #define  SKEY_F2     GLUT_KEY_F2 // normalement c'est le code 113, mais en GLUT c'est 2
  #define  SKEY_F3     GLUT_KEY_F3 // .....
  #define  SKEY_F4     GLUT_KEY_F4
  #define  SKEY_F5     GLUT_KEY_F5
  #define  SKEY_F6     GLUT_KEY_F6
  #define  SKEY_F7     GLUT_KEY_F7
  #define  SKEY_F8     GLUT_KEY_F8
  #define  SKEY_F9     GLUT_KEY_F9
  #define  SKEY_F10    GLUT_KEY_F10
  #define  SKEY_F11    GLUT_KEY_F11 // verouillée : fullscreen
  #define  SKEY_F12    GLUT_KEY_F12 // verouillée : info

  bool      gfl_SetSpecialKeyAction(char key, void (*action)(void), const char *info);

  #ifdef _GFL2D_
    /*========================================================================*
     *                            POINTS DE CONTROLE                          *
     *  se 'branche' sur un GFLpoint defini dans les données globales         *
     *  et lui associe un rayon d'action (en pixel).                          *
     *  >>> un clic dans ce rayon permet de 'capturer' le point pour le       *
     *     sélectionner, le déplacer (boutton enfoncer) et le replacer        *
     *     (boutton relâché) ailleurs.                                        *
     *========================================================================*/
  
    /**@brief This fucntion adds a mouse-controlable point to their table via its address and sets the click detection range.
     * @param P  : adress of the globaly defined point to add to the table.
     * @param ray: an integer (number of pixel) controling how far from the actual point the click on said point will be detected.
     * @return boolean value
     * @retval true  - the point was succesfully created & added to the control point storage
     * @retval false - the point wasn't created                                    */
    bool      gfl_SetControlPoint(GFLpoint* P, int  ray);
  
    /**@brief This function returns the address of the point currently detected as clicked.
     * @return the address of the clicked point
     * @retval NULL - if the point couldn't be created.                            */
    GFLpoint* gfl_GetControlPoint(void);
  
    /**@brief This function inform wether a clickable point exists or not.
     * @retval true  - a point is clicked
     * @retval false - no point clicked                                            */
    bool      gfl_GetCpt();
  
    /**@brief The function take the currently clicked point and re-set its location to the previous one.
     * @return nothing.                                                            */
    void      gfl_ReplaceControlPoint(void);
  
    /**@brief This function remove the point stored at the given address form the table of controlable points.
     * @param pt: the address of the point to be removed
     * @return nothing.                                                            */
    void      gfl_CleanControlPoint(GFLpoint* pt);
  
    /**@brief Remove all points from the table of controlable points.
     * @return nothing.                                                            */
    void      gfl_CleanAllControlPoint(void);
  #endif

  /*========================================================================*
   *                                  SCROLL                                *
   *  les scrollbars, associés à un paramètre réel ou entier                *
   *  liés à l'adresse d'une variable (double/int) globale du programme     *
   *========================================================================*/
  /*                          1°) SCROLL HORIZONTAL                         */
  /**@brief Creates a horizontal scrollbar bound to a floating parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success
   * @retval -1 on a failure.                  */
  int gfl_CreateScrollh_d(const char *nom, double* prm, double min, double max, const char *info);

  /**@brief Creates a horizontal scrollbar bound to a integer parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return <int> the id of the scroll on a success
   * @retval -1 - On a failure.                  */
  int gfl_CreateScrollh_i(const char *nom, int*    prm, int    min, int    max, const char *info);

  /**@deprecated Use CreateScrollh_i or CreateScrollh_d instead.
   * @brief Creates an unbound horizontal scroll.
   * @param name the scroll bar name to display
   * @param init the initial value.
   * @param info  the information about the scroll bar.
   * @return int the scroll's id                             */
  int gfl_CreateAnonymousScrollh(const char *name, double init, const char *info);

  /*                          1°) SCROLL VERTICAL                           */
  /**@brief Creates a vertival scrollbar bound to a floating parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: Optional text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success, -1 otherwise  */
  int gfl_CreateScrollv_d(const char *nom, double* prm, double min, double max, const char *info);

  /**@brief Creates a  vertical scrollbar bound to a integer parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success, -1 otherwise */
  int gfl_CreateScrollv_i(const char *nom, int*    prm, int    min, int    max, const char *info);

  /**@deprecated Use CreateScrollv_i or CreateScrollv_d instead.
   * @brief DEPRECATED Creates an unbound vertical scroll.
   * @param name the scroll bar name to display
   * @param init the initial value.
   * @param info  the information about the scroll bar.
   * @return int the scroll's id.                            */
  int gfl_CreateAnonymousScrollv(const char *name, double init, const char *info);

  /**@deprecated Use of Annonymous scroll cursor is deprecated.
   * @brief This function returns cursor value. Mainly intended for anonymous cursors.
   * @param id: the id of the target cursor.
   * @return a double representinf the current value of the cursor.
   * @see gfl_CreateAnonymousScrollv
   * @see gfl_CreateAnonymousScrollh                         */
  double gfl_GetScrollCursor(int id);

  /**@brief This function sets the background color of the <id> cursor
   * @param id : the id of the cursor
   * @param col: the color we wish to aply.                  */
  void   gfl_SetScrollColor(int id, GFLcolor col);

  /**@brief Adujst ScrollBar width.
   * @param width : the desired width / height. Must be within range [4; 16] (default :9) */
  void   gfl_SetScrollWidth(int width) ;


  /*========================================================================*/
  /*                                BOUTTON                                 */
  /*========================================================================*/
  /**@deprecated use GFLExclBut instead.
   * @brief an exclusive buttton.
   * All button created with this structure are mutualy exclusive. This stays for retro-compatibility purposes. */

   /**@brief This function returns the number of the current button.
    * @retval -1 - current button not set.
    * @retval [0,inf[ - current button ID.                  */
  int gfl_GetButton(void);

  /**@deprecated use gfl_CreateExclusiveButton instead.
   * @brief  <b> DEPRECATED </b> Creates old school buttons
   * @param  name The name of the button
   * @param  info some info to display in the help window
   * @retval -1 - current button not set.
   * @retval [0,inf[ - current button ID.                  */
  int gfl_CreateButton(const char *name, const char *info);

  /*                               SWITCH                                   */
  /*========================================================================*/
  /**@brief This function create a switch button whose attached value and behaviour should be independant from other buttons.
   * @param txt: a pointer to the string contraining the button label.
   * @param flag: a pointer to the boolean controled by this button.
   * @param info: a pointer to the straing holding the briefc explanation of the purpose of the button.
   * @retval true - the button was succesfully creataed and added to the corresponding talbe.
   * @retval false - something failed.                     */
  bool gfl_CreateSwitch(const char *txt, bool *flag, const char *info);

  /**@brief This function returns the number of the current on/off button.
   * @retval -1 - current button not set.
   * @retval [0-inf] - current button ID.                  */
  int  gfl_GetSwitch(void);

  /*========================================================================*/
  /*                             POPUP (action)                             */
  /*========================================================================*/

  /**@brief This creates a new button bound to an action.
   * @param name: an address on the text displayed on the button
   * @param action: a function to be triggered when clicking the buttion, this function shouldn't expect nor retunr anything.
   * @param info: the address of the text explaining briefly the button purpose (less than 127 char).
   * @retval true on a succesfull creation and storage
   * @retval false if either failed.                  */
  bool gfl_CreatePopUp(const char *name, void (*action)(void), const char *info);

  /**@brief This function return the id of the currently selected pop-up button.
   * @retval -1 - no pop-up selected.
   * @retval [0-inf[ - the id of the pop-up.                  */
  int  gfl_GetPopUp(void);

 #endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
