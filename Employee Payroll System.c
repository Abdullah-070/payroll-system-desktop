#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
	int emp_id;
	char name[30];
	int age;
	char organization[100];
	char designation[30];
	float salary;
} Employee;

typedef struct
{
	int user_id;
	int emp_id;
	char username[30];
	char password[30];
	char role[20]; // "admin" or "employee"
} User;

// Function Prototypes
float total_salary();
void details_of_employee(Employee *emp);
void output(Employee *emp);
void integer_input_validation(int *a);
void float_input_validation(float *a);
void string_input_validation(char arr[]);
void save_employees_to_file(Employee emp[], int n);
int load_employees_from_file(Employee emp[]);
void save_users_to_file(User users[], int n);
int load_users_from_file(User users[]);
int search_employee_by_id(Employee emp[], int n, int id);
int search_employee_by_name(Employee emp[], int n, char name[]);
void delete_employee(Employee emp[], int *n, int index);
void display_all_employees(Employee emp[], int n);
void sort_employees_by_salary(Employee emp[], int n);
void generate_payroll_report(Employee emp[], int n);
void clear_screen();
void admin_dashboard(Employee emp[], int *emp_count, User users[], int *user_count);
void employee_dashboard(Employee emp[], int emp_count, User current_user);
int login_user(User users[], int user_count, User *logged_in_user);
void create_default_admin(User users[], int *user_count);
void register_employee(User users[], int *user_count, Employee emp[], int emp_count);
char select_role();

int main()
{
	Employee emp[100];
	User users[100];
	int n = 0;
	int user_count = 0;
	User logged_in_user;
	
	printf("\n=== EMPLOYEE PAYROLL MANAGEMENT SYSTEM ===\n");
	printf("Role-Based Access Control\n");
	printf("© 2025 Made by Abdullah\n\n");
	
	// Load existing data
	n = load_employees_from_file(emp);
	user_count = load_users_from_file(users);
	
	// Create default admin if no users exist
	if(user_count == 0) {
		create_default_admin(users, &user_count);
	}
	
	printf("Loaded %d employees and %d users from file.\n\n", n, user_count);
	
	int choice;
	char selected_role;
	int auth_choice;
	
	do {
		printf("\n--- WELCOME TO PAYROLL SYSTEM ---\n");
		printf("Main Menu\n\n");
		printf("  1. Continue as Admin\n");
		printf("  2. Continue as Employee\n");
		printf("  3. Exit\n\n");
		
		do {
			printf("Enter your choice (1-3): ");
			scanf("%d", &choice);
			if(getchar() != '\n') {
				printf("Invalid input. Please enter a valid option.\n");
				fflush(stdin);
				choice = -1;
			}
		} while(choice < 1 || choice > 3);
		
		if(choice == 1 || choice == 2) {
			selected_role = (choice == 1) ? 'a' : 'e'; // 'a' for admin, 'e' for employee
			
			printf("\n--- %s AUTHENTICATION ---\n\n", (choice == 1) ? "ADMIN" : "EMPLOYEE");
			printf("  1. Login\n");
			printf("  2. Register (Employee Only)\n");
			printf("  3. Back to Main Menu\n\n");
			
			do {
				printf("Enter your choice (1-3): ");
				scanf("%d", &auth_choice);
				if(getchar() != '\n') {
					printf("Invalid input.\n");
					fflush(stdin);
					auth_choice = -1;
				}
			} while(auth_choice < 1 || auth_choice > 3);
			
			if(auth_choice == 1) {
				if(login_user(users, user_count, &logged_in_user) == 1) {
					// Verify role matches selection
					if((selected_role == 'a' && strcmp(logged_in_user.role, "admin") == 0) ||
					   (selected_role == 'e' && strcmp(logged_in_user.role, "employee") == 0)) {
						printf("\n✓ Login successful! Welcome %s\n\n", logged_in_user.username);
						
						if(strcmp(logged_in_user.role, "admin") == 0) {
							admin_dashboard(emp, &n, users, &user_count);
						} else {
							employee_dashboard(emp, n, logged_in_user);
						}
						
						save_employees_to_file(emp, n);
						save_users_to_file(users, user_count);
					} else {
						printf("\n✗ Invalid credentials for %s role.\n", (selected_role == 'a') ? "Admin" : "Employee");
					}
				}
			} else if(auth_choice == 2) {
				if(selected_role == 'e') {
					register_employee(users, &user_count, emp, n);
					save_users_to_file(users, user_count);
				} else {
					printf("\n✗ Registration is only available for Employees.\n");
				}
			}
		} else if(choice == 3) {
			save_employees_to_file(emp, n);
			save_users_to_file(users, user_count);
			printf("\nData saved. Thank you for using Payroll System!\n");
			printf("© 2025 Made by Abdullah\n\n");
			break;
		}
		
		if(choice != 3) {
			clear_screen();
		}
	} while(choice != 3);
	
	return 0;
}

