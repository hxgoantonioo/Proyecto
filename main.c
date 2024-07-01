#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#define fileUser "usuarios.csv"

#define VERDE_T        "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"
#define CYAN    "\x1b[36m"

//Estructura de la AnkiCard
typedef struct{
  char ask[200];
  char answer[200];
  int diff;
  char category[50];
} ankicard;

//Estructura del usuario
typedef struct{
  List *cards;
  char username[20];
  char password[20];
} user;

//TDA Lista
typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
  Node *current;
} List;

//Funcion que imprime las opciones del menu
void logMenu(){
  limpiarPantalla();
  printf("\033[1;34m");
  printf("¡Bienvenido a Memo, el sistema de AnkiCard que te ayudará a estudiar!\n");
  printf("\033[0m");
  puts("= = = = = = = = = = = = = = = = = = = = = = = = = =");
  puts("Por favor, seleccione una opcion:");
  puts("1. Registrarse");
  puts("2. Iniciar sesion");
  puts("3. Continuar como invitado");
  puts("4. Salir del programa");
  puts("= = = = = = = = = = = = = = = = = = = = = = = = = =");
}

//Funcion que remueve la nueva linea que se genera en el csv
void removeNewline(char *str) {
  char *pos;
  if ((pos = strchr(str, '\n')) != NULL) {
      *pos = '\0';
  }
}

//Funcion que guarda el progreso del usuario en un archivo csv
void saveProgress(user *perfil) {
  //Se crea una lista para guardar los datos del usuario
  List *cards = perfil->cards;
  //Se indica el nombre del usuario para indentarlo en el nombre del archivo
  char fileName[50];
  sprintf(fileName, "progreso_%s.csv", perfil->username);
  //Se abre el archivo en modo escritura
  FILE *file = fopen(fileName, "w");
  if (file == NULL) {
      perror("Error opening file");
      return;
  }
  //Se ingresa los parametros del csv
  fprintf(file, "Question,Answer,Difficulty,Category\n");

  //Se recorre la lista de cards y se ingresa una por una en el csv
  for (ankicard *card = list_first(cards); card != NULL; card = list_next(cards)) {
      removeNewline(card->ask);
      removeNewline(card->answer);
      removeNewline(card->category);
      fprintf(file, "%s,%s,%d,%s\n", card->ask, card->answer, card->diff, card->category);
  }
  //Mensaje de confirmacion y cierre del archivo
  puts("Progreso guardado exitosamente.");
  fclose(file);
}

//Funcion que se encarga de ver si el archivo existe o no
bool fileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

//Funcion que carga las preguntas del usuario
void loadQuestions(user *perfil, char *filename)
{
  //Se abre el archivo en modo lectura
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    perror("Error opening file");
    return;
  }
  //Se crea un "header" para encargarse de no guardar la primera linea del csv
  char header[1024];
  fgets(header, sizeof(header), file);

  //Se crea un "line" que guardara la linea actual del csv en la lista
  char line[1024];
  while (fgets(line, sizeof(line), file))
    {
      char *token = strtok(line, ",");
      ankicard *card = (ankicard *)malloc(sizeof(ankicard));
      strcpy(card->ask, token);
      token = strtok(NULL, ",");
      strcpy(card->answer, token);
      token = strtok(NULL, ",");
      card->diff = atoi(token);
      token = strtok(NULL, ",");
      strcpy(card->category, token);
      list_pushBack(perfil->cards, card);
    }
  //Se cierra el archivo csv
  fclose(file);
}

