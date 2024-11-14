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
#include <ctype.h>

#define MEMORY_SIZE (1 << 24) /*/ Define memory size as 2^24*/

int Memory[MEMORY_SIZE];      /*/ Main memory*/
char *code[1000];             /*/ Array to store code lines (adjust size as needed)*/ 
int A, B, PC, SP, idx, cont, exec;
int change[2];                /*/ Array to store changes */
int codeSize = 0;

const char *mnem[] = {
    "ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub",
    "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"
};

/* Function definitions for each mnemonic operation*/
void ldc(int value) {
    B = A;
    A = value;
}

void adc(int value) {
    A += value;
}

void ldl(int offset) {
    B = A;
    A = Memory[SP + offset];
    change[0] = SP + offset;
    exec = 1;
}

void stl(int offset) {
    change[0] = Memory[SP + offset];
    change[1] = A;
    Memory[SP + offset] = A;
    exec = 2;
    A = B;
}

void ldnl(int offset) {
    A = Memory[A + offset];
    change[0] = A + offset;
    exec = 1;
}

void stnl(int offset) {
    change[0] = Memory[A + offset];
    change[1] = B;
    Memory[A + offset] = B;
    exec = 2;
}

void add(int off) {
    A += B;
}

void sub(int off) {
    A = B - A;
}

void shl(int off) {
    A = B << A;
}

void shr(int off) {
    A = B >> A;
}

void adj(int value) {
    SP += value;
}

void a2sp(int off) {
    SP = A;
    A = B;
}

void sp2a(int off) {
    B = A;
    A = SP;
}

void call(int offset) {
    B = A;
    A = PC;
    PC += offset;
}

void ret(int offset) {
    PC = A;
    A = B;
}

void brz(int offset) {
    if (A == 0) {
        PC += offset;
    }
}

void brlz(int offset) {
    if (A < 0) {
        PC += offset;
    }
}

void br(int offset) {
    PC += offset;
}

/*/ Array of function pointers for each operation*/
void (*call_func[])(int) = {
    ldc, adc, ldl, stl, ldnl, stnl, add, sub,
    shl, shr, adj, a2sp, sp2a, call, ret, brz, brlz, br
};


#include <stdio.h>
#include <string.h>

void decToHex(unsigned int num, char *hexStr) {
    int len;

    /*/ Convert `num` to a hexadecimal string and store in `hexStr`*/
    snprintf(hexStr, 9, "%X", num);

    /*/ Calculate the current length of the hexadecimal string*/
    len = strlen(hexStr);

    /*/ Add leading zeros if the length is less than 8*/
    if (len < 8) {
        memmove(hexStr + (8 - len), hexStr, len + 1);  /*/ Shift the string to the right*/
        memset(hexStr, '0', 8 - len);                  /*/ Fill leading spaces with zeros*/
    }
}

