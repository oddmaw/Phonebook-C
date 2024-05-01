#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct contact
{
  char *name;
  char *Number;
  char *Email;
  struct contact *next;
} contact ;

contact *head, *tail;
int size; //for counting contacts...
contact *temp; //used to update contacts data

void initialisation(contact *con)
{
  head = NULL;
  tail = NULL;
  temp = NULL;
  size = 0;
}

// Function prototypes
void addContact_Empty(char *name, char *number, char *email);
void addContact(char *name, char *number, char *email);
int deleteContact(int position);
void clearAllContacts();

void showAllContacts();
void removeTrailingWhitespace(char *str);
int searchContact_byName(char *name);
void changePhoneNumber(char *phoneNumber);
void changeEmail(char *email);
void updateContact(char *name);
void removeNewline(char *str);

//file handling functions
void saveContacts_File(const char *filename);
void loadContacts_File(const char *filename);
//Backup&Restore Options
void backupFile(const char *filename);
void restoreFile(const char *filename);

void addContact_Empty(char *name, char *number, char *email)
{
    contact *new = (contact*)malloc(sizeof(contact));
    if(new==NULL) {
      printf("Memory allocation failed\n");
      return;
    }

    new->name = strdup(name);
    new->Number = strdup(number);
    new->Email = strdup(email);

    head = new;
    tail = new;
    new->next = NULL;
    size++;
}

void addContact(char *name, char *number, char *email)
{
    contact *new = (contact*)malloc(sizeof(contact));
    if(new==NULL) {
      printf("Memory allocation failed\n");
      return;
    }

    new->name = strdup(name);
    new->Number = strdup(number);
    new->Email = strdup(email);


    contact *current = tail;
    new->next = NULL;
    current->next = new; 
    tail = new;
    size++;
}

int deleteContact(int position)
{
  if(position>size || position<=0 || head==NULL) {
        printf("there is a problem. Cannot delete.\n");
        return -1; 
  }

  if(position==1){
    contact *delete = head;
    head=head->next;
    free(delete->name);
    free(delete->Email);
    free(delete->Number);
    free(delete);
    delete = NULL;
    size--;
    return 0;  

  } else {
    contact *current = head;
    for(int i=0; i< position-1 ; i++){
      current = current->next;
    }
    contact *delete = current->next;
    current->next = delete->next;
    free(delete->name);
    free(delete->Email);
    free(delete->Number);
    free(delete);
    delete=NULL;
    size--;
    return 0;
  }
}

void clearAllContacts()
{
  while(head != NULL) {
    deleteContact(1);
  }
}

void showAllContacts()
{
  contact *current = head;
  int i = 1;
  while(current != NULL){
    printf("  %d- %s : %s : %s",i,current->name,current->Number,current->Email);
    current = current->next;
    i++;
  }
  printf("\n");
}

void removeTrailingWhitespace(char *str) {
  int len = strlen(str);
  while (len > 0 && isspace(str[len - 1])) {
    str[len - 1] = '\0';
    len--;
  }
}

int searchContact_byName(char *name)
{
  if (head == NULL) { // Check if list is empty
  printf("Contact list is empty!\n");
  return -1;
  }

  removeTrailingWhitespace(name);
  contact *current = head;
  int i = 1;

  while(current != NULL){
    if(strcmp(current->name, name) == 0){
      printf("  %d- %s : %s : %s",i,current->name,current->Number,current->Email);
      temp = current;
      return 0;
    }
    current = current->next;
    i++;
  }
  printf("Contact not found!\n");
  return -1;
}

void changePhoneNumber(char *phoneNumber)
{
  temp->Number = strdup(phoneNumber);
}

void changeEmail(char *email)
{
  temp->Email = strdup(email);
}

void updateContact(char *name)
{ 
  int choice;
  searchContact_byName(name);
  printf("\nChoose which data u want to update:\n");
  printf("1. Phone number\n");
  printf("2. Email\n");
  scanf("%d",&choice); //read user choice


  char phoneNumber[20], email[40];
  switch (choice)
  {
  case 1:
    printf("Enter the new phone number: \n");
    getchar();
    fgets(phoneNumber, sizeof(phoneNumber), stdin); // Read phone number
    removeNewline(phoneNumber);
    changePhoneNumber(phoneNumber);
    break;
  
  case 2:
    printf("Enter the new email: \n");
    getchar();
    fgets(email, sizeof(email), stdin); // Read email
    removeNewline(email);
    changeEmail(email);
    break;

  default:
    printf("invalid choice!\n");
    break;
  }
}



