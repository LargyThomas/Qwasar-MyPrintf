#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

// Prints a string character by character. Handles NULL by printing "(null)".
int print_string(char *s) {
    if (!s) {
        write(1, "(null)", 6);
        return 6;
    }

    int count = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        write(1, &s[i], 1);
        count++;
    }
    return count;
}

// Prints a single character.
int print_char(int c) {
    char charact = (char)c;
    write(1, &charact, 1);
    return 1;
}

/*
    Calculation --> Octal

    210 % 10 +'0' = 0
    210 /10 = 21

    21 % 10 +'0' = 1
    21 / 10 = 2

    2 % 10 + '0'= 2
    2 / 10 = 0

    end
*/

// Converts an unsigned integer to octal and prints it.
int print_octal(unsigned int valeur) {
    if (valeur == 0) {
        char c = '0';
        write(1, &c, 1);
        return 0;
    }

    int octal = 0, j = 1;

    while (valeur != 0) {
        octal += (valeur % 8) * j;
        valeur /= 8;
        j *= 10;
    }

    char buffer[32];

    // Octal --> String
    int i = 0;
    while (octal > 0) {
        buffer[i] = (octal % 10) + '0';
        octal /= 10;
        i++;
    }

    int count = i;
    // Write in correct order
    while (i > 0) {
        i--;
        write(1, &buffer[i], 1);
    }
    return count;
}

// Prints a signed decimal integer. Handles negative values and zero.
int print_signe_decimal(int valeur) {
    char buffer[12];    // 11 characters (10 digits and the - sign) + '\0'
    int nombre = valeur;

    if (valeur == 0) {
        char c = '0';
        write(1, &c, 1);
        return 2;
    }

    if (valeur < 0) {
        write(1, "-", 1);
        nombre = -nombre;        // Remove the minus sign
    }

    int i = 0;

    // Int --> String
    while (nombre > 0) {
        buffer[i] = (nombre % 10) + '0';
        nombre /= 10;
        i++;
    }

    int count = i;
    // Write in correct order
    while (i > 0) {
        i--;
        write(1, &buffer[i], 1);
    }
    return count;
}

// Prints an unsigned decimal integer.
int print_unsigned_decimal(unsigned int valeur) {
    char buffer[11];
    int nombre = valeur;

    if (valeur == 0) {
        char c = '0';
        write(1, &c, 1);
        return 0;
    }

    int i = 0;
    // Int --> String
    while (nombre > 0) {
        buffer[i] = (nombre % 10) + '0';
        nombre /= 10;
        i++;
    }

    int count = i;
    // Write in correct order
    while (i > 0) {
        i--;
        write(1, &buffer[i], 1);
    }
    return count;
}

// Converts an unsigned integer to hexadecimal (uppercase) and prints it.
int print_hexa(unsigned int decimal) {
    char hexadecimal[100];
    int i = 0;

    while (decimal > 0) {
        int reste = decimal % 16;

        if (reste < 10) {
            hexadecimal[i] = reste + '0';
        }
        else {
            hexadecimal[i] = 'A' + reste - 10;
        }

        decimal /= 16;
        i++;
    }

    int count = i;
    while (i > 0) {
        i--;
        write(1, &hexadecimal[i], 1);
    }
    return count;
}

// Prints a pointer address in hexadecimal format (prefixed with 0x).
int print_pointeur_hexa(void* pointeur) {
    unsigned long adresse = (unsigned long)pointeur;

    if (pointeur == NULL) return 1;

    char *buffer = malloc(2 + 16 + 1);  // 0x prefix + hex characters + '\0'
    if (!buffer) return 1;

    buffer[0] = '0';          // Pointer starts with 0x
    buffer[1] = 'x';

    char *base_hexa = "0123456789abcdef";
    char temp[32];
    int i = 0;

    while(adresse > 0) {
        temp[i++] = base_hexa[adresse % 16];
        adresse /= 16;
    }

    int position = 2;
    int count = position + i;
    while (i--) {
        buffer[position] = temp[i];
        position++;
    }

    buffer[position] = '\0';

    print_string(buffer);
    free(buffer);
    return count;
}

// Dispatches each format specifier to its corresponding print function.
int dispatcher(char c, va_list args) {
    char pourcent = '%';
    int count = 0;
    switch (c) {
        case 'd':
            count += print_signe_decimal(va_arg(args, int));
            break;

        case 'o':
            count += print_octal(va_arg(args, unsigned int));
            break;

        case 'u':
            count += print_unsigned_decimal(va_arg(args, unsigned int));
            break;

        case 'x':
            count += print_hexa(va_arg(args, unsigned int));
            break;

        case 'c':
            count += print_char(va_arg(args, int));
            break;

        case 's':
            count += print_string(va_arg(args, char *));
            break;

        case 'p':
            count += print_pointeur_hexa(va_arg(args, void *));
            break;

        default:
            count += write(1, &pourcent, 1);
            count += write(1, &c, 1);
    }

    return count;
}

// Recreation of printf: parses the format string and calls the dispatcher
// for each format specifier found.
int my_printf(char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int count = 0;

    for(int i = 0; format[i]; i++) {
        if (format[i] != '%') {
            print_char(format[i]);
            count++;
        }
        else {
            i++;
            count += dispatcher(format[i], args);
        }
    }

    va_end(args);
    return count;
}

int main(){
    my_printf("Test pour mon print : \n");

    my_printf("Decimal signed : %d\n", -13543);
    my_printf("Octal : %o\n", 245);
    my_printf("Decimal unsigned : %u\n", 543);
    my_printf("Hexadecimal : %x\n", 235);
    my_printf("Caractere : %c\n", 'A');
    my_printf("String : %s\n", "Hello Wolrd !");
    int test = 34;
    my_printf("Pointeur : %p\n", &test);

    my_printf("Fin du test \n");
    return 0;
}