//Funcion que crea un usuario
void logIn(user *perfil, int identify){
  limpiarPantalla();
  //Crear usuario y contraseña y se inicializan variables de tipo booleano para verificar el estado de cada uno de los parametros
  char registerUser[20];
  char registerPassword[20];
  bool stateUser = false;
  bool statePass = false;
  //Se inicia un bucle para ir verificando que los parametros ingresados fueron bien ingresados
  while (stateUser == false)
    {
      puts("Ingrese un nombre de usuario:");
      scanf("%s", registerUser);
      if (strlen(registerUser) == 0)
      {
        puts("Oops, mensaje de error:");
        puts("El nombre de usuario no puede estar vacio.");
        puts("");
      }
      else if (strlen(registerUser) > 20)
      {
        puts("Oops, mensaje de error:");
        puts("El nombre de usuario no puede contener mas de 20 caracteres.");
        puts("");
      }
      else
      {
        stateUser = true;
      }
    }
  while (statePass == false)
    {
      puts("Ingrese una contraseña (entre 8-20 caracteres):");
      scanf("%s", registerPassword);
      if (strlen(registerPassword) == 0)
        {
          puts("Oops, mensaje de error:");
          puts("La contraseña no puede estar vacia.");
          puts("");
        }
        else if (strlen(registerPassword) < 8)
        {
          puts("Oops, mensaje de error:");
          puts("La contraseña no puede contener menos de 8 caracteres.");
          puts("");
        }
        else if (strlen(registerPassword) > 20)
        {
          puts("Oops, mensaje de error:");
          puts("La contraseña no puede contener mas de 20 caracteres.");
          puts("");
        }
        else
        {
          statePass = true;
        }
    }
  //Se abre el archivo csv y se verifica si existe el archivo, para imprimir la linea de los parametros
  FILE *archivo = fopen(fileUser, "r");
  if (archivo == NULL)
  {
    archivo = fopen(fileUser, "w");
    fprintf(archivo, "Username,Password\n");
    fprintf(archivo, "%s,%s\n", registerUser, registerPassword);
    fclose(archivo);
    //Se encarga de ver si fue ingresado como invitado (identify distinto de 1) o no
    if (identify != 1)
    {
      strcpy(perfil->username, registerUser);
      strcpy(perfil->password, registerPassword);
      saveProgress(perfil);
      puts("");
      puts("Usuario registrado exitosamente.");
    }
    else
    {
      puts("");
      puts("Usuario creado con exito! Ya puedes iniciar sesion con Memo.");
    }
  }
  //Se encarga de ver si el usuario ya existe en el archivo csv y si existe, mensaje de error
  else
  {
    char line[100];
    bool userExists = false;
    while (fgets(line, sizeof(line), archivo))
      {
        char existingUser[20];
        sscanf(line, "%[^,],", existingUser);
        if (strcmp(existingUser, registerUser) == 0)
        {
          userExists = true;
          break;
        }
      }
    fclose(archivo);
    if (userExists)
    {
      puts("Oops, mensaje de error:");
      puts("El nombre de usuario ya existe, por favor intenta con uno distinto.");
      puts("");
    }
    else
    {
      archivo = fopen(fileUser, "a");
      fprintf(archivo, "%s,%s\n", registerUser, registerPassword);
      fclose(archivo);
      if (identify != 1)
        {
          strcpy(perfil->username, registerUser);
          strcpy(perfil->password, registerPassword);
          saveProgress(perfil);
        }
      else
      {
        puts("");
        puts("Usuario creado con exito! Ya puedes iniciar sesion con Memo.");
      }
    }
  }
}

//Funcion que se encarga de verificar si el usuario existe o no
bool verifyCSV(const char *usernameSignIn, const char *passwordSignIn) {
    FILE *archivo = fopen(fileUser, "r");
    if (archivo == NULL) {
        return false; // No se pudo abrir el archivo
    }

    char line[100];
    while (fgets(line, sizeof(line), archivo)) 
    {
        char existingUser[20], existingPassword[20];
        sscanf(line, "%[^,],%s", existingUser, existingPassword);
        if (strcmp(existingUser, usernameSignIn) == 0) 
        {
            if (strcmp(existingPassword, passwordSignIn) == 0)
            {
                fclose(archivo);
                return true;
            }
            else
            {
              fclose(archivo);
              puts("Oops, mensaje de error:");
              puts("Contraseña incorrecta.");
              puts("");
              return false;
            }
        }
    }
  puts("Oops, mensaje de error:");
  puts("El nombre de usuario no existe, por favor registrate o intenta de nuevo.");
  puts("");
  fclose(archivo);
  return false;
}

