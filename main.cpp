#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <iterator>

using namespace std;

void intro(){
    cout << "Program 5: Unscrambler" <<
         "\nCS 141, Fall 2021, UIC" <<
         "\n\nYou will be presented with a board that is filled" <<
         "\nwith scrambled words. Rotate the rows and columns" <<
         "\nto get the words displayed in the right order." <<
         "\n\n13893 words have been read in from the dictionary." <<
         "\n\nEnter Q to quit or B to begin: ";
}

// User inputs size of the board, checks if it's within the constraints (must be at least 4) and makes
// the user re-enter if not.
int boardSize(){
    int userInputSize;
    cout << "Choose your board size (must be a number greater than or equal to 4):";
    cin >> userInputSize;
    while (userInputSize < 4){
        cout << "The board size must be at least 4. Retry." << endl;
        cout << "Choose your board size (must be a number greater than or equal to 4):";
        cin >> userInputSize;
    }
    return userInputSize;
}

// User inputs scramble amount and checks if it's in the constraints, makes them re-enter if not.
int scrambleTimes(){
    int scrambleAmount;
    cout << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1):";
    cin >> scrambleAmount;
    while (scrambleAmount < 1){
        if (scrambleAmount < 1){
            cout << "The number of times you select must be at least 1. Retry." << endl;
            cout << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1):";
            cin >> scrambleAmount;
        }

    }
    return scrambleAmount;
}

// Converts dictionary text file to a vector we can manipulate
void wordsToVect(vector<string> &dictionary){
    ifstream inStream;
    inStream.open("dictionary-3to5-letters.txt");
    assert( inStream.fail() == false );
    dictionary.clear();
    string newWord;
    while( inStream >> newWord) {
        dictionary.push_back(newWord);
    }
    inStream.close();
}

// Uses randomly generated words in a string vector to populate a char vector
void populateLetters(vector<char> &letters, const vector<string>& words){
    for (const auto& s : words){
        for (auto & j : s){
            letters.push_back(j);
        }
        letters.push_back(' ');
    }
}

// Generates extra words or spaces if there is still empty space on the board
void extraWords(vector <char> &letters, vector <string> &words, int userInput, vector <string> dictionary){
    int difference = (userInput * userInput) - letters.size();
    while (letters.size() < (userInput * userInput)+1) {
        if (difference > 2) {
            int randNum = rand() % dictionary.size();
            int wordLength = dictionary.at(randNum).length();
            if (wordLength-1 > difference) {
                while (wordLength > difference){
                    randNum = rand() % dictionary.size();
                    wordLength = dictionary.at(randNum).length();

                }
                words.push_back(dictionary.at(randNum));
                letters.clear();
                populateLetters(letters, words);
            } else {
                words.push_back(dictionary.at(randNum));
                letters.clear();
                populateLetters(letters, words);
            }
        }
        else {
            letters.push_back(' ');
        }
        difference = (userInput * userInput) - letters.size();
    }
}

// Generates random words from the dictionary vector and adds them to the words vector. Populateletters is called
// to put it in a vector along with extra words. The for loop at the end copies the original set of words generated to
// save for later.
void generateWords(vector <string> &dictionary, vector<string>&words, int userInput, vector<char> &letters, vector <char> &original){
    for (int i = 0; i < (userInput); i++) {
        int randomNum = rand() % dictionary.size();
        words.push_back(dictionary.at(randomNum));
    }
    // Populate char vector with letters of chosen random words
    populateLetters(letters, words);
    extraWords(letters, words, userInput, dictionary);

    for (char letter : letters){
        original.push_back(letter);
    }
}

// Creates actual board based off of input size and each placement is an index of a vector so all of the letters show up
// in the right places.
void createBoard(int userInputSize, vector<char> letters){
    cout << endl << "       ";
    for (int row = 0; row < userInputSize; row++){
        cout << row << "   ";
    }

    cout << endl;
    int counter = 0;
    for (int row = 0; row < userInputSize; row++){
        for (int spaces = 0; spaces < 6; spaces++){
            cout<< " ";
        }
        for (int topBorder = 0; topBorder < (userInputSize*4)-1; topBorder++){
            cout<< "-";
        }

        cout << endl << "   " << row;
        for (int col = 0; col < userInputSize; col++){
            if (counter > letters.size()-1) {
                cout << " | " << " ";
            } else {
                cout << " | " << letters.at(counter);

            }
            counter++;
        }

        cout << " | " << endl;
    }

    cout << "      ";

    for (int bottomBorder = 0; bottomBorder < (userInputSize*4)-1; bottomBorder++){
        cout<< "-";
    }
}

