/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>




/*Sturcture for saving the line codes*/
struct code
{
	int pc;
	int line_num;
	int opcode;
	char pnemonic[50];
	char operand[50];
	char label[50];
	struct code* next;
};
/*Declaring the Opcodes for the instructions*/
typedef struct opcode
{
	char name[50];
	int code;
}Opcode;
/*Creating a table for all pnemonics*/
Opcode opcodes[21] = 
{
	{"ldc", 0},
	{"adc", 1},
	{"ldl", 2},
	{"stl", 3},
	{"ldnl", 4},
	{"stnl", 5},
	{"add", 6},
	{"sub", 7},
	{"shl", 8},
	{"shr", 9},
	{"adj", 10},
	{"a2sp", 11},
	{"sp2a", 12},
	{"call", 13},
	{"return", 14},
	{"brz", 15},
	{"brlz", 16},
	{"br", 17},
	{"HALT", 18},
	{"data", 19},
	{"SET", 20}
};
/*Structure for storing errors*/
struct error
{
	int line_num;
	char state[100];
	struct error *next;
};

struct error *head_err = NULL;
int num_errors = 0; /*variable to hold number of errors*/
int num_labels = 0; /* variable to hold number of labels*/

/*Function to add errors encountered during the program*/
void addError(int line_num_s, char *state_s)
{
	/*Initialsing new head*/
	if(head_err == NULL)
	{
		head_err = (struct error*)(malloc(sizeof(struct error)));
		head_err->line_num = line_num_s;
		strcpy(head_err->state, state_s);
		head_err->next = NULL;
		num_errors++;
	}
	/*Adding subsequent errors*/
	else
	{
		struct error* temp = (struct error*)(malloc(sizeof(struct error)));
		struct error* last = head_err;
		strcpy(temp->state, state_s);
		temp->line_num = line_num_s;
		temp->next = NULL;
		/* Traverse till the end of LL from head*/
		while(last->next!= NULL)
		{
			last = last->next;
		}
		last->next = temp;
		num_errors++;
	}
	return;
}
/*Adding valid code lines*/
void insertCode(struct code* head, char *pnemonic_s, char *operands_s, char *label_s, int pcctr_s, int line_num_s)
{
	struct code* temp = (struct code*)(malloc(sizeof(struct code)));
	struct code* last = head;
	strcpy(temp->pnemonic, pnemonic_s);
	strcpy(temp->operand, operands_s);
	strcpy(temp->label, label_s);
	int j, flag;
	flag = 0;
	/*For assigning the opcode value*/
	for(j=0;j<21;j++)
	{
		if (strcmp(pnemonic_s, opcodes[j].name) == 0)
		{
			flag = 1;
			temp->opcode = opcodes[j].code;
		}
	}
	if (flag == 0)
	{
		addError(line_num_s+1, "Pnemonic Doesn't exists Error");
		temp->opcode = -1;
	}
	temp->pc = pcctr_s;
	temp->line_num = line_num_s;
	temp->next = NULL;
	/* Traverse till the end of LL from head*/
	while(last->next!= NULL)
	{
		last = last->next;
	}
	last->next = temp;
	return;
}
/*Master fucntion to create the first level code representation*/
struct code* createCode(char **pnemonics, char **operands, char **labels, int ctr)
{
	/*Code for finding the head*/
	struct code* head = NULL;
	int i, start=0, pcctr=0;
	for(i=0; i<ctr; i++)
	{
		if (strcmp(pnemonics[i], "")!= 0)
		{
			head = (struct code*)(malloc(sizeof(struct code)));
			strcpy(head->pnemonic, pnemonics[i]);
			strcpy(head->operand, operands[i]);
			strcpy(head->label, labels[i]);
			/*Code for finding the opcode*/
			int j, flag;
			flag = 0;
			for(j=0;j<21;j++)
			{
				if (strcmp(pnemonics[i], opcodes[j].name) == 0)
				{
					flag = 1;
					head->opcode = opcodes[j].code;
				}
			}
			if (flag == 0)
			{
				addError(i+1, "Pnemonic Doesn't exists Error");
				head->opcode = -1;
			}
			head->pc = 0;
			head->line_num = i;
			head->next = NULL;
			start = i;
			pcctr++;
			break;
		} 
	}
	if (head != NULL)
	{
		for(i=start+1;i<ctr;i++)
		{
			/*Considering valid lines*/
			if(strcmp(pnemonics[i], "")!= 0)
			{
				insertCode(head, pnemonics[i], operands[i], labels[i], pcctr, i);
				pcctr++;
			}
		}
		return head;
	}
	else
	{
		printf("Head not intialised");
		return NULL;
	}
}
/*Fucntion to display the Code Linked List*/
void displayCodeLL(struct code* head)
{
	struct code* last = head;
	printf("\nCode Compact Representation\n");
	printf("PC, Label, instruction, pnemonic, operand\n");
	while(1)
	{
		printf("%d, ", last->pc);
		printf("%s, ", last->label);
		printf("%s, ", last->pnemonic);
		printf("%d, ", last->opcode);
		printf("%s\n", last->operand);
		last = last->next;
		if(last == NULL)
			break;
	}
}
/*Label Error Checking and Label Value Returning Function*/
int CheckLabelExists(char* label_s, struct code* head, int line_num_s, int pc_s, int type)
{
	struct code* last = head;
	int flag, offset;
	flag = 0;
	while(1)
	{
		if (strcmp(label_s, last->label) == 0)
		{
			flag = 1;
			if (type == 1)
				offset = last->pc - pc_s;
			else if(type == 0)
			{
				if (last->opcode !=20)
					offset = last->pc;
				else if(last->opcode == 20)
				{
					char *end;
					int ret;
					ret = strtol(last->operand, &end, 0);
					offset = ret;
				}
			}
			break;
		}
		last = last->next;
		if(last == NULL)
			break;
	}
	if(flag == 0)
	{
		addError(line_num_s, "Label Doesn't Exists Error");
	}
	return offset;
}
/*This checks if end is not an empty string.

If end is not empty, it means strtol did not consume the entire operand_s, suggesting the presence of non-numeric characters.

This checks if operand_s is exactly the same as end.

If they are the same, it means the entire operand_s is non-numeric, as strtol did not convert any part of it.

Combined Logic:
Condition 1: if (strcmp(end, "") != 0):

Ensures that there are non-numeric characters in operand_s.

Condition 2: if (strcmp(operand_s, end) == 0):

If operand_s is the same as end, it means the whole string is non-numeric, so operand_s is likely a label.


Actions Based on Conditions:
If Entire Operand is a Label:

ret = CheckLabelExists(operand_s, head, line_num_s, pc, type);

Calls CheckLabelExists to check if operand_s is a valid label.

If Operand is Not Entirely Numeric Nor a Valid Label:

addError(line_num_s, "Operand Not a number");

Adds an error indicating the operand is not a valid number.
*/
/*Processing an operand to know if it is label or a number*/
long processOperand(char* operand_s, struct code* head, int line_num_s, int pc, int type)
{
	char *end;
	long ret;
	ret = strtol(operand_s, &end, 0);
	if (strcmp(end, "") != 0)
	{
		if (strcmp(operand_s, end) == 0)
		{
			/*printf("Above is a label\n");*/
			ret = CheckLabelExists(operand_s, head, line_num_s, pc, type);
		}
		else
		{
			addError(line_num_s, "Operand Not a number");
		}
	}
	return ret;	
}
/*Checking if operand exists for the given line*/
int CheckOperandExistence(char* operand_s)
{
	int flag = 1;
	if (strcmp(operand_s, "") == 0)
	{
		flag = 0;
	}
	return flag;	
}
/*Converting a Decimal Number to Hexadecimal Equivalent*/
void dectohex(long decimalnum, char *res)
{
	int neg = 0;
	long neg_const = 16777216;
	if (decimalnum == 0)
	{
		res[0] = '0';
		res[1] = '\0';
		return;
	}
	else if(decimalnum < 0)
	{
		decimalnum = neg_const + decimalnum;
		neg = 1;
	}
    long quotient, remainder;
    int i, j;
    j = 0;
    char hexadecimalnum[100];
    quotient = decimalnum;
    while (quotient != 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;

    }
    int k = 0;
    for (i = j-1; i >= 0; i--)
    {
        res[k] = hexadecimalnum[i];
        k++;
    }
    res[k] = '\0';
}
void printLabelList(FILE *flst, char *label, char *res_pc)
{
	int pc_pad, k;
	pc_pad = 8 - strlen(res_pc);
	for(k=0;k<pc_pad;k++)
	{
		fprintf(flst, "0");
	}	
	fprintf(flst, "%s\t", res_pc);
	for(k=0;k<8;k++)
	{
		fprintf(flst, " ");
	}
	fprintf(flst, "\t");
	fprintf(flst, "%s", label);
	fprintf(flst, ":\n");
}/*Padding Calculation:

pc_pad determines how many zeros are needed to pad the res_pc to a width of 8 characters.

Printing to File:

Pads res_pc with zeros and prints it.

Adds 8 spaces for formatting.

Prints the label followed by a colon.*/

