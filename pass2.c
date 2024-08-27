#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main()
{
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;
    char label[10],opcode[10],operand[10],symbol[20],value[10],symbol1[20],value1[10];
    int add,length=0,start=0,i;
    fp1 = fopen("intermediate.txt","r");
    fp2 = fopen("pgm_length.txt","r");
    fp5 = fopen("object_code.txt","w");
    fscanf(fp2,"%d",&length);
    fscanf(fp1,"\t\t%s\t%s\t%s\n",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        start = atoi(operand);
        fprintf(fp5,"H^%s^%06d^%06d\n",label,start,length);
        fprintf(fp5,"T^%06d^%06d",start,length);
    }
    while(!feof(fp1))
    {
        fscanf(fp1,"%d\t%s\t%s\t%s\n",&add,label,opcode,operand);
        if(strcmp(opcode,"END")==0)
        {
            fprintf(fp5,"\nE^%06d",start);
            break;
        }
        else if((strcmp(opcode,"RESW")==0)||(strcmp(opcode,"RESB")==0))
        {
            continue;
        }
        else if(strcmp(opcode,"WORD")==0)
        {
            fprintf(fp5,"^%06d",atoi(operand));
        }
        else if(strcmp(opcode,"BYTE")==0)
        {
            fprintf(fp5,"^");
            if(operand[0]=='X')
            {
                for(i=2;i<(strlen(operand)-1);i++)
                {
                    fprintf(fp5,"%c",operand[i]);
                }
            }
            else if(operand[0]=='C')
            {
                for(i=2;i<(strlen(operand)-1);i++)
                {
                    fprintf(fp5,"%X",operand[i]);
                }
            }
        }
        else
        {
            fp3 = fopen("optab.txt","r");
            while(!feof(fp3))
            {
                fscanf(fp3,"%s\t%s\n",symbol,value);
                if(strcmp(symbol,opcode)==0)
                {
                    fp4 = fopen("symtab.txt","r");
                    while (!feof(fp4))
                    {
                        fscanf(fp4,"%s\t%s\n",symbol1,value1);
                        if(strcmp(symbol1,operand)==0)
                        {
                            fprintf(fp5,"^%s%s",value,value1);
                            break;
                        }
                    }
                    fclose(fp4);
                    break;
                }
            }
            fclose(fp3);
        }
    }
    printf("\nOBJECT CODE SUCESSFULLY WRITTEN TO THE FILE\n");
    fclose(fp1);
    fclose(fp2);
    fclose(fp5);
}