void admin_dashboard(Employee emp[], int *emp_count, User users[], int *user_count)
{
	int choice, num, index, ch;
	
	do {
		printf("\n--- ADMIN DASHBOARD ---\n");
		printf("Complete Payroll Management Control\n\n");
		printf("  1. Add New Employee\n");
		printf("  2. View All Employees\n");
		printf("  3. Search Employee by ID\n");
		printf("  4. Search Employee by Name\n");
		printf("  5. Update Employee Record\n");
		printf("  6. Delete Employee\n");
		printf("  7. Enter Pay Details for Employee\n");
		printf("  8. Print Pay Slip\n");
		printf("  9. Sort Employees by Salary\n");
		printf(" 10. Generate Payroll Report\n");
		printf(" 11. Logout\n\n");
		
		do {
			printf("Enter your choice (1-11): ");
			scanf("%d", &choice);
			if(getchar() != '\n') {
				printf("Invalid input. Please enter a valid option.\n");
				fflush(stdin);
				choice = -1;
			}
		} while(choice < 1 || choice > 11);
		
		switch(choice)
		{
			case 1:
			if(*emp_count >= 100) {
				printf("\n✗ Maximum employees (100) reached. Cannot add more.\n");
				break;
			}
			do {
				printf("\nEnter Employee ID: ");
				scanf("%d", &num);
				if(getchar() != '\n') {
					printf("Invalid ID. Please enter a valid number.\n");
					fflush(stdin);
					num = -1;
				}
			} while(num <= 0);
			
			if(search_employee_by_id(emp, *emp_count, num) != -1) {
				printf("✗ Employee with ID %d already exists.\n", num);
				break;
			}
			
			emp[*emp_count].emp_id = num;
			printf("Enter Details of Employee\n");
			details_of_employee(&emp[*emp_count]);
			printf("\n✓ Employee Added Successfully!\n");
			(*emp_count)++;
			break;
			
			case 2:
			if(*emp_count == 0) {
				printf("\n✗ No employees in the system.\n");
			} else {
				display_all_employees(emp, *emp_count);
			}
			break;
			
			case 3:
			printf("\nEnter Employee ID to search: ");
			scanf("%d", &num);
			fflush(stdin);
			index = search_employee_by_id(emp, *emp_count, num);
			if(index != -1) {
				printf("\n✓ Employee Found:\n");
				output(&emp[index]);
			} else {
				printf("\n✗ Employee with ID %d not found.\n", num);
			}
			break;
			
			case 4:
			printf("\nEnter Employee Name to search: ");
			fflush(stdin);
			char search_name[30];
			fgets(search_name, sizeof(search_name), stdin);
			search_name[strcspn(search_name, "\n")] = '\0';
			index = search_employee_by_name(emp, *emp_count, search_name);
			if(index != -1) {
				printf("\n✓ Employee Found:\n");
				output(&emp[index]);
			} else {
				printf("\n✗ Employee with name '%s' not found.\n", search_name);
			}
			break;
			
			case 5:
			printf("\nEnter Employee ID to update: ");
			scanf("%d", &num);
			fflush(stdin);
			index = search_employee_by_id(emp, *emp_count, num);
			if(index == -1) {
				printf("✗ Employee with ID %d not found.\n", num);
				break;
			}
			
			printf("\nPress 1 to Update Name\n");
			printf("Press 2 to Update Age\n");
			printf("Press 3 to Update Designation\n");
			printf("Press 4 to Update Salary\n");
			printf("Press 5 to Update All Records\n");
			printf("Press 6 to Cancel\n");
			
			do {
				printf("\nEnter your Choice: ");
				scanf("%d", &ch);
				if(getchar() != '\n') {
					printf("Invalid choice.\n");
					fflush(stdin);
					ch = -1;
				}
			} while(ch < 1 || ch > 6);
			
			switch(ch)
			{
				case 1:
				printf("\nEnter Name: ");
				fflush(stdin);
				fgets(emp[index].name, sizeof(emp[index].name), stdin);
				emp[index].name[strcspn(emp[index].name, "\n")] = '\0';
				string_input_validation(emp[index].name);
				break;
				
				case 2:
				printf("\nEnter Age: ");
				scanf("%d", &emp[index].age);
				integer_input_validation(&emp[index].age);
				break;
				
				case 3:
				printf("\nEnter Designation: ");
				fflush(stdin);
				fgets(emp[index].designation, sizeof(emp[index].designation), stdin);
				emp[index].designation[strcspn(emp[index].designation, "\n")] = '\0';
				string_input_validation(emp[index].designation);
				break;
				
				case 4:
				printf("\nEnter Salary: ");
				scanf("%f", &emp[index].salary);
				float_input_validation(&emp[index].salary);
				break;
				
				case 5:
				printf("Enter Name: ");
				fflush(stdin);
				fgets(emp[index].name, sizeof(emp[index].name), stdin);
				emp[index].name[strcspn(emp[index].name, "\n")] = '\0';
				string_input_validation(emp[index].name);
				
				printf("Enter Age: ");
				scanf("%d", &emp[index].age);
				integer_input_validation(&emp[index].age);
				
				printf("Enter Designation: ");
				fflush(stdin);
				fgets(emp[index].designation, sizeof(emp[index].designation), stdin);
				emp[index].designation[strcspn(emp[index].designation, "\n")] = '\0';
				string_input_validation(emp[index].designation);
				
				printf("Enter Salary: ");
				scanf("%f", &emp[index].salary);
				float_input_validation(&emp[index].salary);
				break;
				
				case 6:
				printf("Update Cancelled!\n");
				break;
			}
			if(ch != 6) {
				printf("\n✓ Employee Record Updated Successfully!\n");
				printf("Updated Information:\n");
				output(&emp[index]);
			}
			break;
			
			case 6:
			printf("\nEnter Employee ID to delete: ");
			scanf("%d", &num);
			fflush(stdin);
			index = search_employee_by_id(emp, *emp_count, num);
			if(index != -1) {
				printf("Are you sure? (1-Yes, 0-No): ");
				scanf("%d", &ch);
				if(ch == 1) {
					delete_employee(emp, emp_count, index);
					printf("✓ Employee deleted successfully!\n");
				} else {
					printf("Deletion cancelled.\n");
				}
			} else {
				printf("✗ Employee with ID %d not found.\n", num);
			}
			break;
			
			case 7:
			printf("\nEnter Employee ID for pay details: ");
			scanf("%d", &num);
			fflush(stdin);
			index = search_employee_by_id(emp, *emp_count, num);
			if(index != -1) {
				printf("Enter Pay details for Employee\n");
				emp[index].salary = total_salary();
				printf("\n✓ Pay Details Added Successfully!\n");
			} else {
				printf("✗ Employee with ID %d not found.\n", num);
			}
			break;
			
			case 8:
			printf("\nEnter Employee ID for pay slip: ");
			scanf("%d", &num);
			fflush(stdin);
			index = search_employee_by_id(emp, *emp_count, num);
			if(index != -1) {
			printf("\n--- PAY SLIP ---\n");
			output(&emp[index]);
			printf("\n");
				printf("✓ Pay slip printed successfully\n");
			} else {
				printf("✗ Employee with ID %d not found.\n", num);
			}
			break;
			
			case 9:
			if(*emp_count == 0) {
				printf("\n✗ No employees in the system.\n");
			} else {
				sort_employees_by_salary(emp, *emp_count);
				printf("\n✓ Employees sorted by salary (ascending):\n");
				display_all_employees(emp, *emp_count);
			}
			break;
			
			case 10:
			if(*emp_count == 0) {
				printf("\n✗ No employees in the system.\n");
			} else {
				generate_payroll_report(emp, *emp_count);
			}
			break;
			
			case 11:
			printf("\n✓ Logging out from Admin Dashboard...\n");
			return;
		}
		
		if(choice != 11) {
			printf("\nPress 1 to continue or 0 to logout: ");
			do {
				scanf("%d", &ch);
				if(getchar() != '\n') {
					printf("Invalid input.\n");
					fflush(stdin);
					ch = -1;
				}
			} while(ch != 0 && ch != 1);
			
			if(ch == 0) {
				printf("\n✓ Logging out from Admin Dashboard...\n");
				break;
			}
		}
		
	} while(choice != 11);
}