/*/ Function to input machine code from a file*/
void inputMacCode() {
    char filename[100];
    printf("Enter file name (e.g., machineCode.o): ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    unsigned int read;
    int pos = 0;
    while (fread(&read, sizeof(int), 1, file)) {
        Memory[pos++] = read;

        char hexStr[9];
        decToHex(read, hexStr);
        code[codeSize] = strdup(hexStr);  /*/ Store a copy of the hex string in `code`*/
        codeSize++;
    }
    fclose(file);
}

/*/ Function to display entry message*/
void entryMsg() {
    printf("Welcome to Emulator\n");
    printf("Following functions are implemented:\n");
    printf("1. Memory Dump using instruction: -dump\n");
    printf("2. Emulate code one line at a time using instruction: -t\n");
    printf("3. Emulate till the end using -run\n");
    printf("4. Show registers and SP values: -reg\n");
    printf("5. Show instruction set using : -isa\n");
    printf("6. Read operations using -read\n");
    printf("7. Write operations using -write\n");
}
int hexToInt(const char *hex) {
    int value;
    sscanf(hex, "%X", &value);
    return value;
}
/*/ Function to print the memory*/
void showMem() {
    int i;
    for ( i = 0; i < codeSize; i += 4) {
        char addrHex[9];
        decToHex(i, addrHex);
        printf("%s ", addrHex);
        int j;
        for ( j = i; j < i + 4 && j < codeSize; j++) {
            printf("%s ", code[j]);
        }
        printf("\n");
    }
}

/*/ Function to display register values*/
void showReg() {
    char aHex[9], bHex[9], spHex[9], pcHex[9];
    decToHex(A, aHex);
    decToHex(B, bHex);
    decToHex(SP, spHex);
    decToHex(PC + 1, pcHex);

    printf("A: %s     B: %s     SP: %s     PC: %s     %s\n", aHex, bHex, spHex, pcHex, mnem[PC]);
}

/*/ Function to print memory read action*/
void readPrint() {
    char pcHex[9], changeHex[9];
    decToHex(PC, pcHex);
    decToHex(change[0], changeHex);

    printf("Reading memory[%s], has value: %s\n", pcHex, changeHex);
}

/*/ Function to print memory write action*/
void writePrint() {
    char pcHex[9], oldHex[9], newHex[9];
    decToHex(PC, pcHex);
    decToHex(change[0], oldHex);
    decToHex(change[1], newHex);

    printf("Writing memory[%s], from %s to %s\n", pcHex, oldHex, newHex);
}


void inset() {
    printf("OpMachineCode Mnemonic Operand\n");
    printf("0      ldc      value \n");
    printf("1      adc      value \n");
    printf("2      ldl      value \n");
    printf("3      stl      value \n");
    printf("4      ldnl     value \n");
    printf("5      stnl     value \n");
    printf("6      add            \n");
    printf("7      sub            \n");
    printf("9      shr            \n");
    printf("10     adj      value \n");
    printf("11     a2sp           \n");
    printf("12     sp2a           \n");
    printf("13     call     offset\n");
    printf("14     return         \n");
    printf("15     brz      offset\n");
    printf("16     brlz     offset\n");
    printf("17     br       offset\n");
    printf("18     HALT           \n");
}

/*/ Function to run code*/
int runCode(int oper, int times) {
    while (times-- && PC < codeSize) {
        cont++;
        if (PC >= codeSize || cont > (int)3e7) {
            printf("Segmentation Fault\n");
            return 0;
        }

        char *cur = code[PC];
        int op = hexToInt(cur + 6);  /*/ Extracting opcode (2 characters after 6th index)*/
        if (op == 18) {  /*/ HALT instruction*/
            printf("HALT found\n");
            printf("%d statements were executed total\n", cont);
            return 1;
        }

        int val = hexToInt(cur);  /*/ Extracting operand (first 6 characters)*/
        if (val >= (1 << 23)) {  /*/ Two's complement adjustment*/
            val -= (1 << 24);
        }

        if (times == 0) {
            showReg();  /*/ Show registers if no limit on times*/
        }

        /*/ Call function corresponding to the opcode (call_func[op] is a function pointer)*/
        call_func[op](val);

        if (oper == 1 && exec == 1) {
            readPrint();
            exec = 0;
        } else if (oper == 2 && exec == 2) {
            writePrint();
            exec = 0;
        }

        PC++;  /*/ Increment the Program Counter*/
        idx++;  /*/ Increment the instruction index*/
    }
    return 1;
}

/*/ Function to run the emulator interactively*/
int runEmu() {
    char s[100];
    printf("Enter command or 0 to exit:\n");
    scanf("%s", s);

    if (strcmp(s, "0") == 0) {
        exit(0);  /*/ Exit the program*/
    } else if (strcmp(s, "-dump") == 0) {
        showMem();  /*/ Show memory dump*/
    } else if (strcmp(s, "-reg") == 0) {
        showReg();  /*/ Show register values*/
    } else if (strcmp(s, "-t") == 0) {
        return runCode(0, 1);  /*/ Run a single instruction*/
    } else if (strcmp(s, "-run") == 0) {
        return runCode(0, (1 << 25));  /*/ Run the code without limit*/
    } else if (strcmp(s, "-isa") == 0) {
        inset();  /*/ Show instruction set*/
    } else if (strcmp(s, "-read") == 0) {
        return runCode(1, (1 << 25));  /*/ Run and show memory read operations*/
    } else if (strcmp(s, "-write") == 0) {
        return runCode(2, (1 << 25));  /*/ Run and show memory write operations*/
    } else {
        printf("Enter correct instruction\n");
    }
    return 1;
}

/*/ Function to display entry message*/
/*void entryMsg() {
    printf("Welcome to Emulator\n");
    printf("Following functions are implemented:\n");
    printf("1. Memory Dump using instruction: -dump\n");
    printf("2. Emulate code one line at time: using instruction: -t\n");
    printf("3. Emulate till the end using -run\n");
    printf("4. Show registers and SP values: -reg\n");
    printf("5. Show instruction set using : -isa\n");
    printf("6. Read operations using -read\n");
    printf("7. Write operations using -write\n");
}*/


/*/ Main function*/
int main() {
    inputMacCode();  /*/ Load machine code*/
    entryMsg();      /*/ Display entry message*/
    while (1) {
        runEmu();    /*/ Start interactive emulator loop*/
    }
    return 0;
}