//Funcion que se encarga de iniciar sesion
bool signIn(user *usuario){
  limpiarPantalla();
  //Se crean variables de tipo booleano para verificar el estado de cada uno de los parametros y se abre el archivo csv en modo lectura
  FILE *archivo = fopen(fileUser, "r");
  if (archivo == NULL)
  {
    puts("Oops, mensaje de error:");
    puts("No existen registros de cuenta en el terminal, registrese con una cuenta.");
    puts("");
    presioneTeclaParaContinuar(0);
    return false;
  }
  fclose(archivo);
  char usernameSignIn[20];
  char passwordSignIn[20];
  bool stateUser = false;
  bool statePass = false;
  bool repeat;

  //Se inicia un bucle para ir verificando que los parametros ingresados fueron bien ingresados
  while (stateUser == false)
    {
      repeat = false;
      puts("Ingrese su nombre de usuario:");
      scanf("%s", usernameSignIn);
      if (strlen(usernameSignIn) == 0)
      {
        puts("Oops, mensaje de error:");
        puts("El nombre de usuario no puede estar vacio.");
        puts("");
      }
      else if (strlen(usernameSignIn) > 20)
      {
        puts("Oops, mensaje de error:");
        puts("El nombre de usuario no puede contener mas de 20 caracteres.");
        puts("");
      }
      else
      {
        while (statePass == false)
          {
            if (repeat == true)
            {
              break;
            }
            puts("Ingrese su contraseña:");
            scanf("%s", passwordSignIn);
            if (strlen(passwordSignIn) == 0)
            {
              puts("Oops, mensaje de error:");
              puts("La contraseña no puede estar vacia.");
              puts("");
            }
            else if (strlen(passwordSignIn) < 8)
            {
              puts("Oops, mensaje de error:");
              puts("La contraseña no puede contener menos de 8 caracteres.");
              puts("");
            }
            else if (strlen(passwordSignIn) > 20)
            {
              puts("Oops, mensaje de error:");
              puts("La contraseña no puede contener mas de 20 caracteres.");
              puts("");
            }
            else
            {
              bool existsUser = verifyCSV(usernameSignIn, passwordSignIn);
              if (!existsUser)
              {
                int existsOption;
                puts("1. Volver al menu principal");
                puts("2. Intentar de nuevo");
                scanf("%d", &existsOption);
                if (existsOption == 1)
                {
                  return false;
                }
                repeat = true;
              }
              else
              {
                strcpy(usuario->username, usernameSignIn);
                strcpy(usuario->password, passwordSignIn);
                statePass = true;
                stateUser = true;
              }
            }
          }
      }
    }
  return true;
}

//Funcion que se encarga de preguntar al usuario de su decision de ingresar como invitado
bool askGuest(user *usuario){
  limpiarPantalla();
  int decision;
  printf("\033[0;31m");
  puts("Ten en cuenta que de si ingresas como invitado podras crear una cuenta de todas formas para guardar tu progreso");
  puts("Si al final de la sesion NO decides crear una cuenta, se perdera TODO el progreso que haya realizado en Memo");
  puts("Asi que, eligue bien tu decision :)");
  printf("\033[0m");
  puts("");
  puts("1. Deseo crearme una cuenta");
  puts("2. Seguire como invitado");
  printf("Opcion: ");
  scanf("%d", &decision);
  if (decision == 1)
  {
    return false;
  }
  char guestUser[20] = "Invitado";
  char guestPassword[20] = "12345678";
  strcpy(usuario->username, guestUser);
  strcpy(usuario->password, guestPassword);
  return true;
}

