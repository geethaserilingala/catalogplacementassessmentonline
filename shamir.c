#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cJSON-master\cJSON.h"  

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';
    }

    fclose(file);
    return content;
}

long long convert_to_decimal(const char *value, int base) {
    return strtoll(value, NULL, base);
}

double lagrange_interpolation(int x[], double y[], int k) {
    double c = 0.0;
    for (int i = 0; i < k; i++) {
        double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - x[j]) / (double)(x[i] - x[j]);
            }
        }
        c += term;
    }
    return c;
}

int main() {
    char *file_content = read_file("input.json");
    if (!file_content) {
        return 1;
    }

    cJSON *json = cJSON_Parse(file_content);
    free(file_content);

    if (!json) {
        printf("Error parsing JSON\n");
        return 1;
    }

    cJSON *keys = cJSON_GetObjectItem(json, "keys");
    if (!keys) {
        printf("Error: 'keys' object not found in JSON\n");
        cJSON_Delete(json);
        return 1;
    }

    int n = cJSON_GetObjectItem(keys, "n")->valueint;
    int k = cJSON_GetObjectItem(keys, "k")->valueint;

    int x_values[k];
    double y_values[k];
    int count = 0;

    for (int i = 1; i <= n; i++) {
        char key[10];
        sprintf(key, "%d", i);

        cJSON *item = cJSON_GetObjectItem(json, key);
        if (!item) continue;

        int base = cJSON_GetObjectItem(item, "base")->valueint;
        const char *value_str = cJSON_GetObjectItem(item, "value")->valuestring;
        long long y = convert_to_decimal(value_str, base);

        x_values[count] = i;
        y_values[count] = y;
        count++;


        if (count == k) break;
    }
    double c = lagrange_interpolation(x_values, y_values, k);

    printf("The constant term (secret) is: %.0f\n", c);
    cJSON_Delete(json);
    return 0;
}

