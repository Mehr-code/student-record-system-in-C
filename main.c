#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define FILENAME "students.db"


typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    double gpa;
} Student;


void display_menu(void);
void add_student(Student student[], int *count, int *last_id);
void update_student(Student student[], int *count);
void delete_student(Student student[], int *count, int *last_id);
void print_all_records(const Student students[], int count);
void save_to_file(const Student student[], int count);
void load_from_file(Student students[], int *count);
int get_last_id(Student students[], int count);
void clear_input_buffer(void);


int main(void){

    Student all_students[MAX_STUDENTS];
    int student_count = 0;
    int choice = 0;

    load_from_file(all_students, &student_count);

    int last_id = get_last_id(all_students, student_count) + 1;

    while(1){

        display_menu();

        if(scanf("%d", &choice)!= 1){

            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice)
        {

        case 1:
            add_student(all_students, &student_count, &last_id);
            break;

        case 2:
            update_student(all_students, &student_count);
            break;

        case 3:
            print_all_records(all_students, student_count);
            break;

        case 4:
            delete_student(all_students, &student_count, &last_id);
            break;
        
        case 5:
            save_to_file(all_students, student_count);
            break;
        
        case 6:
            printf("Exiting program. bye!\n");
            exit(0);
        
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
        
        printf("\n");
    }

    return 0;
}


void display_menu(void){
    printf("--- Student Record System ---\n");
    printf("1. Add Student\n");
    printf("2. Update Student\n");
    printf("3. Display All Records\n");
    printf("4. Delete Student\n");
    printf("5. Save Records to File\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}


void clear_input_buffer(void){

    int c;
    while((c= getchar()) != '\n' && c != EOF){

    }
}

void add_student(Student students[], int *count, int *last_id){

    if (*count >= MAX_STUDENTS){

        printf("Database is full. Cannot add more student.\n");
        return;
    }

    students[*count].id = (*last_id);

    printf("Enter Student Name: ");
    fgets(students[*count].name, MAX_NAME_LEN, stdin);
    students[*count].name[strcspn(students[*count].name, "\n")] = 0;

    printf("Enter Student GPA: ");
    scanf("%lf", &students[*count].gpa);
    clear_input_buffer();

    (*count)++;
    (*last_id)++;
    
    save_to_file(students, *count);

    printf("Student added successfully. \n");
}

void update_student(Student students[], int *count){

    int update_id;
    printf("Please Enter the ID of Student You Would like to edit: ");
    scanf("%d", &update_id);
    clear_input_buffer();

    printf("Enter Student Name: ");
    fgets(students[update_id].name, MAX_NAME_LEN, stdin);
    students[update_id].name[strcspn(students[update_id].name, "\n")] = 0;

    printf("Enter Student GPA: ");
    scanf("%lf", &students[update_id].gpa);
    clear_input_buffer();

    save_to_file(students, *count);

    printf("Student updated successfully. \n");
}

void print_all_records(const Student students[], int count){

    if(count == 0){
        printf("No records to display.\n");
        return;
    }

    printf("\n-- All Student Records ---\n");
    printf("ID      | Name                              | GPA   \n");
    printf("--------|-----------------------------------|-------\n");

    for (int i = 0; i < count; i++){
        printf("%-7d | %-33s | %.2f\n", students[i].id, students[i].name, students[i].gpa);
    }

    printf("----------------------------------------------------\n\n");
}

void delete_student(Student students[], int *count, int *last_id){

    int delete_id;
    printf("Please Enter the ID of Student You Would Like to Delete: ");
    scanf("%d", &delete_id);
    clear_input_buffer();

    delete_id--;

    for (int i = delete_id; i < *count - 1; i++)
    {
        students[i] = students[i + 1];
    }

    (*count)--;
    
    save_to_file(students, *count);
}

void save_to_file(const Student student[], int count){

    FILE *file = fopen(FILENAME, "w");

    if(file == NULL){

        fprintf(stderr, "Error: Could not open file '%s' for writing.\n", FILENAME);
        return;
    }

    for (int i = 0; i < count; i++){

        fprintf(file, "%d,%s,%.2f\n", student[i].id, student[i].name, student[i].gpa);
    }

    fclose(file);
    printf("Successfully save %d record(s) to %s. \n", count, FILENAME);
}

void load_from_file(Student students[], int *count){

    FILE *file = fopen(FILENAME, "r");
    if(file == NULL){

        printf("No existing database file found. Starting fresh.\n");
        return;
    }

    while (*count < MAX_STUDENTS && fscanf(file, "%d,%49[^,],%lf\n", &students[*count].id, students[*count].name, &students[*count].gpa) == 3)
    {
        (*count)++;
    }
    
    fclose(file);
    printf("Successfully loaded %d record(s) from %s.\n", *count, FILENAME);
}

int get_last_id(Student students[], int count){

    int max = 0;
    for (int i = 0; i < count; i++){

        if(students[i].id>max){
            max  = students[i].id;
        }
    }

    return max;
}