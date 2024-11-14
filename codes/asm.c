/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MAX_LABEL_SIZE 100
#define MAX_OPCODES 20
#define INITIAL_CAPACITY 10
#define MAX_MACHINE_CODE_SIZE 256
#define MAX_FILE_NAME_SIZE 256
#define MAX_LINE_SIZE 512
#define MAX_OPERAND_SIZE 50
#define MAX_CLEANED_LINES 1000
/*#define MAX_LABEL_SIZE 50*/
#define MAX_STR_SIZE 100

#define MAX_OPCODE_SIZE 10

#define MAX_ERRORS 100
#define MAX_LINES 1000

typedef struct {
    int code;         /*/ Error code or priority level*/
    char message[256]; /*/ Error message*/
} ErrorType;

struct Label {
    char name[MAX_LABEL_SIZE];
    int line;
} labels[MAX_CLEANED_LINES];
int labels_size = 0;
/*// typedef struct {
//     char label[MAX_LABEL_SIZE];
//     int index;
// } Label;*/
struct Data {
    char label[MAX_LABEL_SIZE];
    char mnemonic[MAX_OPERAND_SIZE];
    char operand[MAX_OPERAND_SIZE];
    int oprType;
    bool labelPresent;
} data[MAX_CLEANED_LINES];
int data_size = 0;


/*struct Table {
    char label[MAX_LABEL_SIZE];
    char mnemonic[MAX_OPCODE_SIZE];
    char operand[MAX_OPERAND_SIZE];
    int oprType;
    bool labelPresent;
};*/

struct OPCEntry {
    char opcode[MAX_OPCODE_SIZE];
    int operandType;
};

struct Error {
    int line;
    char message[100];
};

/*/ Global variables*/
/*struct Table data[MAX_LINES];*/
/*int data_size = 0;*/

struct OPCEntry OPC[20]; /*/ Assuming 20 mnemonics as example*/
int opc_size = 0;

struct Error errors[MAX_ERRORS];
int error_count = 0;





char* generateMachineCode(int lineIndex);

typedef struct {
    int line;
    char machineCode[50];
} MachineCodeEntry;
int machineCode_size = 0;
MachineCodeEntry machineCode[MAX_LINES];
/*/ Comparison function for `qsort`*/
int compareErrors(const void *a, const void *b) {
    /*/ Cast void pointers to ErrorType pointers*/
    const ErrorType *errorA = (const ErrorType *)a;
    const ErrorType *errorB = (const ErrorType *)b;

    /*/ Compare error codes*/
    if (errorA->code < errorB->code) {
        return -1; /*/ `errorA` comes before `errorB`*/
    } else if (errorA->code > errorB->code) {
        return 1; /*/ `errorA` comes after `errorB`*/
    } else {
        /*/ If error codes are the same, compare messages lexicographically*/
        return strcmp(errorA->message, errorB->message);
    }
}


/*/ Global variables (they would have to be declared at the top of your C program)*/
char fileName[MAX_FILE_NAME_SIZE];
char cleaned[MAX_CLEANED_LINES][MAX_OPERAND_SIZE]; /*/ cleaned lines*/
int cleaned_size = 0;
bool haltPresent = false;
int programCounter[MAX_CLEANED_LINES];
int labelsss[MAX_CLEANED_LINES];
int pc = 0;


/*/ Dynamic array to store strings*/
char **temp = NULL;
int temp_size = 0;
int temp_capacity = INITIAL_CAPACITY;

/*/ Label storage*/
struct Label *labelss = NULL;
/*int labels_size = 0;*/
int labels_capacity = INITIAL_CAPACITY;


struct Data *datas = NULL;
int *programCounters = NULL;
struct Data data[1000];

char *decToHex(int number, int add) {
    static char hex[20];
    if (add == 32) {
        snprintf(hex, sizeof(hex), "%X", (unsigned int)number);
        return hex;
    }
    if (number < 0) number += (1 << add);
    snprintf(hex, sizeof(hex), "%X", number);
    return hex;
}

void init() {
    /*/ Initialization of mnemonic and OPCode mappings*/
    OPC[opc_size++] = (struct OPCEntry){"", 1};
    OPC[opc_size++] = (struct OPCEntry){"00", 1}; /*/ ldc*/
    OPC[opc_size++] = (struct OPCEntry){"01", 1}; /*/ adc*/
    OPC[opc_size++] = (struct OPCEntry){"02", 2}; /*/ ldl*/
    OPC[opc_size++] = (struct OPCEntry){"03", 2}; /*/ stl*/
    OPC[opc_size++] = (struct OPCEntry){"04", 2}; /*/ ldnl*/
    OPC[opc_size++] = (struct OPCEntry){"05", 2}; /*/ stnl*/
    OPC[opc_size++] = (struct OPCEntry){"06", 0}; /*/ add*/
    OPC[opc_size++] = (struct OPCEntry){"07", 0}; /*/ sub*/
    OPC[opc_size++] = (struct OPCEntry){"08", 0}; /*/ shl*/
    OPC[opc_size++] = (struct OPCEntry){"09", 0}; /*/ shr*/
    OPC[opc_size++] = (struct OPCEntry){"0A", 1}; /*/ adj*/
    OPC[opc_size++] = (struct OPCEntry){"0B", 0}; /*/ a2sp*/
    OPC[opc_size++] = (struct OPCEntry){"0C", 0}; /*/ sp2a*/
    OPC[opc_size++] = (struct OPCEntry){"0D", 2}; /*/ call*/
    OPC[opc_size++] = (struct OPCEntry){"0E", 0}; /*/ return*/
    OPC[opc_size++] = (struct OPCEntry){"0F", 2}; /*/ brz*/
    OPC[opc_size++] = (struct OPCEntry){"10", 2}; /*/ brlz*/
    OPC[opc_size++] = (struct OPCEntry){"11", 2}; /*/ br*/
    OPC[opc_size++] = (struct OPCEntry){"12", 0}; /*/ HALT*/
    OPC[opc_size++] = (struct OPCEntry){"", 1};   /*/ SET*/
}

