# Documentation de l'animateur

### Animateur

Création et suppression.

- RE_Animator_new()
- RE_Animator_free()

Mise à jour de l'animateur et rendu.

- RE_Animator_update()
- RE_Animator_render()
- RE_Animator_renderF()

Modification et informations sur le rendu.

- RE_Animator_setAlpha()
- RE_Animator_setTransform()
- RE_Animator_getTransform()

### Texture animée

Création, suppression et récupération.

- RE_Animator_createTextureAnim()
- RE_Animator_destroyTextureAnim()
- RE_Animator_getTextureAnim()

Lancement ou arrêt d'une animation.

- RE_Animator_playTextureAnim()
- RE_Animator_resumeTextureAnim()
- RE_Animator_stopTextureAnim()

Personnalisation d'une animation.

- RE_TextureAnim_setCycleTime()
- RE_TextureAnim_setNbCyles()
- RE_TextureAnim_setDelay()
- RE_TextureAnim_setSpeed()
- RE_TextureAnim_setFlags()
- RE_TextureAnim_addFlags()
- RE_TextureAnim_removeFlags()

### Paramètre animé

Création, suppression et récupération.

- RE_Animator_createParamAnim()
- RE_Animator_destroyParamAnim()
- RE_Animator_getParamAnim()

Lancement ou arrêt d'une animation.

- RE_Animator_playParamAnim()
- RE_Animator_resumeParamAnim()
- RE_Animator_stopParamAnim()
- RE_Animator_stopParamAnims()

Définition du paramètre animé.

- RE_ParamAnim_setShift()
- RE_ParamAnim_setAlpha()
- RE_ParamAnim_setAngle()

Personnalisation d'une animation.

- RE_ParamAnim_setCycleTime()
- RE_ParamAnim_setNbCyles()
- RE_ParamAnim_setDelay()
- RE_ParamAnim_setEasing()
- RE_ParamAnim_setPhase()
- RE_ParamAnim_setSpeed()
- RE_ParamAnim_setFlags()
- RE_ParamAnim_addFlags()
- RE_ParamAnim_removeFlags()

### Courbes d'accélération par défaut (Easing functions)

- RE_EasingFct_linear()
- RE_EasingFct_in()
- RE_EasingFct_out()
- RE_EasingFct_inOut()
- RE_EasingFct_cos()
