/**
 * Assignment: Word Search
 *
 * @author Dominik Harmim harmim6@gmail.com
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/// macros
// clears screen
#define CLEAR_SCREEN() printf("\033[H\033[J")

// sets output text color to red
#define TEXT_COLOR_RED() printf("\x1B[31m")

// sets output text color to default
#define TEXT_COLOR_DEFAULT() printf("\x1B[0m")

// gets minutes from seconds
#define TIME_GET_MINUTES(_seconds) ((int) ((_seconds) / 60))

// gets remaining second after subtracting minutes
#define TIME_GET_REMAINING_SECONDS(_seconds) ((int) ((_seconds) - TIME_GET_MINUTES(_seconds) * 60))


/// constants
#define CATEGORIES_COUNT 5 // number of words categories
#define WORDS_COUNT 20 // number of words in each category
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


/// types definition
/**
 * Difficulties types.
 */
typedef enum
{
	DIFFICULTY_EASY, /// easy
	DIFFICULTY_MEDIUM, /// medium
	DIFFICULTY_HARD, /// hard
} Difficulty_type;

/**
 * Direction of word placed in grid.
 */
typedef enum
{
	WORD_DIRECTION_HORIZONTALLY, /// horizontally
	WORD_DIRECTION_VERTICALLY, /// vertically
	WORD_DIRECTION_DIAGONALLY, /// diagonally
} Word_direction;

/**
 * Order of letters in word placed in grid.
 */
typedef enum
{
	WORD_ORDER_ASCENDING, /// ascending
	WORD_ORDER_DESCENDING, /// descending
} Word_order;

/**
 * Character values in grid mask.
 */
typedef enum
{
	GRID_MASK_CHAR_EMPTY = '\0', /// non word placed
	GRID_MASK_CHAR_USED = '.', /// word placed
	GRID_MASK_CHAR_REVEALED = '*', /// word placed but already revealed
} GRID_MASK_CHAR;

/**
 * Game difficulty properties.
 */
typedef struct
{
	int grid_size; /// size of grid
	int words_count; /// number of words to generate
	float time_limit; /// time limit for game
} Difficulty;

/**
 * Structure with word which has been generated.
 */
typedef struct
{
	char *word; /// generated word
	int row; /// row of first letter in grid
	int col; /// column of first letter in grid
	int length; /// length of word
	Word_direction direction; /// direction of word in grid
	Word_order order; /// order of letters of word in grid
	bool found; /// has the word been found?
} Generated_word;


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
		" ---------------------------- \n"
	);
}


/**
 * Gets option from user based on given possible options.
 *
 * @param options array with all possible options
 * @param options_count count of all possible options
 * @param option_name name of options type
 * @return index to options array selected by user or -1 in case of unknown option
 */
int get_option(const char **options, int options_count, const char *option_name)
{
	printf("\nWhat %s would you like?\n\n", option_name);
	for (int i = 0; i < options_count; i++)
	{
		printf("%d. %s\n", i + 1, options[i]);
	}

	printf("\n%s (number 1-%d): ", option_name, options_count);
	int option;
	scanf("%d", &option);
	getchar();

	if (option <= 0 || option > options_count)
	{
		return -1;
	}

	return option - 1;
}


/**
 * Returns difficulty structure based on given difficulty type.
 *
 * @param difficulty_type difficulty type specified by user
 * @return appropriate difficulty structure
 */
Difficulty get_difficulty(Difficulty_type difficulty_type)
{
	Difficulty difficulty = {
		.grid_size = 0,
		.words_count = 0,
		.time_limit = 0.0,
	};

	switch (difficulty_type)
	{
		case DIFFICULTY_EASY:
			difficulty = (Difficulty) {
				.grid_size = 16,
				.words_count = 5,
				.time_limit = 60 * 10,
			};
			break;

		case DIFFICULTY_MEDIUM:
			difficulty = (Difficulty) {
				.grid_size = 20,
				.words_count = 10,
				.time_limit = 60 * 5,
			};
			break;

		case DIFFICULTY_HARD:
			difficulty = (Difficulty) {
				.grid_size = 24,
				.words_count = 15,
				.time_limit = 60 * 2,
			};
	}

	return difficulty;
}


/**
 * Frees a grid of given size.
 *
 * @param grid grid to be freed
 * @param size size of grid
 */
void free_grid(char **grid, int size)
{
	for (int i = 0; i < size; i++)
	{
		free(grid[i]);
	}
	free(grid);
}


