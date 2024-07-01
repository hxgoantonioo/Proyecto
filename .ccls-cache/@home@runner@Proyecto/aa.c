void rellenarMapaGeneros(Map *map_bygenre, Map *map_byid) {

    MapPair* iter = map_first(map_byid); //Agarramos el primer par del mapa con todas las películas

    while (iter != NULL) { //Mientras el par no sea nulo

        Film *peli = iter->value; //Tomaremos la película guardada en el valor del par actual

        char *node = list_first(peli->genres); //Ahora, tomaré el primer género de la película actual

        while (node != NULL) { //Mientras el género actual no sea nulo

            MapPair *par = map_search(map_bygenre, node); //Busco en el mapa si hay algún dato con clave del género actual

            if (par == NULL) { //Si NO existe

                List* lista = list_create(); //Creamos la lista a guardar en el mapa

                map_insert(map_bygenre, strdup(node), lista); //Insertamos la lista en el mapa

                list_pushBack(lista, peli); //Insertamos la película a la lista

            }

            else { //Si existía un par para la clave del género actual

                List* lista = (List*) par->value; //Obtenemos el dato del par, el cual será la lista con las películas del género

                if (lista == NULL) { //Si por cualquier casualidad, la lista es nula

                    lista = list_create(); //Creamos la lista

                    par->value = lista; //Y la asignamos al valor del par con clave del género actual

                }

                list_pushBack(lista, peli); //Insertamos la película a la lista de películas del género actual

            }

            node = list_next(peli->genres); //Avanzamos al siguiente género

        }

        iter = map_next(map_byid); //Avanzamos a la siguiente película

    }

}