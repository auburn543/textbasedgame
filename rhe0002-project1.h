//
//  rhe0002-project1.h
//  rhe0002-project1
//
//  Created by Ryan English on 4/8/16.
//  Copyright © 2016 Ryan English. All rights reserved.
//

#ifndef rhe0002_project1_h
#define rhe0002_project1_h

#include <stdio.h>

#endif /* rhe0002_project1_h */
typedef unsigned short ushort_t;
using namespace std;
enum AttributeType {TIME, INTELLIGENCE, MONEY, NO_ATTRIBUTE};

//******* Menu Functions *******
struct Menu;
/* Creates a new menu structure to be used in the game */
void initializeMenu(Menu& menu, ushort_t menu_size, string name);
/* Adds a new menu option to the menu structure */
int addMenuOption(Menu& menu, string new_option);
/* Displays menu options for user to choose from */
void displayMenu(Menu menu);
/* Invokes user selection from menu and processes request */
bool selectMainMenuOption(Menu menu, string name);

//******* Character Functions *******
struct Character;
/* Initializes newly created character and sets all of the attributes to full */
void initializeCharacter(Character& character, string name);
/* Increases an attribute by some amount */
void increaseAttribute(Character& character, AttributeType attribute, ushort_t amount_change);
/* Decreases an attribute by some amount */
int decreaseAttribute(Character& character, AttributeType attribute, ushort_t amount_change);
/* Allows user to view character's position and attribute levels */
void viewCharacter(Character character);

//******* ScoreBoard Finctions ********
struct ScoreBoard;
typedef ScoreBoard* scoreboard_ptr;
/* Adds a new score to the top of the linked list, uses organizeScores() to finalize that list */
void addNewScore(scoreboard_ptr& scores,string name, ushort_t new_score);
/* Displays all names and high scores in the linked list according to the number of scores to display */
void viewHighScores(scoreboard_ptr scores, ushort_t num_scores_to_display);

//******* Puzzle Functions ********
struct Puzzle;
typedef Puzzle* puzzle_ptr;
/* Initializes the puzzle list */
int initializePuzzles(puzzle_ptr& puzzles);
/* Adds a new puzzle to the top of the puzzle list */
bool addPuzzle(puzzle_ptr& puzzles, string question, string answer, ushort_t score_change, AttributeType attribute, int count);
/* Selects random puzzle from the list for character to play */
bool askQuestion(Character&, puzzle_ptr, int, int&);

//******* System Functions *******
/* Processes selection for menu */
void processMenu();
/* Calculates new score at the end of the game */
ushort_t calculateScore(Character character);
/* Determines whether Character can continue after death or not */
void characterContinue(Character character);
/* Returns a random number */
int getRandomNumber(int limit);
/* Executes the game */
void playGame(string name);
/* Provides detailed instructions on how to play the game */
void howToPlay();
/* User presses any key to advance to the next text output (as desired) */
void pressAnyKey();

//******* ScoreFile Functions *******
/* Loads scores from predefined file (if it exists); returns 0 if succeeded and 1 if not loaded correctly */
bool loadScores(scoreboard_ptr&);
/* Saves scores to a predefined file or creates a new file if it doesn't exist already */
bool saveScores(scoreboard_ptr);

//******* Game Functions *******
int move(Character&, puzzle_ptr, int);
/* Invokes user selection from menu and processes requets */
int selectGameMenuOption(Menu menu, Character& character, puzzle_ptr, int);
/* Returns a string of the attribute that caused the player's death */
string getLosingAttribute(Character&);
/* Player reads some article and gains intelligence but looses time */
int read(Character&);
/* Player sits and begs for money, getting some random amount */
int beg(Character&);
/* Player moves up five spaces, large monetary cost and medium time cost */
int hitchRide(Character&);
/* Asks user if he/she wants to quit the game */
int quitGame();

//*****Test Drivers*****
void testInitializeMenu();
void testAddMenuOption();
void testInitializeCharacter();
void testIncreaseAttribute();
void testDecreaseAttribute();
void testAddNewScore();
void testInitializePuzzles();
void testAddPuzzle();
void testCalculateScore();
void testHitchRide();
