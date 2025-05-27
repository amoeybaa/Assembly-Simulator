/*
	ASSEMBLY CODE COMPILER
	
	Performs the following tasks:
		1.	Assembler design to spot syntactical errors
		2.	Symbol table construction and LC processing 
		3.	Intermediate Code generation (Variant I)
		4.	Object code file creation from intermediate code
*/

#include "Mneumonic.h"
#include "Tables.h"
#include "Intermediate.h"

// Instruction line size in bytes
#define INS_SIZE 80
#define FILENAME_SIZE 50

typedef struct instruction
{
	char line[INS_SIZE];
	struct instruction *next;
} INS;

INS *new = NULL, *head = NULL, *curr = NULL;
int noArg = 0;

void pass2();       // to genrate target code

/*
	****************************************************************
	MAIN FUNCTION
	****************************************************************
*/

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
    SYMTAB *s;
	char buffer[INS_SIZE];
	char *t1 = NULL, *t2 = NULL, *t3 = NULL, *t4 = NULL, *token = NULL;
	int n = 0, syntax_error = 0, symbol_error = 0;

	if (argc != 2)
	{
		printf("\nInvalid number of arguments!\nExiting program!\n");
		exit(0);
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		printf("\nFile does not exist!!\nExiting program!\n");
		exit(0);
	}

	while (!feof(fp))
	{
		fgets(buffer, INS_SIZE, fp);
		if (!feof(fp))
		{
			new = (INS *)malloc(sizeof(INS));
			new->next = NULL;
			strcpy(new->line, buffer);

			if (head == NULL)
				head = curr = new;
			else
				curr = curr->next = new;
		}
	}
	fclose(fp);

	// Assign each token size of INS_SIZE/4 characters
	t1 = (char *)malloc(INS_SIZE / 4 * sizeof(char));
	t2 = (char *)malloc(INS_SIZE / 4 * sizeof(char));
	t3 = (char *)malloc(INS_SIZE / 4 * sizeof(char));
	t4 = (char *)malloc(INS_SIZE / 4 * sizeof(char));

	for (curr = head; curr; curr = curr->next)
	{
		strcpy(t1, "\0");
		strcpy(t2, "\0");
		strcpy(t3, "\0");
		strcpy(t4, "\0");
		printf("\n%s", curr->line);
		remCom(curr->line);

		noArg = sscanf(curr->line, "%s %s %s %s", t1, t2, t3, t4); // tokenization

		if (noArg == -1)
			noArg = 0;

		// print all the tokens of a line along with the number of arguments
		printf("\t---> t1: %s\tt2: %s\tt3: %s\tt4: %s\n\tNumber of arguments: %d\n", t1, t2, t3, t4, noArg);

		// SYNTACTICAL VALIDATION OF ASSEMBLY CODE
		switch (noArg)
		{
			case 0:
				break;

			case 1:
				if ((strcasecmp(t1, "STOP") != 0) &&
					(strcasecmp(t1, "END") != 0) &&
					(strcasecmp(t1, "LTORG") != 0) &&
					(strcasecmp(t1, "START") != 0)) 
				{
					printf("\n\t***Invalid Statement!***\n");
					++syntax_error;
					break;
				}
				printf("\n\tNo error.\n");
				break;

			case 2:
				if (isStorage(t1) ||
					strcasecmp(t1, "STOP") == 0 ||
					strcasecmp(t1, "END") == 0 ||
					isCc(t1))
				{
					printf("\n\t***Invalid Statement!***\n");
					++syntax_error;
					break;
				}

				if ((strcasecmp(t2, "STOP") == 0) ||
					(strcasecmp(t2, "END") == 0) ||
					(strcasecmp(t2, "LTORG") == 0))
				{
					if (isOptab(t1) || isRegtab(t1) || isDirtab(t1) || isStorage(t1) || isCc(t1))
					{
						printf("\n\t***Invalid Label!***\n");
						++syntax_error;
						break;
					}
				}

				if ((strcasecmp(t1, "LTORG") == 0) ||
					(strcasecmp(t1, "READ") == 0) ||
					(strcasecmp(t1, "PRINT") == 0) ||
					(strcasecmp(t1, "START") == 0))
				{
					if (isOptab(t2) || isDirtab(t2) || isRegtab(t2) || isStorage(t2) || isCc(t2))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
				}

				if (strcasecmp(t1, "START") == 0)
				{
					n = atoi(t2);
					if (n <= 0 || n > 999)
					{
						printf("\n\t***Invalid Memory Location!***\n");
						++syntax_error;
						break;
					}
				}

				printf("\n\tNo error.\n");
				break;

			case 3:
				if ((strcasecmp(t2, "READ") == 0) || (strcasecmp(t2, "PRINT") == 0))
				{
					if (isOptab(t1) || isDirtab(t1) || isRegtab(t1) || isStorage(t1) || isCc(t1))
					{
						printf("\n\t***Invalid Label!***\n");
						++syntax_error;
						break;
					}
					if (isOptab(t3) || isDirtab(t3) || isRegtab(t3) || isStorage(t3) || isCc(t3))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
				}
				if ((strcasecmp(t1, "MOVER") == 0) ||
					(strcasecmp(t1, "MOVEM") == 0) ||
					(strcasecmp(t1, "ADD") == 0) ||
					(strcasecmp(t1, "SUB") == 0) ||
					(strcasecmp(t1, "DIV") == 0) ||
					(strcasecmp(t1, "MULT") == 0))
				{
					if (!(isRegtab(t2)))
					{
						printf("\n\t***Invalid Register!***\n");
						++syntax_error;
						break;
					}
					if (isOptab(t3) || isDirtab(t3) || isRegtab(t3) || isStorage(t3) || isCc(t3))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
					if(t3[0] == '=') {
						if((t3[1] != '\'' || t3[strlen(t3)-1] != '\'') && (t3[1] != '\"' || t3[strlen(t3)-1] != '\"')) {
							printf("\n\t***Invalid Literal!***\n");
							++syntax_error;
							break;
						}
					}
				}
				if (isStorage(t2))
				{
					if (isOptab(t1) || isDirtab(t1) || isRegtab(t1) || isStorage(t1) || isCc(t1))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
					if (strcasecmp(t2, "DS") == 0)
					{
						n = atoi(t3);
						if (n <= 0 || n > 999)
						{
							printf("\n\t***Invalid Memory Location!***\n");
							++syntax_error;
							break;
						}
					}
				}
				if (strcasecmp(t1, "BC") == 0)
				{
					if (!isCc(t2))
					{
						printf("\n\t***Invalid Condition Code!***\n");
						++syntax_error;
						break;
					}
					if (isOptab(t3) || isDirtab(t3) || isRegtab(t3) || isStorage(t3) || isCc(t3))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
				}
				printf("\n\tNo error.\n");
				break;

			case 4:
				if (isOptab(t1) || isDirtab(t1) || isRegtab(t1) || isStorage(t1) || isCc(t1))
				{
					printf("\n\t***Invalid Label!***\n");
					++syntax_error;
					break;
				}

				if ((!isOptab(t2)))
				{
					printf("\n\t***Invalid Statement!***\n");
					++syntax_error;
					break;
				}

				if (isOptab(t2))
				{
					if (!isRegtab(t3))
					{
						printf("\n\t***Invalid Register!***\n");
						++syntax_error;
						break;
					}
					if (isOptab(t4) || isDirtab(t4) || isRegtab(t4) || isStorage(t4) || isCc(t4))
					{
						printf("\n\t***Invalid Symbol!***\n");
						++syntax_error;
						break;
					}
				}
				printf("\n\tNo error.\n");
				break;

			default:
				printf("\n\t***Invalid number of arguments!***\n");
				++syntax_error;
				break;
		}
		if(syntax_error) break;
	}
	
	printf("\n----------------------------------------------------------------\n");

	/*
		****************************************************************
		SYNTAX CHECK DONE.

		SYMBOL & LITERAL TABLE CONSTRUCTION NEXT
		****************************************************************
	*/

	if(!syntax_error)
	{
		for(curr = head; curr; curr = curr->next)
		{
			strcpy(t1, "\0");
			strcpy(t2, "\0");
			strcpy(t3, "\0");
			strcpy(t4, "\0");
			printf("%s", curr->line);
			remCom(curr->line);
			noArg = sscanf(curr->line, "%s %s %s %s", t1, t2, t3, t4); //tokenization

			if(noArg == -1) noArg = 0;

            // SYMBOL & LITERAL TABLE CONSTRUCTION
			switch(noArg)
			{
				case 1:
					plc = lc;
					if((strcasecmp(t1, "LTORG") == 0) || strcasecmp(t1, "END") == 0) 		// LTORG
						createPool();
					++lc;
					break;

				case 2:
					if(strcasecmp(t1, "START") == 0)		// START 100
						lc = atoi(t2);
					else	// READ A
					{
						if(isOptab(t1))
						{
							plc = lc;
							enterSymtab(t2, 0);
							++lc;
						}
						else if(strcasecmp(t2, "LTORG") == 0)		// label LTORG
						{
							plc = lc;
							createPool();
							++lc;
							plc = lc;
							enterSymtab(t1, 1);
							++lc;
						}
						else 		// label STOP
						{
							plc = lc;
							enterSymtab(t1, 1);
							++lc;
						}
					}
					break;

				case 3:
					if(strcasecmp(t2, "DS") == 0)
					{
						plc = lc;
						enterSymtab(t1, 1);
						lc += atoi(t3);
					}
					if(strcasecmp(t2, "DC") == 0)
					{
						plc = lc;
						enterSymtab(t1, 1);
						++lc;
					}
					if((strcasecmp(t2, "DS") != 0) && (strcasecmp(t2, "DC") != 0))	// MOVER AREG A or label READ A
					{
						if(isOptab(t1))
						{
							if(t3[0] == '=')
							{
								plc = lc;
								enterLittab(t3);
								++lc;
							}
							else
							{
								plc = lc;
								enterSymtab(t3, 0);
								++lc;
							}
						}
						else	// label READ A
						{
							plc = lc;
							enterSymtab(t1, 1);
							enterSymtab(t3, 0);
							++lc;
						}
					}
					break;

				case 4:		// label MOVER AREG A
					if(t4[0] == '=')
					{
						plc = lc;
						enterLittab(t4);
						enterSymtab(t1, 1);
						++lc;
					}
					else
					{
						plc = lc;
						enterSymtab(t1, 1);
						enterSymtab(t4, 0);
						++lc;
					}
					break;

				default:
					continue;
			}   // end of switch
		}       // end of for loop

		symbol_error = printSymtab();
		printLittab();
		printPool();
	}
	else {
        printf("\nSyntax Error! Cannot print symbol table and literal table!\nExiting program!\n");
        free(t1); free(t2); free(t3); free(t4);
		exit(0);
    }
	printf("\n----------------------------------------------------------------\n");

	/*
		****************************************************************
		SYMBOL & LITERAL TABLE CONSTRUCTION DONE.

		INTERMEDIATE CODE (VARIANT - I) GENERATION NEXT
		****************************************************************
	*/

    if(!symbol_error)
	{
        int loc_cnt = 0, isEnd = 0;
		for(curr = head; curr; curr = curr->next)
		{
			inew = (INTCODE *)malloc(sizeof(INTCODE));
			strcpy(inew->opcode, "\0");
			strcpy(inew->operand, "\0");
			inew->regcode = 0; inew->addr = -1; inew->next = NULL;

			strcpy(t1, "\0");
			strcpy(t2, "\0");
			strcpy(t3, "\0");
			strcpy(t4, "\0");
			printf("%s", curr->line);
			remCom(curr->line);

			noArg = sscanf(curr->line, "%s %s %s %s", t1, t2, t3, t4);      // tokenization

			if(noArg == -1) noArg = 0;

			// INTERMEDIATE CODE GENERATION
			switch(noArg)
			{
				case 1:
					if(strcasecmp(t1, "STOP") == 0) {
						searchIs(t1);
						inew->regcode = 0;
						inew->addr = loc_cnt++;
					}
					else {
						searchAd(t1);					// END or LTORG
						inew->regcode = 0;
						inew->addr = -1;
						isEnd = 1;
					}
					break;

				case 2:
					if(strcasecmp(t1, "START") == 0)		// START n -> n is a memory value between 1 to 999
					{
						searchAd(t1);
						inew->regcode = 0;
						int temp = atoi(t2);
						sprintf(inew->operand, "<C, %d>", temp);
						inew->addr = -1;
						loc_cnt = temp;
					}
					else						// READ A
					{
						if(isOptab(t1))
						{
							searchIs(t1);
							inew->regcode = 0;
							inew->addr = loc_cnt++;
							searchSym(t2);							
						}
						else if(strcasecmp(t2, "LTORG") == 0)		// label LTORG
						{
							searchAd(t1);
							inew->regcode = 0;
							inew->addr = -1;
						}
						else 						// label STOP
						{
							searchIs(t1);
							inew->regcode = 0;
							inew->addr = loc_cnt++;
						}
					}
					break;

				case 3:
					if(strcasecmp(t2, "DS") == 0)
					{
						strcpy(inew->opcode, "<DL, 2>");
						inew->regcode = 0;
						int temp2 = atoi(t3);
						sprintf(inew->operand, "<C, %d>", temp2); 
						inew->addr = loc_cnt;
						loc_cnt += temp2;
					}
					if(strcasecmp(t2, "DC") == 0)
					{
						strcpy(inew->opcode, "<DL, 1>");
						inew->regcode = 0;
						int temp3 = atoi(&t3[1]);
						sprintf(inew->operand, "<C, %d>", temp3);
						inew->addr = loc_cnt++;
					}
					if((strcasecmp(t2, "DS") != 0) && (strcasecmp(t2, "DC") != 0))	// MOVER AREG A or label READ A
					{
						if(isOptab(t1))
						{
							if(strcasecmp(t1, "BC") == 0)
							{
								searchIs(t1);
								searchCc(t2);
								searchSym(t3);
							}
							else
							{
								if(t3[0] == '=')
								{
									searchIs(t1);
									searchReg(t2);
									searchLit(t3);
								}
								else
								{
									searchIs(t1);
									searchReg(t2);
									searchSym(t3);
								}
							}
						}
						else	// label READ A
						{
							searchIs(t2);
							inew->regcode = 0;
							searchSym(t3);
						}
						inew->addr = loc_cnt++;
					}
					break;

				case 4:					// label MOVER AREG A
					if(t4[0] == '=')
					{
						searchIs(t2);
						searchReg(t3);
						searchLit(t4);
					}
					else
					{
						searchIs(t2);
						searchReg(t3);
						searchSym(t4);
					}
					inew->addr = loc_cnt++;
					break;

				default:
					continue;
			}   // end of switch

			inew->next = NULL;
			if(ihead == NULL)
				ihead = icurr = inew;
			else
			{
				icurr->next = inew;
				icurr = inew;
			}

			// Another pass to process literals

			if(isEnd) {
				for(LITTAB *ltemp = lhead; ltemp; ltemp = ltemp->next) {
					inew = (INTCODE *)malloc(sizeof(INTCODE));
					strcpy(inew->opcode, "\0");
					strcpy(inew->operand, "\0");
					inew->regcode = 0; inew->addr = -1; inew->next = NULL;
		
					strcpy(inew->opcode, "<DL, 1>");
					inew->regcode = 0;
					int temp3 = atoi((ltemp->lit)+2);
					sprintf(inew->operand, "<C, %d>", temp3);
					inew->addr = ltemp->addr;

					if(inew->addr != loc_cnt) {
						free(inew);
						continue;
					}
					else {
						loc_cnt++;
						icurr->next = inew;
						icurr = inew;
					}
				}
				isEnd = 0;
			}
		}   // end of for loop

		printIntcode();
        pass2(argv[1]);

        destroySymtab();
        destroyLittab();
        destroyIntcode();

        free(t1); free(t2); free(t3); free(t4);
	}
	else {
        printf("\nSymbol error! Cannot print intermediate code!\nExiting Program!\n");
        destroySymtab();
        destroyLittab();
        free(t1); free(t2); free(t3); free(t4);
    }
	printf("\n");
}
// MAIN FUNCTION ENDS HERE.