int lower_than(void *key1, void *key2) {
    int *d1 = (int *)key1;
    int *d2 = (int *)key2;
    return *d1 < *d2;
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

//Funcion que se encarga de agreagar una pregunta a la lista de preguntas
bool addCard(user *perfil, ankicard *newcard){
  if (perfil->cards == NULL){
    perfil->cards = list_create();
  }
  list_pushBack(perfil->cards, newcard);
  return true;
}
//Funcion de crear una carta
void createCard(user *perfil){
  //Se crea una variable de tipo ankicard para guardar la pregunta y un buffer que guardara todo de tipo char
  char buffer[300];
  ankicard *card = (ankicard *)malloc(sizeof(ankicard));
  
  puts("Ingrese la pregunta: ");
  fgets(buffer, sizeof(buffer), stdin); //Lee la pregunta
  buffer[strcspn(buffer, "\n")] = 0;
  puts(" ");
  strcpy(card->ask, buffer);

  printf("Ingrese la respuesta: \n");
  fgets(buffer, sizeof(buffer), stdin); //Lee la respuesta
  buffer[strcspn(buffer, "\n")] = 0;
  puts(" ");
  strcpy(card->answer, buffer);

  printf("Ingrese la dificultad (numero): \n");
  scanf("%d", &card->diff); //Lee la dificultad
  puts(" ");
  getchar();

  printf("Ingresar categoria: \n");
  fgets(buffer, sizeof(buffer), stdin); //Leer la categoria
  buffer[strcspn(buffer, "\n")] = 0;
  puts(" ");
  strcpy(card->category, buffer);

  bool condition = addCard(perfil, card);
  if (condition)
  {
    puts("Card agregada con exito!");
    puts(" ");
    if (strcmp(perfil->username, "Invitado") != 0)
      {
        saveProgress(perfil);
      }
    presioneTeclaParaContinuar(1);
  }
  else
  {
    puts("Oops, mensaje de error:");
    puts("No se pudo agregar la carta, intente de nuevo.");
    puts("");
    presioneTeclaParaContinuar(1);
  }
}

//Funcion de mensaje de advertencia por cerrar el programa
bool warningMessage()
{
  int decision;
  puts("Estas a punto de cerrar el programa, recuerda que si no guardas tu progreso, se perdera TODO");
  puts("1. Volver al menu");
  puts("2. Cerrar programa");
  printf("Opcion: ");
  scanf("%d", &decision);
  if (decision == 1)
  {
    return false;
  }
  else
  {
    return true;
  }
}

//Funcion que se encarga de remover la pregunta de la lista
void removeQuestion(user *perfil)
{
  //Se inicializa un indice para recorrer la lista y indicar que numero es el de la pregunta que se desea eliminar
  int i = 1;
  puts("Lista de preguntas del usuario");
  puts(" ");
  //Si no hay preuntas ingresadas, se muestra un mensaje de error y se retorna
  if (list_size(perfil->cards) == 0)
  {
    puts("No hay preguntas ingresadas");
    presioneTeclaParaContinuar(1);
    return;
  }
  //Se recorre la lista de preguntas y se muestra el numero de la pregunta y la pregunta
  for(ankicard *card = list_first(perfil->cards); card != NULL; card = list_next(perfil->cards))
    {
      printf("%d. %s\n", i, card->ask);
      i++;
    }
  puts(" ");
  //Se pide al usuario que ingrese el numero de la pregunta que desea eliminar
  int decision;
  printf("Ingrese el numero de la pregunta que desea eliminar: ");
  scanf("%d", &decision);
  if (decision > list_size(perfil->cards) || decision < 1)
  {
    puts("Oops, mensaje de error:");
    puts("El numero de pregunta ingresado no es valido.");
  }
  else
  {
    ankicard *card = list_first(perfil->cards);
    for (i = 1; i < decision; i++)
    {
      card = list_next(perfil->cards);
    }
    perfil->cards->current = perfil->cards->head;
    for (i = 1; i < decision; i++)
    {
      perfil->cards->current = perfil->cards->current->next;
    }
    card = list_popCurrent(perfil->cards);
    free(card);
    puts("Pregunta eliminada exitosamente.");
    if (strcmp(perfil->username, "Invitado") != 0)
      {
        saveProgress(perfil);
      }
    presioneTeclaParaContinuar(0);
  }
}

//Funcion que se encarga de mostrar preguntas de la lista (general)
void mostrarPreguntas(List *cards)
{
  int i = 1;
  puts("Preguntas ingresadas: ");
  puts(" ");
  if (list_size(cards) == 0)
  {
    puts("No hay preguntas ingresadas");
    return;
  }
  //Se recorre la lista de preguntas y se muestra el numero de la pregunta y la pregunta
  for(ankicard *card = list_first(cards); card != NULL; card = list_next(cards))
    {
      printf("%d. %s\n", i, card->ask);
      i++;
    }
  puts("");
}

//Funcion que modifica cualquier parametro de una pregunta de la lista
void modifyParameter(user *perfil)
{
  int decision;
  List *cards = perfil->cards;
  if (list_size(cards) == 0)
  {
    puts("No hay preguntas ingresadas");
    presioneTeclaParaContinuar(1);
    return;
  }
  puts("Opciones de modificacion");
  puts("");
  puts("1. Cambiar pregunta");
  puts("2. Cambiar respuesta");
  puts("3. Cambiar dificultad");
  puts("4. Cambiar categoria");
  printf("Opcion: ");
  scanf("%d", &decision);
  getchar(); // Limpiar el buffer de entrada

  char buffer[300];
  int dificultad;

  switch (decision)
  {
    //Modificar pregunta
    case 1:
      limpiarPantalla();
      mostrarPreguntas(cards);
      printf("Ingrese el numero de la pregunta que desea cambiar: ");
      scanf("%d", &decision);
      getchar(); // Limpiar el buffer de entrada
      if (decision > list_size(cards) || decision < 1)
      {
        puts("Oops, mensaje de error:");
        puts("El numero de pregunta ingresado no es valido.");
      }
      else
      {
        ankicard *card = list_first(cards);
        for (int i = 1; i < decision; i++)
        {
          card = list_next(cards);
        }
        printf("Ingrese la nueva pregunta: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(card->ask, buffer);
        if (strcmp(perfil->username, "Invitado") != 0)
          {
            saveProgress(perfil);
          }
        puts("Pregunta modificada exitosamente.");
      }
      break;
    //Modifica respuesta
    case 2:
      mostrarPreguntas(cards);
      printf("Ingrese el numero de la pregunta que desea cambiar: ");
      scanf("%d", &decision);
      getchar(); // Limpiar el buffer de entrada
      if (decision > list_size(cards) || decision < 1)
      {
        puts("Oops, mensaje de error:");
        puts("El numero de pregunta ingresado no es valido.");
      }
      else
      {
        ankicard *card = list_first(cards);
        for (int i = 1; i < decision; i++)
        {
          card = list_next(cards);
        }
        printf("Ingrese la nueva respuesta: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(card->answer, buffer);
        if (strcmp(perfil->username, "Invitado") != 0)
          {
            saveProgress(perfil);
          }
        puts("Respuesta modificada exitosamente.");
      }
      break;

    //Modifica dificultad
    case 3:
      limpiarPantalla();
      mostrarPreguntas(cards);
      printf("Ingrese el numero de la pregunta que desea cambiar: ");
      scanf("%d", &decision);
      if (decision > list_size(cards) || decision < 1)
      {
        puts("Oops, mensaje de error:");
        puts("El numero de pregunta ingresado no es valido.");
      }
      else
      {
        ankicard *card = list_first(cards);
        for (int i = 1; i < decision; i++)
        {
          card = list_next(cards);
        }
        printf("Ingrese la nueva dificultad: ");
        scanf("%d", &dificultad);
        card->diff = dificultad;
        if (strcmp(perfil->username, "Invitado") != 0)
          {
            saveProgress(perfil);
          }
        puts("Dificultad modificada exitosamente.");
      }
      break;
    //Modifica categoria
    case 4:
      limpiarPantalla();
      mostrarPreguntas(cards);
      printf("Ingrese el numero de la pregunta que desea cambiar: ");
      scanf("%d", &decision);
      getchar();
      if (decision > list_size(cards) || decision < 1)
      {
        puts("Oops, mensaje de error:");
        puts("El numero de pregunta ingresado no es valido.");
      }
      else
      {
        ankicard *card = list_first(cards);
        for (int i = 1; i < decision; i++)
        {
          card = list_next(cards);
        }
        printf("Ingrese la nueva categoria: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(card->category, buffer);
        if (strcmp(perfil->username, "Invitado") != 0)
          {
            saveProgress(perfil);
          }
        puts("Categoria modificada exitosamente.");
      }
      break;

    default:
      puts("Opcion no valida.");
      break;
  }
}

void myCards(user *perfil){
List *cards = perfil->cards;
  int i = 1;

  puts("Preguntas ingresadas: ");
  puts(" ");

  if (list_size(cards) == 0) {
      puts("No hay preguntas ingresadas");
      presioneTeclaParaContinuar(1);
      return;
  }

  for (ankicard *card = list_first(cards); card != NULL; card = list_next(cards)) {
      printf("%d. %s\n", i, card->ask);
      printf("%s\n", card->answer);
      printf("Dificultad: %d\n", card->diff);
      printf("Categoria: %s\n\n", card->category);
      i++;
  }
  presioneTeclaParaContinuar(1);
}

void cardsForDiff(user *perfil){
  printf("Cartas ordenadas por dificultad\n");
  Map *map = sorted_map_create(lower_than);

  Node *current = perfil->cards->head;
  while(current != NULL){
    ankicard *card = (ankicard *)current->data;

    MapPair *pair = map_search(map, &card->diff);
    if(pair == NULL){
      List *list = list_create();
      list_pushBack(list, card);
      map_insert(map, &card->diff, list);
    }
    else{
      List *list = (List *)pair->value;
      list_pushBack(list,card);
    }
    current = current->next;
  }

  MapPair *pair = map_first(map);
  int i = 0;
  while(pair != NULL){
    int difficulty = *(int *)pair->key;
    List *list = (List *)pair->value;
    Node *node = list->head;
    while(node != NULL){
      printf("\033[1;34m");
      printf("====================================\n");
      printf("Dificultad: %d\n", difficulty);
      printf("====================================\n");
      printf("\033[0m\n");
      ankicard *card = (ankicard *)node->data;
      printf(CYAN "'%s'\n\n" COLOR_RESET, card->ask);
      if (i == 0)
      {
        getchar();
        i++;
      }
      verRespuesta(1);
      printf("Respuesta: " CYAN "%s\n" COLOR_RESET, card->answer);
      printf("\n");
      node = node->next;
      sigPregunta(1);
      limpiarPantalla();
    }
    printf("\n");

    pair = map_next(map);
  }
  puts("No quedan mas preguntas por mostrar :(");
  presioneTeclaParaContinuar(1);
}

void cardsForCategory(user *perfil) {
    printf("Cartas ordenadas por Categoria\n");

    Map *map = map_create(is_equal_str);

    Node *current = perfil->cards->head;
    while (current != NULL) {
        ankicard *card = (ankicard *)current->data;
      char *category = card->category;

        MapPair *pair = map_search(map, category);
        if (pair == NULL) {
            List *list = list_create();
            list_pushBack(list, card);
            map_insert(map, category, list);
        } else {
            List *list = (List *)pair->value;
            list_pushBack(list, card);
        }
        current = current->next;
    }

    MapPair *pair = map_first(map);
  int i = 0;
    while (pair != NULL) {
        char *category = (char *)pair->key;
        List *list = (List *)pair->value;
        // Iterar sobre la lista de tarjetas de Anki en esta categoría
        Node *node = list->head;
        while (node != NULL) {
          printf("\033[1;34m");
          printf("====================================\n");
          printf("Categoría: %s\n", category);
          printf("====================================\n");
          printf("\033[0m\n");
            ankicard *card = (ankicard *)node->data;
            printf(CYAN "'%s'\n\n" COLOR_RESET, card->ask);
          if (i == 0)
          {
            getchar();
            i++;
          }
            verRespuesta(1);
            printf("Respuesta: " CYAN "%s\n" COLOR_RESET, card->answer);
            printf("\n");
            node = node->next;
            sigPregunta(1);
            limpiarPantalla();
        }

        printf("\n");

        pair = map_next(map);
    }
  puts("No quedan mas preguntas por mostrar :(");
    presioneTeclaParaContinuar(1); // Suponiendo que esta función espera la entrada del usuario para continuar
}

int main(void) {
  //Bucle de menus donde se trabajara con variables de tipo booleanas para saber si se inicio sesion, o si se ingreso como invitado, etc
  int num;
  int opcion;
  bool logUser = false;
  bool guestState = false;
  bool signInState = false;
  bool closeProgram = false;

  //Se inicializa el usuario
  user *inicializedUser = (user *)malloc(sizeof(user));
  
  inicializedUser->cards = NULL;
  inicializedUser->cards = list_create();
  
  if (inicializedUser->cards == NULL){
    puts("Oops, mensaje de error:");
    puts("No se pudo crear la lista de tarjetas.");
    puts("");
  }

  bool sessionInicialized, askDecision;
  //Se estara en el bucle si y solo si este el programa funcionando
  do{
    //Menu de inicio de sesion
    do{
      if (guestState) break;
        logMenu();
        printf("Opcion: ");
        scanf("%d", &opcion);
        switch(opcion){
           case 1:
              logIn(inicializedUser, 1);
              presioneTeclaParaContinuar(0);
              break;
            case 2:
              sessionInicialized = signIn(inicializedUser);
              if (!sessionInicialized) break;
                logUser = true;
                signInState = true;
                limpiarPantalla();
                break;
            case 3:
                askDecision = askGuest(inicializedUser);
                if (askDecision){
                  guestState = true;
                  signInState = true;
                  limpiarPantalla();
                }
                break;
              case 4:
                limpiarPantalla();
                puts("Gracias por usar Memo, vuelva pronto! :)");
                return 0;
          }
        } while (!logUser);
      char fileName[50];
      sprintf(fileName, "progreso_%s.csv", inicializedUser->username);
      if (fileExists(fileName)){
        loadQuestions(inicializedUser, fileName);
      }
    //Menu de usuario
      do{
        if (!signInState) break;
        
        limpiarPantalla();
        
        puts("Menu de Memo");
        printf("Usuario: %s\n", inicializedUser->username);
        puts(" ");
        puts("1. Crear Tarjeta");
        puts("2. Visualizar tarjetas guardadas.");
        puts("3. Mostrar tarjetas");
        puts("4. Modificar tarjetas");
        puts("5. Eliminar pregunta");
        puts("6. Cerrar sesion");
        puts("7. Cerrar programa");
        if (guestState)
        {
          printf("\033[1;33m");
          puts("8. Guardar progreso");
          printf("\033[0m\n");
        }
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();
        switch(opcion){
            case 1:
              limpiarPantalla();
              createCard(inicializedUser);
              break;
            case 2:
              limpiarPantalla();
              myCards(inicializedUser);
              break;
            case 3:
              limpiarPantalla();
              printf("Formas de iniciar sesion de estudio: \n");
              puts("1. Estudiar tarjetas por dificultad");
              puts("2. Estudiar tarjetas por categoria");
              printf("Opcion: ");
              scanf(" %d", &num);
              switch(num){
                case 1:
                  limpiarPantalla();
                  cardsForDiff(inicializedUser);
                  break;
                case 2:
                  limpiarPantalla();
                  cardsForCategory(inicializedUser);
                  break;
              }
              break;
            case 4:
              limpiarPantalla();
              modifyParameter(inicializedUser);
              presioneTeclaParaContinuar(1);
              break;
            case 5:
              limpiarPantalla();
              removeQuestion(inicializedUser);
              break;
            case 6:
              limpiarPantalla();
              signInState = false;
              if(guestState) guestState = false;
              else logUser = false;
              break;
            case 7:
              limpiarPantalla();
              bool warningAnswer = warningMessage();
              if (warningAnswer){
                limpiarPantalla();
                closeProgram = true;
                signInState = false;
                if(guestState) guestState = false;
                else logUser = false;
                puts("Gracias por usar Memo, vuelva pronto! :)");
              }
              break;
            case 8:
              limpiarPantalla();
              if (guestState)
              {
                puts("Al seguir adelante, se guardara el progreso de estudio.");
                puts("Seleccione una opcion: ");
                puts("1. Guardar progreso");
                puts("2. No guardar progreso");
                printf("Opcion: ");
                scanf("%d", &num);
                switch(num)
                  {
                    case 1:
                    logIn(inicializedUser, 0);
                    guestState = false;
                    signInState = true;
                    break;
                    case 2:
                    break;
                  }
              }
              else
              {
                break;
              }
              break;
          }
      } while (signInState);
    //Si se selecciona cerrar programa, se sale del bucle representando que se ha cerrado el programa
      if (closeProgram)
      {
        break;
      }
    } while (1);
  return 0;
}