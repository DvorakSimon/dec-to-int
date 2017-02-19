#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

typedef int bool;
#define true 1
#define false 0

// maximalni delka pole pro long.
// jedna se o logaritmus pri zaklade 2 cisla v decimalni soustave.
#define BIN_LENGTH 63

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
bool checkParam(char *c_num[])
{
    char * endp;
    long num = strtol(c_num[1], &endp, 10);

    if (num > LONG_MAX || num < LONG_MIN) {
        printf("Zadejte prosim cislo od %ld do %ld.\n", LONG_MAX, LONG_MIN);
        return false;
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
int getBinLength(long decNum) {
    decNum = abs(decNum);
    if (decNum == 0) return 0;
    // novy pocet mist, ktereho bude nabyvat nove cislo v binarce.
    long result = log2l(decNum);
    return result;
}

/**
 * Prevede cislo z decimalky do binarky.
 */
char * convertDecToBin(long decIntToConvert) {
    // deklaruji si staticky alokovane integerove pole o 63 prvku.
    // 63 prvku, protoze round(log2(9223372036854775807)) je 63.
    static char resultBin[BIN_LENGTH] = {0};

    // promenna uchovavajici aktualni index pole "resultBin".
    int k = 0;

    bool negative = false;

    // detekuje, ze je cislo zaporne.
    // pokud je zaporne, prevede ho na kladne.
    // nakonec da na zacatek -1.
    if (decIntToConvert < 0) {
        negative = true;
        decIntToConvert = -decIntToConvert;
    }

    // samotny prevod cisla z desitkovy do binarni soustavy.
    while (decIntToConvert != 0) {
        resultBin[k++] = decIntToConvert % 2;
        decIntToConvert /= 2;
    }

    k--;

    // pokud je cislo zaporne, nastavi prvni cislo na -1.
    if (negative == 1) {
        resultBin[k] = -1;
    }

    return resultBin;
}

/**
 * Vypise vyslednou frazi.
 */
void printBinNumber(long decNum, char * binNum) {
    printf("Cislo v dekadicke soustave %ld je ", decNum);

    int f = BIN_LENGTH;

    while (binNum[f] == 0 && f > 0) {
        f--;
    }

    for(; f >= 0; f--)
        printf("%d", binNum[f]);
    printf(" v binarni soustave.\n");
}

int main(int argc, char *argv[])
{
    bool checkArgCount = checkArgumentCount(argc);

    if (checkArgCount == 1) {

        bool checkParamInt = checkParam(argv);

        if (checkParamInt == 1) {
            // preved char zadany do argumentu na integer.
            long decIntToConvert = atol(argv[1]);

            char * result = convertDecToBin(decIntToConvert);

            printBinNumber(decIntToConvert, result);
        }
    }

    return 0;
}
