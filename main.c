#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef struct {
    double reale;
    double immaginario;
} Complesso;


double sqrt(double numero) {
    double approssimazione = numero / 2.0;
    double differenza = numero - approssimazione * approssimazione;
    while (differenza * differenza > 0.00001 * 0.00001) {
        approssimazione = (approssimazione + numero / approssimazione) / 2.0;
        differenza = numero - approssimazione * approssimazione;
  }
  return approssimazione;
}

Complesso sqrt_complex(double numero) {
    Complesso risultato;

    if (numero >= 0) {
        risultato.reale = sqrt(numero);
        risultato.immaginario = 0;
    } else {
        risultato.reale = 0;
        risultato.immaginario = sqrt(-numero);
    }

    return risultato;
}

void verifica_grado_superiore_2(char* str) {
    char buffer[16];
    
    for (int i = 3; i <= 1000; i++) {
        sprintf(buffer, "X^%d", i);
        if (strstr(str, buffer)) {
            printf("Errore: Non è possibile risolvere equazioni di grado superiore al 2 (trovato X^%d).\n", i);
            exit(0);
        }
    }
}

void print_forma_ridotta(double a, double b, double c) {
    //if (a < 0 && b < 0 && c < 0) {
    //    a = -a;
    //    b = -b;
    //    c = -c;
    //}
    if (a == 0 && b == 0) {
        printf("Forma ridotta: %.2lf = 0\n\n", c);
    } else if (a == 0 && c == 0) {
        printf("Forma ridotta: %.2lf * X = 0\n\n", b);
    } else if (b == 0 && c == 0) {
        printf("Forma ridotta: %.2lf * X^2 = 0\n\n", a);
    } else if (a == 0) {
        printf("Forma ridotta: %.2lf * X %c %.2lf = 0\n\n", b, c < 0 ? '-' : '+', c < 0 ? -c : c);
    } else if (b == 0) {
        printf("Forma ridotta: %.2lf * X^2 %c %.2lf = 0\n\n", a, c < 0 ? '-' : '+', c < 0 ? -c : c);
    } else if (c == 0) {
        printf("Forma ridotta: %.2lf * X^2 %c %.2lf * X = 0\n\n", a, b < 0 ? '-' : '+', b < 0 ? -b : b);
    } else {
        printf("Forma ridotta: %.2lf * X^2 %c %.2lf * X %c %.2lf = 0\n\n", 
               a, b < 0 ? '-' : '+', b < 0 ? -b : b, c < 0 ? '-' : '+', c < 0 ? -c : c);
    }
}

void discriminante_quadrato(double a, double b, double c){
    print_forma_ridotta(a,b,c);
    printf("Grado dell'equazione: 2\n");

    double disc;
    double sol1;
    double sol2;
    double sol;
    Complesso comp1;
    Complesso comp2;

    disc = (b * b) - (4 * a * c);
    printf("Discriminante: %.2lf\n", disc);
    if(disc > 0)
    {
        printf("Il discriminante è positivo quindi abbiamo 2 soluzioni reali\n");
        sol1 = (-b + sqrt(disc))/ (2 * a);
        sol2 = (-b - sqrt(disc))/ (2 * a);
        printf("le soluzioni sono %.2lf e %.2lf\n", sol1, sol2);
    }
    if(disc == 0)
    {
        printf("Il discriminante è 0 quindi abbiamo 1 soluzione reale\n");
        sol = -b / (2 * a);
        printf("la soluzione è %.2lf\n", sol);
    }
    if(disc < 0)
    {
        printf("Il discriminante è negativo quindi abbiamo 2 soluzioni complesse\n");
        comp1 = sqrt_complex(disc);
        comp2 = comp1;

        printf("La prima soluzione complessa è: %.2lf + %.2lfi\n", (-b / (2 * a)), comp1.immaginario / (2 * a));
        printf("La seconda soluzione complessa è: %.2lf - %.2lfi\n", (-b / (2 * a)), comp2.immaginario / (2 * a));
    }
}

void discriminante_lineare(double b, double c){
    print_forma_ridotta(0,b,c);
    printf("Grado dell'equazione: 1\n");
    double sol;
    if (b != 0) {
        sol = -c / b;
        printf("Soluzione: %.2lf\n", sol);
    } else if (b == 0 && c == 0) {
        printf("Qualsiasi numero reale è soluzione.\n");
    } else {
        printf("Non esistono soluzioni.\n");
    }
}

void discriminante_costante(double c)
{
    print_forma_ridotta(0,0,c);
    printf("Grado dell'equazione: 0\n");
    if (c == 0) {
        printf("Qualsiasi numero reale è soluzione.\n");
    } else {
        printf("Non esistono soluzioni.\n");
    }
}

void risolvi_polinomio(double a, double b, double c)
{
    if(a != 0)
        discriminante_quadrato(a,b,c);
    else if(b != 0)
        discriminante_lineare(b,c);
    else
        discriminante_costante(c);
}