/*For printing into BINARY OBJECT FILE*/
void printObjBin(FILE *fbin, char *pc, char *ins)
{
    /*fwrite(pc,sizeof(pc),1,fbin);
    Writing to File:

Writes the instruction ins to the binary file fbin using fwrite.*/

    fwrite(ins,sizeof(ins),1,fbin);
}
/*Prints MACHINE CODE into the list file*/
void printList(FILE *fbin, FILE *flst, char *res_pc, char *res_oc, char *res_op, char *pnemonic, char *operand)
{
	int pc_pad, op_pad, k, l=0;
	char pc[8];
	char ins[8];
	pc_pad = 8 - strlen(res_pc);
	for(k=0;k<pc_pad;k++)
	{
		fprintf(flst, "0");
		pc[l++] = '0';
	}
	fprintf(flst, "%s\t", res_pc);
	for(k=0;k<strlen(res_pc);k++)
	{
		pc[l++] = res_pc[k];
	}
	l = 0;
	op_pad = 6 - strlen(res_op);
	for(k=0;k<op_pad;k++)
	{
		fprintf(flst, "0");
		ins[l++] = '0';
	}
	fprintf(flst, "%s", res_op);
	for(k=0;k<strlen(res_op);k++)
	{
		ins[l++] = res_op[k];
	}
	op_pad = 2 - strlen(res_oc);
	for(k=0;k<op_pad;k++)
	{
		fprintf(flst, "0");
		ins[l++] = '0';
	}
	for(k=0;k<strlen(res_oc);k++)
	{
		ins[l++] = res_oc[k];
	}
	fprintf(flst, "%s\t", res_oc);
	fprintf(flst, "%s ", pnemonic);
	fprintf(flst, "%s\n", operand);
	printObjBin(fbin, pc, ins);
}
/*Padding and Formatting:

Calculates padding for res_pc and prints zeros as needed.

Constructs pc and ins arrays for writing to the binary file.

Printing to List File:

Prints res_pc, res_op, and res_oc to the list file flst.

Also prints the mnemonic and operand.

Writing to Binary File:

Calls printObjBin to write the pc and ins arrays to the binary file fbin.*/
/*Main Function for generating the MACHINE CODE*/
void secondPass(struct code* head, char *file_name)
{
	/*printf("In the secondPass\n");*/
	char name_file[50];
	strcpy(name_file, file_name);
	strcat(file_name, "_listfile.lst");
	FILE *fptr = fopen("listfile.txt", "w");
	if(fptr == NULL)
	{
	      printf("Error in opening list file!");   
	      exit(1);             
	}
	FILE *flst = fopen(file_name, "w");
	if(flst == NULL)
	{
	      printf("Error in opening list file!");   
	      exit(1);             
	}
	/* Opening the binary object file*/
	strcat(name_file, "_obj.o");
	FILE *fbin = fopen(name_file, "wb");
	struct code* last = head;
	int ctr = 0;
	char res_pc[32];
	char res_oc[32];
	char res_op[32];
	int pc_pad, op_pad;
	int type;
	while(1)
	{
		char lst_write[200];
		int opcode = last->opcode;
		/*Grouping opcodes based on their similarity and utility*/
		if (opcode == 0 || opcode == 1 || opcode == 10)
		{	
			/* checking if there is no operand*/
			int flag = CheckOperandExistence(last->operand);
			if (flag == 0)
			{
				addError(last->line_num+1, "Operand doesn't exist error");
			}
			else
			{
				int k;
				type = 0;
				long decoperand = processOperand(last->operand, head, last->line_num+1, last->pc, type);
				fprintf(fptr, "PC\t");
				dectohex(last->pc, res_pc);
				pc_pad = 8 - strlen(res_pc);
				for(k=0;k<pc_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_pc);
				fprintf(fptr, "Opcode\t");
				dectohex(opcode, res_oc);
				op_pad = 2 - strlen(res_oc);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_oc);
				fprintf(fptr, "Operand\t");
				dectohex(decoperand, res_op);
				op_pad = 6 - strlen(res_op);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\n", res_op);
				if (strcmp(last->label, "")!=0)
				{
					printLabelList(flst, last->label, res_pc);
				}
				printList(fbin, flst, res_pc, res_oc, res_op, last->pnemonic, last->operand);
			}
		}
		else if(opcode == 2 || opcode == 3 || opcode == 4 || opcode == 5)
		{
			/* checking if there is no operand*/
			int flag = CheckOperandExistence(last->operand);
			if (flag == 0)
			{
				addError(last->line_num+1, "Operand doesn't exist error");
			}
			else
			{
				int k;
				type = 1;
				long decoperand = processOperand(last->operand, head, last->line_num+1, last->pc, type);
				decoperand = decoperand - 1;
				fprintf(fptr, "PC\t");
				dectohex(last->pc, res_pc);
				pc_pad = 8 - strlen(res_pc);
				for(k=0;k<pc_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_pc);
				fprintf(fptr, "Opcode\t");
				dectohex(opcode, res_oc);
				op_pad = 2 - strlen(res_oc);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_oc);
				fprintf(fptr, "Operand\t");
				dectohex(decoperand, res_op);
				op_pad = 6 - strlen(res_op);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\n", res_op);
				if (strcmp(last->label, "")!=0)
				{
					printLabelList(flst, last->label, res_pc);
				}
				printList(fbin, flst, res_pc, res_oc, res_op, last->pnemonic, last->operand);
			}			
		}
		else if(opcode == 13 || opcode == 15 || opcode == 16 || opcode == 17)
		{
			/* checking if there is no operand*/
			int flag = CheckOperandExistence(last->operand);
			if (flag == 0)
			{
				addError(last->line_num+1, "Operand doesn't exist error");
			}
			else
			{
				int k;
				type = 1;
				long decoperand = processOperand(last->operand, head, last->line_num+1, last->pc, type);
				/*subtracting one from offset*/
				decoperand = decoperand - 1;
				fprintf(fptr, "PC\t");
				dectohex(last->pc, res_pc);
				pc_pad = 8 - strlen(res_pc);
				for(k=0;k<pc_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_pc);
				fprintf(fptr, "Opcode\t");
				dectohex(opcode, res_oc);
				op_pad = 2 - strlen(res_oc);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_oc);
				fprintf(fptr, "Operand\t");
				dectohex(decoperand, res_op);
				op_pad = 6 - strlen(res_op);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\n", res_op);
				if (strcmp(last->label, "")!=0)
				{
					printLabelList(flst, last->label, res_pc);
				}
				printList(fbin, flst, res_pc, res_oc, res_op, last->pnemonic, last->operand);
			}			
		}
		else if(opcode == 19 || opcode == 20)
		{
			int flag = CheckOperandExistence(last->operand);
			if (flag == 0)
			{
				addError(last->line_num+1, "Operand doesn't exist error");
			}
			else
			{
				int k;
				type = 0;
				long decoperand = processOperand(last->operand, head, last->line_num+1, last->pc, type);
				fprintf(fptr, "PC\t");
				dectohex(last->pc, res_pc);
				pc_pad = 8 - strlen(res_pc);
				for(k=0;k<pc_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_pc);
				fprintf(fptr, "Opcode\t");
				dectohex(opcode, res_oc);
				op_pad = 2 - strlen(res_oc);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\t", res_oc);
				fprintf(fptr, "Operand\t");
				dectohex(decoperand, res_op);
				op_pad = 6 - strlen(res_op);
				for(k=0;k<op_pad;k++)
				{
					fprintf(fptr, "0");
				}
				fprintf(fptr, "%s\n", res_op);
				if (strcmp(last->label, "")!=0)
				{
					printLabelList(flst, last->label, res_pc);
				}
				printList(fbin, flst, res_pc, res_oc, res_op, last->pnemonic, last->operand);
			}
		}
		else if(opcode == 6 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 11 || opcode == 12 || opcode == 14 || opcode == 18)
		{
			int flag = CheckOperandExistence(last->operand);
			if (flag == 1)
			{
				addError(last->line_num+1, "Operand Unexpected Error");
			}
			else
			{
				dectohex(last->pc, res_pc);
				dectohex(opcode, res_oc);
				dectohex(0, res_op);
				if (strcmp(last->label, "")!=0)
				{
					printLabelList(flst, last->label, res_pc);
				}
				printList(fbin, flst, res_pc, res_oc, res_op, last->pnemonic, last->operand);
			}			
		}
		last = last->next;
		if(last == NULL)
			break;
		ctr++;
		/*if (ctr == 4)
			break;*/
	}
	fclose(fptr);
}
/*To display if any errors occured during Assembling*/
void displayErrorLL(FILE *flog)
{
	if (head_err == NULL)
	{
		return;
	}
	struct error* last = head_err;
	fprintf(flog, "\nData in the Error LL\n");
	while(1)
	{
		fprintf(flog, "%d, ", last->line_num);
		fprintf(flog, "%s\n", last->state);
		last = last->next;
		if(last == NULL)
			break;
	}
}
/*Checking for Label Not Repated Error*/
int LabelNotRepeated(char *pnemonic, char **labels)
{
	int j=0, flag = 0;
	/* checking if label already exists*/
	for(j=0;j<num_labels;j++)
	{
		if(strcmp(pnemonic, labels[j]) == 0)
		{
			flag = 1;
			break;
		}
	}
	return flag;
}
/*Function for removing spaces*/
void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}
/*Main Function for 1st Pass and initiating the 2nd Pass*/

