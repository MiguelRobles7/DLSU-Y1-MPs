/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts 
learned. I have constructed the functions and their respective algorithms and corresponding code by myself. The 
program was run, tested, and debugged by my own efforts. I further certify that I have not copied in part or whole or 
otherwise plagiarized the work of other students and/or persons.
*********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

struct passenger 
{
    int nIdNumber, nPriorityGroup, nDropOff;
    char strName[999], strDropOffString[80];
};
struct shuttle 
{
    struct passenger passengers[16];
    int nTripNumber, nCount;
    char strEmbarkationPoint[7];
};
// Print Functions
/* printTrips displays all trips */
void 
printTrips();

/* printPrioGroups displays all priority groups */
void 
printPrioGroups();

/* printPersonnelOptions displays all personnel options*/
void 
printPersonnelOptions();

/*  printDropoffs displays all dropoffs
    @param tripNumber - int of shuttle's tripnumber used to determine its exit
*/
void 
printDropoffs(int nTripNumber);

/*  printShuttleState displays all dropoffs
    @param count - int of how many seats is occupied
*/
void 
printShuttleState(int nCount);

/* displayContents displays all contents of a file
    @param shuttles - array of all 20 shuttles 
    Pre-condition: shuttles has content
*/
void 
displayContents(struct shuttle shuttles[20]);

// Conversion Functions
/* tripStringToNumber gets the shuttle's assgined number 
    @param tripString - string with numbers at the end
    @return returns tripString's shuttle number from 1 - 20
    Pre-condition: tripString is proper shuttle trip number (AE1XX) 
    Ex. AE101 gets 1
*/
int 
tripStringToNumber(char *strTripString);

/* dropoffStringToNumber gets the shuttle's assigned dropoff number 
    @param dropoffString - full string of dropoff detail
    @return returns dropoffString's dropoff number
    Pre-condition: dropoffString is proper shuttle dropoff point
    Ex. 3rd Drop-off Point gets 3
*/
int 
dropoffStringToNumber(char *strDropoffString);

/* normalizeTripString gets the array form trip number and returns it to normalized version 
    @param tripNum - shuttle's shuttle number from 1 - 20
    @return returns normalized trip number 
    Pre-condition: tripNum is a valid trip number from 1 - 20
    Ex. 1 returns 101, 10 returns 150
*/
int 
normalizeTripString(int nTripNum);

/* getDropoffString gets the shuttle's full dropoff text  
    @param dropOff - passenger's dropoff point 
    @param tripNum - shuttle's shuttle number from 1 - 20
    @return returns full dropoff text
    Pre-condition: tripNum is a valid trip number from 1 - 20 and dropOff is a valid dropoff point
    Ex. dropOff: 1, tripNum: 1 returns "1st Drop-off Point - Mamplasan Toll Exit"
*/
char 
*getDropoffString(int nDropOff, 
                  int nTripNum);

// Check Input Functions
/* checkID checks if the given ID is a valid DLSU ID
    @param nID - passenger's given ID
    @return returns the remainder, if remainder is 0 then it is a valid ID
    Pre-condition: nID is a valid integer
*/
int 
checkID(int nID);

/* checkDropoff checks if the given dropoff point is valid
    @param tripNum - shuttle's shuttle number from 1 - 20
    @param dropOff - passenger's dropoff point 
    @return returns 0 if the dropoff is valid, else it returns 1
    Pre-condition: tripNum is a valid trip number from 1 - 20 and dropOff is a valid dropoff point
*/
int 
checkDropoff(int nTripNum, 
             int nDropOff);

// Everything Else
/* getDropCount counts the number of passengers using a certain dropoff point
    @param passengers - array of passengers in the shuttle
    @param givenNumber - the dropoff number to look for
    @return returns the amount of passengers with that dropoff point
    Pre-condition: all passengers have valid dropoff points, given number is within trip scope 
*/
int 
getDropCount(struct passenger passengers[16], 
             int nGivenNumber);

/* findShuttleSeat finds a seat for applicant
    @param shuttles - array of all 20 shuttles 
    @param applicant - the applicant trying to get a seat
    @param tripNum - the trip the applicant is trying to board
    Pre-condition: all passed arguements are valid
*/
void 
findShuttleSeat(struct shuttle shuttles[20], 
                struct passenger applicant, 
                int nTripNum);

/* updateFile updates the file with information from shuttles array
    @param fileName - name of file to save content into
    @param shuttles - array of all 20 shuttles which the save will be based on
    Pre-condition: filename exists, shuttles is valid
*/
void 
updateFile(char *strFileName, 
           struct shuttle shuttles[20]);

