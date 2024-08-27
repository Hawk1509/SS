
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

void main() {
    char opcode[10], mnemonic[3], operand[10], label[10], code[10], testlab[10], value[10];
    int locctr, start = 0, length, f = 0;
    FILE *fp1, *fp2, *fp3, *fp4;
    
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("sym.txt", "w+");  // Open in "w+" mode to allow both reading and writing
    fp3 = fopen("inter.txt", "w");
    fp4 = fopen("optab.txt", "r");
    
    fscanf(fp1, "%s%s%s", label, opcode, operand);
    
    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        locctr = start;
        fprintf(fp3, "\t%s %s %s\n", label, opcode, operand);
        fscanf(fp1, "%s%s%s", label, opcode, operand);
    } else {
        locctr = 0;
    }
    
    while (strcmp(opcode, "END") != 0) {
        fprintf(fp3, "%d\t", locctr);
        
        if (strcmp(label, "**") != 0) {
            f = 0;
            rewind(fp2);
            while (fscanf(fp2, "%s%s", testlab, value) != EOF) {
                if (strcmp(label, testlab) == 0) {
                    f = 1;
                    printf("Duplicate Symbol: %s\n", label);
                    break;
                }
            }
            
            if (f == 0) {
                fprintf(fp2, "%s\t%d\n", label, locctr);
            }
        }
        
        //flag = 0;
        rewind(fp4);
        fscanf(fp4, "%s%s", code, mnemonic);
        
        while (!feof(fp4)) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;
                break;
            }
            fscanf(fp4, "%s%s", code, mnemonic);
        }
        
        if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += (3 * atoi(operand));
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'X') {
                locctr += 1;
            } else if (operand[0] == 'C') {
                locctr += (strlen(operand) - 3);
            }
        }
        
        fprintf(fp3, "%s %s %s\n", label, opcode, operand);
        fscanf(fp1, "%s%s%s", label, opcode, operand);
    }
    
    fprintf(fp3, "%d %s %s %s\n", locctr, label, opcode, operand);
    length = locctr - start;
        
    printf("The length of the program is %d\n", length);
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}


/*
OUTPUT

INPUT
Input.txt

COPY START 1000
** LDA FIVE
** STA NUM1
** LDCH NUM2
** STCH C1
NUM1 RESW 2
NUM1 RESW 2
FIVE WORD 5
NUM2 BYTE X'F1'
C1 RESB 1
** END **


OPTAB
optab.txt

LDA 00
STA 0C
LDCH 50
STCH 54

SYMTAB
symtab.txt

NUM1	1012
FIVE	1024
NUM2	1027
C1	1028

INTERMEDIATE
inter.txt

	COPY START 1000
1000	** LDA FIVE
1003	** STA NUM1
1006	** LDCH NUM2
1009	** STCH C1
1012	NUM1 RESW 2
1018	NUM1 RESW 2
1024	FIVE WORD 5
1027	NUM2 BYTE X'F1'
1028	C1 RESB 1

OUTPUT
Duplicate Symbol: NUM1
The length of the program is 29

1029 ** END **
*/