#define NUM_LINES 100
#define CHAR_LEN 100

// Assume addError, LabelNotRepeated, createCode, displayCodeLL, and other functions are defined elsewhere

int main(int argc, char *argv[])
{
    FILE *fp;    /* File pointer */
    FILE *flog;  /* Pointer to the log file */
    char name_log[50];
    char buffer[CHAR_LEN];  /* Buffer for reading lines */
    int i, ctr = 0;
    char *lines[NUM_LINES];  /* Declaring array of char pointers */
    char *lables[NUM_LINES];
    char *pnemonics[NUM_LINES];  /* Declaring array of char pointers */
    char *operands[NUM_LINES];
    char *exact_labels[NUM_LINES];

    for (i = 0; i < NUM_LINES; i++)
    {
        lines[i] = (char *)(malloc(CHAR_LEN * sizeof(char)));
        lables[i] = (char *)(malloc(CHAR_LEN * sizeof(char)));
        exact_labels[i] = (char *)(malloc(CHAR_LEN * sizeof(char)));
        pnemonics[i] = (char *)(malloc(CHAR_LEN * sizeof(char)));
        operands[i] = (char *)(malloc(CHAR_LEN * sizeof(char)));
    }

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *file_name = argv[1];
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    strcpy(name_log, file_name);
    strcat(name_log, "_logfile.txt");
    flog = fopen(name_log, "w");
    if (flog == NULL)
    {
        printf("Error in opening log file!");
        exit(1);
    }

    // Read lines from the file using fgets
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Remove newline character if it exists
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Retrieved line: %s\n", buffer);
        int colon = 0;
        char *store = (char *)(malloc(CHAR_LEN * sizeof(char)));

        for (i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == ';') // Comment start
                break;
            if (buffer[i] == ':')
            {
                colon = i;
                break;
            }
            store[i] = buffer[i]; // Copy character to store
        }
        store[i] = '\0'; // Null-terminate the store string
