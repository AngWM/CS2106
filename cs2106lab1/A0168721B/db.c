// We bring in various headers to support printf, string compare, etc.
// When we use angle brackets "<" and ">" in our #include, gcc will search
// for the header in /usr/include and other directories specified by the gcc -I
// option. man gcc for details.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// We include our own header for db.h so that we have the data structure, enum and
// function prototypes. Note we use quotes here in the #include, which causes C to
// search for the header file in the current directory.

#include "db.h"


// We first declare some variables that we need to store our phonebook.
// These are not declared in the db.h file, but this does not guarantee that
// they're not accessible outside of db.c. We can, for example, declare
// "extern int numRecords;" in another program file, and that will give us
// access to numRecords in db.c. To prevent this, all variables are declared
// static, which makes them accessible only to db.c.

// Maximum size of DB
static int maxSize = 0;

// Current number of records
static int numRecords = 0;

// The database
static TPhonebook *database = NULL;

void initPhonebook(int maxRecords)
{

	// The database variable was initialized to be NULL. If it isn't NULL
	// It had previously been initialized. We free the memory from the 
	// previous initialization.
	if (database != NULL)
		free(database);

	// Maintain maximum size of phonebook, and initialize number of 
	// records to 0.
	maxSize = maxRecords;
	numRecords = 0;

	// Call calloc to allocate memory for phonebook. "Calloc" is different from malloc
	// because it lets you specify the number of variables to create, as well as the
	// size of each variable. calloc also clears the memory it allocates.
	database = (TPhonebook *) calloc(maxSize, sizeof(TPhonebook));
}

void addPerson(char *name, char *countryCode, char *phoneNumber, int *result)
{
	// Check if we have reached the maximum size of this phonebook. If so we return MAX_REACHED
	// in result. Note because result is defined as a pointer, we use "*" to dereference.
	// This is called "call by pointer" and it allows us to return values in a parameter.
	// By default all C parameters are "call by value", which prevents us from changing
	// the value of an argument permanently.
	if (numRecords >= maxSize)
		*result = MAX_REACHED;
	else
	{
		if (findPerson(name) != NULL)
			*result = DUPLICATE;
		else
		{
			// We use strncpy to copy strings because C does not allow
			// assignment of strings with "=". We use strncpy rather than strcpy
			// because it lets us specify the maximum number of characters to copy,
			// making stack overflow attacks harder.
			strncpy(database[numRecords].name, name, NAME_LENGTH);
			strncpy(database[numRecords].countryCode, countryCode, C_LENGTH);
			strncpy(database[numRecords].phoneNumber, phoneNumber, NUM_LENGTH);

			// This is the deleted flag
			database->deleted = 0;
			database->index = numRecords++;
			*result = OK;
		}
	}
}

TPhonebook *findPerson(char *name)
{
	int i;
	int found = 0;
	TPhonebook *result = NULL;


	// Run through every record, or until we find what we want.
	for (i=0; i<numRecords && !found; i++)
	{
		// In strcmp if two strings are equal it returns a 0.
		// This statement sets "found" to be true if strcmp
		// returns 0.
		found = (strcmp(name, database[i].name) == 0);

		// We don't count records marked as "deleted".
		if (found && !database[i].deleted)
			result = &database[i];
	}

	return result;
}

void listPhonebook()
{
	printf("\nPHONE LISTING\n");
	printf(  "=============\n\n");

	// Check first if database is initialized.
	if (database != NULL)
	{
		int i;

		// Go over every record that is not marked "deleted" and print the details.
		for (i=0; i<numRecords; i++)
			if (!database[i].deleted)
				printf("%d: %s (%s)-(%s)\n", i+1, database[i].name, database[i].countryCode, database[i].phoneNumber);
	}
	else
		printf("*** EMPTY ***\n\n");
}

int deletePerson(char *name)
{

	// We reuse the findPerson function to locate the person we want to delete.
	TPhonebook *result = findPerson(name);

	// If findPerson returns a NULL, it means that we cannot find this person
	// in the phonebook. We return CANNOT_FIND. Note we could have used a "results"
	// parameter like in addPerson, but we just want to try something new.
	if (result == NULL)
		return CANNOT_FIND;
	else
	{
		// If found, set the deleted flag to true and return OK.
		// Note that we really should implement an undelete function.
		result->deleted = 1;
		return OK;
	}
}

