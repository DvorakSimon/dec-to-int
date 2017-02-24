#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

/*
 Maximalni delka pole pro long.
 63 protoze long muze byt nejvice 64 bitu, ale ve skutecnosti je
 to jen 63. (velikost je dana podle toho kolikabitovy je system)
*/
#define BIN_NUM_LENGTH 63
#define BASE 2

struct BinaryObject {
    long decNumber;
    int * binNumber;
    int length;
};

/**
 * Zkontroluje argumenty.
 * pokud jich je moc nebo malo -> vyhodi chybovou hlasku.
 */
bool checkArgumentCount(int argc)
{
    if (argc - 1 == 0 || argc - 1 > 1) {
        printf("Zadejte prosim prave jeden argument.\n");
        printf("Tvar: dec-to-bin.exe dec_num\n");
        printf("Napriklad: dec-to-bin.exe 15786\n");
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
struct BinaryObject convertDecToBin(long decimal) {
    /*
     deklaruji si strukturu obsahujici binarni cislo
     a delku tohoto cisla resp. kde cislo konci.
    */
    struct BinaryObject binaryNumber;
    binaryNumber.decNumber = decimal;

    /*
     deklaruji si staticky alokovane integerove pole o BIN_NUM_LENGTH prvcich.
    */
    static int binary[BIN_NUM_LENGTH] = {0};

    /*
     docasne pole, ktere se pak bude prepisovat do pole "binary"
     ve spravnem poradi.
    */
    int tempBinary[BIN_NUM_LENGTH] = {0};

    // promenna uchovavajici aktualni index pole "tempBinary".
    int i = 0;

    // promenna uchovavajici aktualni index pole "binary".
    int j = 0;

    /*
     promenna, ktera rika jestli je cislo kladne nebo zaporne.
     true -> je zaporne a false -> je kladne
    */
    bool negative = false;

    // detekuje, ze je cislo zaporne.
    // pokud je zaporne, prevede ho na kladne.
    // nakonec da na zacatek -1.
    if (decimal < 0) {
        negative = true;
        decimal = -decimal;
    }

    // samotny prevod cisla z desitkove do binarni soustavy.
    while (decimal != 0) {
        tempBinary[i++] = decimal % BASE;
        decimal /= BASE;
    }

    if (i > 0)
        i--;

    binaryNumber.length = i;

    // pokud je cislo zaporne, nastavi prvni cislo na -1.
    if (negative)
        tempBinary[i] = -1;

    // prepise hodnoty z docasneho pole do vysledeneho ve spravnem poradi.
    while (i != -1)
        binary[j++] = tempBinary[i--];

    binaryNumber.binNumber = binary;

    return binaryNumber;
}

/**
 * Vypise vyslednou frazi.
 */
void printBinNumber(struct BinaryObject binaryNumber) {
    printf("Cislo v dekadicke soustave %ld je ", binaryNumber.decNumber);

    int i = 0;
    for (; i <= binaryNumber.length; i++)
        printf("%d", binaryNumber.binNumber[i]);

    printf(" v binarni soustave.\n");
}

int main(int argc, char *argv[])
{
    bool checkArgCount = checkArgumentCount(argc);

    if (checkArgCount) {

        bool checkParamInt = checkParam(argv);

        if (checkParamInt) {
            // preved char zadany do argumentu na long.
            long iDecimal = atol(argv[1]);

            struct BinaryObject binaryNumber = convertDecToBin(iDecimal);

            printBinNumber(binaryNumber);
        }
    }

    return 0;
}