/*Reading Lines: Uses fgets to read lines from the file.

Remove Newline: Removes the newline character from the end of the line.

Initialize Variables: Initializes colon to check for colons, store to store characters.

Loop Through Line: Loops through each character in the line.

Comment Check: Breaks if a semicolon is found.

Colon Check: Updates colon if a colon is found and breaks.

Copy to Store: Copies characters to store.*/
        /* Pre-processing each line */
        if (colon == 0)
        {
            strcpy(lines[ctr], store);
            strcpy(lables[ctr], "");
        }
        else if (colon != 0)
        {
            char *processed = (char *)(malloc(CHAR_LEN * sizeof(char)));
            for (i = colon + 1; i < strlen(buffer); i++)
            {
                if (buffer[i] == ';') // Comment start
                    break;
                if (buffer[i] == '\t')
                {
                    processed[i - colon - 1] = ' '; // Replace tab with space
                    continue;
                }
                processed[i - colon - 1] = buffer[i]; // Copy character to processed
            }
            processed[i - colon - 1] = '\0'; // Null-terminate the processed string
            strcpy(lines[ctr], processed);
            strcpy(lables[ctr], store);
            free(processed); // Free allocated memory for processed
        }
        ctr += 1;
        free(store); // Free allocated memory for store
    }
/*No Colon: If no colon is found, stores the line in lines and sets lables to an empty string.

With Colon: If a colon is found:

Initialize Processed: Allocates memory for processed.

Loop through Remaining Line: Processes characters after the colon.

Comment Check: Breaks if a semicolon is found.

Tab Replacement: Replaces tabs with spaces.

Copy Characters: Copies characters to processed.

Null-Terminate: Null-terminates processed.

Store Processed: Stores processed in lines and store in lables.

Free Memory: Frees allocated memory for processed and store*/
    printf("\nPrinting the lines\n");
    for (i = 0; i < ctr; i++)
    {
        printf("%s\n", lines[i]);
        char dum[CHAR_LEN];
        strcpy(dum, lines[i]);
        char *token = strtok(dum, " ");
        int j = 0;
        /* initializing */
        strcpy(pnemonics[i], "");
        strcpy(operands[i], "");
        while (token)
        {
            if (j == 0)
            {
               char* c = token;
				while (isspace(*c))
    				++c;
				strcpy(pnemonics[i], c);
				j++;
			}
			else if(j == 1)
			{
				strcpy(operands[i], token);
				j++;
			}
			/* Checking for more than one operand*/
			else if (j > 1)
			{
				addError(i+1, "More than 1 operand Error");
			}
    		printf("token: %s\n", token);
    		token = strtok(NULL, " ");
		}
    }
    printf("\nPrinting the Pnemonics and Operands\n");
    for(i=0;i<ctr;i++)
    {
    	printf("Pnemonic : %s, Operand : %s\n", pnemonics[i], operands[i]);
    }
    /*printf("\nPrinting and Processing the lables\n");*/
    for(i=0;i<ctr;i++)
    {
    	if (strcmp(exact_labels[i], "")!=0)
    		continue;
    	/*printf("%s\n", lables[i]);*/
    	/*Checking if label is alphnumeric*/
    	int j = 0, flag = 0;
    	for(j=0;j<strlen(lables[i]);j++)
    	{
    		if (j == 0 && !isalpha(lables[i][j]))
    		{
    			addError(i+1, "Wrong Label Name Error");
    			flag = 1;
    			break;
    		}
    		if(!isalnum(lables[i][j]))
    		{
    			addError(i+1, "Wrong Label Name Error");
    			flag = 1;
    			break;
    		}
    	}
    	if (strcmp(lables[i], "") != 0 && flag == 0) 
    	{
    		if(LabelNotRepeated(lables[i], exact_labels) == 0)
    		{
	    		if(strcmp(pnemonics[i], "") != 0)
	    		{
	    			strcpy(exact_labels[i], lables[i]);
	    		}
	    		else
	    		{
	    			if (strcmp(lables[i+1], "") == 0)
	    			{
	    				int k = i+1;
	    				while(1)
	    				{
		    				if(strcmp(pnemonics[k], "")!=0)
		    				{
			    				strcpy(exact_labels[i], "");
			    				strcpy(exact_labels[k], lables[i]);
			  					break;
		    				}
		    				k++;
	    				}/*If the mnemonic for the label is not empty, it copies lables[i] to exact_labels[i].

If the mnemonic is empty and the next label is also empty, it searches for the next non-empty mnemonic and assigns the label to it.

Adds an error if there's a conflict in labels.*/
	    			}
	    			else
	    			{
	    				addError(i+1, "Conflicting Labels Error");
	    			}

	    		}
    		}
    		else
    		{
    			addError(i+1, "Label repeated error");
    		}
    	}
    	else
    	{
    		strcpy(exact_labels[i], "");
    	}
    	/*printf("%s\n", exact_labels[i]);*/
    	num_labels++;
    }
    fclose(fp);
    if (buffer)
        free(buffer);
    struct code* head = createCode(pnemonics, operands, exact_labels, ctr);
    displayCodeLL(head);
    /*If error detected then not going further with 2nd Pass*/
    printf("Please look at the log file %s for details of the compilation\n", name_log);
    if (num_errors > 0)
    {
    	fprintf(flog, "Error Detected in asm file. Cannot go ahead!\n");
    	displayErrorLL(flog);
    	exit(EXIT_FAILURE);
    }
    else
    {
    	char* token = strtok(file_name, ".");
    	secondPass(head, file_name);
    	if (num_errors == 0)
    	{
    		char* token = strtok(file_name, ".");
    		token = strtok(token, "_");
    		fprintf(flog, "Code Compiled Succesfully. Generated %s_listfile.lst and %s_obj.bin files\n", token, token);
    	}
    	else if (num_errors > 0)
    	{
	    	fprintf(flog, "Error Detected in asm file. Cannot go ahead!\n");
	    	displayErrorLL(flog);
	    	exit(EXIT_FAILURE);    		
    	}
    	exit(EXIT_SUCCESS);
	}
}/*This part handles logging, checks for errors, and exits if errors are found. If no errors are found, it performs the second pass of the compilation.*/