void employee_dashboard(Employee emp[], int emp_count, User current_user)
{
	int choice, index;
	
	// Find employee record for current user
	index = search_employee_by_id(emp, emp_count, current_user.emp_id);
	
	do {
		printf("\n--- EMPLOYEE DASHBOARD ---\n");
		printf("Personal Payroll Information\n\n");
		printf("  Welcome, %s!\n\n", current_user.username);
		printf("  1. View My Profile\n");
		printf("  2. View My Salary\n");
		printf("  3. View My Pay Slip\n");
		printf("  4. Logout\n\n");
		
		do {
			printf("Enter your choice (1-4): ");
			scanf("%d", &choice);
			if(getchar() != '\n') {
				printf("Invalid input.\n");
				fflush(stdin);
				choice = -1;
			}
		} while(choice < 1 || choice > 4);
		
		switch(choice)
		{
			case 1:
			if(index != -1) {
			printf("\n--- MY PROFILE ---\n");
			output(&emp[index]);
			printf("\n");
			} else {
				printf("\n✗ Your employee record not found in system.\n");
			}
			break;
			
			case 2:
			if(index != -1) {
				printf("\nYour Current Salary: %.2f\n", emp[index].salary);
			} else {
				printf("\n✗ Your employee record not found in system.\n");
			}
			break;
			
			case 3:
			if(index != -1) {
			printf("\n--- MY PAY SLIP ---\n");
			output(&emp[index]);
			printf("\n");
			} else {
				printf("\n✗ Your employee record not found in system.\n");
			}
			break;
			
			case 4:
			printf("\n✓ Logging out from Employee Dashboard...\n");
			return;
		}
		
		if(choice != 4) {
			printf("\nPress 1 to continue or 0 to logout: ");
			int ch;
			do {
				scanf("%d", &ch);
				if(getchar() != '\n') {
					printf("Invalid input.\n");
					fflush(stdin);
					ch = -1;
				}
			} while(ch != 0 && ch != 1);
			
			if(ch == 0) {
				printf("\n✓ Logging out from Employee Dashboard...\n");
				break;
			}
		}
		
	} while(choice != 4);
}