/**
 * Dynamically creates and initialises an array of size * size (grid), returns a pointer to the grid
 * or NULL in case of allocation error.
 *
 * @param size size of grid
 * @return dynamically created array (grid) or NULL in case of allocation error
 */
char **create_grid(int size)
{
	// allocates memory for pointers to cols
	char **grid;
	if (!(grid = (char **) malloc(size * sizeof(char *))))
	{
		return NULL;
	}

	// allocates memory for particular column values
	for (int i = 0; i < size; i++)
	{
		if (!(grid[i] = (char *) malloc(size * sizeof(char))))
		{
			// in case of error deallocates all already allocated memory
			free_grid(grid, i);
			return NULL;
		}

		// initialises grid values
		for (int j = 0; j < size; j++)
		{
			grid[i][j] = GRID_MASK_CHAR_EMPTY;
		}
	}

	return grid;
}


/**
 * Frees array with generated words.
 *
 * @param generated_words array with generated words to be freed
 * @param count number of generated words
 */
void free_generated_words_array(Generated_word *generated_words, int count)
{
	for (int i = 0; i < count; i++)
	{
		free(generated_words[i].word);
	}
	free(generated_words);
}


/**
 * Creates and allocates memory for array of generated words.
 *
 * @param count count of generated words
 * @return array for generated words or NULL in case of allocation error
 */
Generated_word *create_generated_words_array(int count)
{
	return (Generated_word *) malloc(count * sizeof(Generated_word));
}


/**
 * Prints contents of grid with marked revealed words.
 *
 * @param grid grid to be printed out
 * @param grid_mask gird mask
 * @param size size of grid
 */
void print_grid(char **grid, char **grid_mask, int size)
{
	putchar('\n');
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (grid_mask[i][j] == GRID_MASK_CHAR_REVEALED)
			{
				TEXT_COLOR_RED();
				printf(" %c ", grid[i][j]);
				TEXT_COLOR_DEFAULT();
			}
			else
			{
				printf(" %c ", grid[i][j]);
			}
		}
		putchar('\n');
	}
	putchar('\n');
}


/**
 * Takes array (grid) and size, randomly sets each element uppercase letter.
 *
 * @param grid array (grid) to be randomised
 * @param size size of grid
 */
void fill_grid_with_random_letters(char **grid, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			grid[i][j] = (char) (65 + random() % 26);
		}
	}
}


/**
 * Finds word which has been generated.
 *
 * @param word word to be find
 * @param generated_words generated words
 * @param generated_words_count number of generated words
 * @return index of word in generated words array if word has been found, -1 otherwise
 */
int find_word(const char *word, Generated_word *generated_words, int generated_words_count)
{
	for (int i = 0; i < generated_words_count; i++)
	{
		if (strcasecmp(word, generated_words[i].word) == 0)
		{
			return i;
		}
	}

	return -1;
}


/**
 * Performs grid movement based on word direction and order.
 *
 * @param direction word direction
 * @param order word order
 * @param row pointer to row number
 * @param col pointer to column number
 */
void grid_move(Word_direction direction, Word_order order, int *row, int *col)
{
	switch (direction)
	{
		case WORD_DIRECTION_HORIZONTALLY:
			(*col)++;
			break;

		case WORD_DIRECTION_VERTICALLY:
			(*row)++;
			break;

		case WORD_DIRECTION_DIAGONALLY:
			switch (order)
			{
				case WORD_ORDER_ASCENDING:
					(*row)++;
					(*col)++;
					break;

				case WORD_ORDER_DESCENDING:
					(*row)--;
					(*col)--;
			}
	}
}


/**
 * Insert word into grid.
 *
 * @param word word to be inserted
 * @param grid words grid where word will be inserted to
 * @param grid_mask grid mask where inserted word will be marked
 * @param generated_word pointer to place in array of generated words where word will be added
 * @param direction direction of word
 * @param order order of word
 * @param difficulty game difficulty
 * @return false in case of memory allocation error, true otherwise
 */