// Scrambles the board initially based off of the number the user inputs. Shuffled board copies the rotations in case
// the user selects restart eventually.
void rotate(int userInputSize, vector<char> &letters, int scramble, vector <char> &shuffledBoard){

    // scrambles column and row for input amount
    for (int i = 0; i < scramble; i++) {
        // randomly generate which of the rows or columns out of userInput will be selected
        int randomRow = rand() % userInputSize;
        int lastIndex = (randomRow + 1) * userInputSize - 1;
        char lastChar = letters.at( lastIndex);

        int k;
        for( k=lastIndex; k>lastIndex - userInputSize + 1; k--) {
            letters.at( k) = letters.at( k-1);
        }
        letters.at( k) = lastChar;


        // Random column is generated here and rotated below
        int randCol = rand() % userInputSize;

        int x = randCol;
        // X is the max value in the selected column, found by the for loop below
        for (int b = 0; b < userInputSize - 1; b++){
            x+=userInputSize;
        }

        int lastColIndex = x;
        char lastColChar = letters.at( lastColIndex);

        int a;

        // Rotates letters based on number input by user and randomly generated column
        for( a=lastColIndex; a>randCol; a-=userInputSize) {
            letters.at( a) = letters.at( a-userInputSize);
        }
        letters.at( a) = lastColChar;
    }
    // Copies original SHUFFLED board in the event user wants to restart
    for (char letter : letters){
        shuffledBoard.push_back(letter);
    }
}

// Print out the words with commas and a period at the end by printing out the words vector. This is for the
// original set of words.
void printWordsComma(vector<string> words){
    for (int i = 0; i < words.size(); i++){
        cout << words.at(i);
        if (i < words.size() - 1){
            cout << ", ";
        } else {
            cout << ". " << endl;
        }
    }
}

// Prints out the letters char vector and is used when words are rotated and are not the original words
void printWords(vector<char>letters){
    cout << endl << "Current board words: ";
    for (char letter : letters){
        cout << letter;
    }
}

// Prints second set of options
void secondOption(){
    cout << "\nEnter one of the following: "
            "   R to rotate a row or column, \n"
            "   C to view what the completed board should look like, \n"
            "   B to reset the board back to the beginning, \n"
            "   G to generate a new board with new random words, \n"
            "   S to have the computer automatically solve a board that you enter (5 points extra credit), \n"
            "   Q to quit. \n"
            "Your choice: ";
}

// Boolean returns true if the original set of letters is the same as the current set of letters after performing
// a rotation of a column or row
bool checkIfWin(vector<char> letters, vector<char> originalBoard){
    int equalCounter=0;

    for (int i = 0; i < letters.size(); i++){
        if (letters.at(i) == originalBoard.at(i)){
            equalCounter++;
        }
    }
    if (equalCounter == letters.size()){
        return true;
    } else{
        return false;
    }
}
// Rotates an individual row. Amnt accounts for if it is less than zero or greater than the userInput. If it's greater than the user
// input then mod is used to find the remainder, which is then generated as the amount of rotation. If it's less than zero, the size of the
// board is added to it to make it the same effect as a positive shift. 
void rotateRow(int userInputSize, vector <char> &letters, int rowNum, int amnt) {
    if (amnt < 0){
        amnt = amnt + (userInputSize);
    } else if (amnt > userInputSize){
        amnt = (amnt % userInputSize) + 1;
    }
    for (int i = 0; i < amnt; i++) {
        int lastIndex = (rowNum + 1) * userInputSize - 1;
        char lastChar = letters.at(lastIndex);

        int k;
        for (k = lastIndex; k > lastIndex - userInputSize + 1; k--) {
            letters.at(k) = letters.at(k - 1);
        }
        letters.at(k) = lastChar;

    }
}

// The same as the previous function and uses elements from the first rotate function as well, but it is altered 
// for user entry and to account for amnt not always being within the constraints to correctly output a number.
void rotateColumn(int userInputSize, vector <char> &letters, int columnNum, int amnt) {
    if (amnt < 0){
        amnt = amnt + (userInputSize);
    } else if (amnt > userInputSize){
        amnt = (amnt % userInputSize) + 1;
    }
    for (int i = 0; i < amnt; i++) {
        int x = columnNum;

        for (int b = 0; b < userInputSize - 1; b++) {
            x += userInputSize;
        }
        int lastColIndex = x;
        char lastColChar = letters.at(lastColIndex);

        int a;

        for (a = lastColIndex; a > columnNum; a -= userInputSize) {
            letters.at(a) = letters.at(a - userInputSize);
        }
        letters.at(a) = lastColChar;
    }

}

// Instructions called if user selects rotate 
void rotateInstructions(int userInputSize){
    cout << "  Enter the row/column you would like to rotate, and the number of positions to rotate by.\n"
            "   This should be in the format of <R or C> <row/column number> <number of positions to rotate>, \n"
            "   where valid row and column numbers are between 0 and " << userInputSize-1 << 
            ", \n"
            "   E.g. R 0 1 would rotate the top row (row 0) of the board to the right once, \n"
            "        c 1 -2 would rotate the second column (col 1) of the board upwards twice. \n"
            "   Your choice:" << endl;
}