int login_user(User users[], int user_count, User *logged_in_user)
{
	char username[30], password[30];
	int i;
	
	printf("\n--- LOGIN SCREEN ---\n");
	printf("\nEnter Username: ");
	fflush(stdin);
	fgets(username, sizeof(username), stdin);
	username[strcspn(username, "\n")] = '\0';
	
	printf("Enter Password: ");
	fflush(stdin);
	fgets(password, sizeof(password), stdin);
	password[strcspn(password, "\n")] = '\0';
	
	for(i = 0; i < user_count; i++) {
		if(strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
			*logged_in_user = users[i];
			return 1;
		}
	}
	
	printf("\n✗ Invalid username or password!\n");
	return 0;
}

void create_default_admin(User users[], int *user_count)
{
	users[0].user_id = 1;
	users[0].emp_id = 0;
	strcpy(users[0].username, "admin");
	strcpy(users[0].password, "admin123");
	strcpy(users[0].role, "admin");
	(*user_count)++;
	printf("✓ Default admin account created: admin / admin123\n");
}

void register_employee(User users[], int *user_count, Employee emp[], int emp_count)
{
	char username[30], password[30];
	int emp_id, i;
	
	printf("\n--- EMPLOYEE REGISTRATION ---\n");
	
	printf("\nEnter Employee ID (must exist in system): ");
	scanf("%d", &emp_id);
	fflush(stdin);
	
	// Check if employee exists
	int emp_index = search_employee_by_id(emp, emp_count, emp_id);
	if(emp_index == -1) {
		printf("✗ Employee with ID %d not found in system.\n", emp_id);
		return;
	}
	
	// Check if already registered
	for(i = 0; i < *user_count; i++) {
		if(users[i].emp_id == emp_id) {
			printf("✗ This employee is already registered.\n");
			return;
		}
	}
	
	printf("Enter Username: ");
	fgets(username, sizeof(username), stdin);
	username[strcspn(username, "\n")] = '\0';
	
	// Check if username already exists
	for(i = 0; i < *user_count; i++) {
		if(strcmp(users[i].username, username) == 0) {
			printf("✗ Username already exists. Try a different one.\n");
			return;
		}
	}
	
	printf("Enter Password: ");
	fgets(password, sizeof(password), stdin);
	password[strcspn(password, "\n")] = '\0';
	
	if(strlen(password) < 6) {
		printf("✗ Password must be at least 6 characters long.\n");
		return;
	}
	
	// Register new user
	users[*user_count].user_id = *user_count + 1;
	users[*user_count].emp_id = emp_id;
	strcpy(users[*user_count].username, username);
	strcpy(users[*user_count].password, password);
	strcpy(users[*user_count].role, "employee");
	(*user_count)++;
	
	printf("\n✓ Registration successful! You can now login with your credentials.\n");
}