void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}



// Function to save contacts to a file
void saveContacts_File(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    contact *current = head;
    while (current != NULL) {
        removeNewline(current->Email);
        fprintf(file, "%s:%s:%s\n",current->name,current->Number,current->Email);
        current = current->next;
    }

    fclose(file);
}


// Function to load contacts from a file
#define MAX_LINE_LENGTH 50
void loadContacts_File(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[MAX_LINE_LENGTH]; // Adjust size according to our needs
    size_t bufsize = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        // Skip empty lines
        if (isspace(line[0])) {
            continue;
        }

        char *name = strtok(line, ":");
        char *number = strtok(NULL, ":");
        char *email = strtok(NULL, ":");

        if(size<=0) addContact_Empty(name, number, email);
        else addContact(name, number, email);
    }

    fclose(file);
}

//backup function 
void backupFile(const char *filename)
{
  FILE *file = fopen(filename, "w");
  if(file == NULL){
    printf("Error creating backup file!\n");
    return;
  }

  contact *current = head;
  while(current != NULL) {
    removeNewline(current->Email);
    fprintf(file, "%s,%s,%s\n",current->name,current->Number,current->Email);
    current = current->next;
  }

  fclose(file);
}

//restore function test test
void restoreFile(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if(file == NULL){
    printf("Error opening backup file for restoring!\n");
    return;
  }

  clearAllContacts();

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line),file) != NULL)
  {
    if (isspace(line[0])) {
      continue;
    }

    char *name = strtok(line, ",");
    char *number = strtok(NULL, ",");
    char *email = strtok(NULL, "\n");

    if(size == 0){
      addContact_Empty(name, number, email);
    } else {
      addContact(name, number, email);
    }
  }

  fclose(file);
}




int main() {
    int choice;
    int position;
    int backupChoice;
    char name[20], number[20], email[40];

    const char *filename = "contacts.txt"; // Specify the filename
    loadContacts_File(filename);

    here:
    while (1) {
        printf("\n");
        printf("*****************************************************\n");
        printf("****************__Phonebook Menu__*******************\n");
        printf("*****************************************************\n");
        printf("  1. Add Contact\n");
        printf("  2. Search Contact by Full Name\n");
        printf("  3. Update Contact\n");
        printf("  4. Delete Contact\n");
        printf("  5. Display All Contacts\n");
        printf("  6. BackUp & Restore Option\n");
        printf("  7. Clear all contacts\n");
        printf("  8. Exit & Save\n");
        printf("*****************************************************\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); // consume newline character

        switch (choice) {
            case 1:
                printf("Enter the Full Name: \n");
                fgets(name, sizeof(name), stdin);
                removeNewline(name);

                printf("Enter the phone number: \n");
                fgets(number, sizeof(number), stdin);
                removeNewline(number);

                printf("Enter the email: \n");
                fgets(email, sizeof(email), stdin);
                removeNewline(email);

                if(size == 0) addContact_Empty(name, number, email);
                else addContact(name, number, email);
                break;
            case 2:
                printf("Enter the Full Name of the contact: \n");
                fgets(name, sizeof(name), stdin);
                searchContact_byName(name);
                break;
            case 3:
                printf("Enter the Full Name of the contact: \n");
                fgets(name, sizeof(name), stdin);
                updateContact(name);
                break;
            case 4:
                printf("Enter the position of the contact u want to delete: \n");
                scanf("%d",&position);
                deleteContact(position);
                break;
            case 5:
                printf("\n There is %d contacts!\n", size);
                printf("  --------------------Phone List:--------------------- \n");
                showAllContacts();
                break;
            case 6:
                printf("Choose Option u want to implement: \n");
                printf(" 1. Create backup\n");
                printf(" 2. Restore from previous backup\n");
                printf(" 3. Go back\n");
                printf("-----------------------------------\n");
                printf("Enter yout choice:  ");
                scanf("%d", &backupChoice);
                printf("\n");
                switch (backupChoice)
                {
                case 1:
                  backupFile("backup.txt");
                  break;
                case 2:
                  restoreFile("backup.txt");
                  break;
                case 3:
                  goto here;
                default:
                  printf("Enter 1 or 2 pls!!");
                  break;
                }
                break;
            
            case 7:
                printf("Deleting...\n");
                clearAllContacts();
                printf("All contacts deleted successfully!");
                break;
            case 8:
                printf("Saving...\n");
                saveContacts_File(filename);
                printf("Saved successfully to *contacts.txt* !.\n");
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a number from 1 to 6.\n");
        }
    }

    return 0;
}