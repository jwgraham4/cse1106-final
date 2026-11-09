//this will be our main src file
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pigpio.h>
#define PIN 17

#define READING_COUNT 5

int turnOnLED() {
    gpioInitialise();
    gpioSetMode(PIN, PI_OUTPUT);
    gpioWrite(PIN, 1);
    getchar();
    gpioWrite(PIN, 0);
    gpioTerminate();
    return 0;
}
double calculateAverage(double readings[], int count) {
    double total = 0;
    for (int i = 0; i < count; i++) {
    total += readings[i];
    }
    return total / count;
}

void writeToFile(double readings[], int count, int warnings[]) {
    FILE* file = fopen("tempurature_log.txt", "a");

    if (file == NULL) {
        printf("Unable to open file\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "Classroom #%d: %s", i + 1, warnings[i] ? "In Range" : "Warning");
    }
    fclose(file);
}

int isOutOfRange(double temperature, int upper, int lower) {
    if (temperature <= upper && temperature >= lower){
        return 1;
    }
    else {
        
        return 0;
    }
}

void checkReadings(double readings[], int count, int warnings[], int upper, int lower) {
    printf("\nReadings\n");
    for (int i = 0; i < count; i++) {
        warnings[i] = isOutOfRange(readings[i], upper, lower);
    }
}

int main(void) {
    double readings[READING_COUNT];
    int warnings[READING_COUNT];
    int upperBound = 76, lowerBound = 68;
    char choice;

    printf("========================================\n");
    printf("LEGACY CLASSROOM TEMPERATURE MONITOR\n");
    printf("========================================\n\n");

    do {
        //read in the values of the room temps
        printf("what are the readings for the classrooms\n");
        for (int i = 0; i < READING_COUNT; i++) {
            printf("class room #%d ", i + 1);
            scanf("%lf", &readings[i]);
        }
        getchar();
        
            
        printf("\nAverage temperature: %.1f F\n",
            calculateAverage(readings, READING_COUNT));
        checkReadings(readings, READING_COUNT, warnings, upperBound, lowerBound);
        

        for (int i = 0; i < READING_COUNT; i++) {
            printf("Classroom %d: ", i + 1);
            if (warnings[i] == 1) {
                printf("In Range\n");
                getchar();
            }
            else {
                printf("Out of Range\n");
                turnOnLED();
            }
        }
        writeToFile(readings, READING_COUNT, warnings);
        
        getchar();
        printf("Do you want to change the range for acceptable temperatures? (y/n): ");
        scanf("%c", &choice);
        getchar();
        if (choice == 'y') {
            printf("Enter the Upper Bound: ");
            scanf("%d", &upperBound);

            printf("Enter the Lower Bound: ");
            scanf("%d", &lowerBound);
        }
        else{
        printf("exiting\n");
        }

   } while (choice == 'y');
    
	
    return 0;
}
