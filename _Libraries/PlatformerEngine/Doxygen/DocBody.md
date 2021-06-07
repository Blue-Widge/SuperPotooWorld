# Documentation de PE_Body

### Création / suppression d'un corps

- PE_World_createBody()
- PE_World_removeBody()

### Création / suppression d'un collider

- PE_Body_createCollider()
- PE_Body_removeCollider()

### Informations sur le corps et modification

- PE_Body_wakeUp()
- PE_Body_sleep()
- PE_Body_getType()
- PE_Body_setPosition()
- PE_Body_getPosition()
- PE_Body_getInterpolation()
- PE_Body_setVelocity()
- PE_Body_getVelocity()
- PE_Body_setCollisionResponse()
- PE_Body_applyForce()
- PE_Body_applyImpulse()
- PE_Body_clearForces()
- PE_Body_setXDamping()
- PE_Body_getXDamping()
- PE_Body_setYDamping()
- PE_Body_getYDamping()
- PE_Body_setGravityScale()
- PE_Body_getGravityScale()
- PE_Body_setUserData()
- PE_Body_getUserData()
- PE_Body_getWorld()

### Enumération des colliders affectés à un corps

- PE_Body_getNbColliders()
- PE_Body_getColliderIterator()
- PE_ColliderIterator_moveNext()
- PE_ColliderIterator_current()