int saveDB(char *filename)
{
	// This is how you open a file. The "w" parameter means we open only for
	// writing. fopen returns a pointer to a structure of type FILE, which
	// contains important information like where we are currently in a file.
	FILE *fp = fopen(filename, "w");

	// fopen returns NULL if there's been an error.
	if (fp != NULL)
	{
		// Write the maximum size of the phonebook and current number of
		// non empty records.
		fprintf(fp, "%d %d\n", maxSize, numRecords);

		// Go over all records and store everything. We also store deleted records.
		for (int i=0; i<numRecords; i++)
			fprintf(fp, "%d\n%d\n%s\n%s\n%s\n", database[i].index, database[i].deleted, database[i].name, database[i].countryCode, database[i].phoneNumber);

		fclose(fp);
		return OK;
	}
	else
		return SAVE_FAIL;
}

// The fgets function reads everything in a line of text including the newline
// character. This is usually undesirable, so we define this function that
// checks if there's a newline character at the end of str and replaces
// it with a NULL terminator.

// Note that this function's prototype is NOT listed in db.h, making this 
// function accessible only locally within db.c.

void stripNL(char *str)
{
	// strlen returns the length of the string excluding the terminating '\0'
	// The newline character is at the end. Since we index from 0
	// this would be strlen(str) - 1
	int NLIdx = strlen(str) - 1;

	if (str[NLIdx] == '\r' || str[NLIdx] == '\n')
		str[NLIdx] = '\0';

}

int loadDB(char *filename)
{
	// Open the file for reading.
	FILE *fp = fopen(filename, "r");

	int dbSize, dbRecords;

	if (fp != NULL)
	{
		// Read in the maximum size of the phonebook and # of records including
		// deleted records
		fscanf(fp, "%d %d\n", &dbSize, &dbRecords);

		printf("Read %d records max DB size is %d\n", dbRecords, dbSize);

		// Initialize the phonebook. Remember that this will deallocate
		// any previously initialized phonebooks.
		initPhonebook(dbSize);

		// Go over each record
		for (int i=0; i<dbRecords; i++){
			char buffer[128];

			// Read in the index and delete flags
			fscanf(fp, "%d\n", &database[i].index);
			fscanf(fp, "%d\n", &database[i].deleted);

			// We make use of fgets instead of fscanf because fscanf reads only up to
			// the first white space or newline. So with a name like "Jabba the Hutt",
			// fscanf will only read in "Jabba". Subsequent fscanf statements will read in
			// "the", and "Hutt".
			// fgets will read all the way to the newline character. The problem is that it
			// also reads in the newline character.
			fgets(buffer, NAME_LENGTH, fp);

			// So we must call our stripNL function from earlier on to remove it.
			stripNL(buffer);
			printf("%d: Name: %s\n", i, buffer);
			strcpy(database[i].name, buffer);

			fgets(buffer, NAME_LENGTH, fp);
			stripNL(buffer);
			printf("%d: Country Code: %s\n", i, buffer);
			strcpy(database[i].countryCode, buffer);

			fgets(buffer, NAME_LENGTH, fp);
			stripNL(buffer);
			printf("%d: Phone Number: %s\n", i, buffer);
			strcpy(database[i].phoneNumber, buffer);
		}

		// Record maximum phonebook size and number of non empty records.
		maxSize = dbSize;
		numRecords = dbRecords;
		return OK;
	}
	else 
		return LOAD_FAIL;
}

void resizeDB(int numNewRecords)
{
	// Increment maximum size by numNewRecords.
	maxSize += numNewRecords;

	// realloc allocates new memory, copies the contents from the previous memory pointed
	// to by database, then frees the old memory. We need to save the address returned by
	// realloc because this points to the new expanded memory.
	database = (TPhonebook *) realloc(database, sizeof(TPhonebook) * maxSize);
}

void getDBSize(int *nr, int *ms)
{
	*nr = numRecords;
	*ms = maxSize;
}

void freePhonebook()
{
	if (database != NULL)
		free(database);
}