int main() {
    srand(1);
    intro();
    vector<string> dictionary, words;
    vector<char> letters, originalBoard, shuffledBoard;
    bool rotation = false;
    char play;
    cin >> play;
    
    if (play == 'B' || play == 'b') {
        int userInputSize = boardSize();
        int scramble = scrambleTimes();

        // Set up words and board
        wordsToVect(dictionary);
        generateWords(dictionary, words, userInputSize, letters, originalBoard);
        rotate(userInputSize, letters, scramble, shuffledBoard);

        cout << "These are the words that you should try to spell out using the board, in order:" << endl;
        printWordsComma(words);
        cout << endl << "Current board: " << endl;
        createBoard(userInputSize, letters);
        printWords(letters);

        // The game runs until q is entered
        while (play != 'q') {

            secondOption();
            char selection;
            cin >> selection;
            
            // Rotation function, takes in 3 inputs and checks if they are within the constraints. Uses rotate column 
            // and row from above.
            if (selection == 'r' || selection == 'R') {
                rotateInstructions(userInputSize);
                char rowOrCol;
                int rcNum, rotateBy;

                cin >> rowOrCol >> rcNum >> rotateBy;
                
                // These two while loops will run until the user inputs a correct value for each variable
                while (!(rowOrCol == 'r' || rowOrCol == 'R' || rowOrCol == 'c' || rowOrCol == 'C')){
                    cout << " First input must be 'R' or 'C'. Try again." << endl;
                    cin >> rowOrCol >> rcNum >> rotateBy;
                    rotateInstructions( userInputSize);
                }
                while (rcNum > userInputSize-1 || rcNum < 0){
                    cout <<  "Number must be between 0 and " << userInputSize-1 << ". Try again." << endl;
                    cin >> rowOrCol >> rcNum >> rotateBy;
                    rotateInstructions(userInputSize);
                }
                // Performs rotate action based on the users entry
                if (rowOrCol == 'r' || rowOrCol == 'R'){
                    rotateRow(userInputSize, letters, rcNum, rotateBy);

                } else if (rowOrCol == 'c' || rowOrCol == 'C'){
                    rotateColumn(userInputSize, letters, rcNum, rotateBy);
                    
                }
                // Checks if the rotation solved the board and ends the game if it did.
                if (checkIfWin(letters, originalBoard)){
                    rotation = true;
                    play = 'q';
                } else {
                    cout << "These are the words that you should try to spell out using the board, in order:" << endl;
                    printWordsComma(words);
                    cout << endl << "Current board: " << endl;

                }
                // Prints new board with new shifted letters
                createBoard(userInputSize, letters);
                printWords(letters);
                
                // Stores the original board to be printed out here, using createBoard and calling originalBoard instead
                // of the manipulated letters vector. Then prints out current state of the board. 
            } else if (selection == 'c' || selection == 'C') {
                cout << " The completed board should look like: " << endl;
                createBoard(userInputSize, originalBoard);
                printWords(originalBoard);
                cout << "These are the words that you should try to spell out using the board, in order:" << endl;
                printWordsComma(words);
                cout << endl << "Current board: " << endl;
                createBoard(userInputSize, letters);
                printWords(letters);

                // Shuffled board was stored earlier for this purpose, it was a copy of the very first rotations and
                // is restored if the user enters B. Letters is updated with the values of the shuffled original board.
            } else if (selection == 'b' || selection == 'B') {
                // Reset board back to the beginning
                letters.clear();
                for (char i : shuffledBoard){
                    letters.push_back(i);
                }
                cout << "These are the words that you should try to spell out using the board, in order:" << endl;
                printWordsComma(words);
                cout <<  endl << "Current board: " << endl;
                createBoard(userInputSize, letters);
                printWords(letters);
                
                // Clear ALL vectors to make sure none of the remaining items are in any vectors. Then call generate words
                // to regenerate a new set of random words. Populate board and vectors with new words. 
            } else if (selection == 'g' || selection == 'G') {
                // Generate new random words
                words.clear();
                letters.clear();
                originalBoard.clear();
                cout << "These are the words that you should try to spell out using the board, in order:" << endl;
                generateWords(dictionary, words, userInputSize, letters, originalBoard);
                printWordsComma(words);

                rotate(userInputSize, letters, scramble, shuffledBoard);
                cout << endl << "Current board: " << endl;
                createBoard(userInputSize, letters);
                printWords(letters);
                
                // Quit game
            } else if (selection == 'q' || selection == 'Q') {
                play = 'q';
            }
        }
    }
    
    // Outside of while loop, checks if Q is entered at all through the game. 
    if (play == 'q' || play == 'Q'){
        // If checkIfWin was set to true inside of the rotation if statement, then the user wins. 
        if (checkIfWin(letters, originalBoard) && rotation){
            cout << "Congratulations, you won! ";
        }
        cout << "Thank you for playing!\n"
                "Exiting program..." << endl;
        exit(-1);
    }
}
