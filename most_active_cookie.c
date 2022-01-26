#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 2048

// Checks if the day (in the Gregorian calendar) is 
// a valid date
// returns 1 if so, and 0 otherwise
// used for format parsing in (main)
int isValidDate(int year, int month, int day) {
    int valid = 0;
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }

    // Months with 31 days
    int longMonths[7] = {1, 3, 5, 7, 8, 10, 12};

    if (day==31) {
        for (int i = 0; i < 7; i++) {
            if (month==longMonths[i]) {
                valid=1;
            }
        }
        return valid;
    }
    
    // check for February (short month)
    // and leap year days
    if (month==2) {
        if (day > 29) {
            return 0;
        }
        else if (day==29 && year%4!=0) {
            return 0;
        }
        return 1;
    }
    else {
        return 1;
    }
}

// Checks if the time (using millitary time) is 
// a valid time
// returns 1 if so, and 0 otherwise
// used for format parsing in (getMostActiveCookies)
int isValidTime(int t0, int t1, int t2, int t3, int t4) {
    int valid = 0;
    if (t0 < 24 && t0 >= 0) {
        if ((t1 < 60 && t1 >= 0) && (t2 < 60 && t2 >= 0) && 
        (t3 < 100 && t3 >= 0) && (t4 < 100 && t4 >= 0)) {
            valid = 1;
        }
    }
    return valid;
}

// count the number of frequencies of each string in an 
// array of strings
// return an array containing the most frequent strings
// used to find the most active cookie
void getFrequentCookie(char **arr, int size, char **freq, int *sizef) {
    int counts[size];
    for (int i = 0; i < size; i++) {
        char *str = arr[i];
        int j = 0;
        while (strcmp(str,arr[j])!=0 && j < i) {
            j++;
        }
        // found a duplicate string 
        if (j < i) {
            counts[j]++;
            counts[i] = 0;
        }
        else {
            counts[i] = 1;
        }
    }

    // find the highest frequency in the list of strings
    int max = 1;
    for (int i = 0; i < size; i++) {
        if (max < counts[i]) {
            max = counts[i];
        }
    }

    // add the most frequent strings to the resulting array
    for (int i = 0; i < size; i++) {
        if (counts[i]==max) {
            strcpy(freq[*sizef],arr[i]);
            (*sizef)++;
        }
    }

}

// returns an array of strings containing the most active cookies
void getMostActiveCookies(char *logFile, int year, int month, int day, char ***cookies, int *total) {
    FILE *file = fopen(logFile,"r");
    if (file==NULL) {
        fprintf(stderr,"An error occured when opening file.\n");
        return;
    }
    char pstr[MAXLEN];
    char* timef;
    int y, m, d, t0, t1, t2, t3, t4;
    int dayPassed = 0;
    int numCookies = 0;
    int sz = 0;
    int lines = 0;

    // count maximum number of cookies in logfile
    // use the count to create an array big enough to 
    // store the cookies for the day
    while (fgets(pstr, MAXLEN, file)!=NULL) {
        sz++;
    }
    char** dailyCookies = (char**) malloc(sizeof(char*) * sz);
    for (int i = 0; i < sz; i++) {
        dailyCookies[i] = (char*) malloc(sizeof(char) * MAXLEN);
    }
    fseek(file, 0, SEEK_SET);

    while (fgets(pstr, MAXLEN, file)!=NULL && dayPassed==0) {
        timef = strtok(pstr, ",");
        timef = strtok(NULL, ",");
        if (timef != NULL) {
           lines = sscanf(timef, "%d-%d-%dT%d:%d:%d+%d:%d\n", &y,&m,&d,&t0,&t1,&t2,&t3,&t4);
        }
        else {
           fprintf(stderr,"File is in bad format.\n");
           for (int i = 0; i < sz; i++) {
               free(dailyCookies[i]);
           }
           free(dailyCookies);
           fclose(file);
           return;
        }
        
        // check that all input was valid
        if (lines==8 && isValidDate(y,m,d) && isValidTime(t0,t1,t2,t3,t4)) {
            if (y==year && m==month && d==day) {
                strcpy(dailyCookies[numCookies], pstr);
                numCookies++;
            }
            else if (y < year || m < month || d < day) {
                dayPassed = 1;
            }
        }
    }

    char** currCookies = (char**) malloc(sizeof(char*) * numCookies);
    for (int i = 0; i < numCookies; i++) {
        currCookies[i] = (char*) malloc(sizeof(char) * MAXLEN);
    }

    for (int i = 0; i < sz; i++) {
        if (i < numCookies) {
            strcpy(currCookies[i],dailyCookies[i]);
        }
        free(dailyCookies[i]);
    }
    free(dailyCookies);

    char** frequentCookies = (char**) malloc(sizeof(char*) * numCookies);
    for (int i = 0; i < numCookies; i++) {
        frequentCookies[i] = (char*) malloc(sizeof(char) * MAXLEN);
    }

    // compute the "mode" of the array of cookie (strings), a.k.a the most active cookie
    getFrequentCookie(currCookies, numCookies, frequentCookies, total);

    *cookies = (char**) malloc(sizeof(char*) * numCookies);
    for (int i = 0; i < *total; i++) {
        (*cookies)[i] = (char*) malloc(sizeof(char) * MAXLEN);
        strcpy((*cookies)[i],frequentCookies[i]);
    }

    // free dynamically allocated memory
    for (int i = 0; i < numCookies; i++) {
        free(currCookies[i]);
        free(frequentCookies[i]);
    }
    free(currCookies);
    free(frequentCookies);
    fclose(file);
}

int main(int argc, char* argv[]) {
    int c = 0;
    int dflag = 0;
    char *date;
    char *logFile;

    // file, switch, and specified date are required, else
    // print Usage and exit
    if (argc != 4) {
        printf("Usage: ./most_active_cookie logfile -d date\n");
        printf("where date is in yyyy-mm-dd format.\n");
        exit(1);
    }

    // scan for date argument
    while ( (c = getopt(argc, argv, "-d:")) != -1) {
        switch (c) {
        case 'd':
            dflag = 1;
            date = optarg;
            break;
        default:
            break;
        }
    }

    // check for valid argument for -d switch
    if (dflag != 1) {
        fprintf(stderr,"Date should be specified with -d option.\n");
        return 0;
    }

    if (strcmp(argv[1],"-d") != 0) {
        logFile = argv[1];
    }
    else {
        logFile = argv[3];
    }

    int month;
    int day;
    int year;
    int f = sscanf(date,"%d-%d-%d",&year,&month,&day);
    // check if date entered was valid
    if (f != 3 || strlen(date)!=10 || isValidDate(year, month, day)==0 ) {
        fprintf(stderr,"Date must be a valid yyyy-mm-dd format.\n");
        exit(1);
    }
    char** cookies = NULL;
    int total = 0;

    getMostActiveCookies(logFile, year, month, day, &cookies, &total);

    // print most active cookies to standard output
    for (int i = 0; i < total; i++) {
        printf("%s\n", cookies[i]);
        free(cookies[i]);
    }
    if (cookies != NULL) {
        free(cookies);
    }

    return 0;
}