void details_of_employee(Employee *emp)
{
	printf("\nEnter Name: ");
	fflush(stdin);
	fgets(emp->name, sizeof(emp->name), stdin);
	emp->name[strcspn(emp->name, "\n")] = '\0';
	string_input_validation(emp->name);
	
	printf("Enter Age: ");
	scanf("%d", &emp->age);
	integer_input_validation(&emp->age);
	
	printf("Enter Organization: ");
	fflush(stdin);
	fgets(emp->organization, sizeof(emp->organization), stdin);
	emp->organization[strcspn(emp->organization, "\n")] = '\0';
	string_input_validation(emp->organization);
	
	printf("Enter Designation: ");
	fflush(stdin);
	fgets(emp->designation, sizeof(emp->designation), stdin);
	emp->designation[strcspn(emp->designation, "\n")] = '\0';
	string_input_validation(emp->designation);
	
	printf("Enter Salary: ");
	scanf("%f", &emp->salary);
	float_input_validation(&emp->salary);
	fflush(stdin);
}

float total_salary()
{
	int days;
	float rate, basic, allow, over, bonus, deduct, total;
	float hr_a, trans_a, med_a;
	float annual, perform;
	float tax_d, loan_d, adv_d;
	
	printf("Enter Working Days: ");
	scanf("%d", &days);
	integer_input_validation(&days);
	
	printf("Enter Rate Per Day: ");
	scanf("%f", &rate);
	float_input_validation(&rate);
	
	basic = days * rate;
	printf("Basic Pay: %.2f\n", basic);
	
	printf("Enter House Rent Allowance: ");
	scanf("%f", &hr_a);
	float_input_validation(&hr_a);
	
	printf("Enter Transport Allowance: ");
	scanf("%f", &trans_a);
	float_input_validation(&trans_a);
	
	printf("Enter Medical Allowance: ");
	scanf("%f", &med_a);
	float_input_validation(&med_a);
	
	allow = hr_a + trans_a + med_a;
	float gross = basic + allow;
	printf("Gross Pay: %.2f\n\n", gross);
	
	printf("Enter Overtime Hours: ");
	scanf("%f", &over);
	float_input_validation(&over);
	
	printf("Enter Annual Bonus: ");
	scanf("%f", &annual);
	float_input_validation(&annual);
	
	printf("Enter Performance Bonus: ");
	scanf("%f", &perform);
	float_input_validation(&perform);
	
	bonus = annual + perform;
	
	printf("Enter Income Tax: ");
	scanf("%f", &tax_d);
	float_input_validation(&tax_d);
	
	printf("Enter Loan Deduction: ");
	scanf("%f", &loan_d);
	float_input_validation(&loan_d);
	
	printf("Enter Advance Deduction: ");
	scanf("%f", &adv_d);
	float_input_validation(&adv_d);
	
	deduct = tax_d + loan_d + adv_d;
	total = gross + over + bonus - deduct;
	
	return total;
}

void output(Employee *emp)
{
	printf("ID: %d\n", emp->emp_id);
	printf("Name: %s\n", emp->name);
	printf("Age: %d\n", emp->age);
	printf("Organization: %s\n", emp->organization);
	printf("Designation: %s\n", emp->designation);
	printf("Salary: %.2f\n", emp->salary);
}

void integer_input_validation(int *a)
{
	while(*a < 0) {
		printf("Invalid Data (negative value). Try Again: ");
		fflush(stdin);
		scanf("%d", a);
	}
}

void float_input_validation(float *a)
{
	while(*a < 0) {
		printf("Invalid Data (negative value). Try Again: ");
		fflush(stdin);
		scanf("%f", a);
	}
}

void string_input_validation(char arr[])
{
	while(arr[0] == '\0' || arr[0] == ' ') {
		printf("Please enter valid data: ");
		fflush(stdin);
		fgets(arr, 30, stdin);
		arr[strcspn(arr, "\n")] = '\0';
	}
}

