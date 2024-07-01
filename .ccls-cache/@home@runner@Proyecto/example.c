#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char *str = "Hola";
  char *str2 = "Hola";
  if (strcmp(str, str2) == 0)
  {
    printf("Son iguales");
  }
  else
  {
    printf("No son iguales");
  }
}