/*
	****************************************************************
	OBJECT CODE FILE CREATION NEXT
	****************************************************************
*/

void pass2(char *fname)		// OBJECT CODE GENERATION
{
	FILE *fp2 = NULL;
	int i;
	char finalname[FILENAME_SIZE], *dot = NULL;

	strcpy(finalname, fname);
    dot = strrchr(finalname, '.');
    strcpy(dot, ".obj");

	fp2 = fopen(finalname, "w");
	if (fp2 == NULL) {
		printf("\nFailed to open file!\nExiting program!\n");
		exit(0);
	}

	for(INTCODE *temp = ihead; temp; temp = temp->next)
	{
		int opc = -1, reg = temp->regcode, opr = -1, cnt_sym = 1;

		if(temp->operand[1] == 'S')
		{
			for(SYMTAB *temp2 = shead; temp2 != NULL; temp2 = temp2->next)
			{
				if(cnt_sym == atoi(&temp->operand[4])) {
					opr = temp2->addr;
					break;
				}
				++cnt_sym;
			}
		}
		else if (temp->operand[1] == 'L') {
			for(LITTAB *temp3 = lhead; temp3; temp3 = temp3->next)
			{
				if (cnt_sym == atoi(&temp->operand[4])) {
					opr = temp3->addr;
					break;
				}
				++cnt_sym;
			}
		}
		else
			opr = atoi(&temp->operand[4]);

		if (temp->opcode[1] == 'D') {
			fprintf(fp2, "%d\t%d\n", temp->addr, opr);
			continue;
		}
		else {
			opc = atoi(&temp->opcode[5]);
			if (opc == 0)
			{
				fprintf(fp2, "%d\t000000\n", temp->addr);
				continue;
			}
		}

		if (temp->opcode[1] == 'A')
			continue;
		else if (opc <= 9)
			fprintf(fp2, "%d\t0%d%d%d\n", temp->addr, opc, reg, opr);
		else
			fprintf(fp2, "%d\t%d%d%d\n", temp->addr, opc, reg, opr);
	}
	fprintf(fp2, "-1");
	fclose(fp2);
    printf("\nObject file \'%s\' created successfully!\n", finalname);
}
