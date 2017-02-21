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
#define SEPARATOR 2
#define BASE 2

struct BinaryObject {
    long decNumber;
    char * binNumber;
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
    static char binary[BIN_NUM_LENGTH] = {0};

    /*
     pro pripad, ze uzivatel zada 0 nebo -0, nastavim prvni prvek na SEPARATOR tj. 2.
     separator musi byt cislo, ktere neni obsazeno v soustave (coz neni).
     pokud to nula nebude, tak si ji sam prepise na jednicku.
    */
    binary[1] = SEPARATOR;

    /*
     docasne pole, ktere se pak bude prepisovat do pole "binary"
     ve spravnem poradi.
    */
    char tempBinary[BIN_NUM_LENGTH] = {0};

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
    while (i != -1) {
        binary[j++] = tempBinary[i--];
    }

    // zarazka oznacuje, kde cislo konci.
    if (j != 0)
        binary[j++] = SEPARATOR;

    binaryNumber.binNumber = binary;

    return binaryNumber;
}

/**
 * Vypise vyslednou frazi.
 */
void printBinNumber(struct BinaryObject binaryNumber) {
    //Cislo v dekadicke soustave 10 je 1010 v binarni soustave.
    printf("Cislo v dekadicke soustave %ld je ", binaryNumber.decNumber);

    int i = 0;
    for (; i <= binaryNumber.length; i++) {
        printf("%d", binaryNumber.binNumber[i]);
    }

    printf(" v binarni soustave.\n");
}

int main(int argc, char *argv[])
{
    int i = -128;
    for (; i < 128; i++) {
        struct BinaryObject binaryNumber = convertDecToBin(i);
        printBinNumber(binaryNumber);
    }

    bool checkArgCount = checkArgumentCount(argc);
    checkArgCount = false;

    if (checkArgCount) {

        bool checkParamInt = checkParam(argv);

        if (checkParamInt) {
            // preved char zadany do argumentu na integer.
            long iDecimal = atol(argv[1]);

            struct BinaryObject binaryNumber = convertDecToBin(iDecimal);

            printBinNumber(binaryNumber);
        }
    }

    return 0;
}
