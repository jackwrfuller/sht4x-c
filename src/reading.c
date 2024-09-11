#include <stdio.h>

#include "reading.h"
#include "sensirion_i2c_hal.h"
#include "sensirion_common.h"
#include "sht4x_i2c.h"
#include "cJSON.h"


void takeReading(Reading* reading) {
    int16_t error = NO_ERROR;
    uint32_t serial_number = 0;
    float a_temperature = 0.0;
    float a_humidity = 0.0;
    
    sensirion_i2c_hal_init();
    sht4x_init(SHT40_I2C_ADDR_44);
    sht4x_soft_reset();
    sensirion_i2c_hal_sleep_usec(10000);

    error = sht4x_serial_number(&serial_number);
    if (error != NO_ERROR) {
        printf("error executing serial_number(): %i\n", error);
        return;
    }
    reading->serial_number = serial_number;

    sensirion_i2c_hal_sleep_usec(20000);
    error = sht4x_measure_lowest_precision(&a_temperature, &a_humidity);
    if (error != NO_ERROR) {
        printf("error executing measure_lowest_precision(): %i\n", error);
    }
    reading->temp = a_temperature;
    reading->humidity = a_humidity;
}


void printReading(Reading* reading) {
    if (reading == NULL) {
        return;
    }
    printf("Temperature: %.2f\n", reading->temp);
    printf("Humidity: %.2f\n", reading->humidity);
    printf("Serial number: %u\n", reading->serial_number);
}

/*
 * Construct a JSON string representation of a Reading struct.
 *
 * Return char* must be deallocated by function caller.
 */
char* readingToJSON(Reading* reading) {
    char* json_string;
    cJSON* temp;
    cJSON* humidity;
    cJSON* serial_number;

    cJSON* reading_json = cJSON_CreateObject();
    
    temp = cJSON_CreateNumber(reading->temp);
    humidity = cJSON_CreateNumber(reading->humidity);
    serial_number = cJSON_CreateNumber(reading->serial_number);

    cJSON_AddItemToObject(reading_json, "temp", temp);
    cJSON_AddItemToObject(reading_json, "humidity", humidity);
    cJSON_AddItemToObject(reading_json, "serial", serial_number);

    json_string = cJSON_PrintUnformatted(reading_json);

    cJSON_Delete(reading_json);
    
    return json_string;
};

