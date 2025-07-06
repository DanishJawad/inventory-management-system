#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure to store product information
typedef struct
{
    char name[50];
    int price;
    int id;
    int quantity;
} products;


// Structure to store user credentials
typedef struct
{
    char username[50];
    char password[50];
} user_credentials;


// Function declarations
int user_validation(user_credentials *user, FILE *s);
void create_user(user_credentials *user, FILE *s);
void item_entry(products *p, int n);
void update_items(products *p, int n);
void sorting(products *p, int n);
void disp_all_items(products *p, int n , FILE *fptr);
int load_inventory(products *p, const char *filename);

int main()
{
	
	printf("\n=============================================\n");
	printf("       Inventory Management System     \n");
    printf("=============================================\n");

    user_credentials current_user;
    char choice;
    int n;
    products p[100];
    FILE *lptr;

    do
    {
        printf("\n1. Create User\n");
        printf("2. Log In\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice)
        {
        case '1':
        	// Create a new user and save credentials
        	lptr = fopen("credentials.txt","a");
            create_user(&current_user, lptr);
            fclose(lptr);
            printf("User Created Successfully\n");
            break;

        case '2':
        	char inventory_filename[100];
        	lptr = fopen("credentials.txt","r");
            if (user_validation(&current_user , lptr ) == 1)
            {
            	fclose(lptr);
                printf("\n\n\t\t\tWelcome, %s!\n\n", current_user.username);
                
				// Load existing inventory for this user
				sprintf(inventory_filename, "%s_inventory.txt", current_user.username);
				int existing = load_inventory(p, inventory_filename);
				if (existing > 0) {
				    printf("\nExisting Inventory:\n\n");
				    printf("%-4s %-20s %-10s %-10s %-10s\n", "No.", "Name", "Price", "ID", "Qty");
				    printf("-------------------------------------------------------------\n");
				
				    for (int i = 0; i < existing; i++) {
				        printf("%-4d %-20s Rs %-7d %-10d %-10d\n", 
				               i + 1, p[i].name, p[i].price, p[i].id, p[i].quantity);
				    }
				} else {
				    printf("No existing items found.\n");
				}

				// Add new products
				printf("\nHow many new products do you want to add? ");
				scanf("%d", &n);
				item_entry(p + existing, n);
				n += existing;
				printf("Items added successfully");
				
				// Sort if requested
                printf("\nDo you want to sort the products? (Y/N): ");
                scanf(" %c", &choice);
                if (choice == 'Y' || choice == 'y')
                    sorting(p, n);

                sprintf(inventory_filename, "%s_inventory.txt", current_user.username);
				
				// Save all products to file
				FILE *fptr = fopen(inventory_filename, "w");
                disp_all_items(p, n , fptr);
				fclose(fptr);
				
				//Display all items if asked
				printf("Do you want to view all existing items? (Y/N): ");
                scanf(" %c", &choice);
                if (choice == 'Y' || choice == 'y')
                {
                    int existing = load_inventory(p, inventory_filename);
					if (existing > 0) {
					    printf("\nExisting Inventory:\n\n");
					    printf("%-4s %-20s %-10s %-10s %-10s\n", "No.", "Name", "Price", "ID", "Qty");
					    printf("-------------------------------------------------------------\n");
					
					    for (int i = 0; i < existing; i++) {
					        printf("%-4d %-20s Rs %-7d %-10d %-10d\n", 
					               i + 1, p[i].name, p[i].price, p[i].id, p[i].quantity);
					    }
					    printf("\n");
					} else {
					    printf("No existing items found.\n");
					}
                }
				
				// Update items if needed
                printf("Do you want to update existing items? (Y/N): ");
                scanf(" %c", &choice);

                if (choice == 'Y' || choice == 'y')
                {
                    update_items(p, n);
                    fptr = fopen(inventory_filename, "w");
					disp_all_items(p, n , fptr);
					printf("Item updated succesfully");
					fclose(fptr);
                }
                else
                {
                    printf("See you later %s\n", current_user.username);
                }
            }
            else
            {
                printf("Invalid login credentials\n");
            }
            break;
			
        case '3':
            printf("Exiting the program\n");
            break;

        default:
            printf("Invalid choice. Please enter again.\n");
            break;
        }

    } while (choice != '3');
    
    

    return 0;
}

// Validates user login by checking credentials from file
int user_validation(user_credentials *user, FILE *s)
{
    
    char entered_username[50];
    char entered_password[50];
    char file_username[50];
    char file_password[50];

    printf("Please Enter your Username and Password\n");
    printf("Username: ");
    scanf("%s", entered_username);
    printf("Password: ");
    scanf("%s", entered_password);

    while (fscanf(s, "%s %s", file_username, file_password) != EOF) {
        if (strcmp(entered_username, file_username) == 0 &&
            strcmp(entered_password, file_password) == 0) {
            printf("\nLogged in successfully\n");
            strcpy(user->username, entered_username);
            return 1;
        }
    }

    printf("Login failed\n");
    return 0;
}


// Registers a new user by writing credentials to file
void create_user(user_credentials *user, FILE *s)
{
    printf("Enter a new username: ");
    scanf("%s", user->username);

    printf("Enter a new password: ");
    scanf("%s", user->password);

    fprintf(s, "%s %s\n", user->username, user->password);
}

// Takes input for n products
void item_entry(products *p, int n)
{
    int j = 0;

    for (int i = 0; i < n; i++)
    {

        printf("\nEnter Details of Product No # %d", ++j);

        printf("\nEnter Product Name : ");
        scanf("%s", p[i].name);

        printf("Enter Product Price : Rs ");
        scanf("%d", &p[i].price);

        printf("Enter Product ID : ");
        scanf("%d", &p[i].id);

        printf("Enter Quantity of Product : ");
        scanf("%d", &p[i].quantity);
    }
}

// Updates product quantity or price
void update_items(products *p, int n)
{
    int op;
    int prod_id;

    printf("Enter the ID of the product you want to update\nProduct ID : ");
    scanf("%d", &prod_id);

    for (int i = 0; i < n; i++)
    {

        if (prod_id == p[i].id)
        {

            printf("\nProduct Found : ");
            puts(p[i].name);

            printf("\nEnter 1 to change price \nEnter 2 to change Quantity\n");
            scanf("%d", &op);

            switch (op)
            {

            case 1:
                printf("\nEnter new price of product : ");
                scanf("%d", &p[i].price);
                break;

            case 2:
                printf("\nEnter quantity of product : ");
                scanf("%d", &p[i].quantity);
                break;

            default:
                printf("Invalid Choice");
                break;
            }
        }
    }
}

// Sorts products by ID in ascending order
void sorting(products *p, int n)
{
    printf("\nProducts are sorted on the basis of ID number\n");
    products temp;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].id > p[j].id) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}


// Writes product info to the user's file
void disp_all_items(products *p, int n , FILE *s)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(s,"%s %d %d %d\n", p[i].name, p[i].price, p[i].id, p[i].quantity);
    }
}

// Loads previous product entries from file
int load_inventory(products *p, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    int i = 0;
    while (fscanf(f, "%s %d %d %d", p[i].name, &p[i].price, &p[i].id, &p[i].quantity) == 4) {
        i++;
    }

    fclose(f);
    return i;
}

