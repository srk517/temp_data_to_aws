#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <cJSON.h>

char* parseIntegerToJSON(const char *parameter1,const char *parameter2, int value1,int value2) {
    
    char valueString1[20];
    char valueString2[20];
    sprintf(valueString1, "%d", value1);
    sprintf(valueString2, "%d", value2);
    // Create a cJSON object to hold the parameter
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, parameter1, valueString1);
    cJSON_AddStringToObject(root, parameter2, valueString2);

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