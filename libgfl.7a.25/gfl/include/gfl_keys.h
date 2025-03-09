/**
 * @file    gfl_keys.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Feb.2023 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _GFL_KEYS_H
  #define _GFL_KEYS_H
/**@endcond                   */

  #include <gfl_types.h>

  #define	BACKSPACE	    8
  #define	TAB	          9
  #define	ENTER	       13
  #define	SHIFT	       16
  #define	CTRL	       17
  #define	ALT     	   18
  #define	PAUSE	       19
  #define	CAPS_LOCK	   20
  #define	ESC	         27
  #define	SPACEBAR	   32
  #define	PAGE_UP	     33
  #define	PAGE_DOWN	   34
  #define	END	         35
  #define	HOME	       36
  #define	ARROW_LEFT   37
  #define	ARROW_UP	   38
  #define	ARROW_RIGHT	 39
  #define	ARROW_DOWN 	 40
  #define	PRINT_SCRN   44
  #define	INSERT	     45
  #define	DELETE	     46
  #define	NUMPAD_0	   96
  #define	NUMPAD_1	   97
  #define	NUMPAD_2	   98
  #define	NUMPAD_3	   99
  #define	NUMPAD_4	  100
  #define	NUMPAD_5	  101
  #define	NUMPAD_6	  102
  #define	NUMPAD_7	  103
  #define	NUMPAD_8	  104
  #define	NUMPAD_9	  105
  #define	NUMPAD_*	  106
  #define	NUMPAD_+	  107
  #define	NUMPAD_-	  109
  #define	NUMPAD_.	  110
  #define	NUMPAD_/	  111
  #define	F1	        112
  #define	F2	        113
  #define	F3 	        114
  #define	F4	        115
  #define	F5	        116
  #define	F6	        117
  #define	F7	        118
  #define	F8	        119
  #define	F9	        120
  #define	F10	        121
  #define	F11	        122
  #define	F12	        123
  #define	NUM_LOCK	  144
  #define	SCROLL_LOCK	145

  /*-----------------------------KEYS (action)------------------------------*/
  /**@brief   associates an action to a key (ASCII).
   * @warning some keys are pre-defined and thus should not be used to avoid conflict with core functionalities.
   *          |SPACE|ESC|F11|F12|q|Q|Ctrl+f|Ctrl+r|Ctrl+w|+|-|=|?|
  !*/
  typedef struct
  {
    void (*action)(void);  /**@brief   action to be done at keypressed                            */
    char key;              /**@brief   key value (ASCII)
                            * @warning see struct GFLkey documentation for pre-defined behaviour  */
    char  info[_GFL_INFOSIZE_+1];/**@brief text for the info pannel (key '?')                           */
  } GFLkey;

  /**@brief This function binds a key to a spécific action and stores the printable inforamtion about the action.
   * @warning as stated in GFLkey these key are not available to binding
   * @param key: a single character that should be obtainable with a keyboard only.
   * @param action: an function pointer to the deired behavious should the key be pressed.
   * @param info: a string detailing the action briefly (less than 127 character available.)
   * @return true if the key and action could be bound, false otherwise.          */
  bool      gfl_SetKeyAction(char key, void (*action)(void), const char *info);

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
