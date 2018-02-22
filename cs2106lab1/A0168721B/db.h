// This is the header file (aka ".h file") for the db library
// that contains the routine for maintaining our phonebook.

// When creating.h files, we always add in the following #ifndef statement to check if
// this header file has been previously included. If the PHONEBOOK symbol has
// never been defined before, then we know this is the first time we are including
// this header file. We then define the PHONEBOOK symbol. This will cause
// subsequent attempts to include this header file again to stop.

#ifndef PHONEBOOK

#define PHONEBOOK

// We use #define to define constants, rather than to use numeric values in the code
// so that if we, for example, wanted to change the maximum length of the name,
// we can just change it here, rather than have to locate and change a value
// that might be scattered all over the source code

#define NAME_LENGTH		64
#define C_LENGTH		4
#define NUM_LENGTH		10

// The typedef keyword lets us define a new datatype. In this case we define
// a new data type called TPhonebook which is a structure consisting of 
// an index number, a deleted flag, the person's name, country code and 
// phone number.
// Notice we use the constants like NAME_LENGTH defined above.
//

typedef struct
{
	unsigned index;
	int deleted;
	char name[NAME_LENGTH];
	char countryCode[C_LENGTH];
	char phoneNumber[NUM_LENGTH];
} TPhonebook;

// We want our functions to return codes that indicate success or failure.
// Rather than define the various return codes as #define, it is
// neater to define sets of values using enums
// Here we specified values for each constant, e.g. OK is 0. In actual
// fact we don't need to; C automatically sets the first constant to 0 
// and increments subsequent constants sequentially. But we use the 
// = to assign values just to show how it is done.

enum
{
	OK=0,
	MAX_REACHED=1,
	DUPLICATE=2,
	CANNOT_FIND=3,
	SAVE_FAIL=4,
	LOAD_FAIL=5
};

// Function prototypes. This is probably the most important part of the .h
// file because it allows your main program to access the functions that are
// inside our library. In the following comments you will see "Pre" and "Post"
// conditions. "Pre" conditions state the assumed state of the system, and 
// the contents of the parameters. "Post" conditions state the final state of 
// the system, and any return values. It is always a good idea to state Pre
// and Post conditions.

// Initializes the phonebook for a maximum of maxRecords file.
// Pre: Phonebook is uninitialized, maxRecords contains maximum
// number of records in our phonebook
// Post: Phonebook is initialized to maxRecords records, each of which is empty.
void initPhonebook(int maxRecords);

// Frees the phonebook.
// Pre: Phonebook was previously initialized by initPhonebook
// Post: Phonebook is freed and no longer accessible.
void freePhonebook();

// Adds in a new person into the phonebook
// Pre: Phonebook has been initialized. name = Name of person, countryCode = 3 digit country code, phoneNumber = 7 digit phone number
// Post: Person is inserted into phonebook if he isn't already there. The "result" parameter returns OK, MAX_REACHED or DUPLICATE
void addPerson(char *name, char *countryCode, char *phoneNumber, int *result);

// Looks for a person in the phonebook. We do a full string match, and cannot do partial matches.
// Pre: Phonebook has been initialized. name = Name of person to search for.
// Post: Returns a pointer to the structure containing details of the person if found, or NULL if not found.
TPhonebook *findPerson(char *name);

// Lists contents of phone book
// Pre: Phonebook has been initialized.
// Post: Phonebook is listed on stdout.
void listPhonebook();

// Deletes a person.
// Pre: Phonebook is initialized. name = Name of person to delete. Full string matches only.
// Post: Person in "name" is removed if he exists and deletePerson returns OK. If he doesn't
// exists, function returns CANNOT_FIND
int deletePerson(char *name);

// Save phonebook
// Pre: Phonebook is initialized. filename = name of file to write phonebook to.
// Post: Returns OK if successful and data is written to filename, or SAVE_FAIL
// if an error occurs. Data is not guaranteed to be saved in this case.
int saveDB(char *filename);

// Load phonebook
// Pre: filename = name of file to load phonebook from. Phonebook does not need
// to be initialized.
// Post: If successful, phonebook is initialized and contents of filename are loaded,
// and function returns OK. If failure, function returns LOAD_FAIL and phonebook
// may be invalid.
int loadDB(char *filename);

// Resize phonebook
// Pre: Phonebook is initially initialized to a maximum size
// in number of records.
// numNewRecords = Maximum number of records is incremented by this number
// Post: Maximum size of phonebook is incremented by numNewRecords.
void resizeDB(int numNewRecords);

// Get size of phonebook
// Pre: Phonebook is initialized.
// Post: nr = Number of non-empty records in phonebook, ms = Maximum size of phonebook in records.
void getDBSize(int *nr, int *ms);

// Endif for the #ifndef at the start
#endif
