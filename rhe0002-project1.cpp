//
//  Ryan English
//  Auburn ID: 903627104
//  rhe0002-project1.cpp
//

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "rhe0002-project1.h"

//#define TESTING
#define FINAL_PRODUCT

using namespace std;
using std::fstream;
typedef string* string_ptr;
typedef unsigned short ushort_t;

//***** Menu Structure *****
struct Menu {
    string menu_name;
    string_ptr menu_options;
    ushort_t num_menu_options;
    ushort_t max_menu_size;
};

//***** Character Structure *****
struct Character {
    string name;
    ushort_t time;
    ushort_t intelligence;
    ushort_t money;
    ushort_t position;
};

//***** Scoreboard Structure (linked list) *****
struct ScoreBoard {
    string name;
    ushort_t score;
    ScoreBoard* next;
};
typedef ScoreBoard* scoreboard_ptr;

//***** Puzzle Structure (linked list) *****
struct Puzzle {
    string question;
    string answer;
    ushort_t score_change;
    AttributeType attribute_affected;
    int num_puzzles;
    Puzzle* next;
};
typedef Puzzle* puzzle_ptr;

int main() {
#ifdef TESTING
    cout << "Beginning to run test drivers..." << endl << endl;
    testInitializeMenu();
    testAddMenuOption();
    testInitializeCharacter();
    testIncreaseAttribute();
    testDecreaseAttribute();
    testAddNewScore();
    testInitializePuzzles();
    testAddPuzzle();
    testCalculateScore();
    testHitchRide();
    cout << "\nThis concludes the testing" << endl;
#endif
#ifdef FINAL_PRODUCT
    srand((unsigned) time(0));
    
    //Prompt user for name
    string character_name;
    cout << "Please enter your name: ";
    getline(cin, character_name);
    
    //Welcome Screen
    cout << "\n*********************************************" << endl;
    cout << "\t\t\tWelcome, " << character_name << endl;
    cout << "*********************************************" << endl;
    
    //Create Main Menu Structure
    Menu main;
    initializeMenu(main, 4, "Main Menu");
    if (addMenuOption(main, "1 - Start a New Game of Concourse Chaos!") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(main, "2 - How to Play") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(main, "3 - View High Scores") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(main, "4 - Quit") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    
    do {
        displayMenu(main);
    } while (selectMainMenuOption(main, character_name));
    
    return 0;
#endif
}

//********* ScoreBoard/ScoreFile Functions *********
bool loadScores(scoreboard_ptr& scores) {
    ifstream in_stream;
    in_stream.open("scores.txt");
    if (!in_stream.is_open()) return false;
    else {
        string name;
        ushort_t score;
        while (in_stream >> name >> score) {
            addNewScore(scores, name, score);
        }
        in_stream.close();
        return true;
    }
}

void addNewScore(scoreboard_ptr& scores, string name, ushort_t new_score) {
    scoreboard_ptr newNode = new ScoreBoard;
    newNode -> name = name;
    newNode -> score = new_score;
    newNode -> next = NULL;
    if (!scores) scores = newNode;
    else {
        scoreboard_ptr headPtr, currPtr, prevPtr;
        headPtr = scores;
        currPtr = headPtr;
        
        if (new_score >= headPtr -> score) { //newNode has highest score
            newNode -> next = headPtr;
            headPtr = newNode;
        }
        
        else if (headPtr -> next == NULL) { //scores only has one ScoreBoard item
            headPtr -> next = newNode;
        }
        
        else { //scores has 2 or more nodes
            while (currPtr -> next != NULL) {
                prevPtr = currPtr;
                currPtr = currPtr -> next;
                if (new_score >= currPtr -> score) { //insert node between 2 nodes
                    prevPtr -> next = newNode;
                    newNode -> next = currPtr;
                    break;
                }
            }
            //Assumes at end of linked list and node has NOT been inserted
            if (currPtr -> score > new_score) {
                currPtr -> next = newNode;
            }
        }
        
        scores = headPtr;
    }
}

void viewHighScores(scoreboard_ptr scores, ushort_t num_scores_to_display) {
    int count = 1;
    cout << "Here are the top " << num_scores_to_display << " high scores:" << endl << endl;
    if (scores != NULL && count > 0) {
        cout << count << ") " << scores -> name << " | " << scores -> score << endl;
        while (scores -> next != NULL && ++count <= num_scores_to_display) {
            scores = scores -> next;
            cout << count << ") " << scores -> name << " | " << scores -> score << endl;
        }
    }
    pressAnyKey();
}

void calculateScore(ushort_t& score, ushort_t money, ushort_t time, ushort_t intelligence, ushort_t position) {
    if (time == 600 || position == 0)
        score = 0;
    else if (position < 20) score = (money * time * intelligence) / (20 - position);
    else score = money * time * intelligence;
}

bool saveScores(scoreboard_ptr scores) {
    remove("scores.txt");
    ofstream newFile("scores.txt", std::ios_base::app);
    if (!newFile.is_open()) return false;
    if (scores == NULL) return false;
    else {
        newFile << scores -> name << endl;
        newFile << scores -> score << endl;
        while (scores -> next != NULL) {
            scores = scores -> next;
            newFile << scores -> name << endl;
            newFile << scores -> score << endl;
        }
    }
    newFile.close();
    return true;
}


//********* Main Menu Functions *********
void initializeMenu(Menu& menu, ushort_t menu_size, string name) {
    menu.max_menu_size = menu_size;
    menu.menu_name = name;
    menu.menu_options = new string[menu_size];
    menu.num_menu_options = 0;
}

int addMenuOption(Menu& menu, string new_option) {
    //Returns error code if there is not enough room in the array - 2 checks
    if (menu.num_menu_options == menu.max_menu_size) return 1;
    if (menu.num_menu_options < menu.max_menu_size) {
        menu.menu_options[menu.num_menu_options] = new_option;
        menu.num_menu_options++;
        return 0;
    }
    else return 1;
}

void displayMenu(Menu menu) {
    if (menu.num_menu_options != 0 && menu.menu_options[0] != "") {
        cout << "\n======= " << menu.menu_name << " =======" << endl;
        for (int i = 0; i < menu.num_menu_options; i++) {
            cout << menu.menu_options[i] << endl;
        }
    }
    else {
        cout << "This menu does not exist or there are no menu options available. Please contact developer!" << endl;
    }
}

bool selectMainMenuOption(Menu menu, string name) {
    int choice = 0;
    scoreboard_ptr scores = NULL;
    do {
        cout << "\nPlease enter a menu option: ";
        cin >> choice;
        if (choice < 1 || choice > menu.num_menu_options)
            cout << "That option is not on the menu!" << endl;
    } while (choice < 1 || choice > menu.num_menu_options);
    
    switch (choice) {
        //Play Game
        case 1: playGame(name);
            return true;
            break;
            //How to play
        case 2:
            howToPlay();
            return true;
            break;
        //View high scores
        case 3:
            if (!loadScores(scores)) {
                cout << "There are no high scores yet. Play the game to get your name up on top first!" << endl;
                pressAnyKey();
            }
            else {
                viewHighScores(scores, 10);
            }
            return true;
            break;
        //Quit
        case 4:
            return false;
            break;
        default:
            return true;
            break;
    }
}


//********** GamePlay Functions **********
void playGame(string name) {
    //Create Game Menu Structure
    Menu game;
    initializeMenu(game, 6, "Game Menu");
    if (addMenuOption(game, "1 - Move Forward (takes time and could be risky)") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(game, "2 - Read Newspaper (takes time but boosts inelligence)") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(game, "3 - Beg for Money (takes time but boosts money)") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(game, "4 - Hitch a ride on a golf cart (move 5 spaces without any risks; costs $80 and takes time)") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(game, "5 - View Character") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    if (addMenuOption(game, "6 - Quit Game") == 1)
        cerr << "There was an error creating the Main Menu. Please contact developer for assistance.";
    cout << "\n\n=========================\nApproaching Concourse..." << endl << "Let the Chaos begin!" << endl << "=========================\n" << endl;
    
    //Create Puzzle Structure
    puzzle_ptr puzzles;
    int num_puzzles;
    ushort_t finalScore;
    num_puzzles = initializePuzzles(puzzles);
    
    //Create new character
    Character character;
    int gameStatus = 0;
    initializeCharacter(character, name);
    while (gameStatus == 0) {
        viewCharacter(character);
        displayMenu(game);
        gameStatus = selectGameMenuOption(game, character, puzzles, num_puzzles);
    }
    if (gameStatus == 1) { //Display Win Screen
        cout << "\nCongratulations, " << name << "!! You made it all the way down the concourse alive!";
        pressAnyKey();
    }
    else if (gameStatus == 2) { //Display Game Over Screen
        cout << "\nOh no! Your " << getLosingAttribute(character) << " has fallen to 0 and you were not able to make it all the way down the concourse!" << endl;
        if (20 - character.position < 5) {
            cout << "You were so close! Only " << (20 - character.position) << " steps away from the goal!";
        }
        pressAnyKey();
    }
    else if (gameStatus == 3) { //End Game
        cout << "\n----Game Over!----" << endl;
        pressAnyKey();
    }
    cout << "\nHere are your final results:" << endl;
    viewCharacter(character);
    pressAnyKey();
    calculateScore(finalScore, character.money, character.time, character.intelligence, character.position);
    cout << "Your final score: " << finalScore << endl;
    pressAnyKey();
    scoreboard_ptr scores;
    loadScores(scores);
    addNewScore(scores, name, finalScore);
    if (saveScores(scores)) cout << "\nYour score was saved successfully!" << endl;
    pressAnyKey();
}

string getLosingAttribute(Character& character) {
    if (character.time == 0) return "time";
    else if (character.intelligence == 0) return "intelligence";
    else if (character.money == 0) return "money";
    else return "";
}

void initializeCharacter(Character& character, string name) {
    character.name = name;
    character.money = 100;
    character.intelligence = 10;
    character.time = 600;
    character.position = 0;
}

void increaseAttribute(Character& character, AttributeType attribute, ushort_t amount_change) {
    if (attribute == 1) { //INTELLIGENCE
        if (character.intelligence <= 10 && character.intelligence >= (10 - amount_change)) {
            character.intelligence = 10;
        }
        else {
            character.intelligence = character.intelligence + amount_change;
        }
    }
    else if (attribute == 2) { //MONEY
        if (character.money <= 100 && character.money >= (100 - amount_change)) {
            character.money = 100;
        }
        else {
            character.money = character.money + amount_change;
        }
    }
    else { //NONE
        return;
    }
}

int decreaseAttribute(Character& character, AttributeType attribute, ushort_t amount_change) {
    if (attribute == 0) { //TIME
        if (character.time <= amount_change) {
            character.time = 0;
            return 2;
        }
        else {
            character.time = character.time - amount_change;
            return 0;
        }
    }
    else if (attribute == 1) { //INTELLIGENCE
        if (character.intelligence <= amount_change) {
            character.intelligence = 0;
            return 2;
        }
        else {
            character.intelligence = character.intelligence - amount_change;
            return 0;
        }
    }
    else if (attribute == 2) { //MONEY
        if (character.money <= amount_change) {
            character.money = 0;
            return 2;
        }
        else {
            character.money = character.money - amount_change;
            return 0;
        }
    }
    else { //NONE
        return 0;
    }
}

void viewCharacter(Character character) {
    cout << "\n" << character.name << "'s Attributes:" << endl;
    cout << "\tIntelligence: " << character.intelligence << endl;
    cout << "\tTime: " << character.time << endl;
    cout << "\tMoney: $" << character.money << endl;
    cout << "\tDistance to goal: " << 20 - character.position << " steps" << endl;
}

int initializePuzzles(puzzle_ptr& puzzles) {
    ifstream in_stream("questions.txt", ios::in);
    string question, answer;
    ushort_t score_change;
    AttributeType attribute;
    int attribute_num, count;
    if (!in_stream) {
        cout << "Questions file failed to open" << endl;
    }
    for (count = 1; !in_stream.eof(); count++) {
        getline(in_stream >> std::ws, question, '\n');
        getline(in_stream >> std::ws, answer, '\n');
        in_stream >> attribute_num;
        in_stream >> score_change;
        if (attribute_num == 1) attribute = INTELLIGENCE;
        else if (attribute_num == 2) attribute = MONEY;
        else attribute = NO_ATTRIBUTE;
        if (!addPuzzle(puzzles, question, answer, score_change, attribute, count)) {
            cerr << "Puzzle could not be added. Please contact developer." << endl;
        }
    }
    in_stream.close();
    return count;
}

bool addPuzzle(puzzle_ptr& puzzles, string question, string answer, ushort_t score_change, AttributeType attribute, int count) {
    puzzle_ptr cur = new Puzzle;
    if (cur == NULL) return false;
    cur -> question = question;
    cur -> answer = answer;
    cur -> score_change = score_change;
    cur -> attribute_affected = attribute;
    cur -> num_puzzles = count;
    cur -> next = NULL;
    
    if (puzzles == NULL) puzzles = cur;
    else {
        cur -> next = puzzles;
        puzzles = cur;
    }
    return true;
}

bool askQuestion(Character& character, puzzle_ptr puzzles, int num_puzzles, int& gameStatus) {
    string player_answer = "";
    int randomNum = getRandomNumber(num_puzzles);
    puzzle_ptr temp;
    temp = puzzles;
    for (int i = 0; i < randomNum; i++) {
        temp = temp -> next;
    }
    string question = temp -> question;
    string answer = temp -> answer;
    cout << "Question: " << question << "\nAnswer: ";
    //cin.ignore(1000, '\n');
    getline(cin, player_answer);
    if (player_answer.compare(answer) == 0) { //Player answers correctly
        cout << "Congratulations! You've answered correctly, which means you're a true Auburn University Student!" << endl;
        pressAnyKey();
        increaseAttribute(character, temp -> attribute_affected, temp -> score_change);
        return true;
    }
    else { //Player answers incorrectly
        cout << "I'm sorry, that answer is incorrect! Better luck next time!" << endl;
        pressAnyKey();
        gameStatus = decreaseAttribute(character, temp -> attribute_affected, temp -> score_change);
        return false;
    }
}

int selectGameMenuOption(Menu menu, Character& character, puzzle_ptr puzzles, int num_puzzles) {
    int choice = 0, gameStatus = 0;
    do {
        cout << "\nWhat's your next move? Choose carefully: ";
        cin >> choice;
        if (choice < 1 || choice > menu.num_menu_options)
            cout << "That option is not on the menu!" << endl;
    } while (choice < 1 || choice > menu.num_menu_options);
    
    switch (choice) {
        //Move
        case 1:
            gameStatus = move(character, puzzles, num_puzzles);
            break;
        //Read
        case 2:
            gameStatus = read(character);
            break;
        //Beg
        case 3:
            gameStatus = beg(character);
            break;
        //Hitch a ride
        case 4:
            gameStatus = hitchRide(character);
            break;
        //View Character
        case 5:
            gameStatus = 0;
            break;
        //Quit Game
        case 6:
            gameStatus = quitGame();
            break;
        default:
            break;
    }
    return gameStatus;
}

int getRandomNumber(int limit) {
    return rand() % limit;
}

void pressAnyKey() {
    cin.clear();
    cout << "Press any key to continue...";
    cin.ignore().get();
}

int move(Character& character, puzzle_ptr puzzles, int num_puzzles) { //***Might want to change this function to return int to indicate game over, win, or continue
    int randomNum = getRandomNumber(100);
    int gameStatus = 0;
    
    //25% chance nothing happens and the player moves forward
    if (randomNum >= 0 && randomNum < 25) {
        cout << "Good news, you were able to move forward without anyone grabbing your attention! What a relief!" << endl;
        pressAnyKey();
        character.position++;
        if (character.position == 20) gameStatus = 1;
        else gameStatus = decreaseAttribute(character, TIME, 20);
    }
    
    //30% chance player is stopped by an organization (puzzle)
    else if (randomNum >= 25 && randomNum < 55) {
        cout << "\nAs you were walking, someone from an organization grabbed your attention and pulled you aside to ask you a question for some prize money." << endl;
        pressAnyKey();
        
        if (askQuestion(character, puzzles, num_puzzles, gameStatus)) { //if player answers question correctly
            gameStatus = decreaseAttribute(character, TIME, 40);
            if (gameStatus == 0) {
                character.position++;
                if (character.position == 20) gameStatus = 1;
            }
        }
        else { //if player answers question incorrectly
            if (gameStatus == 0) {
                gameStatus = decreaseAttribute(character, TIME, 40);
                if (gameStatus == 0) {
                    character.position++;
                    if (character.position == 20) gameStatus = 1;
                }
            }
        }
    }
    
    //10% chance the player runs into a professor and discusses grades
    else if (randomNum >= 55 && randomNum < 65) {
        cout << "\nOh no, you see your professor ahead and now you've made eye contact! He's coming straight for you!" << endl;
        pressAnyKey();
        
        //5% chance the professor has good news
        if (randomNum < 60) {
            cout << "Good news! Your professor likes the progress you've made in the class and says you're right on track to make an A in the course! Good Job!" << endl;
            cout << "As a result of this good news, your intelligence has increased.\n" << endl;
            pressAnyKey();
            increaseAttribute(character, INTELLIGENCE, 1);
            gameStatus = decreaseAttribute(character, TIME, 50);
            if (gameStatus == 0) {
                character.position++;
                if (character.position == 20) gameStatus = 1;
            }
        }
        
        //5% chance the professor has bad news
        if (randomNum >= 60) {
            cout << "Uh oh! Your professor just informed you that you need to step it up because your grade is well below average in the class!" << endl;
            cout << "As a result, you feel more self conscious about your grades and your intelligence decreases." << endl;
            pressAnyKey();
            cout << endl;
            gameStatus = decreaseAttribute(character, INTELLIGENCE, 1);
            gameStatus = decreaseAttribute(character, TIME, 50);
            if (gameStatus == 0) {
                character.position++;
                if (character.position == 20) gameStatus = 1;
            }
        }
    }
    
    //15% chance the player is given a flyer and asked for a donation
    else if (randomNum >= 65 && randomNum < 80) {
        cout << "\nSomeone has stopped you, given you a flyer and asked for a donation to their cause." << endl;
        pressAnyKey();
        cout << "You generously give $10. As a result, they educate you on their cause and your intelligence increases by 2." << endl;
        pressAnyKey();
        cout << endl;
        gameStatus = decreaseAttribute(character, MONEY, 10);
        increaseAttribute(character, INTELLIGENCE, 2);
        gameStatus = decreaseAttribute(character, TIME, 40);
        if (gameStatus == 0) {
            character.position++;
            if (character.position == 20) gameStatus = 1;
        }
    }
    
    //10% chance the player is hit by a bike
    else if (randomNum >= 80 && randomNum < 90) {
        cout << "\nCRASH! Some stereotypical college student was riding his bike and texting at the same time and crashed right into you!" << endl;
        pressAnyKey();
        cout << "He said he'd pay you $5 if you didn't tell anyone about the incident. You're also hurt pretty bad, so it takes a little time to get back on your feet and heading towards your destination." << endl;
        pressAnyKey();
        cout << endl;
        increaseAttribute(character, MONEY, 5);
        gameStatus = decreaseAttribute(character, TIME, 50);
        if (gameStatus == 0) {
            character.position++;
            if (character.position == 20) gameStatus = 1;
        }
    }
    
    //10% chance the player gets free stuff
    else {
        cout << "\nOh no, another person starts walking towards you from the side of the concourse." << endl;
        pressAnyKey();
        cout << "They reach out to give you a free gift and say \"Have a nice day!\"" << endl;
        pressAnyKey();
        cout << endl;
        increaseAttribute(character, MONEY, 2);
        gameStatus = decreaseAttribute(character, TIME, 30);
        if (gameStatus == 0) {
            character.position++;
            if (character.position == 20) gameStatus = 1;
        }
    }
    
    return gameStatus;
}

int read(Character& character) {
    int randomNum = getRandomNumber(3);
    int gameStatus = 0;
    if (randomNum == 0) {
        cout << "\nYou find a newspaper on the ground, so you stop and read it for a minute." << endl;
        pressAnyKey();
        cout << endl;
        increaseAttribute(character, INTELLIGENCE, 3);
        gameStatus = decreaseAttribute(character, TIME, 50);
    }
    else if (randomNum == 1) {
        cout << "\nYou decide to take a break from walking to do a little studying before heading to your next class." << endl;
        pressAnyKey();
        cout << endl;
        increaseAttribute(character, INTELLIGENCE, 5);
        gameStatus = decreaseAttribute(character, TIME, 70);
    }
    else {
        cout << "\nFor whatever reason, you decide to pull out a flyer someone handed you earlier and read the whole thing instead of just glancing at it." << endl;
        pressAnyKey();
        cout << endl;
        increaseAttribute(character, INTELLIGENCE, 1);
        gameStatus = decreaseAttribute(character, TIME, 30);
    }
    return gameStatus;
}

int beg(Character& character) {
    int gameStatus = 0;
    int randomNum = getRandomNumber(10) + 1;
    cout << "\nYou sit on a bench and pull out a premade sign from your backpack, silently begging for money." << endl;
    pressAnyKey();
    cout << "Some generous colleague of yours has had a nice day and decides to give you $" << randomNum << "!" << endl;
    pressAnyKey();
    increaseAttribute(character, MONEY, randomNum);
    gameStatus = decreaseAttribute(character, TIME, 40);
    return gameStatus;
}

int hitchRide(Character& character) {
    int gameStatus = 0;
    string decision;
    if (character.money<= 80) {
        cout << "\nHey! You don't have enough money to do that!" << endl;
        pressAnyKey();
        gameStatus = 0;
    }
    else {
        cout << "\nA golf cart is approaching and you wave them down to ask if you can get a ride." << endl;
        cout << "$80 for a safe distance of 5 spaces is what they offer." << endl;
        cout << "Are you sure you want to do this? (yes/no): ";
        cin.clear();
        cin.ignore(1000, '\n');
        getline(cin, decision);
        if (decision.compare("yes") == 0) {
            cout << "The cart driver gladly accepts your payment of $80 and gets you safely ahead 5 spaces." << endl;
            pressAnyKey();
            gameStatus = decreaseAttribute(character, MONEY, 80);
            gameStatus = decreaseAttribute(character, TIME, 30);
            if (gameStatus == 0) {
                character.position = character.position + 5;
                if (character.position >= 20) {
                    character.position = 20;
                    gameStatus = 1;
                }
            }
        }
        else {
            cout << "You change your mind because, let's be honest, you're a poor college stuednt." << endl;
            pressAnyKey();
            gameStatus = 0;
        }
    }
    return gameStatus;
}

int quitGame() {
    int gameStatus = 0;
    string decision;
    cout << "Are you sure you want to quit the game? (yes/no): ";
    cin.ignore(1000, '\n');
    cin.clear();
    getline(cin, decision);
    if (decision.compare("yes") == 0) gameStatus = 3;
    else gameStatus = 0;
    return gameStatus;
}

void howToPlay() {
    cout << "\tYou are an Auburn University student that has just left class. It is a Monday afternoon and to get to your next class you must walk through the ever-so-dreaded concourse. Here, random organizations are set up trying to get you to give in to their cause or come to their events." << endl;
    pressAnyKey();
    cout << "\tIt is your task to walk through the concourse without losing all of your money, intelligence for your next class, or, of course, running out of time before your next class starts. For this game's purposes, the distance of the concourse is 20 spaces. At the beginning of the game, you will receive $100, 10 points of intelligence, and 600 seconds of time." << endl;
    pressAnyKey();
    cout << "\tIn the game, you will be presented with a menu with different options you can use to your advantage according to your strategy to get across the concourse. Each forward step you take has risks, other than running out of time before reaching your destination. There are encounters you will face, such as making eye contact with a professor for one of your classes, getting hit by a bike, or being pulled aside by one of the organizations to play a little trivia game for some prize money (or intelligence)" << endl;
    pressAnyKey();
    cout << "\tA new update to this game is that, if you have the money, you can hire a golf cart driver to get you safely across 5 spaces with no interruptions and minimal time loss." << endl;
    pressAnyKey();
    cout << "\tNow that you know a little about the game, go show that concourse who's boss!" << endl;
    pressAnyKey();
}

//******* Test Drivers *******
void testInitializeMenu() {
    cout << "Testing Initialize Menu - standard case" << endl;
    Menu testMenu;
    initializeMenu(testMenu, 3, "Test Menu");
    assert(testMenu.max_menu_size == 3);
    assert(testMenu.menu_name.compare("Test Menu") == 0);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testAddMenuOption() {
    cout << "Testing Add Menu Option - standard case" << endl;
    Menu testMenu;
    initializeMenu(testMenu, 3, "Test Menu");
    assert(addMenuOption(testMenu, "option1") == 0);
    assert(addMenuOption(testMenu, "option2") == 0);
    assert(addMenuOption(testMenu, "option3") == 0);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Add Menu Option - overload case" << endl;
    assert(addMenuOption(testMenu, "option4") == 1);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testInitializeCharacter() {
    cout << "Testing Initialize Character - standard case" << endl;
    Character character;
    initializeCharacter(character, "Ryan");
    assert(character.name.compare("Ryan") == 0);
    assert(character.money == 100);
    assert(character.intelligence == 10);
    assert(character.time == 600);
    assert(character.position == 0);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testIncreaseAttribute() {
    Character character;
    initializeCharacter(character, "Ryan");
    cout << "Testing Increase Attribute - adding to an attribute already at full capacity" << endl;
    increaseAttribute(character, MONEY, 10);
    assert(character.money == 100);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Increase Attribute - adding to an attribute that isn't full, but adding over capacity" << endl;
    character.intelligence = 8;
    increaseAttribute(character, INTELLIGENCE, 5);
    assert(character.intelligence == 10);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Increase Attribute - standard case" << endl;
    character.money = 50;
    increaseAttribute(character, MONEY, 10);
    assert(character.money == 60);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testDecreaseAttribute() {
    Character character;
    initializeCharacter(character, "Ryan");
    cout << "Testing Decrease Attribute - standard case" << endl;
    assert(decreaseAttribute(character, MONEY, 5) == 0);
    assert(character.money == 95);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Decrease Attribute - attribute falls to zero" << endl;
    assert(decreaseAttribute(character, MONEY, 100) == 2);
    assert(character.money == 0);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
    
}

void testAddNewScore() {
    cout << "Testing Add New Score - add to empty list" << endl;
    scoreboard_ptr scores = NULL;
    addNewScore(scores, "Ryan", 1000);
    assert(scores -> score == 1000);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Add New Score - add lower score to back" << endl;
    addNewScore(scores, "Ryan1", 900);
    assert(scores -> next -> score == 900);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Add New Score - add higher score to front" << endl;
    addNewScore(scores, "Ryan2", 1200);
    assert(scores -> score == 1200);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testInitializePuzzles() {
    puzzle_ptr puzzles = NULL;
    cout << "Testing Initialize Puzzles - standard case" << endl;
    initializePuzzles(puzzles);
    assert(puzzles != NULL);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testAddPuzzle() {
    puzzle_ptr puzzles = NULL;
    cout << "Testing Add Puzzle - add first puzzle to null puzzle linked list" << endl;
    addPuzzle(puzzles, "Is this a test?", "Yes!", 2, MONEY, 1);
    assert(puzzles != NULL && puzzles -> next == NULL);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Add Puzzle - add puzzle to a non empty list" << endl;
    addPuzzle(puzzles, "Is this a test?", "No!", 2, INTELLIGENCE, 2);
    assert(puzzles -> next != NULL);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testCalculateScore() {
    ushort_t score;
    cout << "Testing Calculate Score - person who hasn't moved has a score of zero" << endl;
    calculateScore(score, 100, 600, 10, 0);
    assert(score == 0);
    cout << "Test Case Passed..." << endl;
    cout << "Testing Calculate Score - standard case" << endl;
    calculateScore(score, 15, 100, 2, 20);
    assert(score == 3000);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}

void testHitchRide() {
    Character character;
    initializeCharacter(character, "Ryan");
    cout << "Testing Hitch Ride - player does not have enough money" << endl;
    character.money = 20;
    hitchRide(character);
    assert(character.money == 20);
    cout << "Test Case Passed..." << endl;
    pressAnyKey();
}
