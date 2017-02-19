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
    /*
     deklaruji si staticky alokovane integerove pole o 63 prvcich.
     63 protoze long muze byt nejvice 64 bitu, ale ve skutecnosti je
     to jen 63. (velikost je dana podle toho kolikabitovy je system)
    */
    static char binary[BIN_NUM_LENGTH] = {0};
    /*
     nastavi vsechny prvky vyslednyho pole na 2, aby se zjistilo,
     kde doopravdy cislo konci. V binarni soustave neni cislo 2,
     takze nedojde k chybam.
    */
    memset(binary, 2, BIN_NUM_LENGTH * sizeof(binary[0]));

    /*
     pro pripad, ze uzivatel zada 0 nebo -0, nastavim prvni prvek na 0
     pokud to nula nebude, tak si ji sam prepise na jednicku.
    */
    binary[0] = 0;

    /*
     docasne pole, ktere se pak bude prepisovat do pole "binary"
     ve spravnem poradi.
    */
    char tempBinary[63] = {0};

    // promenna uchovavajici aktualni index pole "pTempBinary".
    int i = 0;

    // promenna uchovavajici index pole "binary".
    int j = 0;

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
        tempBinary[i++] = decimal % 2;
        decimal /= 2;
    }

    i--;

    // pokud je cislo zaporne, nastavi prvni cislo na -1.
    if (negative == 1)
        tempBinary[i] = -1;

    while (i != -1) {
        binary[j++] = tempBinary[i--];
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
