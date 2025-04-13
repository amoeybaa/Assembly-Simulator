// Object code simulator program

#include "Simulator.h"

#define FILENAME_SIZE 50

int main()
{
	int ch = -1;
	char fname[FILENAME_SIZE];
	while(1)
	{
		printf("\n\n1. Load .obj file\n2. Print Program\n3. Execute Program\n4. Trace \n5. Exit\n\n---> Enter your choice: ");
		scanf("%d", &ch);
		switch (ch)
		{
            case 1:
                if(flag) {
                    printf("\nFile already loaded!\n");
                    break;
                }
                else {
                    while (!fp)
                    {
                        printf("\nEnter file name: ");
                        scanf("%s", fname);
                        if(strcasecmp(fname, "exit") == 0) {
                            printf("\nExiting Progeam...\n");
                            if(fp) fclose(fp);
                            if(memory) free(memory);
                            exit(0);
                        }
                        fp = fopen(fname, "r");
                        if(!fp)
                            printf("\nInvalid file name!\nEnter again!\n");
                    }
                    load_file();
                    printf("\nFile loaded successfully!\n");
                    ++flag;
                    break;
                }

            case 2:
                if(!flag) {
                    printf("\nFile not loaded!");
                    break;
                }
                printf("\n");
                display();
                break;

            case 3:
                if(!flag) {
                    printf("\nFile not loaded!\n");
                    break;
                }
                printf("\n");
                execute();
                break;

            case 4:
                if(!flag) {
                    printf("\nFile not loaded!\n");
                    break;
                }
                printf("\n");
                trace();
                break;

            case 5:
                if(flag) {
                    fclose(fp);
                    printf("\nFile closed successfully! Exiting program...\n\n");
                    free(memory);
                }
                exit(0);

            default:
                printf("\nEnter a valid choice!\n");
                break;
		}
	}
}
