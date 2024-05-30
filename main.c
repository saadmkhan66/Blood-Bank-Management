#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DONORS 1000

typedef struct Donor SD;

struct Donor {
    char name[25];
    char group[6];
    char location[15];
    char contact[15];
    int age;
    float hemoglobin;
};

// Global variables
int donors_size = 0;
char state[100] = "welcome";
char action;

// Function declarations
void init(SD donors[]);
void welcome();
void main_menu();
void purchase_donor(SD donors[]);
void add_donor(SD donors[]);
void edit_donor(SD donors[]);
void delete_donor(SD donors[]);
void view_donor(SD donors[]);
void save(SD donors[]);
void search_donors(char grp[], int searched_index[], SD donors[]);
void view_list(int size, SD list[]);
void delete_index(int index, SD list[]);

int main() {
    SD donors[MAX_DONORS];
    init(donors);

    while (strcmp(state, "EXIT") != 0) {
        if (strcmp(state, "welcome") == 0) {
            welcome();
        } else if (strcmp(state, "main_menu") == 0) {
            main_menu();
        } else if (strcmp(state, "purchase_donor") == 0) {
            purchase_donor(donors);
        } else if (strcmp(state, "add_donor") == 0) {
            add_donor(donors);
        } else if (strcmp(state, "edit_donor") == 0) {
            edit_donor(donors);
        } else if (strcmp(state, "view_donor") == 0) {
            view_donor(donors);
        } else if (strcmp(state, "delete_donor") == 0) {
            delete_donor(donors);
        }
    }

    return 0;
}

void init(SD donors[]) {
    FILE *f = fopen("donors.dat", "r");
    if (f == NULL) {
        printf("Could not open donors.dat for reading.\n");
        return;
    }

    while (1) {
        SD d;
        if (!fgets(d.name, sizeof(d.name), f)) break;
        d.name[strcspn(d.name, "\n")] = '\0';
        fscanf(f, "%s", d.group);
        fscanf(f, "%s", d.location);
        fscanf(f, "%s", d.contact);
        fscanf(f, "%d", &d.age);
        fscanf(f, "%f", &d.hemoglobin);
        fgetc(f);
        donors[donors_size] = d;
        donors_size++;
    }

    fclose(f);
}

void welcome() {
    printf("\n\n\n\n");
    printf("                        ====================================================\n");
    printf("                        ++================================================++\n");
    printf("                        ||                    Welcome                     ||\n");
    printf("                        ||                      To                        ||\n");
    printf("                        ||                 NSU BloodBank                  ||\n");
    printf("                        ||                                                ||\n");
    printf("                        ||                                                ||\n");
    printf("                        ||                               hotline: 101010  ||\n");
    printf("                        ++================================================++\n");
    printf("                        ====================================================\n");
    printf("                        press any key to continue to main menu....\n");
    getchar();
    strcpy(state, "main_menu");
}