/* reads a file and replaces shuttle array contents with file contents
    @param fileName - name of file to read into
    @param shuttles - array where information will be saved
    Pre-condition: file is formatted properly, shuttle array is of correct size
*/
void 
readFile(struct shuttle *shuttles, 
         char *strFileName);

int 
main() 
{
    int nUserType, nTripNum = 1, i, j, nUserConfirm = 1, nLAST_LAGUNA_TRIP = 20, nPersonnelOption = 0, bNotFinished = 1, nCount;
    char strFileName[20], strTripString[6], strSearch[99], strDate[11], *strStored, *strDump, strToSearch[99], *strPersonnelStrDump = malloc(sizeof(strPersonnelStrDump)); 
    struct shuttle shuttles[20], loadedShuttles[20];
    struct passenger applicant, loadApplicant;
    struct tm *time_ptr;
    time_t t;
    time(&t);
    time_ptr = localtime(&t);
    strftime(strFileName, sizeof(strFileName), "Trip-%d-%m-%Y.txt", time_ptr);
    readFile(shuttles, strFileName);

    printf("Good Day! Welcome to Arrows Express! File Loaded: %s\n\n", strFileName);
    // Entire loop is one day so keep going until personnel exits (indicates that the day is done)
    while(bNotFinished) 
    {
        // Get user type
        printf("    What is your user type?\n[1] Passenger\t[2] Personnel: ");
        scanf("%d", &nUserType);
        // Passenger
        if(nUserType == 1) 
        {
            printf("Please fill out the Embarkation Information Card\n");//  encode the embarkation information that was mentioned above
            do 
            {
                do //get trip number
                {
                    printTrips();
                    scanf("%d", &nTripNum);
                    if(nTripNum < 1 || nTripNum > nLAST_LAGUNA_TRIP) printf("Invalid Trip Number!\n");
                } while(nTripNum < 1 || nTripNum > nLAST_LAGUNA_TRIP); //Continue until user provides valid trip number
                printf("Here's what this shuttle looks like: \n");
                // BONUS: Let user see shuttle state and confirm if they still want to use the shuttle
                printShuttleState(shuttles[nTripNum-1].nCount);
                printf("NOTE: If the shuttle is full and you have a lower priority, you will be moved to the latest free shuttle\n"
                       "Choose Shuttle?\n[1] Yes\t[2] No: ");
                scanf("%d", &nUserConfirm);  
            } while(nUserConfirm != 1);
            
            //get full name
            printf("\nFull Name (FN LN): ");
            getchar(); //Console eats up first input 
            //Better than scanf for strings
            fgets(applicant.strName, sizeof(applicant.strName), stdin);
            applicant.strName[strcspn(applicant.strName, "\n")] = 0; //Removes trailing '\n' after input
        
            // get ID Number
            do 
            {
                printf("ID: ");
                scanf(" %d", &applicant.nIdNumber);
                // BONUS: Check if ID is valid DLSU ID
                if(checkID(applicant.nIdNumber) != 0) printf("Invalid ID!\n");
            } while(checkID(applicant.nIdNumber));

            // get Priority Group
            do 
            {
                printf("\nPriority Group (1 - 6): \n");
                printPrioGroups();
                scanf("%d", &applicant.nPriorityGroup);
                if(applicant.nPriorityGroup < 1 || applicant.nPriorityGroup > 6) printf("Invalid Priority Group!");
            } while(applicant.nPriorityGroup < 1 || applicant.nPriorityGroup > 6); 

            // get Dropoff
            do 
            {
                printf("\nChoose Dropoff: \n");
                printDropoffs(nTripNum);
                scanf("%d", &applicant.nDropOff);
                if(checkDropoff(nTripNum, applicant.nDropOff) != 0) printf("Invalid Dropoff!\n\n");
            } while(checkDropoff(nTripNum, applicant.nDropOff)); // Keeps going unt il user gives a valid drop off point
            strcpy(applicant.strDropOffString, getDropoffString(applicant.nDropOff, nTripNum));
            findShuttleSeat(shuttles, applicant, nTripNum);
        }
        else if(nUserType == 2) //personnel
        {
            do
            {
                printf("Hello Personnel! What would you like to do?\n");
                printPersonnelOptions();
                scanf("%d", &nPersonnelOption);
                if(nPersonnelOption < 1 || nPersonnelOption > 8) printf("Invalid Input!\n\n");
            } while (nPersonnelOption < 1 || nPersonnelOption > 8);
            if(nPersonnelOption == 1) // View the number of passengers on a certain trip
            {
                do 
                {
                    printTrips();
                    scanf("%d", &nTripNum);
                    if(nTripNum < 1 || nTripNum > 20) printf("Invalid Trip Number!");
                } while(nTripNum < 1 || nTripNum > 20);
                printShuttleState(shuttles[nTripNum - 1].nCount);
            }
            else if(nPersonnelOption == 2) // View the count of passengers of a specific drop-off point
            {
                do //Keep going until given valid trip number
                {
                    printTrips();
                    scanf("%d", &nTripNum);
                    if(nTripNum < 1 || nTripNum > 20) printf("Invalid Trip Number!");
                } while(nTripNum < 1 || nTripNum > 20);
                if(nTripNum <= 5) //Mamplasan Exit Dropoffs
                {
                    printf("1st drop-off point - Mamplasan Toll Exit - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 1));
                    printf("2nd drop-off point - Phase 5, San Jose Village - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 2));
                    printf("3rd drop-off point - Milagros Del Rosario (MRR) Building - East Canopy - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 3));
                }
                else if(nTripNum <= 9) //Eton Exir Dropoffs
                {
                    printf("1st Drop-off Point - Laguna Blvd. Guard House (across Paseo PHOENIX Gasoline Station) - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 1));
                    printf("2nd Drop-off Point - Milagros Del Rosario (MRR) Building - East Canopy - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 2));
                }
                else if(nTripNum <= 15) //Estrada Route Dropoffs
                {
                    printf("1st Drop-off Point - Petron Gasoline Station along Gil Puyat Avenue - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 1));
                    printf("2nd Drop-off Point - Gate 4: Gokongwei Gate - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 2));
                    printf("3rd Drop-off Point - Gate 2: North Gate (HSSH) - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 3));
                    printf("4th Drop-off Point - Gate 1: South Gate (LS Building Entrance) - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 4));
                }
                else //Buendia/LRT Route Dropoffs
                {
                    printf("1st Drop-off Point - Petron Gasoline Station along Gil Puyat Avenue - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 1));
                    printf("2nd Drop-off Point - College of St. Benilde (CSB) along Taft Avenue - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 2));
                    printf("3rd Drop-off Point - Gate 4: Gokongwei Gate - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 3));
                    printf("4th Drop-off Point - Gate 2: North Gate (HSSH) - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 4));
                    printf("5th Drop-off Point - Gate 1: South Gate (LS Building Entrance - %d\n", getDropCount(shuttles[nTripNum-1].passengers, 5));
                }
            }
            // View passenger information 
            else if(nPersonnelOption == 3)
            {
                do //Keep going until given valid trip number
                {
                    printTrips();
                    scanf("%d", &nTripNum);
                    if(nTripNum < 1 || nTripNum > 20) printf("Invalid Trip Number!");
                } while(nTripNum < 1 || nTripNum > 20);
                nTripNum--;
                // Sort by priority number
                for(j = 1; j <= 6; j++) //Loop through all priority groups
                {
                    for(i = 0; i < 16; i++) //Loop through all passengers
                    {
                        if(shuttles[nTripNum].passengers[i].nPriorityGroup == j) //Find all passengers with the same priority group as j's value
                            printf("Name: %s\nID Number: %d\nPriority: %d\n\n", shuttles[nTripNum].passengers[i].strName, shuttles[nTripNum].passengers[i].nIdNumber, shuttles[nTripNum].passengers[i].nPriorityGroup);
                    }
                }
            }
            else if(nPersonnelOption == 4) // Load passenger information
            {
                printf("Input Text File Name: ");
                getchar();
                fgets(strSearch, sizeof(strSearch), stdin);
                strSearch[strcspn(strSearch, "\n")] = 0; //Remove trailing '\n' after input

                FILE *fptr = fopen(strSearch, "r");
                if(fptr == NULL) 
                {
                    printf("No Such File Exists!");
                    fclose(fptr);
                }
                else //No errors in file
                {
                    while(!feof(fptr))  //Put file information into load applicant 
                    {
                        fscanf(fptr, "%s", strTripString);
                        nTripNum = tripStringToNumber(strTripString);
                        fscanf(fptr, "%s %[^\n] %d %d %[^\n]", 
                                strPersonnelStrDump, 
                                loadApplicant.strName, 
                                &loadApplicant.nIdNumber, 
                                &loadApplicant.nPriorityGroup, 
                                loadApplicant.strDropOffString);
                        loadApplicant.nDropOff = dropoffStringToNumber(loadApplicant.strDropOffString);
                    }
                    findShuttleSeat(shuttles, loadApplicant, nTripNum);
                }
            }
            else if (nPersonnelOption == 5) // Search passenger
            {
                nCount = 0;
                printf("Input Name to Search: ");
                getchar();
                gets(strSearch);
                strSearch[strcspn(strSearch, "\n")] = 0; //Remove trailing '\n' after input
                for(i = 0; i < 20; i++) //Loop through all shuttles
                {
                    for (j = 0; j < 16; j++) //Loop through all passenger in shuttle
                    {
                        if(shuttles[i].passengers[j].strName[0] != '\0' && shuttles[i].passengers[j].nPriorityGroup >= 1 &&
                           shuttles[i].passengers[j].nPriorityGroup <= 6) //Make sure passenger is valid
                        {
                            strcpy(strToSearch, shuttles[i].passengers[j].strName);
                            strDump = strtok(strToSearch, " ");
                            while (strDump != NULL) //Keep going until we get user's last name (the last word in string)
                            {
                                strStored = strDump;
                                strDump = strtok(NULL, " ");
                            }
                            if(!strcmp(strStored, strSearch)) //If match with search then passenger's last name is same as what is being looked for
                            {
                                nCount++;
                                printf("Result #%d\nName: %s\nID: %d\nTrip Number: AE%d\nPriority Group: %d\nEmbarkation Point: %s\nDropoff Point: %s\n\n",
                                nCount, shuttles[i].passengers[j].strName, shuttles[i].passengers[j].nIdNumber, normalizeTripString(shuttles[i].nTripNumber + 1), 
                                shuttles[i].passengers[j].nPriorityGroup, shuttles[i].strEmbarkationPoint, shuttles[i].passengers[j].strDropOffString);
                            }
                        }
                    }
                }
            }
            else if(nPersonnelOption == 6) // Terminate Program, save current information to a text file
            {
                printf("Saving and Exiting....");
                bNotFinished = 0;
                updateFile(strFileName, shuttles);
            }
            else if(nPersonnelOption == 7) // View any recent trip files via a file load feature.
            {
                printf("Input Text File Name: ");
                getchar();
                gets(strSearch);
                strSearch[strcspn(strSearch, "\n")] = 0; //Remove trailing '\n' after input
                
                FILE *fptr = fopen(strSearch, "r");
                if(fptr == NULL) // Check if file is empty
                {
                    printf("No Such File Exists!");
                    fclose(fptr);
                }
                else 
                {
                    readFile(loadedShuttles, strSearch);
                    displayContents(loadedShuttles);
                }
            }
            else if(nPersonnelOption == 8) // BONUS: Change the date to something else/manually input date
            {
                printf("Input Date (DD-MM-YYYY): ");
                getchar();
                gets(strDate);
                strcpy(strSearch, "Trip-");
                strcat(strSearch, strDate);
                strcat(strSearch, ".txt");
                
                FILE *fptr = fopen(strSearch, "r");
                if(fptr == NULL) 
                {
                    printf("No Such File Exists!");
                    fclose(fptr);
                }
                else 
                {
                    memset(shuttles, 0, sizeof(shuttles)); //Clear all elements
                    strcpy(strFileName, strSearch); //Set as new default file
                    readFile(shuttles, strFileName);
                    printf("New File Loaded\n\n"
                           "Good Day! Welcome to Arrows Express! File Loaded: %s\n\n", strFileName);
                }
            }
        }
        else printf("Invalid User Type!\n");
    }
    return 0;
}

void 
printTrips() 
{
    printf("\nChoose a Trip Number: \n"
           "Manila, Mamplasan Exit:\n[1] AE101\t[2] AE102\t[3] AE103\t[4] AE104\t[5] AE105\n"
           "Manila, ETON Exit:\n[6] AE106\t[7] AE107\t[8] AE108\t[9] AE109\n\n"
           "Laguna, via Estrada:\n[10] AE150\t[11] AE151\t[12] AE152\t[13] AE153\t[14] AE154\t[15] AE155\n"
           "Laguna, vie Buendia:\n[16] AE156\t[17] AE157\t[18] AE158\t[19] AE159\t[20] AE160\n\n");
}

void 
printDropoffs(int nTripNumber) 
{
    if(nTripNumber <= 5) // Mamplasan Exit
    {
        printf("[1] Mamplasan Toll Exit\n"
               "[2] Phase 5, San Jose Village\n"
               "[3] Milagros Del Rosario (MRR) Building - East Canopy\n");
    }
    else if(nTripNumber <= 9) // ETON Exit
    {
        printf("[1] Laguna Blvd. Guard House (across Paseo PHOENIX Gasoline Station)\n"
               "[2] Milagros Del Rosario (MRR) Building - East Canopy\n");
    }
    else if(nTripNumber <= 15) // Estrada
        printf("[1] Petron Gasoline Station along Gil Puyat Avenue\n"
               "[2] Gate 4: Gokongwei Gate\n"
               "[3] Gate 2: North Gate (HSSH)\n"
               "[4] Gate 1: South Gate (LS Building Entrance)\n");
    else //Buendia/LRT
    {
        printf("[1] Petron Gasoline Station along Gil Puyat Avenue\n"
               "[2] College of St. Benilde (CSB) along Taft Avenue\n"
               "[3] Gate 4: Gokongwei Gate \n"
               "[4] Gate 2: North Gate (HSSH) "
               "[5] Gate 1: South Gate (LS Building Entrance)\n");
    }
}

void 
printPrioGroups()
{
    printf("[1] Faculty and ASF with Inter-campus assignments\n"
           "[2] Students with Inter-campus enrolled subjects or enrolled in thesis using Inter-campus facilities\n"
           "[3] Researchers\n"
           "[4] School Administrators (Academic Coordinators level and up for Faculty and ASF, and Director level and up for APSP)\n"
           "[5] University Fellows\n[6] Employees and Students with official business \n");
}

void 
printPersonnelOptions()
{
    printf("[1] View Trip\n[2] Drop-off Point Count\n[3] View Passenger Information\n[4] Load Passenger Information\n"
    "[5] Search Passenger\n[6] Save and Exit\n[7] View Recent Trip Files\n[8] Load File from Old Dates\n");
}

void 
printShuttleState(int nCount) 
{
    int j; 
    printf("Number of Seats Taken: %d\n", nCount);
    if(nCount <= 13) //Not Worst Case
    {
        printf("+---+---+---+\n|");
        for(j = 1; j <= 13; j++)
        {
            if(j <= nCount) printf(" X |");
            else printf(" O |");
            if (j % 3 == 0) printf("\n+---+---+---+\n|");
        }
        printf("   D   |\n+---+---+---+\n");
    }
    else //Worst-case
    {
        printf("+---+---+---+---+\n|");
        for(j = 1; j <= 16; j++)
        {
            if(j == 13)
            {
                printf("   |");
            }
            if(j <= 15) 
            {
                if(j <= nCount) printf(" X |");
                else printf(" O |");
            }
            if (j % 4 == 0) printf("\n+---+---+---+---+\n|");
            if(j == 16)
            {
                if(j <= nCount) printf("   X   |");
                else printf("   O   |");
            }
        }
        printf("   D   |\n+---+---+---+---+\n");
    }
    printf("\nLegends: O - Free Seats  X - Occupied Seats  D - Driver\n");
}

int 
tripStringToNumber(char *strTripString) 
{
    const char *strLastThreeChars = &strTripString[strlen(strTripString)-3]; //Gets the last 3 chars in the string
    int nTripNum = atoi(strLastThreeChars); //converts last 3 to int
    if(nTripNum % 100 < 10)
        nTripNum %= 100;
    else if(nTripNum % 100 >= 50 && nTripNum % 100 <= 60)
        nTripNum = nTripNum % 150 + 10;
    else
        nTripNum = 0; //Should be impossible, but if ever then default to first shuttle
    return nTripNum;
}

int 
normalizeTripString(int nTripNum) 
{
    if(nTripNum + 100 < 110) nTripNum += 100; //AE109 and below
    else nTripNum += 140; //AE150 and above
    return nTripNum;
}

int 
getDropCount(struct passenger passengers[16], 
             int nGivenNumber) 
{
    int nCount = 0, i;
    for(i = 0; i < 16; i++) 
    {
        if(passengers[i].nDropOff == nGivenNumber) nCount++;
    }
    return nCount;
}

void 
updateFile(char *strFileName, 
           struct shuttle shuttles[20])
{
    int i, j, bFirstLine = 1; 
    FILE *fptr = fopen(strFileName, "w");
    for(i = 0; i < 20; i++) //Loop through shuttles
    {
        for(j = 0; j < 16; j++) //Loop through passengers
        {
            //Make sure content of shuttle is valid before writing
            if(shuttles[i].passengers[j].strName[0] != '\0' && shuttles[i].passengers[j].nPriorityGroup > 0 && 
               shuttles[i].passengers[j].nPriorityGroup <= 6 && shuttles[i].nTripNumber <= 19 && shuttles[i].nTripNumber >= 0 &&
               checkID(shuttles[i].passengers[j].nIdNumber) == 0 && checkDropoff(shuttles[i].nTripNumber + 1, shuttles[i].passengers[j].nDropOff) == 0) 
            {
                if(bFirstLine == 1) 
                {
                    fprintf(fptr, "AE%d\n%s\n%s\n%d\n%d\n%s\n.\n.", normalizeTripString(shuttles[i].nTripNumber + 1), shuttles[i].strEmbarkationPoint, shuttles[i].passengers[j].strName, shuttles[i].passengers[j].nIdNumber, shuttles[i].passengers[j].nPriorityGroup, shuttles[i].passengers[j].strDropOffString);
                    bFirstLine=0; 
                }
                else fprintf(fptr, "\nAE%d\n%s\n%s\n%d\n%d\n%s\n.\n.", normalizeTripString(shuttles[i].nTripNumber + 1), shuttles[i].strEmbarkationPoint, shuttles[i].passengers[j].strName, shuttles[i].passengers[j].nIdNumber, shuttles[i].passengers[j].nPriorityGroup, shuttles[i].passengers[j].strDropOffString);
            }
        }
    }
}
void 
displayContents(struct shuttle shuttles[20])
{
    int i, j;
    printf("File Contents: \n");
    for(i = 0; i < 20; i++) //Loop through shuttles
    {
        for(j = 0; j < 16; j++) //Loop through passengers
        {
            //Make sure content of shuttle is valid before writing
            if(shuttles[i].passengers[j].strName[0] != '\0' && shuttles[i].passengers[j].nPriorityGroup > 0 && 
               shuttles[i].passengers[j].nPriorityGroup <= 6 && shuttles[i].nTripNumber <= 19 && shuttles[i].nTripNumber >= 0 &&
               checkID(shuttles[i].passengers[j].nIdNumber) == 0 && checkDropoff(shuttles[i].nTripNumber + 1, shuttles[i].passengers[j].nDropOff) == 0) 
            {
                printf("\nName: %s\nID: %d\nPriority Group: %d\nTrip Number: AE%d\nEmbarkation Point: %s\nDropoff Point: %s\n\n",
                    shuttles[i].passengers[j].strName, 
                    shuttles[i].passengers[j].nIdNumber, 
                    shuttles[i].passengers[j].nPriorityGroup, 
                    normalizeTripString(shuttles[i].nTripNumber + 1), 
                    shuttles[i].strEmbarkationPoint,  
                    shuttles[i].passengers[j].strDropOffString);
            }
        }
    }
}
int 
checkDropoff(int nTripNum, 
             int nDropOff)
{
    int bIsInvalidDropoff = 0;
    if(nDropOff >= 1 && ((nTripNum <= 5 && nDropOff <= 3) || 
    (nTripNum > 5 && nTripNum <= 9 && nDropOff <= 2) || 
    (nTripNum > 9 && nTripNum <= 15 && nDropOff <= 4) || 
    (nTripNum > 15 && nTripNum <= 20 && nDropOff <= 5))) //Check different dropoff combinations
        bIsInvalidDropoff = 0; //Valid
    else
        bIsInvalidDropoff = 1; //Invalid
    return bIsInvalidDropoff;
}

int 
checkID(int nID) 
{
	int nRemainder, nMultiplier = 1, nLastDigit, nSum = 0;
    while(nID > 0)//To check if a DLSU ID is valid, multiply the 1st Digit by 8. 2nd by 7 and so on. 
    {
        nLastDigit = nID % 10;
        nID /= 10;
        nSum += nLastDigit * nMultiplier;
        nMultiplier++;
    }
    nRemainder = nSum % 11; // The total of a DLSU ID will always be divisible by 11 
    return nRemainder;	
}

char 
*getDropoffString(int nDropOff, 
                  int nTripNum)
{
    char *strDropoffString = malloc(sizeof (char) * 86);
    if(nTripNum <= 5) // Mamplasan Exit
    {
        if(nDropOff == 1) strcpy(strDropoffString, "1st Drop-off Point - Mamplasan Toll Exit");
        else if(nDropOff == 2) strcpy(strDropoffString, "2nd Drop-off Point - Phase 5, San Jose Village");
        else if(nDropOff == 3) strcpy(strDropoffString, "3rd Drop-off Point - Milagros Del Rosario (MRR) Building - East Canopy");
    }
    else if(nTripNum <= 9) //ETON Exit
    {
        if(nDropOff == 1) strcpy(strDropoffString, "1st Drop-off Point - Laguna Blvd. Guard House (across Paseo PHOENIX Gasoline Station)");
        else if(nDropOff == 2) strcpy(strDropoffString, "2nd Drop-off Point - Milagros Del Rosario (MRR) Building - East Canopy");
    }
    else if(nTripNum <= 15) //Via Estrada
    {
        if(nDropOff == 1) strcpy(strDropoffString, "1st Drop-off Point - Petron Gasoline Station along Gil Puyat Avenue");
        else if(nDropOff == 2) strcpy(strDropoffString, "2nd Drop-off Point - Gate 4: Gokongwei Gate");
        else if(nDropOff == 3) strcpy(strDropoffString, "3rd Drop-off Point - Gate 2: North Gate (HSSH)");
        else if(nDropOff == 4) strcpy(strDropoffString, "4th Drop-off Point - Gate 1: South Gate (LS Building Entrance)");
    }
    else //Via Buendia/LRT 
    {
        if(nDropOff == 1) strcpy(strDropoffString, "1st Drop-off Point - Petron Gasoline Station along Gil Puyat Avenue");
        else if(nDropOff == 2) strcpy(strDropoffString, "2nd Drop-off Point - College of St. Benilde (CSB) along Taft Avenue");
        else if(nDropOff == 3) strcpy(strDropoffString, "3rd Drop-off Point - Gate 4: Gokongwei Gate");
        else if(nDropOff == 4) strcpy(strDropoffString, "4th Drop-off Point - Gate 2: North Gate (HSSH)");
        else if(nDropOff == 5) strcpy(strDropoffString, "5th Drop-off Point - Gate 1: South Gate (LS Building Entrance)");
    }
    return strDropoffString;
}

int 
dropoffStringToNumber(char *strDropoffString)
{
    const char *firstChar= &strDropoffString[0]; //Get the first character 
    int nDropOff = atoi(firstChar); //Convert to int
    
    return nDropOff;
}

void 
readFile(struct shuttle *shuttles, 
         char *strFileName) 
{
    char strTripString[6], strDump[6];
    int nTripNum, i, bIsFileNotEmpty = 1;//Not empty
    //File Debugging
    //If file doesn't exist then make it and see if there is still error, notify user if there is
    FILE *fptr = fopen(strFileName, "r");
    if(fptr == NULL) 
    {
        fclose(fptr);
        fptr = fopen(strFileName, "w");
        if(fptr == NULL) 
        {
            printf("Error in Creating File!");   
        }
        fclose(fptr);
        fptr = fopen(strFileName, "r");
    }
    // Check if file is empty
    fseek(fptr, 0, SEEK_END); // goto end of file
    if (ftell(fptr) == 0) 
    {
        bIsFileNotEmpty = 0; //File is Empty
    }
    rewind(fptr);

    // Assign the shuttle's trip number (in array form) and embarkation point
    for(i = 0; i < 20; i++) 
    {
        shuttles[i].nTripNumber = i;
        shuttles[i].nCount = 0;
        if(i <= 9) strcpy(shuttles[i].strEmbarkationPoint, "Manila");
        else strcpy(shuttles[i].strEmbarkationPoint, "Laguna");
    }
    //Populate structure with values from file. Don't run if file is empty.
    while(!feof(fptr) && bIsFileNotEmpty) 
    {
        fscanf(fptr, "%s", strTripString);
        nTripNum = tripStringToNumber(strTripString) - 1;
        shuttles[nTripNum].nTripNumber = nTripNum;
        fscanf(fptr, "%s %[^\n] %d %d %[^\n] %s %s", 
                strDump, shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].strName, 
                &shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].nIdNumber, 
                &shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].nPriorityGroup, 
                shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].strDropOffString, strDump, strDump);
        shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].nDropOff = dropoffStringToNumber(shuttles[nTripNum].passengers[shuttles[nTripNum].nCount].strDropOffString);
        shuttles[nTripNum].nCount++;
    }
    fclose(fptr);
}

void 
findShuttleSeat(struct shuttle shuttles[20], 
                struct passenger applicant, 
                int nTripNum) 
{
    int nTripNumArr = nTripNum - 1, bIsBusFound = 0, nLowestPriority = 0, bIsSeatAvailable = 1, i, j, nMaxCapacity = 16, nNextTripArr = 0;
    //The system will automatically assign the passenger to the selected trip if that trip is not yet full.
    if(shuttles[nTripNumArr].nCount < nMaxCapacity) 
    {
        printf("Shuttle has a free seat, you have been assigned to seat %d\n\n", shuttles[nTripNumArr].nCount + 1);
        shuttles[nTripNumArr].passengers[shuttles[nTripNumArr].nCount] = applicant;
        shuttles[nTripNumArr].nCount += 1;
    }
    /*In the event that the trip is full and a high priority passenger wants to 
    board, the first lowest priority passenger listed will be removed and moved to the next 
    trip. The removed passenger will be notified(display a message on screen).*/
    else if(shuttles[nTripNumArr].nCount >= nMaxCapacity)
    {
        for(i = nMaxCapacity - 1; bIsBusFound == 0; i--) 
        {
            //Make sure that shuttle still has slots, if not move to next available  
            //19 in array is the last trip in Laguna and 9 is the last trip in Manila
            //Get the lowest priority number in shuttle
            for(j = nMaxCapacity - 1; j != 0; j--)
            {
                if(shuttles[nTripNumArr].passengers[j].nPriorityGroup > nLowestPriority)
                {
                    nLowestPriority = shuttles[nTripNumArr].passengers[j].nPriorityGroup;
                }
            }
            //In Arr
            nNextTripArr = nTripNumArr + 1;
            //Check if person in the seat is the lowest in the shuttle and is valid
            if((shuttles[nTripNumArr].passengers[i].nPriorityGroup == nLowestPriority || shuttles[nTripNumArr].passengers[i].nPriorityGroup == 0) && 
               (applicant.nPriorityGroup < shuttles[nTripNumArr].passengers[i].nPriorityGroup || shuttles[nTripNumArr].passengers[i].nPriorityGroup == 0) && 
               shuttles[nTripNumArr].passengers[i].strName[0] != '\0' && shuttles[nTripNumArr].passengers[i].nPriorityGroup >= 1 && 
               shuttles[nTripNumArr].passengers[i].nPriorityGroup <= 6 && shuttles[nTripNumArr].nTripNumber >= 0 &&
               checkID(shuttles[nTripNumArr].passengers[i].nIdNumber) == 0 && checkDropoff(shuttles[nTripNumArr].nTripNumber + 1, shuttles[nTripNumArr].passengers[i].nDropOff) == 0) 
            {
                printf("You have been assigned to shuttle AE%d seat %d\n", normalizeTripString(nTripNum), i + 1);
                // See if there is a shuttle with seats available
                if(nNextTripArr < 20)
                {
                    do 
                    {
                        // Give only Manila shuttles if trip was originally in Manila
                        if((nTripNum <= 9 && nNextTripArr > 9) || (nTripNum > 9 && nNextTripArr > 19))
                            bIsSeatAvailable = 0; //No Seats Available in any of the next busses
                        nNextTripArr++;
                    } while(shuttles[nNextTripArr].nCount > 15 && bIsSeatAvailable == 1);
                }
                else 
                {
                    if((nTripNum <= 9 && nNextTripArr > 9) || (nTripNum > 9 && nNextTripArr > 19))
                        bIsSeatAvailable = 0; //No Seats Available in any of the next busses
                    else bIsSeatAvailable = 1;
                }
                //There is still a next shuttle going to destination
                if(bIsSeatAvailable == 1) 
                {
                    // Tell the removed passenger their new bus
                    printf("%s, with I.D. %d, A higher priority user wants to use this shuttle, you have been moved to shuttle AE%d, seat %d\n\n", shuttles[nTripNumArr].passengers[i].strName, shuttles[nTripNumArr].passengers[i].nIdNumber, normalizeTripString(nNextTripArr), shuttles[nNextTripArr].nCount + 1);
                    // If next shuttle has different exit then change accordingly
                    if(shuttles[nTripNumArr].nTripNumber <= 4 && shuttles[nTripNumArr].nTripNumber + 1 > 4) 
                    {
                        if(shuttles[nTripNumArr].passengers[i].nDropOff == 3) 
                        {
                            shuttles[nTripNumArr].passengers[i].nDropOff = 2;
                            strcpy(shuttles[nTripNumArr].passengers[i].strDropOffString, getDropoffString(shuttles[nTripNumArr].passengers[i].nDropOff, 6)); //6 is ETON
                        }
                    }
                    else if(shuttles[nTripNumArr].nTripNumber <= 14 && shuttles[nTripNumArr].nTripNumber + 1 > 14)
                    {
                        if(shuttles[nTripNumArr].passengers[i].nDropOff == 3 && 
                           shuttles[nTripNumArr].passengers[i].nDropOff != 1 ) //1 for both exits are the same
                        {
                            shuttles[nTripNumArr].passengers[i].nDropOff += 1; //from Estrada to Buendia one was added and everything was moved back 1 spot
                            strcpy(shuttles[nTripNumArr].passengers[i].strDropOffString, getDropoffString(shuttles[nTripNumArr].passengers[i].nDropOff, 16));//16 is Buendia
                        }
                    }
                    shuttles[nNextTripArr].passengers[shuttles[nNextTripArr].nCount] = shuttles[nTripNumArr].passengers[i];
                    shuttles[nNextTripArr].nTripNumber = nNextTripArr - 1;
                    shuttles[nTripNumArr].passengers[i] = applicant;
                }
                //No more shuttles left
                else 
                {
                    printf("%s, with I.D. %d, A higher priority user wants to use this shuttle. You have been moved to the special shuttle\n\n", shuttles[nTripNumArr].passengers[i].strName, shuttles[nTripNumArr].passengers[i].nIdNumber);
                    shuttles[nTripNumArr].passengers[i] = applicant;
                }
                bIsBusFound++;
            } 
            if(i == 0) 
            {
                i = 16;
                if((nTripNum <= 9 && nTripNum + 1 > 9) || (nTripNum > 9 && nTripNum + 1 > 20)) 
                {
                    printf("Sorry, all the shuttles are full and you have a lower priority over everyone else, please try again another day!\n\n");
                    bIsBusFound++;
                }
                nTripNum++;
                nTripNumArr = nTripNum - 1;
            }
        }
    }
}