bool inserted_word(
	const char *word,
	char **grid,
	char **grid_mask,
	Generated_word *generated_word,
	Word_direction direction,
	Word_order order,
	Difficulty difficulty
)
{
	// initialise generated word structure
	int word_length = (int) strlen(word);
	*generated_word = (Generated_word) {
		.length = word_length,
		.found = false,
		.direction = direction,
		.order = order,
	};

	// copy generated word to structure and allocates memory for it before
	if (!(generated_word->word = (char *) malloc(word_length * sizeof(char))))
	{
		return false;
	}
	strcpy(generated_word->word, word);

	// generates valid (not overlapping) word start coordinates
	int row = 0, col = 0;
	bool valid_coords;
	do
	{
		valid_coords = true;

		switch (direction)
		{
			case WORD_DIRECTION_HORIZONTALLY:
				row = (int) (random() % difficulty.grid_size);
				col = (int) (random() % (difficulty.grid_size - word_length + 1));
				break;

			case WORD_DIRECTION_VERTICALLY:
				row = (int) (random() % (difficulty.grid_size - word_length + 1));
				col = (int) (random() % difficulty.grid_size);
				break;

			case WORD_DIRECTION_DIAGONALLY:
				switch (order)
				{
					case WORD_ORDER_ASCENDING:
						row = (int) (random() % (difficulty.grid_size - word_length + 1));
						col = (int) (random() % (difficulty.grid_size - word_length + 1));
						break;

					case WORD_ORDER_DESCENDING:
						row = (int) (word_length - 1 + random() % (difficulty.grid_size - word_length + 1));
						col = (int) (word_length - 1 + random() % (difficulty.grid_size - word_length + 1));
				}
		}

		for (int i = 0, current_row = row, current_col = col; i < word_length; i++)
		{
			if (grid_mask[current_row][current_col] != GRID_MASK_CHAR_EMPTY)
			{
				valid_coords = false;
				break;
			}
			grid_move(direction, order, &current_row, &current_col);
		}
	}
	while (!valid_coords);
	generated_word->row = row;
	generated_word->col = col;

	// insert generated word to grid and mark this word as generated in grid mask
	for (int i = 0, current_row = row, current_col = col; i < word_length; i++)
	{
		grid[current_row][current_col] = (char) toupper(word[i]);
		//grid[current_row][current_col] = word[i]; // for debugging purpose only
		grid_mask[current_row][current_col] = GRID_MASK_CHAR_USED;
		grid_move(direction, order, &current_row, &current_col);
	}

	return true;
}


/**
 * Generates words.
 *
 * @param grid grid for insert generated words
 * @param grid_mask grid mask for marking generated words
 * @param generated_words array of generated words to fill
 * @param category words category
 * @param difficulty game difficulty
 * @return false in case of memory allocation error, true otherwise
 */
bool generate_words(char **grid, char **grid_mask, Generated_word *generated_words, int category, Difficulty difficulty)
{
	fill_grid_with_random_letters(grid, difficulty.grid_size);

	// words generating
	for (int i = 0; i < difficulty.words_count; i++)
	{
		// gets random word which hasn't already been generated
		int word_index = (int) (random() % WORDS_COUNT);
		while (word_index < WORDS_COUNT && find_word(words[category][word_index], generated_words, i) != -1)
		{
			word_index++;
		}
		if (word_index == WORDS_COUNT)
		{
			i--;
			continue;
		}

		// finds out direction and order of word to be generated
		Word_direction direction;
		Word_order order = WORD_ORDER_ASCENDING;
		if (i < difficulty.words_count * 0.4) // first 40% horizontally
		{
			direction = WORD_DIRECTION_HORIZONTALLY;
		}
		else if (i < difficulty.words_count * 0.4 * 2) // second 40% vertically
		{
			direction = WORD_DIRECTION_VERTICALLY;
		}
		else // last 20% diagonally
		{
			direction = WORD_DIRECTION_DIAGONALLY;
			// 50% chance that word order will be descending
			if (random() & 1)
			{
				order = WORD_ORDER_DESCENDING;
			}
		}

		// insert generated word
		if (!inserted_word(words[category][word_index], grid, grid_mask, &generated_words[i], direction, order, difficulty))
		{
			return false;
		}
	}

	return true;
}


/**
 * Marks word in generated words array and in grid mask as found word.
 *
 * @param generated_words generated word
 * @param grid_mask grid mask
 */
void mark_word_as_found(Generated_word *generated_word, char **grid_mask)
{
	generated_word->found = true;
	for (int j = 0, row = generated_word->row, col = generated_word->col; j < generated_word->length; j++)
	{
		grid_mask[row][col] = GRID_MASK_CHAR_REVEALED;
		grid_move(generated_word->direction, generated_word->order, &row, &col);
	}
}


/**
 * Performs game playing.
 *
 * @param grid words grid
 * @param grid_mask words grid mask
 * @param generated_words generated words
 * @param category selected words category
 * @param difficulty selected game difficulty
 */
