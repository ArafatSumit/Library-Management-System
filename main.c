#include <stdio.h>
#include <string.h>
#define MAX_BOOKS 100
#define MAX_USERS 20
#define MAX_TITLE 50
#define MAX_AUTHOR 30
#define MAX_NAME 30
#define MAX_PASSWORD 15
#define MAX_BORROW 3

// Represent a user
struct User 
{
    char name[MAX_NAME];
    char password[MAX_PASSWORD];  
    int borrowedBooksCount;
    char borrowedBookTitles[MAX_BORROW][MAX_TITLE];  
};
// Represent a book
struct Book {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int isBorrowed;  
    int borrowedBy;  
};
// Function to login a user
int userLogin(struct User users[], int numUsers) 
{
    char username[MAX_NAME];
    char password[MAX_PASSWORD];

    printf("Enter username: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';  
    
    for (int i = 0; i < numUsers; i++) 
    {
        if (strcmp(users[i].name, username) == 0 && strcmp(users[i].password, password) == 0) 
        {
            printf("Login successful!\n");
            return i;  // Return index of the logged in user
        }
    }

    printf("Invalid username or password. Please try again.\n");
    return -1;
}
// Function to add a new book to the library
void addBook(struct Book library[], int *numBooks)
{
    if (*numBooks >= MAX_BOOKS) 
    {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    struct Book newBook;

    printf("Enter book title: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';  

    printf("Enter book author: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0'; 
    library[*numBooks] = newBook;  
    (*numBooks)++; 

    printf("Book added successfully!\n");
}
// Function to display all books in the library
void displayBooks(struct Book library[], int numBooks, struct User users[], int currentUser)
{
    if (numBooks == 0) 
    {
        printf("No books available in the library.\n");
        return;
    }

    // Display each book's details
    for (int i = 0; i < numBooks; i++) 
    {
        // Check if the book is borrowed and who borrowed it
        if (library[i].isBorrowed) {
            // If borrowed, display the user who borrowed it
            printf("Book %d: Title: %s, Author: %s, Borrowed: Yes, Borrowed By: %s\n", 
                i + 1, library[i].title, library[i].author, users[library[i].borrowedBy].name);
        } else {
            // If not borrowed, just display that it is available
            printf("Book %d: Title: %s, Author: %s, Borrowed: No\n", 
                i + 1, library[i].title, library[i].author);
        }
    }
}

// Function to search a book by title
void searchBook(struct Book library[], int numBooks) 
{
    char title[MAX_TITLE];
    printf("Enter book title to search: ");
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = '\0';  

    for (int i = 0; i < numBooks; i++) 
    {
        if (strcmp(library[i].title, title) == 0) 
        {
            printf("Found book: Title: %s, Author: %s\n", library[i].title, library[i].author);
            return;
        }
    }

    printf("No book found with title '%s'.\n", title);
}
// Function to borrow book
void borrowBook(struct Book library[], int numBooks, struct User users[], int currentUser) 
{
    if (users[currentUser].borrowedBooksCount >= MAX_BORROW) 
    {
        printf("No more books for you.\n");
        return;
    }

    char title[MAX_TITLE];
    printf("Enter book title to borrow: ");
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = '\0';  

    for (int i = 0; i < numBooks; i++) 
    {
        if (strcmp(library[i].title, title) == 0 && !library[i].isBorrowed) 
        {
            library[i].isBorrowed = 1;
            library[i].borrowedBy = currentUser;
            strcpy(users[currentUser].borrowedBookTitles[users[currentUser].borrowedBooksCount], title);
            users[currentUser].borrowedBooksCount++;
            printf("You have successfully borrowed the book '%s'.\n", title);
            return;
        }
    }

    printf("Book '%s' is either not available or already borrowed.\n", title);
}
// Function to return a book
void returnBook(struct Book library[], int numBooks, struct User users[], int currentUser) 
{
    if (users[currentUser].borrowedBooksCount == 0) 
    {
        printf("You have no borrowed books to return.\n");
        return;
    }

    char title[MAX_TITLE];
    printf("Enter book title to return: ");
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = '\0';  

    for (int i = 0; i < numBooks; i++) 
    {
        if (strcmp(library[i].title, title) == 0 && library[i].isBorrowed && library[i].borrowedBy == currentUser) 
        {
            library[i].isBorrowed = 0;
            library[i].borrowedBy = -1;

            // Remove from borrowed books list
            for (int j = 0; j < users[currentUser].borrowedBooksCount; j++) 
            {
                if (strcmp(users[currentUser].borrowedBookTitles[j], title) == 0) 
                {
                    for (int k = j; k < users[currentUser].borrowedBooksCount - 1; k++) 
                    {
                        strcpy(users[currentUser].borrowedBookTitles[k], users[currentUser].borrowedBookTitles[k + 1]);
                    }
                    users[currentUser].borrowedBooksCount--;
                    break;
                }
            }
            printf("You have successfully returned the book '%s'.\n", title);
            return;
        }
    }

    printf("You have not borrowed the book '%s'.\n", title);
}
// delete a book admin

void deleteBook(struct Book library[], int *numBooks, struct User users[], int currentUser) 
{
    if (strcmp(users[currentUser].name, "admin") != 0) 
    {
        printf("You do not have permission to delete books.\n");
        return;
    }

    if (*numBooks == 0) 
    {
        printf("No books available to delete.\n");
        return;
    }

    char title[MAX_TITLE];
    printf("Enter book title to delete: ");
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = '\0';  

    int found = 0;
    for (int i = 0; i < *numBooks; i++)
    {
        if (strcmp(library[i].title, title) == 0) 
        {
            // Shift books down to delete the selected one
            for (int j = i; j < *numBooks - 1; j++)
            {
                library[j] = library[j + 1];
            }
            (*numBooks)--;  // Decrease the book count
            printf("Book '%s' deleted successfully.\n", title);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("No book found with title '%s'.\n", title);
    }
}

// Function to delete a user (only by admin)
void deleteUser(struct User users[], int *numUsers, int currentUser) 
{
    if (strcmp(users[currentUser].name, "admin") != 0) 
    {
        printf("You do not have permission to delete users.\n");
        return;
    }

    if (*numUsers <= 1) 
    {
        printf("Cannot delete the last user (admin).\n");
        return;
    }

    char username[MAX_NAME];
    printf("Enter username to delete: ");
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';  

    int found = 0;
    for (int i = 0; i < *numUsers; i++) 
    {
        if (strcmp(users[i].name, username) == 0) 
        {
            if (strcmp(users[i].name, "admin") == 0) 
            {
                printf("su!cide is not the only option.\n");
                return;
            }
            for (int j = i; j < *numUsers - 1; j++) 
            {
                users[j] = users[j + 1];
            }

            (*numUsers)--;  // Decrease the user count
            printf("User '%s' deleted successfully.\n", username);
            found = 1;
            break;
        }
    }

    if (!found) 
    {
        printf("No user found with username '%s'.\n", username);
    }
}

// Function to add a new user (only for admin)
void addUser(struct User users[], int *numUsers) 
{
    if (*numUsers >= MAX_USERS) 
    {
        printf("Cannot add more users. The user list is full.\n");
        return;
    }

    struct User newUser;
    printf("Enter new username: ");
    fgets(newUser.name, MAX_NAME, stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0';

    printf("Enter password: ");
    fgets(newUser.password, MAX_PASSWORD, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = '\0';
    users[*numUsers] = newUser;
    (*numUsers)++;

    printf("New user '%s' added successfully.\n", newUser.name);
}
int main()
{
    struct Book library[MAX_BOOKS];  // store books
    struct User users[MAX_USERS];  //  store users
    int numBooks = 0;  // Track number of books
    int numUsers = 0;  // Track number of users
    int currentUser = -1;  // Track the current user
    int choice;

    // Users with pass
    strcpy(users[0].name, "admin");
    strcpy(users[0].password, "admin123");  // Admin
    numUsers++;

    strcpy(users[1].name, "Sami");
    strcpy(users[1].password, "sami123");
    numUsers++;

    strcpy(users[2].name, "Sumit");
    strcpy(users[2].password, "sumit123");
    numUsers++;
    
    strcpy(users[3].name, "Akash Ghosh");
    strcpy(users[3].password, "akash123");
    numUsers++;

    strcpy(users[4].name, "Hasmi");
    strcpy(users[4].password, "hasmi123");
    numUsers++;

    strcpy(users[5].name, "Farsi");
    strcpy(users[5].password, "farsi123");
    numUsers++;

    strcpy(users[6].name, "Arafat");
    strcpy(users[6].password, "arafat123");
    numUsers++;

    strcpy(users[7].name, "Meow");
    strcpy(users[7].password, "meow123");
    numUsers++;

    strcpy(users[8].name, "Ali");
    strcpy(users[8].password, "ali123");
    numUsers++;

    strcpy(users[9].name, "Rai");
    strcpy(users[9].password, "rai123");
    numUsers++;

    while (1)
    {
        if (currentUser == -1) 
        {
            // No user logged in, show login menu
            printf("\nLibrary Management System\n");
            printf("1. Login\n");
            printf("2. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); 

            if (choice == 1)
            {
                currentUser = userLogin(users, numUsers);
            } 
            else if (choice == 2) 
            {
                printf("Exiting program..\n");
                return 0;
            } 
            else 
            {
                printf("Invalid choice! Please try again.\n");
            }
        }
        else 
        {
            // Show menu for logged-in users
            printf("\nHello, %s!\n", users[currentUser].name);
            printf("1. Add Book\n");
            printf("2. Display All Books\n");
            printf("3. Search Book\n");
            printf("4. Borrow Book\n");
            printf("5. Return Book\n");
            printf("6. Logout\n");
            
            // Only show to admin
            if (strcmp(users[currentUser].name, "admin") == 0) 
            {
                printf("7. Delete book\n");
                printf("8. Add New User\n");
                printf("9. View Borrowed Books\n");
                printf("10. Delete User\n"); 
            }

            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar ();

            switch (choice) 
            {
                case 1:
                    addBook(library, &numBooks);
                    break;
                    
                case 2:
                    displayBooks(library, numBooks, users, currentUser);
                    break;
                    
                case 3:
                    searchBook(library, numBooks);
                    break;
                    
                case 4:
                    borrowBook(library, numBooks, users, currentUser);
                    break;
                    
                case 5:
                    returnBook(library, numBooks, users, currentUser);
                    break;
                    
                case 6:
                    printf("Logging out...\n");
                    currentUser = -1;
                    break;
                    // only for admin
                case 7:
                    
                    if (strcmp(users[currentUser].name, "admin") == 0)
                    {
                        deleteBook(library, &numBooks, users, currentUser);
                    } 
                    else 
                    {
                        printf("apni delete korte parben na.\n");
                    }
                    break;

                case 8:
                    
                    if (strcmp(users[currentUser].name, "admin") == 0)
                    {
                        addUser(users, &numUsers);
                    } 
                    else 
                    {
                        printf("You do not have permission to add new users.\n");
                    }
                    break;
                    
                case 9:
                    
                    if (strcmp(users[currentUser].name, "admin") == 0) 
                    {
                        displayBooks(library, numBooks, users, currentUser);
                    }
                    else
                    {
                        printf("You do not have permission to view borrowed books.\n");
                    }
                    break;

                case 10:
                    
                    if (strcmp(users[currentUser].name, "admin") == 0) 
                    {
                        deleteUser(users, &numUsers, currentUser);
                    } 
                    else 
                    {
                        printf("You do not have permission to delete users.\n");
                    }
                    break;

                default:
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }
}