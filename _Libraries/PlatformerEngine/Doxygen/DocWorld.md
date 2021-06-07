# Documentation de PE_World

### Création / suppression du monde

- PE_World_new()
- PE_World_free()

### Création / suppression de corps dans un monde

- PE_World_createBody()
- PE_World_removeBody()

### Informations sur le monde et modification

- PE_World_setGravity()
- PE_World_getGravity()
- PE_World_fixedUpdate()

### Fonctions de rappel (callback) globales

- PE_World_setOnCollisionEnter()
- PE_World_setOnCollisionStay()
- PE_World_setOnCollisionExit()

### Enumération des corps présents dans le monde

- PE_World_getNbBodies()
- PE_World_getBodyIterator()
- PE_BodyIterator_moveNext()
- PE_BodyIterator_current()