void rimuovi_spazi(char* str){
    char* i = str;
    char* j = str;
    while(*j != '\0'){
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = '\0';
}

void verifica_uguale(char* str){
    unsigned long i = 0;
    int count = 0;
    while(i <= strlen(str)){
        if (str[i] == '=') {
            count++;
        }
        i++;
    }
    if( count != 1)
    {
        printf("Ci deve essere un solo simobolo =\n");
        exit(1);
    }
}

void verifica_segnali_consecutivi(char* str) {
    char prev = '\0';
    for (unsigned long i = 0; i < strlen(str); i++) {
        // Controlla se il carattere corrente è un segno
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
            // Se anche il carattere precedente è un segno, c'è un errore
            if (prev == '+' || prev == '-' || prev == '*' || prev == '/') {
                printf("Errore: segni consecutivi non ammessi (trovato '%c%c').\n", prev, str[i]);
                exit(1);
            }
        }
        // Aggiorna il precedente carattere
        prev = str[i];
    }
}

void verifica_parte_equazione(char* parte, char* lato) {
    if (parte == NULL || strlen(parte) == 0) {
        printf("Errore: la parte %s dell'equazione è vuota.\n", lato);
        exit(1);
    }
    
    for (unsigned long i = 0; i < strlen(parte); i++) {
        if (!(isdigit(parte[i]) || parte[i] == 'X' || parte[i] == '+' || parte[i] == '-' || parte[i] == '*' || parte[i] == '^')) {
            printf("Errore: carattere non valido trovato nella parte %s dell'equazione.\n", lato);
            exit(1);
        }
    }
}

void analisi_lato(char* lato, int segno, double* a, double* b, double* c)
{
    char* p = lato;
    double coeff;
    int esponente;

    while (*p != '\0') {
        // Rileva il segno di ciascun termine
        int segno_termine = (*p == '-') ? -1 : 1;
        if (*p == '+' || *p == '-') {
            p++;  // Salta il segno
        }

        // Verifica se ci sono il coefficiente e l'esponente
        if (sscanf(p, "%lf*X^%d", &coeff, &esponente) == 2) {
            if (esponente == 2) {
                *a += segno * segno_termine * coeff;
            } else if (esponente == 1) {
                *b += segno * segno_termine * coeff;
            } else if (esponente == 0) {
                *c += segno * segno_termine * coeff;
            }
            else
            {
                printf("Errore: Il formato non è accettabile, gli elementi devono essere nella forma 'a*X^p'\n");
                exit(1);
            }
        }
        else
        {
            printf("Errore: Il formato non è accettabile, gli elementi devono essere nella forma 'a*X^p'\n");
            exit(1);
        }
        //else if (sscanf(p, "%lf*X", &coeff) == 1) {
        //    // Caso in cui abbiamo "X" senza esponente esplicito
        //    *b += segno * segno_termine * coeff;
        //} 
        //else if (sscanf(p, "%lf", &coeff) == 1) {
        //    // Caso di termine costante
        //    *c += segno * segno_termine * coeff;
        //} 
        //else if (strstr(p, "X^2")) {
        //    *a += segno * segno_termine * 1;
        //} else if (strstr(p, "X^1") || strstr(p, "X")) {
        //    *b += segno * segno_termine * 1;
       //}


        // Avanza al prossimo termine
        while (*p != '+' && *p != '-' && *p != '\0') {
            p++;
        }
    }
}

void parser(char *equa)
{
    double a = 0;
    double b = 0;
    double c = 0;
    //double temp_a = 0;
    //double temp_b = 0;
    //double temp_c = 0;

    verifica_uguale(equa);
    rimuovi_spazi(equa);
    verifica_segnali_consecutivi(equa);
    verifica_grado_superiore_2(equa);

    char *left = strtok(equa, "=");
    char *right = strtok(NULL, "=");

    printf("LEFT: %s\n", left);
    printf("RIGHT: %s\n\n", right);

    verifica_parte_equazione(left, "sinistra");
    verifica_parte_equazione(right, "destra");

    analisi_lato(left, 1, &a, &b, &c);
    analisi_lato(right, -1, &a, &b, &c);
    //if (left) {
    //    sscanf(left, "%lf*X^0%lf*X^1%lf*X^2", &temp_c, &temp_b, &temp_a);
    //    a += temp_a;
    //    b += temp_b;
    //    c += temp_c;
    //}
//
    //temp_a = 0;
    //temp_b = 0;
    //temp_c = 0;
//
    //if (right) {
    //    sscanf(right, "%lf*X^0%lf*X^1%lf*X^2", &temp_c, &temp_b, &temp_a);
    //    a -= temp_a;
    //    b -= temp_b;
    //    c -= temp_c;
    //}

    printf("Coefficiente trovati:\na = %lf\nb = %lf\nc = %lf\n\n", a, b, c);
    risolvi_polinomio(a,b,c);
}

int main(int argc, char** argv){

    char equa[256];

    if (argc != 2)
    {
        printf("Utilizzo: %s \"equazione\"\n", argv[0]);
        return 0;
    }

    strcpy(equa,argv[1]);

    printf("Equazione inserita: %s\n\n", equa);
    parser(equa);

    return 0;
}
