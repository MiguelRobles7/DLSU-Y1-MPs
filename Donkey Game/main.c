#include <stdio.h>
#if defined(__unix__) || defined(__linux__) 
    #include <sys/time.h>
#endif

//Make sleep and clear functions work on Windows & Linux
#if defined(__unix) || defined(__linux__) 
void mSleep(int time) {
    struct timeval tv; //From time.h library 
    tv.tv_sec  = time / 1000; // Turn milliseconds to seconds
    tv.tv_usec = (time % 1000) * 1000; //Turn milliseconds to microseconds
    // Select() function checks whether a file descriptor is set and time out if not set.  
    // We're using that as a pseudo sleep that works for linux. 
    select(0, NULL, NULL, NULL, &tv);
}

void clearScreen(){
    system("clear");
}
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
void mSleep(int time) {
    Sleep(time);
}

void clearScreen(){
    system("cls");
}
// If not in common OS then just scrap animation
#else
void mSleep(int time) {
    printf(""); 
}

void clearScreen(){
    printf("\e[1;1H\e[2J");
}
#endif

//Prints the current state of the game
void printState(int d1HasCrossed,int d2HasCrossed, int d3HasCrossed, int d4HasCrossed, int ownerHasCrossed) {
    d1HasCrossed ? printf("   | ~~~ | D1\n") : printf("D1 | ~~~ |\n");
    mSleep(50);
    d2HasCrossed ? printf("   | ~~~ | D2\n") : printf("D2 | ~~~ |\n");
    mSleep(50);
    d3HasCrossed ? printf("   | ~~~ | D3\n") : printf("D3 | ~~~ |\n");
    mSleep(50);
    d4HasCrossed ? printf("   | ~~~ | D4\n") : printf("D4 | ~~~ |\n");
    mSleep(50);
    ownerHasCrossed ? printf("   | ~~~ | OW\n") : printf("OW | ~~~ |\n");
}

/* Applies the speedup condition to the donkeys (must be minimum 1)
   Speedups are applied to both donkeys when applied but donkeys must 
   have a minimum time to travel which is one (1). */
int applySpeedup(int donkey){
    return (donkey - 2 < 1) ? 1 : donkey - 2;
}

// Checks if all donkeys are in the stable (all is values are 1) stops the loop if that is the case 
int checkIfGameDone(int d1, int d2, int d3, int d4){
    if(d1 && d2 && d3 && d4) return 1;
    return 0;
}

//Bonus Points Stuff
void printStart(){
    clearScreen();
    for(int i=0; i<3; i++) {
        printf(" _____ _           ______                ______            _                           \n");
        printf("|_   _| |          |  ___|               |  _  \\          | |                         \n");
        printf("  | | | |__   ___  | |_ ___  _   _ _ __  | | | |___  _ __ | | _____ _   _ ___          \n");
        printf("  | | | '_ \\ / _ \\ |  _/ _ \\| | | | '__| | | | / _ \\| '_ \\| |/ / _ \\ | | / __|   \n");
        printf("  | | | | | |  __/ | || (_) | |_| | |    | |/ / (_) | | | |   <  __/ |_| \\__ \\       \n");
        printf("  \\_/ |_| |_|\\___| \\_| \\___/ \\__,_|_|    |___/ \\___/|_| |_|_|\\_\\___|\\__, |___/\n");
        printf("                                                                     __/ |             \n");
        printf("                                                                    |___/              \n");
        mSleep(400);
        clearScreen();
        mSleep(400);
    }

    printf(" _____ _           ______                ______            _                           \n");
    printf("|_   _| |          |  ___|               |  _  \\          | |                         \n");
    printf("  | | | |__   ___  | |_ ___  _   _ _ __  | | | |___  _ __ | | _____ _   _ ___          \n");
    printf("  | | | '_ \\ / _ \\ |  _/ _ \\| | | | '__| | | | / _ \\| '_ \\| |/ / _ \\ | | / __|   \n");
    printf("  | | | | | |  __/ | || (_) | |_| | |    | |/ / (_) | | | |   <  __/ |_| \\__ \\       \n");
    printf("  \\_/ |_| |_|\\___| \\_| \\___/ \\__,_|_|    |___/ \\___/|_| |_|_|\\_\\___|\\__, |___/\n");
    printf("                                                                     __/ |             \n");
    printf("                                                                    |___/              \n");
    printf("                               [A] - Start Game                                        \n");
    printf("                               [B] - Help                                              \n");
    printf("                               [X] - Exit                                              \n");
    printf("                               Choice - ");
}

