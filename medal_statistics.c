#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The name of the country and the amounts of gold, silver and bronze medals

struct Country {
	char name[50];
	int gold;
	int silver;
	int bronze;
};

// Add country to the list of countries

struct Country *addCountry(struct Country *c_list, const char *name) 
{
	int len = 0;
	int i = 1;
	while (c_list[len].name[0] != '\0') // Check the amount of countries in the list
	{
		len++;
		i++;
	}
	struct Country *arr = (struct Country*)realloc(c_list, (i + 2) * sizeof(struct Country));
	strcpy(arr[len].name, name);
	// Initialize the amounts of medals to zero
	arr[len].gold = 0;
	arr[len].silver = 0;
	arr[len].bronze = 0;
	arr[i].name[0] = '\0';
	return arr;
}

// Add medals to the given country

struct Country *updateMedals(struct Country *c_list, char *name, int golds, int silvers, int bronzes) 
{
	int len = 0;
	int i = 1;
	while (strcmp(c_list[len].name, name) != 0)
	{
		if (c_list[len].name[0] == '\0') //Check if the country is in the list
		{
			printf("\nThe country is not in the list!\n");
			return c_list;
		}
		len++;
		i++;
	}
	// Udate the medals to the given country
	c_list[len].gold = c_list[len].gold + golds;
	c_list[len].silver = c_list[len].silver + silvers;
	c_list[len].bronze = c_list[len].bronze + bronzes;
	printf("\nAmount of medals updated!\n");
	return c_list;
}

// Helper function to qsort function which sorts the list of countries by the amount of medals

int compareMedals(const void *a, const void *b)
{
	const struct Country *a_name = a;
	const struct Country *b_name = b;
	if (a_name->gold > b_name->gold)
	{
		return -1;
	}
	else
	{
		if (a_name->gold == b_name->gold)
		{
			if (a_name->silver > b_name->silver)
			{
				return -1;
			}
			else if (a_name->silver == b_name->silver)
			{
				if (a_name->bronze > b_name->bronze)
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
			return 1;
		}
		return 1;
	}
}

// Print the countries in the list and their amount of medals in the correct order

void printMedals(struct Country *c_list) 
{
	int len = 0;
	while (c_list[len].name[0] != '\0')
	{
		len++;
	}
	qsort(c_list, len, sizeof(struct Country), compareMedals);
	int k = 0;
	while (c_list[k].name[0] != '\0')
	{
		printf("%s: Gold medals: %d, Silver medals: %d, Bronze medals: %d\n", c_list[k].name, c_list[k].gold, c_list[k].silver, c_list[k].bronze);
		k++;
	}
}

// Save countries and the medal statistics to a txt-file

void saveMedals(struct Country *c_list, char *filename)
{
	FILE *f = fopen(filename, "w"); // Open the file for writing and see if it was successful
	if (f == NULL)
	{
		printf("Something went wrong\n\n");
		return;
	}
	int len = 0;
	while (c_list[len].name[0] != '\0') // Print the list to a txt-file
	{
		fprintf(f, "%s %d %d %d\n", c_list[len].name, c_list[len].gold, c_list[len].silver, c_list[len].bronze);
		len++;
	}
	fclose(f);
	printf("\nFile created!\n");
	return;
}

// Upload txt-file and create a new list countries and their medal statistics from it

struct Country* uploadMedals(char *filename)
{
	struct Country *arr = (struct Country*)calloc(1, sizeof(struct Country));
	FILE *f = fopen(filename, "r"); // Open a file for reading and see if it was successful
	if (f == NULL) 
	{
		printf("\nFile is not found!\n\n");
		return NULL;
	}
	int len = 0;
	int j = 1;
	// If 4 values are read from the file, they are added to a list and the size of the list is updated
	while (fscanf(f, "%s %d %d %d\n", arr[len].name, &arr[len].gold, &arr[len].silver, &arr[len].bronze) == 4)
	{
		arr = (struct Country*)realloc(arr, (j + 1) * sizeof(struct Country));
		len++;
		j++;
	}
	arr[len].name[0] = '\0';
	printf("\nThe file is uploaded!\n");
	fclose(f);
	return arr;
}

// Main function

int main(void)
{
	struct Country *c_list = (struct Country*)calloc(1, sizeof(struct Country)); // Create a list of countries and their statistics
	c_list[0].name[0] = '\0';
	printf("###################################################\n");
	printf("## WELCOME TO UPDATE COUNTRIES MEDAL STATISTICS! ##\n");
	printf("###################################################\n");
	printf("                       MENU                        \n");
	char input[200];
	input[0] = 0;
	char letter[2];
	while (input[0] != 'Q')
	{
		printf("\nEnter a letter to select an action\nA: Add a country\nM: Update medal statistics\nL: Print medal statistics\nW: Save medal statistics\nO: Load a file\nQ: Exit program\n");
		fgets(input, sizeof(input), stdin); // Read the input of the user
		if (input[0] == 'A')// Add country to the list
		{
			char countryname[40];
			sscanf(input,"%s %s", letter, countryname); // Take a letter and a coutry name from the input
			if (strlen(countryname) > 28)
			{
				printf("\nCountry name too long! Enter max. 28 character long country name.\n");
			}
			else
			{
				c_list = addCountry(c_list, countryname);
				printf("\Country added!\n");
			}
		}
		else if (input[0] == 'M') // Update the amount of medals to the given country
		{
			char countryname[29];
			int gold, silver, bronze;
			sscanf(input,"%s %s %d %d %d", letter, countryname, &gold, &silver, &bronze);
			c_list = updateMedals(c_list, countryname, gold, silver, bronze);
		}
		else if (input[0] == 'L') // Print the countries and their medal statistics
		{
			printMedals(c_list);
		}
		else if (input[0] == 'W') // Save the list to a file
		{
			char cfile[20];
			sscanf(input, "%s %s", letter, cfile); // Take a letter and the country from the input
			saveMedals(c_list, cfile);
		}
		else if (input[0] == 'O') // Upload new list from a file and free the memory from the old list
		{
			char cfile[20];
			sscanf(input,"%s %s", letter, cfile); // Take a letter and the contry from the input
			free(c_list); // Free the memory from the old list
			c_list = uploadMedals(cfile);
		}
	}
	free(c_list);// Free the memory from the list
	printf("\nExiting the program\n");
	// Exit
	return 0;
}