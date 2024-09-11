#include <inttypes.h>

typedef struct {
    uint32_t serial_number;
    float temp;
    float humidity;
} Reading;

void takeReading(Reading *);
void printReading(Reading *);
char* readingToJSON(Reading *);
