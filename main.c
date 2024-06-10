#include <stdio.h>
#include <stdbool.h>
#include "tdas/extra.h"
#define fileUser "usuarios.csv"

typedef struct{
  char respuesta;
  char pregunta;
  int dificultad;
}ankicard;

typedef struct{
  char user[20];
  char password[20];
  ankicard tarjetas;
}user;

void logMenu()
{
  limpiarPantalla();
  //Welcome message
  puts("¡Bienvenido a Memo, el sistema de AnkiCard que te ayudara a estudiar!");
  puts("");
  puts("= = = = = = = = = = = = = = = = = = = = = = = = = =");
  puts("Por favor, seleccione una opcion:");
  puts("1. Registrarse");
  puts("2. Iniciar sesion");
  puts("3. Continuar como invitado");
  puts("4. Salir del programa");
  puts("= = = = = = = = = = = = = = = = = = = = = = = = = =");
}

void logIn(int identify)
{
  limpiarPantalla();
  //Crear usuario y contraseña
  char registerUser[20];
  char registerPassword[20];
  bool stateUser = false;
  bool statePass = false;
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
  FILE *archivo = fopen(fileUser, "r");
  if (archivo == NULL)
  {
    archivo = fopen(fileUser, "w");
    fprintf(archivo, "Username,Password\n");
    fprintf(archivo, "%s,%s\n", registerUser, registerPassword);
    fclose(archivo);
    if (identify != 1)
    {
      //Logica por si se quiere iniciar sesion como invitado, y guarda progreso en archivo.csv
    }
    else
    {
      puts("");
      puts("Usuario creado con exito! Ya puedes iniciar sesion con Memo.");
    }
  }
  else
  {
    //Verificar si el usuario existe en el CSV
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
          //Logica por si se quiere iniciar sesion como invitado, y guarda progreso en archivo.csv
        }
      else
      {
        puts("");
        puts("Usuario creado con exito! Ya puedes iniciar sesion con Memo.");
      }
    }
  }
}

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

bool signIn(user *usuario)
{
  limpiarPantalla();
  FILE *archivo = fopen(fileUser, "r");
  if (archivo == NULL)
  {
    puts("Oops, mensaje de error:");
    puts("No existen registros de cuenta en el terminal, registrese con una cuenta.");
    puts("");
    presioneTeclaParaContinuar();
    return false;
  }
  fclose(archivo);
  char usernameSignIn[20];
  char passwordSignIn[20];
  bool stateUser = false;
  bool statePass = false;
  bool repeat;
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
                strcpy(usuario->user, usernameSignIn);
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

bool askGuest(user *usuario)
{
  limpiarPantalla();
  int decision;
  puts("Ten en cuenta que de si ingresas como invitado podras crear una cuenta de todas formas para guardar tu progreso");
  puts("Si al final de la sesion NO decides crear una cuenta, se perdera TODO el progreso que haya realizado en Memo");
  puts("Asi que, eligue bien tu decision :)");
  puts("");
  puts("1. Me arrepenti, deseo cuentar una cuenta");
  puts("2. Se tomar mis decisiones, asi que ingreso como invitado");
  printf("Opcion: ");
  scanf("%d", &decision);
  if (decision == 1)
  {
    return false;
  }
  char guestUser[20] = "Invitado";
  char guestPassword[20] = "12345678";
  strcpy(usuario->user, guestUser);
  strcpy(usuario->password, guestPassword);
  return true;
}

int main(void) {
  int opcion;
  bool logUser = false;
  bool guestState = false;
  user *inicializedUser = (user *)malloc(sizeof(user));
  bool sessionInicialized, askDecision;
  do
    {
      if (guestState)
      {
        break;
      }
      logMenu();
      printf("Opcion: ");
      scanf("%d", &opcion);
      switch(opcion)
        {
          case 1:
            logIn(1);
            presioneTeclaParaContinuar();
            break;
          case 2:
            sessionInicialized = signIn(inicializedUser);
            if (!sessionInicialized)
            {
              break;
            }
            logUser = true;
            break;
          case 3:
            askDecision = askGuest(inicializedUser);
            if (askDecision)
            {
              guestState = true;
              limpiarPantalla();
            }
            break;
          case 4:
            limpiarPantalla();
            puts("Gracias por usar Memo, vuelva pronto! :)");
            return 0;
            break;
      }
    } while (!logUser);
  printf("TADAAA, menu del programa:)");
  return 0;
}