void printGameOver(){
    getchar();
    clearScreen();
    for(int i=0; i<3; i++) {
        printf(" _____                        _____                _ \n");
        printf("|  __ \\                      |  _  |              | |\n");
        printf("| |  \\/ __ _ _ __ ___   ___  | | | |_   _____ _ __| |\n");
        printf("| | __ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__| |\n");
        printf("| |_\\ \\ (_| | | | | | |  __/ \\ \\_/ /\\ V /  __/ |  |_|\n");
        printf(" \\____/\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|  (_)\n");
        mSleep(400);
        clearScreen();
        mSleep(400);
    }

    printf(" _____                        _____                _ \n");
    printf("|  __ \\                      |  _  |              | |\n");
    printf("| |  \\/ __ _ _ __ ___   ___  | | | |_   _____ _ __| |\n");
    printf("| | __ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__| |\n");
    printf("| |_\\ \\ (_| | | | | | |  __/ \\ \\_/ /\\ V /  __/ |  |_|\n");
    printf(" \\____/\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|  (_)\n");
    printf("             Time to transfer was ");
}

void printHelp(){
    printf("Overview:"
    "\n- Four donkeys need to be transferred to another stable."
    "\n- Each donkey needs time to arrive at the other stable."
    "\n- The owner will travel along with the donkeys during the trip." 
    "\n\nGameplay:" 
    "\n- The owner can only take at most two donkeys at one time but should always have one donkey being ridden." 
    "\n- The time it takes for the owner to get from one stable to another is the time of the slowest donkey." 
    "\n- Once at the other stable, the owner needs to take at least one donkey to get back to the first stable."
    "\n- The return trip will take time which is also equivalent to the time for the slowest donkey." 
    "\n- With each passing trip the donkey undertakes, the donkey gets slower and will take an additional hour to get to the destination."
    "\n- The task ends once all the donkeys are transferred to the second stable."
    "\n\nPowerup Mechanics:"
    "\n- The owner can give speedup medicine to make each donkey faster by two hours."
    "\n- The medicine can only be used for two trips regardless of the number of donkeys per trip."
    "\n\nPress any key to exit ");
    getchar();
}
int main() {
    int time = 0, chosen1, chosen2, chosenVal1, chosenVal2, 
    
    hasPowerup = 0, powerUpNum = 2, isGameDone = 0, slower,
    // Has Crossed
    d1HasCrossed = 0,d2HasCrossed = 0,d3HasCrossed = 0,d4HasCrossed = 0, ownerHasCrossed = 0,
    // Speeds
    d1Speed = 1, d2Speed = 2, d3Speed = 4, d4Speed = 6, nStop = 1;

    char cChoice;
    do
	{
        printStart();
		scanf(" %c", &cChoice);
        clearScreen();
        mSleep(300);
		switch(cChoice) {
            case 'a': case 'A':
                do {
                    //Set to avoid error
                    hasPowerup = 0;
                    // Print game state
                    printState(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed, ownerHasCrossed);
                    mSleep(50);
                    printf("\nSpeed: D1=%d D2=%d D3=%d D4=%d\n", d1Speed, d2Speed, d3Speed, d4Speed);
                    mSleep(50);
                    printf("Speedup: %d\n", powerUpNum);
                    mSleep(50);
                    printf("Time elapsed: %d\n", time);

                    // Get user input
                    mSleep(50);
                    printf("\nSelect first donkey: ");
                    scanf("%d", &chosen1);
                    printf("Select second donkey: ");
                    scanf("%d", &chosen2);

                    /*Checking for Selection Errors
                      Must check if game done as if user enters the same number at the start, the game 
                      isGameDone variable would be 0, ending the loop immediately and going straight to the results*/
                    if(chosen1 == 0 && chosen2 == 0){
                        printf("\nSelection Error! The owner should always have atleast one donkey!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    if(chosen1 > 4 || chosen2 > 4){
                        printf("\nSelection Error! Number must be from 1 - 4 only! Choose 0 if you do not wish to send a donkey\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    } 
                    if(chosen1 == chosen2){
                        printf("\nSelection Error! Must choose different donkeys!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    if((chosen1==1 || chosen2==1) && (d1HasCrossed != ownerHasCrossed)) {
                        printf("\nSelection error! Donkey chosen and owner must be in the same place!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    if((chosen1==2 || chosen2==2) && (d2HasCrossed != ownerHasCrossed)) {
                        printf("\nSelection error! Donkey chosen and owner must be in the same place!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    if((chosen1==3 || chosen2==3) && (d3HasCrossed != ownerHasCrossed)) {
                        printf("\nSelection error! Donkey chosen and owner must be in the same place!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    
                    if((chosen1==4 || chosen2==4) && (d4HasCrossed != ownerHasCrossed)) {
                        printf("\nSelection error! Donkey chosen and owner must be in the same place!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    // Speedups should no longer be available once two have been used throughout the game.
                    if(powerUpNum > 0){
                        printf("Use powerup (type 1)? ");
                        scanf("%d", &hasPowerup);
                    }
                    
                    if(hasPowerup > 1){
                        printf("\nSelection Error! Powerup input must only be 0 or 1!\n\n");
                        isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                        continue;
                    }
                    printf("\n");

                    // Proceed to apply changes
                    if(chosen1==1 || chosen2==1) {
                        // When the speedup is applied, the slow down is also applied after, so check if there's a speedup first
                        if(hasPowerup) d1Speed = applySpeedup(d1Speed);
                        if(chosen1 == 1) chosenVal1 = d1Speed;
                        else chosenVal2 = d1Speed;
                        d1HasCrossed = !ownerHasCrossed;
                        d1Speed++;
                    }
                    if(chosen1==2 || chosen2==2) {
                        if(hasPowerup) d2Speed = applySpeedup(d2Speed);
                        if(chosen1 == 2) chosenVal1 = d2Speed;
                        else chosenVal2 = d2Speed;
                        d2HasCrossed = !ownerHasCrossed;
                        d2Speed++;
                    }
                    if(chosen1==3 || chosen2==3) {
                        if(hasPowerup) d3Speed = applySpeedup(d3Speed);
                        if(chosen1 == 3) chosenVal1 = d3Speed;
                        else chosenVal2 = d3Speed;
                        d3HasCrossed = !ownerHasCrossed;
                        d3Speed++;
                    }
                    if(chosen1==4 || chosen2==4) {
                        if(hasPowerup) d4Speed = applySpeedup(d4Speed);
                        if(chosen1 == 4) chosenVal1 = d4Speed;
                        else chosenVal2 = d4Speed;
                        d4HasCrossed = !ownerHasCrossed;
                        d4Speed++;
                    }
                    
                    if(chosen1 == 0) chosenVal1 = 0;
                    if(chosen2 == 0) chosenVal2 = 0;
                    // Owner goes to the other side every turn
                    ownerHasCrossed = (ownerHasCrossed) ? 0 : 1;

                    // Check which value is bigger and declare that as slower & Check if use powerup and reduce speed if so
                    slower = (chosenVal1 >= chosenVal2) ? chosenVal1 : chosenVal2;
                    if(hasPowerup) powerUpNum--; 

                    // The elapsed time will consider the time after the speedup is applied but before the slowdown is applied.
                    // Speed++ is slowdown so we negate that when a speedup is used
                    time = (hasPowerup) ? time + slower-- : time + slower;
                    isGameDone = checkIfGameDone(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed);
                } while(!isGameDone);
                // Game Over, Show final stats
                printf("Final Results: \n");
                printState(d1HasCrossed, d2HasCrossed, d3HasCrossed, d4HasCrossed, ownerHasCrossed);
                mSleep(50);
                printf("\nSpeed: D1=%d D2=%d D3=%d D4=%d\n", d1Speed, d2Speed, d3Speed, d4Speed);
                mSleep(50);
                printf("Speedup: %d\n", powerUpNum);
                mSleep(50);
                printf("Time elapsed: %d\n", time);
                printf("\nPress enter to end game ");
                // If the time it took for all the donkeys to be transferred is over 13 hours, 
                // then that is regarded as slow. Otherwise, this should be regarded as fast.
                getchar();
                printGameOver();
                mSleep(500);
                if(time > 13) printf("SLOW!");
                else printf("FAST!");
                printf("\n          Press any key to go back to menu ");
                getchar();

                //Reset variables
                time = 0, hasPowerup = 0, powerUpNum = 2, isGameDone = 0,
                d1HasCrossed = 0,d2HasCrossed = 0,d3HasCrossed = 0,d4HasCrossed = 0, ownerHasCrossed = 0,
                d1Speed = 1, d2Speed = 2, d3Speed = 4, d4Speed = 6;
                break;

            case 'b': case 'B':
				printHelp();
                getchar();
                break;

            case 'x': case 'X':
				nStop = 0;
				break;

			default:
				printf("Input error!\n");
                mSleep(3000);
        }
    } while(nStop);
	return 0;
}

/******************************************************************************
This is to certify that this project is my own work, based on my personal
efforts in studying and applying the concepts learned. I have constructed
the functions and their respective algorithms and corresponding code by
myself. The program was run, tested, and debugged by my own efforts. I
further certify that I have not copied in part or whole or otherwise
plagiarized the work of other students and/or persons.
******************************************************************************/