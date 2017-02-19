#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

// maximalni delka pole pro long.
// jedna se o logaritmus pri zaklade 2 cisla v decimalni soustave.
#define BIN_NUM_LENGTH 63

/**
 * Zkontroluje argumenty.
 * pokud jich je moc nebo malo -> vyhodi chybovou hlasku.
 */
bool checkArgumentCount(int argc)
{
    if (argc - 1 == 0 || argc - 1 > 1) {
        printf("Zadejte prosim prave jeden argument.\n");
        return false;
    } else if (argc - 1 == 1) {
        return true;
    }

    return false;
}

/**
 * Zkontroluje parametr jestli se jedna o celociselne cislo (integer).
 * rovnez jestli neni cislo prilis velke.
 */
bool checkParam(char * pcDecimal[])
{
    errno = 0;
    char * endptr;
    long num = strtol(pcDecimal[1], &endptr, 10);

    if ((errno == ERANGE && (num == LONG_MAX || num == LONG_MIN)) || (errno != 0 && num == 0)) {
        printf("Zadejte prosim cislo od %ld do %ld.\n", LONG_MIN, LONG_MAX);
        return false;
    }

    if (!*endptr != '\0') {
        return true;
    }

    printf("Zadany argument neni cele cislo.\n");
    return false;
}

/**
 * Prevede cislo z decimalky do binarky.
 */
char * convertDecToBin(long decimal) {
    // deklaruji si staticky alokovane integerove pole o 63 prvcich.
    // 63 prvku, protoze round(log2(9223372036854775807)) je 63.
    static char binary[BIN_NUM_LENGTH] = {0};
    memset(binary, 2, BIN_NUM_LENGTH * sizeof(binary[0]));

    char pTempBinary[63] = {0};

    // promenna uchovavajici aktualni index pole "binary".
    int i = 0;

    bool negative = false;

    // detekuje, ze je cislo zaporne.
    // pokud je zaporne, prevede ho na kladne.
    // nakonec da na zacatek -1.
    if (decimal < 0) {
        negative = true;
        decimal = -decimal;
    }

    // samotny prevod cisla z desitkovy do binarni soustavy.
    while (decimal != 0) {
        pTempBinary[i++] = decimal % 2;
        decimal /= 2;
    }

    i--;

    // pokud je cislo zaporne, nastavi prvni cislo na -1.
    if (negative == 1) {
        pTempBinary[i] = -1;
    }

    int j = 0;
    while (i != -1) {
        binary[j++] = pTempBinary[i--];
    }

    return binary;
}

/**
 * Vypise vyslednou frazi.
 */
void printBinNumber(long decimal, char * binary) {
    printf("Cislo v dekadicke soustave %ld je ", decimal);

    int k = 0;
    while (binary[k] != 2 && k < BIN_NUM_LENGTH)
        printf("%d", binary[k++]);
    printf(" v binarni soustave.\n");
}

int main(int argc, char *argv[])
{
    bool checkArgCount = checkArgumentCount(argc);

    if (checkArgCount == 1) {

        bool checkParamInt = checkParam(argv);

        if (checkParamInt == 1) {
            // preved char zadany do argumentu na integer.
            long iDecimal = atol(argv[1]);

            char * pResult = convertDecToBin(iDecimal);

            printBinNumber(iDecimal, pResult);
        }
    }

    return 0;
}