void save_employees_to_file(Employee emp[], int n)
{
	FILE *file = fopen("employees.dat", "wb");
	if(file == NULL) {
		printf("Error: Could not save employees.\n");
		return;
	}
	fwrite(&n, sizeof(int), 1, file);
	fwrite(emp, sizeof(Employee), n, file);
	fclose(file);
}

int load_employees_from_file(Employee emp[])
{
	FILE *file = fopen("employees.dat", "rb");
	int n = 0;
	if(file == NULL) {
		return 0;
	}
	fread(&n, sizeof(int), 1, file);
	if(n > 0 && n <= 100) {
		fread(emp, sizeof(Employee), n, file);
	}
	fclose(file);
	return n;
}

void save_users_to_file(User users[], int n)
{
	FILE *file = fopen("users.dat", "wb");
	if(file == NULL) {
		printf("Error: Could not save users.\n");
		return;
	}
	fwrite(&n, sizeof(int), 1, file);
	fwrite(users, sizeof(User), n, file);
	fclose(file);
}

int load_users_from_file(User users[])
{
	FILE *file = fopen("users.dat", "rb");
	int n = 0;
	if(file == NULL) {
		return 0;
	}
	fread(&n, sizeof(int), 1, file);
	if(n > 0 && n <= 100) {
		fread(users, sizeof(User), n, file);
	}
	fclose(file);
	return n;
}

int search_employee_by_id(Employee emp[], int n, int id)
{
	for(int i = 0; i < n; i++) {
		if(emp[i].emp_id == id) {
			return i;
		}
	}
	return -1;
}

int search_employee_by_name(Employee emp[], int n, char name[])
{
	for(int i = 0; i < n; i++) {
		if(strcasecmp(emp[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void delete_employee(Employee emp[], int *n, int index)
{
	for(int i = index; i < *n - 1; i++) {
		emp[i] = emp[i + 1];
	}
	(*n)--;
}

void display_all_employees(Employee emp[], int n)
{
	printf("\n%-5s %-20s %-5s %-20s %-20s %-12s\n", "ID", "Name", "Age", "Organization", "Designation", "Salary");
	printf("---\n");
	for(int i = 0; i < n; i++) {
		printf("%-5d %-20s %-5d %-20s %-20s %.2f\n", emp[i].emp_id, emp[i].name, emp[i].age, emp[i].organization, emp[i].designation, emp[i].salary);
	}
	printf("\n");
}

void sort_employees_by_salary(Employee emp[], int n)
{
	for(int i = 0; i < n - 1; i++) {
		for(int j = 0; j < n - i - 1; j++) {
			if(emp[j].salary > emp[j + 1].salary) {
				Employee temp = emp[j];
				emp[j] = emp[j + 1];
				emp[j + 1] = temp;
			}
		}
	}
}

void generate_payroll_report(Employee emp[], int n)
{
	float total_salary = 0, avg_salary = 0, max_salary = 0, min_salary = 999999;
	
	printf("\n--- PAYROLL REPORT ---\n");
	printf("Total Employees: %d\n", n);
	
	for(int i = 0; i < n; i++) {
		total_salary += emp[i].salary;
		if(emp[i].salary > max_salary) {
			max_salary = emp[i].salary;
		}
		if(emp[i].salary < min_salary && emp[i].salary > 0) {
			min_salary = emp[i].salary;
		}
	}
	
	avg_salary = total_salary / n;
	
	printf("Total Salary Payable: %.2f\n", total_salary);
	printf("Average Salary: %.2f\n", avg_salary);
	printf("Highest Salary: %.2f\n", max_salary);
	printf("Lowest Salary: %.2f\n", min_salary);
	printf("\n");
}

void clear_screen()
{
	printf("\n[Press Enter to continue...]\n");
	getchar();
}

char select_role()
{
	int choice;
	printf("\n╔════════════════════════════════════════════════════════════════╗\n");
	printf("║                      SELECT YOUR ROLE                          ║\n");
	printf("╚════════════════════════════════════════════════════════════════╝\n");
	printf("\n");
	printf("  1. Admin\n");
	printf("  2. Employee\n\n");
	
	do {
		printf("Enter your choice (1-2): ");
		scanf("%d", &choice);
		if(getchar() != '\n') {
			printf("Invalid input.\n");
			fflush(stdin);
			choice = -1;
		}
	} while(choice < 1 || choice > 2);
	
	return (choice == 1) ? 'a' : 'e';
}
