// Include system headers for stdin/stdout operations, string operations and standard C operations.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Include header file to our library that contains all the functions needed by our phonebook.
#include "db.h"

// These are prototypes to functions we will call inside phonebook.c. Having these prototypes up
// here allow us to write the main function right at the top. Otherwise the main function will come
// after all these functions and become hard to find.

void flushInput();
void showMenu();
void loadPhonebook();
void savePhonebook();
void newEntry();
void displayEntry();
void listEntries();
void deleteEntry();
void readName(char *name, int maxlen);


// Our main function. ac = number of arguments on command line for this program
// av is an array of strings containing the arguments.
int main(int ac, char **av)
{
	if (ac != 2)
	{
		// av[0] contains the path to the current program
		// av[1] contains the command line argument, which should be the number of records to create
		// ac contains the number of arguments on the command line including the program name
		printf("\nUsage: %s <num records>\n\n", av[0]);
		exit(-1);
	}

	// Use strtol to convert from string to integer. A 0 is returned if av[1] is not a number.
	// This is more robust than atoi which has an undefined behavior if av[1] is not a number.
	// First argument to strtol is a pointer to the string to be converted, the third is the base of av[1].
	int numRecords = strtol(av[1], NULL, 10);

	if (numRecords < 1)
	{
		// Invalid number of records
		printf("Number of records must be a positive integer. You gave %d\nj", numRecords);

		// We conventionally return -1 to the shell to indicate error.
		return -1;
	}

	initPhonebook(numRecords);
	printf("Hello welcome to Phonebook\n");
	showMenu();
	printf("\nGoodbye!");

	// We return 0 to the shell to indicate success
	return 0;
}

// Reliably flushes stdin of stray characters after scanf.
// This is because when we enter data into an scanf statement, the newline character
// caused by us pressing "Enter" will remain stuck in the input stream. This
// lets us flush any such stray characters.
void flushInput()
{
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

// This function uses fgets to read names, because fscanf will only read up
// to the first space character, whereas fgets reads all the way to the newline.
// Only problem is we have to remove the newline character.
void readName(char *name, int maxlen)
{
	fgets(name, maxlen, stdin);
	name[strlen(name)-1] = '\0';
}

// Displays the menu.
void showMenu()
{
	int exit = 0;

	while(!exit)
	{
		int nr, ms;
		getDBSize(&nr, &ms);
		
		printf("Phonebook records %d of %d\n\n", nr, ms);
		printf("1. Enter new entry\n");
		printf("2. Display entry\n");
		printf("3. List phonebook\n");	
		printf("4. Delete entry\n");	
		printf("5. Save phonebook\n");	
		printf("6. Load phonebook\n");	
		printf("0. Quit\n");
		
		printf("\n Enter choice: ");
		int num;
		scanf("%d", &num);
		flushInput();

		switch(num)
		{
			case 1:
				newEntry();
				break;
			
			case 2:
				displayEntry();
				break;
			
			case 3:
				listEntries();
				break;

			case 4:
				deleteEntry();
				break;

			case 5:
				savePhonebook();
				break;

			case 6:
				loadPhonebook();
				break;

			case 0:
				exit = 1;
				break;

			default:
				printf("Illegal entry. You entered %d. Please try again\n", num);
		}
	}
}

// Nothing really spectacular going on here: Mostly we are just
// calling our functions in db.c, then looking at the results and
// printing appropriate messages.
void loadPhonebook()
{
	printf("\nLOAD\n");
	printf(  "====\n\n");

	char filename[128];
	printf("Enter phonebook filename: ");
	scanf("%s", filename);
	flushInput();

	printf("\n");
	int result = loadDB(filename);

	if (result == OK)
		printf("\n** Load OK! **\n\n");
	else
		printf("\n** Load FAILED! **\n\n");
}

void savePhonebook()
{
	printf("\nSAVE\n");
	printf(  "====\n\n");

	char filename[128];
	printf("Enter phonebook filename: ");
	scanf("%s", filename);
	flushInput();

	printf("\n");
	int result = saveDB(filename);

	if (result == OK)
		printf("** Save OK! **\n\n");
	else
		printf("** Save FAILED! **\n\n");
}

void newEntry()
{
	printf("\nNEW ENTRY\n");
	printf(  "=========\n\n");

	char name[NAME_LENGTH];
	char countryCode[C_LENGTH];
	char phoneNumber[NUM_LENGTH];

	printf("Enter name: ");

	// We don't call flushInput after fgets because it already eats up the \n
	readName(name, NAME_LENGTH);

	printf("\nEnter country code: ");
	scanf("%s", countryCode);
	flushInput();

	printf("\nEnter phone number: ");
	scanf("%s", phoneNumber);
	flushInput();

	int result;

	// Recall that addPerson returns the outcome via a parameter called
	// result, using call-by-pointer. This is how you pass in an argument
	// by pointer, by using the "&" operator. The "&" operator is read as
	// "address of". Therefore &result is "address of result".
	addPerson(name, countryCode, phoneNumber, &result);

	switch(result)
	{
		case OK:
			printf("\n** NEW ENTRY OK! **\n");
			break;

		case DUPLICATE:
			printf("\n** ERR: Person already exists. **\n\n");
			break;

		case MAX_REACHED:
			printf("\n** ERR: Maximum records reached. **\n\n");
			break;

		default:
			printf("\n** ERR: *SOMETHING* went wrong. **\n\n");
	}
}

void displayEntry()
{
	printf("\nDISPLAY\n");
	printf(  "=======\n\n");

	char name[NAME_LENGTH];

	printf("Enter name to search: ");
	readName(name, NAME_LENGTH);
	TPhonebook *entry = findPerson(name);

	if (entry != NULL)
	{
		printf("\nName: %s\n", entry->name);
		printf("Phone: (%s)-(%s)\n\n", entry->countryCode, entry->phoneNumber);
	}
	else
		printf("\n** Cannot find entry %s **\n\n", name);
}

void listEntries()
{
	listPhonebook();
}

void deleteEntry()
{
	printf("\nDELETE\n");
	printf(  "======\n\n");

	char name[NAME_LENGTH];
	printf("Enter name: ");
	readName(name, NAME_LENGTH);

	int result = deletePerson(name);

	if (result == OK)
		printf("\n** Deletion OK! **\n\n");
	else
		printf("\n** Cannot find %s **\n\n", name);
}


