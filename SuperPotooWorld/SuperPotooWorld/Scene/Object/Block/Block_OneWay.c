#include "Block.h"
#include "Block_OneWay.h"

#include "../../Scene.h"

#include <stdio.h>
#include <stdlib.h>

int OneWay_onStart(Block *block)
{
    // TODO
    // Inspirez-vous de la fonction Ground_onStart()

    printf("OneWay_onStart : Initialisation d'un bloc OneWay\n");

    return EXIT_SUCCESS;
}

// Les blocs OneWay n'ont pas de fonction render() puisqu'ils sont déjà dessinés dans la Tilemap.