void play_game(char **grid, char **grid_mask, Generated_word *generated_words, int category, Difficulty difficulty)
{
	// prints starting game message
	printf(
		"\nThere are %d words from category %s hidden in this grid.\n"
		"Can you find them all?\n"
		"You have %02d:%02d seconds!\n",
		difficulty.words_count,
		categories[category],
		TIME_GET_MINUTES(difficulty.time_limit),
		TIME_GET_REMAINING_SECONDS(difficulty.time_limit)
	);

	// initialises some variables
	int found_words = 0;
	const int max_word_length = 255;
	char word[max_word_length + 1];
	struct timeval time_start, time_end;
	gettimeofday(&time_start, NULL);

	// game cycle
	while (true)
	{
		// gets remaining time
		gettimeofday(&time_end, NULL);
		float time_remaining = difficulty.time_limit - (time_end.tv_sec - time_start.tv_sec);

		// prints words grid
		print_grid(grid, grid_mask, difficulty.grid_size);

		if (time_remaining <= 0) // time is up
		{
			printf("\nTime is up! You found only %d from %d words!!\n", found_words, difficulty.words_count);
			break;
		}

		if (found_words == difficulty.words_count) // all words has been found
		{
			float final_time = difficulty.time_limit - time_remaining;
			printf(
				"\nCongratulations! You found all the %d words in %02d:%02d seconds!!\n",
				found_words,
				TIME_GET_MINUTES(final_time),
				TIME_GET_REMAINING_SECONDS(final_time)
			);
			break;
		}

		// gets word from user
		printf(
			"\n%d words left.\n"
			"You have %02d:%02d seconds.\n"
			"Enter word: ",
			difficulty.words_count - found_words,
			TIME_GET_MINUTES(time_remaining),
			TIME_GET_REMAINING_SECONDS(time_remaining)
		);
		fgets(word, max_word_length + 1, stdin);
		char *new_line = strchr(word, '\n');
		if (new_line)
		{
			*new_line = '\0';
		}

		// marks entered word as revealed if it is in grid
		int found_word_index = find_word(word, generated_words, difficulty.words_count);
		if (found_word_index != -1 && generated_words[found_word_index].found == false)
		{
			found_words++;
			mark_word_as_found(&generated_words[found_word_index], grid_mask);
		}

		CLEAR_SCREEN();
	}
}


/**
 * Main function, entry point of program.
 *
 * @return EXIT_SUCCESS if program will end successfully, EXIT_FAILURE otherwise
 */
int main()
{
	// initialises random seed
	srandom((unsigned int) time(NULL));

	// prints welcome message
	print_welcome_message();

	// gets category from user
	int category = get_option(categories, CATEGORIES_COUNT, "category");
	if (category == -1)
	{
		fprintf(stderr, "Unknown category.\n");
		return EXIT_FAILURE;
	}

	// gets difficulty from user
	int difficulty_type = get_option(difficulties, DIFFICULTIES_COUNT, "difficulty");
	if (difficulty_type == -1)
	{
		fprintf(stderr, "Unknown difficulty.\n");
		return EXIT_FAILURE;
	}
	Difficulty difficulty = get_difficulty((Difficulty_type) difficulty_type);

	// initialises grid
	char **grid;
	if (!(grid = create_grid(difficulty.grid_size)))
	{
		fprintf(stderr, "Allocation error.\n");
		return EXIT_FAILURE;
	}

	// initialises grid mask
	char **grid_mask;
	if (!(grid_mask = create_grid(difficulty.grid_size)))
	{
		free_grid(grid, difficulty.grid_size);
		fprintf(stderr, "Allocation error.\n");
		return EXIT_FAILURE;
	}

	// initialises generated words array
	Generated_word *generated_words;
	if (!(generated_words = create_generated_words_array(difficulty.words_count)))
	{
		free_grid(grid, difficulty.grid_size);
		free_grid(grid_mask, difficulty.grid_size);
		fprintf(stderr, "Allocation error.\n");
		return EXIT_FAILURE;
	}

	if (!generate_words(grid, grid_mask, generated_words, category, difficulty))
	{
		free_grid(grid, difficulty.grid_size);
		free_grid(grid_mask, difficulty.grid_size);
		free_generated_words_array(generated_words, difficulty.words_count);
		fprintf(stderr, "Allocation error.\n");
		return EXIT_FAILURE;
	}

	// start game playing
	play_game(grid, grid_mask, generated_words, category, difficulty);

	// frees allocated memory
	free_grid(grid, difficulty.grid_size);
	free_grid(grid_mask, difficulty.grid_size);
	free_generated_words_array(generated_words, difficulty.words_count);

	return EXIT_SUCCESS;
}