void genErrors(int line, const char *type) {
    if (error_count < MAX_ERRORS) {
        errors[error_count].line = line + 1;
        snprintf(errors[error_count].message, sizeof(errors[error_count].message), "Error at line: %d -- Type: %s", line, type);
        error_count++;
    }
}

char *clean(char *s, int line) {
    static char temp[MAX_STR_SIZE];
    int i, j, len;
    int temp_index = 0;

    /*/ Reverse and trim leading spaces or tabs from both ends*/
    int r;
    for (r = 0; r < 2; r++) {
        len = strlen(s);
        for (i = len - 1; i >= 0 && (s[i] == ' ' || s[i] == '\t'); i--);
        s[i + 1] = '\0';
        for (i = strlen(s) - 1, j = 0; i >= 0; i--, j++) temp[j] = s[i];
        temp[j] = '\0';
        strcpy(s, temp);
    }

    temp[0] = '\0';  /*/ Reset temp for building cleaned string*/

    /*/ Remove comments, and normalize spaces*/
    len = strlen(s);
    for (i = 0; i < len; i++) {
        if (s[i] == ';') break;  /*/ Stop at comment symbol*/

        /*/ If colon, add ':' and add a space if not followed by one*/
        if (s[i] == ':') {
            temp[temp_index++] = ':';
            if (i == len - 1 || s[i + 1] != ' ') temp[temp_index++] = ' ';
            continue;
        }

        /*/ If character is not space or tab, add it to temp*/
        if (s[i] != ' ' && s[i] != '\t') {
            temp[temp_index++] = s[i];
            continue;
        }

        /*/ Add a single space and skip consecutive spaces/tabs*/
        temp[temp_index++] = ' ';
        while (s[i] == ' ' || s[i] == '\t') i++;
        i--;
    }

    /*/ Trim trailing spaces in temp*/
    while (temp_index > 0 && (temp[temp_index - 1] == ' ' || temp[temp_index - 1] == '\t'))
        temp[--temp_index] = '\0';

    /*/ Count spaces and generate error if more than 2 are found*/
    int spac = 0;
    for (i = 0; i < temp_index; i++)
        if (temp[i] == ' ') spac++;
    if (spac > 2) genErrors(line + 1, "Invalid syntax");

    temp[temp_index] = '\0';  /*/ Null-terminate*/
    return temp;
}

