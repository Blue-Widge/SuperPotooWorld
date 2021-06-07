# Documentation de PE_Collider

### Création / suppression d'un collider

- PE_Body_createCollider()
- PE_Body_removeCollider()

### Définition de la forme du collider

- PE_Shape_setAsBox()
- PE_Shape_setAsTriangle()

### Informations sur le collider et modification

- PE_Collider_getFilter()
- PE_Collider_setTrigger()
- PE_Collider_isTrigger()
- PE_Collider_getBody()
- PE_Collider_getShape()
- PE_Collider_getAABB()
- PE_Collider_setUserData()
- PE_Collider_getUserData()

### Fonctions de rappel (callback) du collider

- PE_Collider_setOnCollisionEnter()
- PE_Collider_setOnCollisionStay()
- PE_Collider_setOnCollisionExit()
- PE_Collider_setOnTriggerEnter()
- PE_Collider_setOnTriggerStay()
- PE_Collider_setOnTriggerExit()

- PE_Collider_getOnCollisionEnter()
- PE_Collider_getOnCollisionStay()
- PE_Collider_getOnCollisionExit()
- PE_Collider_getOnTriggerEnter()
- PE_Collider_getOnTriggerStay()
- PE_Collider_getOnTriggerExit()

### Enumération des collisions impliquant le collider

- PE_Collider_getNbCollisions()
- PE_Collider_getCollisionIterator()
- PE_CollisionIterator_moveNext()
- PE_CollisionIterator_current()