#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

typedef int bool;
#define true 1
#define false 0

/**
 * Zkontroluje argumenty.
 * pokud jich je moc nebo malo -> vyhodi chybovou hlasku.
 */
bool checkArgumentCount(int argc)
{
    if (argc - 1 == 0 || argc - 1 > 1) {
        printf("Zadejte prosim prave jeden argument.\n");
        exit(0);
    } else if (argc - 1 == 1) {
        return true;
    }

    return false;
}

/**
 * Zkontroluje parametr jestli se jedna o celociselne cislo (integer).
 * rovnez jestli neni cislo prilis velke.
 */
bool checkParam(char *c_num[])
{
    char * endp;
    long num = strtol(c_num[1], &endp, 10);

    if (num > INT_MAX || num < INT_MIN) {
        printf("Zadejte prosim cislo od %d do %d.\n", INT_MIN, INT_MAX);
        exit(0);
    }

    if (!*endp != '\0') {
        return true;
    }

    printf("Zadany argument neni cislo.\n");
    return false;
}

/**
 * Vypocita, jak bude dlouhe cislo v binarce.
 */
int getBinLength(int decNum) {
    decNum = abs(decNum);
    // novy pocet mist, ktereho bude nabyvat nove cislo v binarce.
    return log(decNum)/log(2);
}

/**
 * Prevede cislo z decimalky do binarky.
 */
int * convertDecToBin(int decIntToConvert) {
    bool negative = false;

    // detekuje, ze je cislo zaporne.
    // pokud je zaporne, prevede ho na kladne.
    // nakonec da na zacatek -1.
    if (decIntToConvert < 0) {
        negative = true;
        decIntToConvert *= -1;
    }

    // ziska delku cisla v binarce.
    int binLength = getBinLength(decIntToConvert);

    // deklaruji si dynamicky alokovane integerove pole o "binLength" prvku.
    // +1 pro terminator \0
    int * resultBin = malloc((binLength + 1) * sizeof(int));

    // promenna uchovavajici aktualni index pole "resultBin".
    int idx = binLength;

    // samotny prevod cisla z desitkovy do binarni soustavy.
    while (decIntToConvert != 0) {
        resultBin[idx] = decIntToConvert % 2;
        decIntToConvert /= 2;
        idx--;
    }

    // pokud je cislo zaporne, nastavi prvni cislo na -1.
    if (negative == 1) {
        resultBin[0] = -1;
    }

    return resultBin;
}

/**
 * Vypise vyslednou frazi.
 */
void printBinNumber(int binLength, int decNum, int * binNum) {
    printf("Cislo v dekadicke soustave %d je ", decNum);

    int f = 0;
    for(;f <= binLength; f++)
        printf("%d", binNum[f]);
    printf(" v binarni soustave.\n");
}

int main(int argc, char *argv[])
{
    bool checkArgCount = checkArgumentCount(argc);
    bool checkParamInt = checkParam(argv);

    if (checkArgCount == 1 && checkParamInt == 1) {
        // preved char zadany do argumentu na integer.
        int decIntToConvert = atoi(argv[1]);

        // zkouska, ze uzivatel nezadal nula (nebo minus nula).
        // aby nedoslo k deklaraci pole o velikosti 0 -> nastala by chyba.
        // -0 = 0 (ano cecko to vi).
        if (decIntToConvert == 0) {
            int zero_array[1] = {0};
            printBinNumber(0, decIntToConvert, zero_array);
            return 0;
        }

        int * result = convertDecToBin(decIntToConvert);

        int binLength = getBinLength(decIntToConvert);
        printBinNumber(binLength, decIntToConvert, result);

        free(result);
    }

    return 0;
}