/*/ Function to push SET instructions to an array of strings*/
void pushSETinstructions(char ***temp, int *temp_size, const char *token, const char *s, int j) {
    /*/ Check if the string is large enough to avoid out-of-bounds access*/
    if (strlen(s) <= j + 5) {
        return;
    }

    /*/ Reallocate memory to store more instructions*/
    *temp = realloc(*temp, (*temp_size + 7) * sizeof(char*));

    /*/ Ensure memory allocation was successful*/
    if (*temp == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    /*/ Add instructions to the array*/
    (*temp)[*temp_size] = strdup("adj 10000");             /*/ Stack pointer at random memory*/
    (*temp_size)++;
    
    (*temp)[*temp_size] = strdup("stl -1");                 /*/ Here to load A*/
    (*temp_size)++;
    
    (*temp)[*temp_size] = strdup("stl 0");                  /*/ Here B*/
    (*temp_size)++;
    
    char ldc_operand[100];                                  /*/ Prepare space for ldc operand*/
    snprintf(ldc_operand, sizeof(ldc_operand), "ldc %s", s + j + 6); /*/ Extract value from s*/
    (*temp)[*temp_size] = strdup(ldc_operand);
    (*temp_size)++;

    snprintf(ldc_operand, sizeof(ldc_operand), "ldc %.*s", j, token); /*/ Extract token prefix*/
    (*temp)[*temp_size] = strdup(ldc_operand);
    (*temp_size)++;

    (*temp)[*temp_size] = strdup("stnl 0");                 /*/ Load A, B*/
    (*temp_size)++;
    
    (*temp)[*temp_size] = strdup("ldl 0");                  /*/ Load A*/
    (*temp_size)++;
    
    (*temp)[*temp_size] = strdup("ldl -1");                 /*/ Load B*/
    (*temp_size)++;

    (*temp)[*temp_size] = strdup("adj -10000");             /*/ Adjust stack pointer*/
    (*temp_size)++;
}

/*/ Helper function to free dynamically allocated memory for temp*/
void freeInstructions(char **temp, int temp_size) {
    int i;
    for ( i = 0; i < temp_size; i++) {
        free(temp[i]);
    }
    free(temp);
}

void implementSET(char **cleaned, int cleaned_size) {
    temp = (char **)malloc(temp_capacity * sizeof(char*));
    labelss = (struct Label *)malloc(labels_capacity * sizeof(struct Label));
    int i;
    for ( i = 0; i < cleaned_size; i++) {
        char *line = cleaned[i];
        char cur[MAX_LABEL_SIZE] = "";
        bool state = false;
        int j;
        for ( j = 0; j < (int)strlen(line); j++) {
            strncat(cur, &line[j], 1); /*/ Append character to cur*/

            if (line[j] == ':') {
                cur[strlen(cur) - 1] = '\0'; /*/ Remove ':' from label*/

                if (strlen(line) > j + 5 && strncmp(line + j + 2, "SET", 3) == 0) {
                    state = true;
                    int label_index = findLabelIndex(cur);

                    if (label_index >= 0 && abs(labelss[label_index].line) == i) {
                        labels[label_index].line = temp_size - 1;

                        char data_line[MAX_LINE_SIZE];
                        snprintf(data_line, sizeof(data_line), "%.*s data %s", j + 1, line, line + j + 6);
                        addLine(data_line);
                    } else {
                        pushSETinstructions(&temp, &temp_size, cur, line, j);
                    }
                    break;
                }
            }
        }

        if (!state && strlen(line) > 0) {
            addLine(line);
        }
    }

    /*/ Replace cleaned with temp contents*/
    /*int i;*/
    for ( i = 0; i < temp_size && i < cleaned_size; i++) {
        strncpy(cleaned[i], temp[i], MAX_OPERAND_SIZE - 1);
        cleaned[i][MAX_OPERAND_SIZE - 1] = '\0';
    }
}void addLine(char *line) {
    if (temp_size >= temp_capacity) {
        temp_capacity *= 2;
        temp = realloc(temp, temp_capacity * sizeof(char*));
    }
    temp[temp_size++] = strdup(line);
}

int findLabelIndex(const char *label) {
    int i;
    for ( i = 0; i < labels_size; i++) {
        if (strcmp(labelss[i].name, label) == 0) {
            return i;
        }
    }
    return -1;
}

void addOrUpdateLabel(const char *label, int index) {
    int label_index = findLabelIndex(label);
    if (label_index >= 0) {
        labelss[label_index].line = index;
    } else {
        if (labels_size >= labels_capacity) {
            labels_capacity *= 2;
            labelss = realloc(labels, labels_capacity * sizeof(struct Label));
        }
        strncpy(labelss[labels_size].name, label, MAX_LABEL_SIZE - 1);
        labelss[labels_size].line = index;
        labels_size++;
    }
}

/*/ Remember to free temp and labels when done*/
/*void freeInstructions(char **temp, int temp_size) {
    for (int i = 0; i < temp_size; i++) {
        free(temp[i]);
    }
    free(temp);
}*/

void freeLabels(struct Label *labels) {
    free(labels);
}



void processLabel() {
    char cur[MAX_LABEL_SIZE];
    int i, j;

    for (i = 0; i < cleaned_size; i++) {
        cur[0] = '\0';
        int cur_index = 0;

        for (j = 0; cleaned[i][j] != '\0'; j++) {
            if (cleaned[i][j] == ':') {
                if (!validName(cur)) {
                    genErrors(i + 1, "Invalid label name");
                    break;
                }

                bool found = false;
                int k;
                for ( k = 0; k < labels_size; k++) {
                    if (strcmp(labels[k].name, cur) == 0) {
                        found = true;
                        if (strstr(&cleaned[i][j + 2], "SET") == &cleaned[i][j + 2]) {
                            continue;
                        }
                        if (strstr(&cleaned[i][j + 2], "data") == &cleaned[i][j + 2] && labels[k].line < 0) {
                            labels[k].line = i;
                            continue;
                        }
                        genErrors(i + 1, "Multiple declaration of label");
                    }
                }

                if (!found) {
                    if (strstr(&cleaned[i][j + 2], "SET") == &cleaned[i][j + 2]) {
                        labels[labels_size].line = -i;
                    } else {
                        labels[labels_size].line = i;
                    }
                    strcpy(labels[labels_size++].name, cur);
                }
                break;
            }
            cur[cur_index++] = cleaned[i][j];
            cur[cur_index] = '\0';
        }
    }
}

void fillData(int i, const char *one, const char *two, const char *three, int type) {
    strncpy(data[i].label, one, MAX_LABEL_SIZE);
    strncpy(data[i].mnemonic, two, MAX_OPERAND_SIZE);
    strncpy(data[i].operand, three, MAX_OPERAND_SIZE);
    data[i].oprType = type;
    if (i >= data_size) data_size = i + 1;
}

int calType(char *s) {
    if (s[0] == '\0') return 0;

    if (s[0] == '+' || s[0] == '-') {
        memmove(s, s + 1, strlen(s));
    }

    if (s[0] == '\0') return -1;
    if (isDecimal(s)) return 10;
    if (isOctal(s)) return 8;
    if (isHexadecimal(s)) return 16;
    if (validName(s)) return 1;

    return -1;
}

void tableForm() {
    char ans[4][MAX_OPERAND_SIZE];
    int i, j, k, ptr;

    for (i = 0; i < cleaned_size; i++) {
        /*/ Initialize ans array and current token buffer*/
        for (j = 0; j < 4; j++) strcpy(ans[j], "");
        char cur[MAX_OPERAND_SIZE] = "";
        ptr = 1;

        for (j = 0; cleaned[i][j] != '\0'; j++) {
            if (cleaned[i][j] == ':') {
                strcpy(ans[0], cur);
                strcpy(cur, "");
                j++;
                continue;
            } else if (cleaned[i][j] == ' ') {
                strcpy(ans[ptr++], cur);
                strcpy(cur, "");
                continue;
            }
            strncat(cur, &cleaned[i][j], 1);
            if (cleaned[i][j + 1] == '\0') strcpy(ans[ptr++], cur);
        }

        /*/ Check if label is present*/
        data[i].labelPresent = (strlen(ans[1]) > 0);

        /*/ Check if mnemonic is HALT*/
        if (strcmp(ans[1], "HALT") == 0) haltPresent = true;

        /*/ Set label address in the labels array if label exists*/
        if (strlen(ans[0]) > 0) labelsss[i] = pc;

        /*/ Update program counter for current instruction*/
        programCounter[i] = pc;

        /*/ If no mnemonic is present, fillData with empty values and continue*/
        if (ptr == 1) {
            fillData(i, ans[0], "", "", 0);
            continue;
        }
        pc++;

        /*/ Validate and store mnemonic and operand in data structure*/
        if (!isOpcodeValid(ans[1])) {
            genErrors(i + 1, "Invalid Mnemonic");
            continue;
        }

        if (expectedOperandCount(ans[1]) != ptr - 2) {
            genErrors(i + 1, "Invalid OPCode-Syntax combination");
            continue;
        }
        fillData(i, ans[0], ans[1], ans[2], calType(ans[2]));

        if (data[i].oprType == 1 && !labelExists(data[i].operand)) {
            genErrors(i + 1, "No such label / data variable");
        } else if (data[i].oprType == -1) {
            genErrors(i + 1, "Invalid number");
        }
    }
}

void makeDataSegment() {
    char instr[MAX_CLEANED_LINES][MAX_OPERAND_SIZE];
    char dataseg[MAX_CLEANED_LINES][MAX_OPERAND_SIZE];
    int instr_size = 0, dataseg_size = 0;
    int i, j;

    for (i = 0; i < cleaned_size; i++) {
        bool state = false;

        for (j = 0; cleaned[i][j] != '\0'; j++) {
            if (strncmp(&cleaned[i][j], "data", 4) == 0 && j + 4 < strlen(cleaned[i])) {
                strcpy(dataseg[dataseg_size++], cleaned[i]);
                state = true;
                break;
            }
            if (cleaned[i][strlen(cleaned[i]) - 1] == ':' &&
                i + 1 < cleaned_size && strncmp(cleaned[i + 1], "data", 4) == 0) {
                strcpy(dataseg[dataseg_size++], cleaned[i]);
                state = true;
                break;
            }
        }
        if (!state) {
            strcpy(instr[instr_size++], cleaned[i]);
        }
    }

    /*/ Combine instr and dataseg back into cleaned in proper order*/
    for (i = 0; i < instr_size; i++) {
        strcpy(cleaned[i], instr[i]);
    }
    for (j = 0; j < dataseg_size; j++) {
        strcpy(cleaned[instr_size + j], dataseg[j]);
    }
    cleaned_size = instr_size + dataseg_size;
}


/*/ void firstPass() {
//     FILE *infile;
//     char line[MAX_LINE_SIZE];
    
//     printf("Enter ASM file name to assemble:\n");
//     scanf("%s", fileName);
    
//     infile = fopen(fileName, "r");
//     if (infile == NULL) {
//         printf("Input file doesn't exist, please make sure the file is in the same directory as the code!\n");
//         exit(0);
//     }
    
//     while (fgets(line, sizeof(line), infile)) {
//         char *cur = clean(line, cleaned_size);  // Assuming `clean()` is defined elsewhere
//         strcpy(cleaned[cleaned_size++], cur);
//     }
    
//     fclose(infile);
    
//     init();  // Initializing the mnemonics (assuming `init()` is defined elsewhere)
//     processLabel();  // Process labels (assuming `processLabel()` is defined elsewhere)

//     if (errors.size() == 0) {
//         implementSET();  // Implementing SET mnemonic (assuming `implementSET()` is defined elsewhere)
//     }

//     // Memory allocation (dynamically resizing arrays would have to be handled)
//     data = realloc(data, cleaned_size * sizeof(struct Data)); // Assuming `data` is a global array of `Data` structures
//     programCounter = realloc(programCounter, cleaned_size * sizeof(int));

//     makeDataSegment();  // Separates code into data segment and code (assuming `makeDataSegment()` is defined elsewhere)
//     tableForm();  // Converts the code into table form (assuming `tableForm()` is defined elsewhere)
// }*/



void firstPass() {
    FILE *infile;
    char fileName[100];
    char line[MAX_LINE_SIZE];

    printf("Enter ASM file name to assemble:\n");
    scanf("%s", fileName);
    
    infile = fopen(fileName, "r");
    if (infile == NULL) {
        printf("Input file doesn't exist, please make sure the file is in the same directory as the code!\n");
        exit(0);  /*/ Exit if the file cannot be opened*/
    }

    while (fgets(line, sizeof(line), infile)) {
        char *cur = clean(line, cleaned_size);  /*/ Clean the line and store it*/
        strcpy(cleaned[cleaned_size++], cur);
    }
    fclose(infile);  /*/ Close the input file after reading*/

    init();
    processLabel();

    if (error_count == 0) {
        implementSET(cleaned, cleaned_size);
    }

    /*/ Dynamically allocate memory for data and programCounter based on cleaned_size*/
    datas = (struct Data*)realloc(data, cleaned_size * sizeof(struct Data));
    programCounters = (int*)realloc(programCounters, cleaned_size * sizeof(int));

    makeDataSegment();
    tableForm();

    /*/ Now populate the machineCode array and update machineCode_size*/
    machineCode_size = 0;  /*/ Reset machineCode_size before starting to populate it*/
    int i;
    for (i = 0; i < cleaned_size; i++) {
        /*/ Generate machine code for this line*/
        char *generatedMachineCode = generateMachineCode(cleaned[i]);

        if (generatedMachineCode != NULL) {
            /*/ Store line number*/
            machineCode[machineCode_size].line = i;

            /*/ Store generated machine code (ensure null termination)*/
            strncpy(machineCode[machineCode_size].machineCode, generatedMachineCode, sizeof(machineCode[machineCode_size].machineCode) - 1);
            machineCode[machineCode_size].machineCode[sizeof(machineCode[machineCode_size].machineCode) - 1] = '\0';  /*/ Ensure null termination*/

            machineCode_size++;  /*/ Increment the size of machineCode array*/
        } else {
            printf("Error: Invalid instruction on line %d.\n", i);
        }
    }
}
/*/ bool seeErrors() {
//     FILE *outErrors = fopen("logFile.log", "w");
//     if (outErrors == NULL) {
//         printf("Unable to open log file for writing.\n");
//         return false;
//     }

//     fprintf(outErrors, "Log code generated in: logFile.log\n");

//     if (errors.size() == 0) {
//         printf("No errors found!\n");
//         if (!haltPresent) {
//             printf("1 warning detected\n");
//             fprintf(outErrors, "Warning: HALT not present!\n");
//         }
//         fprintf(outErrors, "Machine code generated in: machineCode.o\n");
//         fprintf(outErrors, "Listing code generated in: listCode.l\n");
//         fclose(outErrors);
//         return true;
//     }

//     // Sort the errors (C does not have `std::sort`, but we can use qsort or implement sorting ourselves)
//     qsort(errors, errors.size(), sizeof(ErrorType), compareErrors);  // You would need to define `compareErrors`

//     printf("%d errors encountered! See logFile.log\n", errors.size());

//     // Output errors
//     for (int i = 0; i < errors.size(); i++) {
//         fprintf(outErrors, "%s\n", errors[i].second);
//     }

//     fclose(outErrors);
//     return false;
// }
 // for reverse function (if using C++), or implement manually in C
 // Example implementation of addError*/
void addError(int line, const char* message) {
    if (error_count < MAX_ERRORS) {
        errors[error_count].line = line;
        strcpy(errors[error_count].message, message);
        error_count++;
    }
}

/*/ Compares errors for sorting by line number*/

bool seeErrors() {
    FILE *outErrors = fopen("logFile.log", "w");
    if (outErrors == NULL) {
        printf("Unable to open log file for writing.\n");
        return false;
    }

    fprintf(outErrors, "Log code generated in: logFile.log\n");

    if (error_count == 0) {
        printf("No errors found!\n");
        if (!haltPresent) {
            printf("1 warning detected\n");
            fprintf(outErrors, "Warning: HALT not present!\n");
        }
        fprintf(outErrors, "Machine code generated in: machineCode.o\n");
        fprintf(outErrors, "Listing code generated in: listCode.l\n");
        fclose(outErrors);
        return true;
    }

    fprintf(outErrors, "Errors found!\n");
    fclose(outErrors);
    return false;
}



/*/ Small function to append zero at the end to make it of 24-bit
// Example: "F" changes to "00000F"*/
char* appZero(char* s, int sz) {
    int len = strlen(s);
    
    /*/ If the string length is less than `sz`, pad with zeros*/
    while (len < sz) {
        strcat(s, "0");
        len++;
    }

    /*/ Reverse the string to append zeros to the right*/
    char* reversed = (char*)malloc(sizeof(char) * (len + 1));
    int i;
    for ( i = 0; i < len; i++) {
        reversed[i] = s[len - i - 1];
    }
    reversed[len] = '\0';

    /*/ Reverse the string back to original order*/
    /*int i;*/
    for ( i = 0; i < len; i++) {
        s[i] = reversed[len - i - 1];
    }
    s[len] = '\0';

    free(reversed);
    return s;
}
int findOPCIndex(const char* mnemonic) {
    int i;
    for ( i = 0; i < 20; i++) {  /*/ Assuming OPC has 20 entries*/
        if (strcmp(OPC[i].opcode, mnemonic) == 0) {
            return i;
        }
    }
    return -1;  /*/ Not found*/
}

/*/ void secondPass() {
//     // Second pass of the assembler that generates machine code and listing code
//     int i;

//     for (i = 0; i < data_size; i++) {  // Assuming `data_size` holds the number of data entries
//         if (strlen(cleaned[i]) == 0) {
//             continue;
//         }

//         char location[10];
//         strcpy(location, appZero(decToHex(programCounter[i], 8), 8)); // Assuming `appZero` and `decToHex` are defined

//         if (strcmp(data[i].mnemonic, "") == 0) {
//             // Blank line, no machine code
//             machineCode[i] = (MachineCodeEntry){i, "        "};  // Assuming `MachineCodeEntry` structure is defined
//             continue;
//         }

//         if (data[i].oprType == 1) {
//             int decForm;
//             if (OPC[data[i].mnemonic].operandType == 2) {
//                 int val = labels[data[i].operand];
//                 decForm = val - (programCounter[i] + 1);
//             } else {
//                 decForm = labels[data[i].operand];
//             }

//             char curMacCode[20];
//             sprintf(curMacCode, "%s%s", appZero(decToHex(decForm,24), 6), OPC[data[i].mnemonic].first);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         } else if (data[i].oprType == 0) {
//             char curMacCode[20];
//             sprintf(curMacCode, "000000%s", OPC[data[i].mnemonic].first);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         } else {
//             int sz = 6, add = 24;
//             if (strcmp(data[i].mnemonic, "data") == 0) {
//                 sz = 8;
//                 add = 32;
//             }
//             int decForm = strtol(data[i].operand, NULL, data[i].oprType);
//             char curMacCode[20];
//             sprintf(curMacCode, "%s%s", appZero(decToHex(decForm, add), sz), OPC[data[i].mnemonic].first);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         }
//     }
// }*/

int getLabelAddress(const char* labelName) {
    int i;
    for ( i = 0; i < labels_size; i++) {
        if (strcmp(labels[i].name, labelName) == 0) {
            return labels[i].line;
        }
    }
    /*/ Return -1 if label is not found; handle this case in secondPass*/
    return -1;
}


/*/ void secondPass() {
//     int i;
//     for (i = 0; i < data_size; i++) {  // `data_size` holds the number of data entries
//         if (strlen(cleaned[i]) == 0) {
//             continue;
//         }

//         char location[10];
//         strcpy(location, appZero(decToHex(programCounter[i], 8), 8));

//         if (strcmp(data[i].mnemonic, "") == 0) {
//             machineCode[i] = (MachineCodeEntry){i, "        "};
//             continue;
//         }

//         int opcIndex = findOPCIndex(data[i].mnemonic);
//         if (opcIndex == -1) {
//             printf("Error: Unknown mnemonic '%s'\n", data[i].mnemonic);
//             continue;
//         }

//         if (data[i].oprType == 1) {
//             int decForm;
//             if (OPC[opcIndex].operandType == 2) {
//                 int val = labels[data[i].operand];
//                 decForm = val - (programCounter[i] + 1);
//             } else {
//                 decForm = labels[data[i].operand];
//             }

//             char curMacCode[20];
//             sprintf(curMacCode, "%s%s", appZero(decToHex(decForm, 24), 6), OPC[opcIndex].opcode);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         } else if (data[i].oprType == 0) {
//             char curMacCode[20];
//             sprintf(curMacCode, "000000%s", OPC[opcIndex].opcode);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         } else {
//             int sz = 6, add = 24;
//             if (strcmp(data[i].mnemonic, "data") == 0) {
//                 sz = 8;
//                 add = 32;
//             }
//             int decForm = strtol(data[i].operand, NULL, data[i].oprType);
//             char curMacCode[20];
//             sprintf(curMacCode, "%s%s", appZero(decToHex(decForm, add), sz), OPC[opcIndex].opcode);
//             machineCode[i] = (MachineCodeEntry){i, curMacCode};
//         }
//     }
// }*/
void secondPass() {
    int i;
    for (i = 0; i < data_size; i++) {  
        if (strlen(cleaned[i]) == 0) {
            continue;
        }

        char location[10];
        strcpy(location, appZero(decToHex(programCounter[i], 8), 8));

        if (strcmp(data[i].mnemonic, "") == 0) {
            machineCode[i] = (MachineCodeEntry){i, "        "};
            continue;
        }

        int opcIndex = findOPCIndex(data[i].mnemonic);
        if (opcIndex == -1) {
            printf("Error: Unknown mnemonic '%s'\n", data[i].mnemonic);
            continue;
        }

        if (data[i].oprType == 1) {
            int decForm;
            int operandAddress = getLabelAddress(data[i].operand);
            if (operandAddress == -1) {
                printf("Error: Undefined label '%s'\n", data[i].operand);
                continue;
            }

            if (OPC[opcIndex].operandType == 2) {
                decForm = operandAddress - (programCounter[i] + 1);
            } else {
                decForm = operandAddress;
            }

            char curMacCode[20];
            sprintf(curMacCode, "%s%s", appZero(decToHex(decForm, 24), 6), OPC[opcIndex].opcode);
            machineCode[i] = (MachineCodeEntry){i, curMacCode};
        } else if (data[i].oprType == 0) {
            char curMacCode[20];
            sprintf(curMacCode, "000000%s", OPC[opcIndex].opcode);
            machineCode[i] = (MachineCodeEntry){i, curMacCode};
        } else {
            int sz = 6, add = 24;
            if (strcmp(data[i].mnemonic, "data") == 0) {
                sz = 8;
                add = 32;
            }
            int decForm = strtol(data[i].operand, NULL, data[i].oprType);
            char curMacCode[20];
            sprintf(curMacCode, "%s%s", appZero(decToHex(decForm, add), sz), OPC[opcIndex].opcode);
            machineCode[i] = (MachineCodeEntry){i, curMacCode};
        }
    }
}

void writeToFile() {
    /*/ Writes machine code and listing code to files*/
    FILE *outList = fopen("listCode.l", "w");
    if (outList == NULL) {
        printf("Error opening listing code file.\n");
        return;
    }

    int i;
    for (i = 0; i < machineCode_size; i++) {  
        /*/ Assuming `machineCode_size` holds the number of entries*/
        fprintf(outList, "%s %s %s\n", appZero(decToHex(programCounter[machineCode[i].line], 24), 6), 
                machineCode[i].machineCode, cleaned[machineCode[i].line]);
    }

    fclose(outList);

    FILE *outMachineCode = fopen("machineCode.o", "wb");
    if (outMachineCode == NULL) {
        printf("Error opening machine code file.\n");
        return;
    }

    for (i = 0; i < machineCode_size; i++) {
        if (strlen(machineCode[i].machineCode) == 0 || strcmp(machineCode[i].machineCode, "        ") == 0) {
            continue;
        }

        unsigned int x;
        sscanf(machineCode[i].machineCode, "%x", &x);
        fwrite(&x, sizeof(unsigned int), 1, outMachineCode);
    }

    fclose(outMachineCode);

    printf("Log code generated in: logFile.log\n");
    printf("Machine code generated in: machineCode.o\n");
    printf("Listing code generated in: listCode.l\n");
}




int main() {
    /*/ Calling required functions*/
    firstPass();
    /*if (seeErrors()) {*/
        secondPass();
        writeToFile();
    /*}*/
    system("pause");
    return 0;
}

/*/ Function to check whether the string is in octal form or not///
// bool isOctal(const char *s) {
//     if (strlen(s) < 2) {
//         return false;  // Octal numbers must have at least two characters (starting with '0')/
//     }
    
//     bool ok = true;
//     int i;
//     for (i = 1; i < strlen(s); i++) {
//         ok &= (s[i] >= '0' && s[i] <= '7');  // Check if each character is in range '0'-'7'/
//     }
    
//     return ok && (s[0] == '0');  // Octal numbers start with '0'/
// }

// // Function to check whether the string is in hexadecimal form or not/
// bool isHexadecimal(const char *s) {
//     if (strlen(s) < 3) {
//         return false;  // Hexadecimal numbers must have at least three characters (0x prefix)/
//     }

//     bool state = (s[0] == '0') && (s[1] == 'x' || s[1] == 'X');  // Check if it starts with '0x' or '0X'/
//     int i;
//     for (i = 2; i < strlen(s); i++) {
//         bool st = (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F');
//         state &= st;  // All characters after prefix must be valid hexadecimal digits/
//     }

//     return state;
// }

// // Function to check whether the string is in decimal form or not/
// bool isDecimal(const char *s) {
//     int i;
//     for (i = 0; i < strlen(s); i++) {
//         if (!(s[i] >= '0' && s[i] <= '9')) {
//             return false;  // If any character is not between '0' and '9', it is not a decimal number/
//         }
//     }
//     return true;
// }

// // Check whether the label name is valid. Example: 1var is invalid, val1 is valid/
// bool validName(const char *cur) {
//     int i;
//     if (!((cur[0] >= 'a' && cur[0] <= 'z') || (cur[0] >= 'A' && cur[0] <= 'Z') || (cur[0] == '_'))) {
//         return false;  // First character must be a letter or underscore/
//     }

//     for (i = 1; i < strlen(cur); i++) {
//         char to = cur[i];
//         if (!((to >= 'a' && to <= 'z') || (to >= 'A' && to <= 'Z') || (to >= '0' && to <= '9') || (to == '_'))) {
//             return false;  // Remaining characters must be alphanumeric or underscore/
//         }
//     }
    
//     return true;
// }*/


int isOctal(const char *s) {
    if (strlen(s) < 2) {
        return 0;  /*/ Octal numbers must have at least two characters (starting with '0')*/
    }

    int ok = 1;
    int i;
    for (i = 1; i < strlen(s); i++) {
        ok &= (s[i] >= '0' && s[i] <= '7');  /*/ Check if each character is in range '0'-'7'*/
    }

    return ok && (s[0] == '0');  /*/ Octal numbers start with '0'*/
}

int isHexadecimal(const char *s) {
    if (strlen(s) < 3) {
        return 0;  /*/ Hexadecimal numbers must have at least three characters (0x prefix)*/
    }

    int state = (s[0] == '0') && (s[1] == 'x' || s[1] == 'X');  /*/ Check if it starts with '0x' or '0X'*/
    int i;
    for ( i = 2; i < strlen(s); i++) {
        int st = (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F');
        state &= st;  /*/ All characters after prefix must be valid hexadecimal digits*/
    }

    return state;
}

int isDecimal(const char *s) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (!(s[i] >= '0' && s[i] <= '9')) {
            return 0;  /*/ If any character is not between '0' and '9', it is not a decimal number*/
        }
    }
    return 1;
}

