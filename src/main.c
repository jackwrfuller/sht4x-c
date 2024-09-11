#include "reading.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Reading reading;
    takeReading(&reading);
    printReading(&reading);
    
    char* json = readingToJSON(&reading);
    printf("%s\n", json);
    free(json);
    return 0;
}



