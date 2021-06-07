# Documentation de PE_Collision et PE_Trigger


### Affectation d'une fonction de rappel à un collider

Fonctions de rappel pour les collisions :

- PE_Collider_setOnCollisionEnter()
- PE_Collider_setOnCollisionStay()
- PE_Collider_setOnCollisionExit()

Fonctions de rappel pour les détections :

- PE_Collider_setOnTriggerEnter()
- PE_Collider_setOnTriggerStay()
- PE_Collider_setOnTriggerExit()

### Récupération des informations lors d'une collision

- PE_Collision_isTouching()
- PE_Collision_getCollider()
- PE_Collision_getOtherCollider()
- PE_Collision_getBody()
- PE_Collision_getOtherBody()
- PE_Collision_getRelativePosition()
- PE_Collision_getManifold()

Exemple de définition d'une fonction de rappel pour une collision.

    void Callback_onCollisionStay(PE_Collision *collision)
    {
        PE_Body *thisBody = PE_Collision_getBody(collision);
        int relPos = PE_Collision_getRelativePosition(collision);

        PE_Vec2 velocity;
        PE_Body_getVelocity(thisBody, &velocity);

        switch (relPos)
        {
        case PE_ABOVE:
        case PE_BELOW:
            velocity.y = 0.f;
            break;

        case PE_RIGHT:
        case PE_LEFT:
            velocity.x = 0.f;
            break;
        default:
            break;
        }
        PE_Body_setCollisionResponse(thisBody, &velocity);
    }

### Récupération des informations lors d'une détection

- PE_Trigger_getCollider()
- PE_Trigger_getOtherCollider()
- PE_Trigger_getBody()
- PE_Trigger_getOtherBody()

Exemple de définition d'une fonction de rappel pour une détection.

    void Callback_onTriggerEnter(PE_Trigger *trigger)
    {
        PE_Body *thisBody = PE_Trigger_getBody(trigger);
        PE_Body *otherBody = PE_Trigger_getOtherBody(trigger);
        
        printf("Le corps %p detecte le corps %p\n", thisBody, otherBody);
    }