int validName(const char *cur) {
    if (!((cur[0] >= 'a' && cur[0] <= 'z') || (cur[0] >= 'A' && cur[0] <= 'Z') || (cur[0] == '_'))) {
        return 0;  /*/ First character must be a letter or underscore*/
    }
    int i;
    for ( i = 1; i < strlen(cur); i++) {
        char to = cur[i];
        if (!((to >= 'a' && to <= 'z') || (to >= 'A' && to <= 'Z') || (to >= '0' && to <= '9') || (to == '_'))) {
            return 0;  /*/ Remaining characters must be alphanumeric or underscore*/
        }
    }

    return 1;
}
/*/ Function to check if a given mnemonic is a valid opcode*/
int isOpcodeValid(const char *mnemonic) {
    int i;
    for (i = 0; i < 20; i++) {
        if (strcmp(OPC[i].opcode, mnemonic) == 0) {
            return 1;
        }
    }
    return 0;
}

/*/ Function to get the expected operand count for a given opcode*/
int expectedOperandCount(const char *mnemonic) {
    int i;
    for ( i = 0; i < MAX_OPCODES; i++) {
        if (strcmp(OPC[i].opcode, mnemonic) == 0) {
            if (OPC[i].operandType == 0) {
                return 0; /*/ No operands expected*/
            } else if (OPC[i].operandType == 1) {
                return 1; /*/ One operand expected*/
            } else if (OPC[i].operandType == 2) {
                return 2; /*/ Two operands expected (if applicable)*/
            }
        }
    }
    return -1; /*/ Indicate that the opcode is invalid*/
}

