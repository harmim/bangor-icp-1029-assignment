/**
 * Assignment: Word Search
 *
 * @author Dominik Harmim harmim6@gmail.com
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/// macros
#define CLEAR() printf("\033[H\033[J") // clears screen

#define TEXT_COLOR_RED() printf("\x1B[31m") // sets output text color to red

#define TEXT_COLOR_DEFAULT() printf("\x1B[0m") // sets output text color to default


/// constants
#define CATEGORIES_COUNT 5 // number of words categories
#define WORDS_COUNT 20 // number of words in each catory
#define DIFFICULTIES_COUNT 3 // number of difficulties


/// global constant variables
/**
 * Array with words categories.
 */
const char *categories[CATEGORIES_COUNT] = {
	"Animals",
	"Furniture",
	"Food",
	"Sport",
	"Countries",
};

/**
 * Array of arrays of words in each category.
 * The order of categories is the same as the order in array of categories.
 */
const char *words[CATEGORIES_COUNT][WORDS_COUNT] = {
	{
		"dog",
		"cat",
		"camel",
		"crab",
		"cow",
		"donkey",
		"duck",
		"dolphin",
		"frog",
		"fox",
		"fish",
		"giraffe",
		"gorilla",
		"kangaroo",
		"lion",
		"monkey",
		"mouse",
		"shark",
		"sheep",
		"zebra",
	},
	{
		"chair",
		"bench",
		"bed",
		"table",
		"desk",
		"bookcase",
		"cupboard",
		"drawer",
		"workbench",
		"washstand",
		"dresser",
		"lamp",
		"light",
		"mirror",
		"nightstand",
		"rack",
		"seat",
		"shelf",
		"wardrobe",
		"bookshelf",
	},
	{
		"bacon",
		"beer",
		"bread",
		"cake",
		"coffee",
		"cookies",
		"eggs",
		"ham",
		"honey",
		"jerky",
		"milk",
		"pizza",
		"spaghetti",
		"toast",
		"rice",
		"pasta",
		"yogurt",
		"almond",
		"cheese",
		"corn",
	},
	{
		"hockey",
		"football",
		"badminton",
		"tennis",
		"rugby",
		"volleyball",
		"basketball",
		"netball",
		"baseball",
		"cricket",
		"archery",
		"cycling",
		"judo",
		"golf",
		"gymnastics",
		"curling",
		"running",
		"sailing",
		"skiing",
		"rowing",
	},
	{
		"angola",
		"algeria",
		"fiji",
		"finland",
		"france",
		"canada",
		"china",
		"poland",
		"india",
		"columbia",
		"greece",
		"mexico",
		"zimbabwe",
		"turkey",
		"sweden",
		"spain",
		"singapore",
		"peru",
		"japan",
		"brazil",
	},
};

/**
 * Array with difficulties.
 */
const char *difficulties[DIFFICULTIES_COUNT] = {
	"Easy",
	"Medium",
	"Hard",
};


/**
 * Prints welcome message.
 */
void print_welcome_message()
{
	printf(
		" ---------------------------- \n"
		"|                            |\n"
		"|   Welcome to Word Search   |\n"
		"|                            |\n"
		" ---------------------------- \n\n"
	);
}


/**
 * Gets option from user based on given options.
 *
 * @param options array with all possible options
 * @param options_count count of all possible options
 * @param option_name name of options type
 * @return index to options array selected by user or -1 in case of unknown option
 */
int get_option(const char **options, int options_count, const char *option_name)
{
	printf("What %s would you like?\n\n", option_name);
	for (int i = 0; i < options_count; i++)
	{
		printf("%d. %s\n", i + 1, options[i]);
	}

	printf("\n%s (number 1-%d): ", option_name, options_count);
	int option;
	scanf("%d", &option);

	if (option < 0 || option > CATEGORIES_COUNT)
	{
		return -1;
	}

	return option - 1;
}


/**
 * Main function, entry point of program.
 *
 * @return EXIT_SUCCESS success if program will end successfully, EXIT_FAILURE otherwise
 */
int main()
{
	print_welcome_message();

	int category = get_option(categories, CATEGORIES_COUNT, "category");
	if (category == -1)
	{
		fprintf(stderr, "Unknown category.");
		return EXIT_FAILURE;
	}

	int difficulty = get_option(difficulties, DIFFICULTIES_COUNT, "difficulty");
	if (difficulty == -1)
	{
		fprintf(stderr, "Unknown difficulty.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