void main_menu() {
    system("cls || clear");
    printf("<<<<<<<<<<<<<<<<<<<<<<   Main Menu   >>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
    printf("1. Purchase Donation\n");
    printf("2. Add Donor\n");
    printf("3. Edit Donor\n");
    printf("4. View Available Donors\n");
    printf("5. Delete Donor\n");
    printf("6. Exit\n\n");
    printf("press any option...");
    action = getchar();
    getchar();

    switch (action) {
        case '1':
            strcpy(state, "purchase_donor");
            break;
        case '2':
            strcpy(state, "add_donor");
            break;
        case '3':
            strcpy(state, "edit_donor");
            break;
        case '4':
            strcpy(state, "view_donor");
            break;
        case '5':
            strcpy(state, "delete_donor");
            break;
        case '6':
            strcpy(state, "EXIT");
            break;
        default:
            printf("wrong input");
            getchar();
            break;
    }
}

void purchase_donor(SD donors[]) {
    system("cls || clear");
    char grp[6];
    int searched_index[MAX_DONORS] = {-1};

    printf("<<<<<<<<<<<<<<<<<<<<<< Purchase Donor  >>>>>>>>>>>>>>>>>>>>>>>\n\n");
    printf("Info:-\n");
    printf("Enter blood group: ");
    scanf("%s", grp);
    getchar();
    search_donors(grp, searched_index, donors);
    printf("Press the index of donor which you would like to purchase...\n\n");
    printf("press x to exit to main menu...\n");

    action = getchar();
    getchar();

    if (action == 'x') {
        strcpy(state, "main_menu");
        return;
    }

    int index = searched_index[action - '0' - 1];
    if (index >= 0 && index < donors_size) {
        system("cls || clear");
        view_list(1, &donors[index]);
        printf("Are you sure you want to purchase this donor?\n");
        printf("Any purchase will remove the donor from database.\n");
        printf("Press y/n:\n");
        action = getchar();
        getchar();
        if (action == 'y') {
            printf("Donor %s has been purchased.\n", donors[index].name);
            delete_index(index, donors);
            save(donors);
        } else {
            printf("Didn't complete purchase.\n");
        }
    } else {
        printf("Invalid index.\n");
    }

    printf("Press any key to continue to main menu...\n");
    getchar();
    strcpy(state, "main_menu");
}

void add_donor(SD donors[]) {
    system("cls || clear");
    SD d;

    printf("<<<<<<<<<<<<<<<<<<<<<<    Add Donor    >>>>>>>>>>>>>>>>>>>>>>>\n\n");
    printf("Leave name empty to go back to main menu...\n\n");
    printf("Donor Info:\n");

    printf("Donor Name: ");
    fgets(d.name, sizeof(d.name), stdin);
    d.name[strcspn(d.name, "\n")] = '\0';

    if (strcmp(d.name, "") == 0) {
        printf("Didn't create donor, going back to main menu...");
        getchar();
        strcpy(state, "main_menu");
        return;
    }

    printf("Blood group: ");
    fgets(d.group, sizeof(d.group), stdin);
    d.group[strcspn(d.group, "\n")] = '\0';
    printf("Location: ");
    fgets(d.location, sizeof(d.location), stdin);
    d.location[strcspn(d.location, "\n")] = '\0';

    printf("Contact: ");
    fgets(d.contact, sizeof(d.contact), stdin);
    d.contact[strcspn(d.contact, "\n")] = '\0';
    printf("Age: ");
    scanf("%d", &d.age);

    printf("Hemoglobin Level: ");
    scanf("%f", &d.hemoglobin);
    getchar();

    donors[donors_size] = d;
    donors_size++;
    save(donors);

    printf("Donor added to database...\n");
    printf("Press any key to continue.");
    getchar();
    strcpy(state, "main_menu");
}

void edit_donor(SD donors[]) {
    system("cls || clear");
    printf("<<<<<<<<<<<<<<<<<<<<<<    Edit Donor   >>>>>>>>>>>>>>>>>>>>>>>\n\n");

    printf("These are the donors added in the database:-\n");
    view_list(donors_size, donors);

    printf("Enter x to exit.\n");
    printf("Enter the name of the donor to edit: ");
    char name[25];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    if (strcmp(name, "x") == 0) {
        strcpy(state, "main_menu");
        return;
    }

    SD *d = NULL;
    for (int i = 0; i < donors_size; i++) {
        if (strcmp(name, donors[i].name) == 0) {
            d = &donors[i];
            break;
        }
    }

    if (d == NULL) {
        printf("No donor named %s found in the database. Please try again.\n", name);
        getchar();
        return;
    }

    system("cls || clear");
    view_list(1, d);

    printf("Enter the new info of the donor\n");

    printf("Donor Name (%s): ", d->name);
    fgets(d->name, sizeof(d->name), stdin);
    d->name[strcspn(d->name, "\n")] = '\0';

    printf("Blood group (%s): ", d->group);
    fgets(d->group, sizeof(d->group), stdin);
    d->group[strcspn(d->group, "\n")] = '\0';

    printf("Location (%s): ", d->location);
    fgets(d->location, sizeof(d->location), stdin);
    d->location[strcspn(d->location, "\n")] = '\0';

    printf("Contact (%s): ", d->contact);
    fgets(d->contact, sizeof(d->contact), stdin);
    d->contact[strcspn(d->contact, "\n")] = '\0';

    printf("Age (%d): ", d->age);
    scanf("%d", &d->age);

    printf("Hemoglobin Level (%.2f): ", d->hemoglobin);
    scanf("%f", &d->hemoglobin);
    getchar();

    save(donors);

    printf("Donor updated in database...\n");
    printf("Press any key to continue.");
    getchar();
    strcpy(state, "main_menu");
}

void delete_donor(SD donors[]) {
    system("cls || clear");
    printf("<<<<<<<<<<<<<<<<<<<<<<    Delete Donor >>>>>>>>>>>>>>>>>>>>>>>\n\n");

    printf("These are the donors added in the database:-\n");
    view_list(donors_size, donors);

    printf("Enter x to exit.\n");
    printf("Enter the name of the donor to delete: ");
    char name[25];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    if (strcmp(name, "x") == 0) {
        strcpy(state, "main_menu");
        return;
    }

    int index = -1;
    for (int i = 0; i < donors_size; i++) {
        if (strcmp(name, donors[i].name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("No donor named %s found in the database. Please try again.\n", name);
        getchar();
        return;
    }

    delete_index(index, donors);
    save(donors);

    printf("Donor deleted from database...\n");
    printf("Press any key to continue.");
    getchar();
    strcpy(state, "main_menu");
}

void view_donor(SD donors[]) {
    system("cls || clear");
    printf("<<<<<<<<<<<<<<<<<<<<<< View Donors >>>>>>>>>>>>>>>>>>>>>>>\n\n");

    view_list(donors_size, donors);

    printf("Press any key to continue to main menu...");
    getchar();
    strcpy(state, "main_menu");
}

void save(SD donors[]) {
    FILE *f = fopen("donors.dat", "w");
    if (f == NULL) {
        printf("Could not open donors.dat for writing.\n");
        return;
    }

    for (int i = 0; i < donors_size; i++) {
        fprintf(f, "%s\n", donors[i].name);
        fprintf(f, "%s\n", donors[i].group);
        fprintf(f, "%s\n", donors[i].location);
        fprintf(f, "%s\n", donors[i].contact);
        fprintf(f, "%d\n", donors[i].age);
        fprintf(f, "%.2f\n", donors[i].hemoglobin);
    }

    fclose(f);
}

void search_donors(char grp[], int searched_index[], SD donors[]) {
    int count = 0;

    for (int i = 0; i < donors_size; i++) {
        if (strcmp(donors[i].group, grp) == 0) {
            searched_index[count] = i;
            count++;
        }
    }

    printf("Search results for blood group %s:\n", grp);
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, donors[searched_index[i]].name);
    }

    if (count == 0) {
        printf("No donors with blood group %s found.\n", grp);
    }
}

void view_list(int size, SD list[]) {
    for (int i = 0; i < size; i++) {
        printf("Donor %d:\n", i + 1);
        printf("Name: %s\n", list[i].name);
        printf("Blood Group: %s\n", list[i].group);
        printf("Location: %s\n", list[i].location);
        printf("Contact: %s\n", list[i].contact);
        printf("Age: %d\n", list[i].age);
        printf("Hemoglobin Level: %.2f\n", list[i].hemoglobin);
        printf("\n");
    }
}

void delete_index(int index, SD list[]) {
    for (int i = index; i < donors_size - 1; i++) {
        list[i] = list[i + 1];
    }

    donors_size--;
}