/*/ Function to check if a given label already exists*/

int labelExists(const char *label) {
    int i;
    for (i = 0; i < labels_size; i++) {
        if (strcmp(labels[i].name, label) == 0) {
            return 1;
        }
    }
    return 0;
}

char* generateMachineCode(int lineIndex) {
    /* Declare a static buffer to hold the generated machine code */
    static char machineCode[20];

    /* Retrieve the current line of data */
    struct Data currentData = data[lineIndex];
    char location[10];
    strcpy(location, appZero(decToHex(programCounter[lineIndex], 8), 8));

    /* If the mnemonic is empty, return an empty machine code */
    if (strcmp(currentData.mnemonic, "") == 0) {
        strcpy(machineCode, "        ");
        return machineCode;
    }

    /* Find the corresponding OPC entry for the mnemonic */
    int opcIndex = findOPCIndex(currentData.mnemonic);
    if (opcIndex == -1) {
        printf("Error: Unknown mnemonic '%s'\n", currentData.mnemonic);
        strcpy(machineCode, "        ");
        return machineCode;
    }

    /* Handle different operand types */
    if (currentData.oprType == 1) {
        int decForm;
        int operandAddress = getLabelAddress(currentData.operand);
        
        /* If the operand address is undefined, print an error and return empty machine code */
        if (operandAddress == -1) {
            printf("Error: Undefined label '%s'\n", currentData.operand);
            strcpy(machineCode, "        ");
            return machineCode;
        }

        /* Depending on the operand type, calculate the machine code */
        if (OPC[opcIndex].operandType == 2) {
            decForm = operandAddress - (programCounter[lineIndex] + 1);
        } else {
            decForm = operandAddress;
        }

        /* Generate the machine code and return it */
        sprintf(machineCode, "%s%s", appZero(decToHex(decForm, 24), 6), OPC[opcIndex].opcode);
        return machineCode;
    } 
    else if (currentData.oprType == 0) {
        /* Generate machine code for no operand */
        sprintf(machineCode, "000000%s", OPC[opcIndex].opcode);
        return machineCode;
    } 
    else {
        /* Handle data operand type with specific size and address format */
        int sz = 6, add = 24;
        if (strcmp(currentData.mnemonic, "data") == 0) {
            sz = 8;
            add = 32;
        }
        int decForm = strtol(currentData.operand, NULL, currentData.oprType);
        /* Generate the machine code and return it */
        sprintf(machineCode, "%s%s", appZero(decToHex(decForm, add), sz), OPC[opcIndex].opcode);
        return machineCode;
    }
}
