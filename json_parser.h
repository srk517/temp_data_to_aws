#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <cJSON.h>

char* parseIntegerToJSON(const char *parameter, int value) {
    
    char valueString[20];
    sprintf(valueString, "%d", value);
    
    // Create a cJSON object to hold the parameter
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, parameter, valueString);

    // Convert the cJSON object to a string
    char *jsonString = cJSON_PrintUnformatted(root);

    // Print the JSON string
    //printf("JSON String: %s\n", jsonString);
    
    // Insert an escaped double quote before the temperature value
   
    
     return jsonString;

    // Clean up memory
    cJSON_Delete(root);
    free(jsonString);
}
#endif