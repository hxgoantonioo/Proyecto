#include <stdio.h>

int main(void) 
{
    // Colores de texto
    printf("\033[1;31mTexto rojo brillante\033[0m\n");
    printf("\033[1;32mTexto verde brillante\033[0m\n");
    printf("\033[1;33mTexto amarillo brillante\033[0m\n");
    printf("\033[1;34mTexto azul brillante\033[0m\n");
    printf("\033[1;35mTexto magenta brillante\033[0m\n");
    printf("\033[1;36mTexto cyan brillante\033[0m\n");
    printf("\033[1;37mTexto blanco brillante\033[0m\n");

    // Colores de fondo y texto
    printf("\033[1;41m\033[1;37mTexto blanco sobre fondo rojo brillante\033[0m\n");
    printf("\033[1;42m\033[1;30mTexto negro sobre fondo verde brillante\033[0m\n");

    // Estilos de texto
    printf("\033[1mTexto en negrita\033[0m\n");
    printf("\033[4mTexto subrayado\033[0m\n");
    printf("\033[7mTexto con inversión de colores\033[0m\n");

    // Posicionamiento del cursor
    printf("\033[10;20HTexto en la fila 10, columna 20\n");

    // Borrado de pantalla
    printf("\033[2J"); // Borra toda la pantalla

    return 0;
}
