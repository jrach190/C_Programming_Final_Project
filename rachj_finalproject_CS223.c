/* -----------------------------------------------
 Submitted By: JONATHAN RACH
 Homework Number: FINAL PROJECT
 Credit to:
 LIAM GUNTER, FOR BAR GRAPH AND LOGIC ASSISTANCE
 
 Submitted On: 2 DECEMBER 2014
 
 By submitting this program with my name,
 I affirm that the creation and modification
 of this program is primarily my own work.
 ------------------------------------------------ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Define constant values to be used throughout the project
#define MAXCHAR 50
#define MAXATTEMPTS 3
#define MAXASTEROIDS 20
#define XYZ 3
#define X 0
#define Y 1
#define Z 2
#define MAXDISTANCE 1736
#define VELOCITY 25
#define WARNINGDISTANCE 750
#define DASHVALUE 40
#define RANDMIN 1
#define RANDMAX 1000

//Prototype methods in the project
int passcheck(void);
int makeasteroids(void);
int distancefunc(void);
double distancevaluefunc(double x, double y, double z);
int barfunc(void);

//Prototype structure needed for password function
struct passstruct
{
    char username[MAXCHAR];
    char password[MAXCHAR];
};


//Declare variables to be usable by all functions
int rowofclosest;
double distance;
double closest=MAXDISTANCE;
char asteroidletter='a';
char asteroidletterclosest;

//Prototype the 2D array for the asteroids' name and location
double asteroidlocation[MAXASTEROIDS][XYZ];

int main(void){
    int passyn,astret, distret, barret;
    /*FILE *fpassfile;
    fpassfile=fopen("ident.txt", "w");
    fprintf(fpassfile, "leia\t12345\ndarth\t23456\nr2d2\t34567\nsolo\t45678\njabba\t56789");
    fclose(fpassfile);
    */
    passyn=passcheck();
    if (passyn==1){
        astret=makeasteroids();
        distret=distancefunc();
        barret=barfunc();
        return astret;
    }
    return 1;
}



//Method to return the value of 
int passcheck(void) {
    int i=1;
    FILE *fident;
    fident=fopen("ident.txt", "r");
    if (fident==NULL){
        printf("Error: Username and password file not found. Please restart the program and try again.");
        return -1;
    }
    
    struct passstruct fileunpw;
    for (i=0;i<MAXATTEMPTS;i++){
        struct passstruct userinfo;
        printf ("Please enter your username.\n");
        scanf("%s",userinfo.username);
        while (fscanf(fident, "%s%s", fileunpw.username, fileunpw.password) !=EOF){
            if ((strcmp(userinfo.username, fileunpw.username))==0){
                printf ("Please enter your password.\n");
                scanf ("%s",userinfo.password);
                if ((strcmp(userinfo.password, fileunpw.password))==0){
                    fclose(fident);
                    return 1;
                }
            }
        }
        printf("Incorrect username/password.\n");
    }
    printf("Incorrect password tries too many times. Goodbye.\n");
    fclose(fident);
    return 0;
}


//Function to write the random numbers to asteroids.txt file
int makeasteroids (void)
{
    //Makes pointer to asteroids.txt and opens it for writing
    FILE *fasteroid;
    fasteroid=fopen("asteroids.txt","w");
    
    //For loop counter
    int i;
    double randnumx, randnumy, randnumz;
    
    //Sets the seed for the random number generator as time
    srand(time(NULL));
    
    //Stores asteroid letter and x,y and z coordinates to the asteroids.txt file
    for (i=0; i<MAXASTEROIDS; i++)
    {
        randnumx= RANDMIN + rand() % RANDMAX;
        randnumy= RANDMIN + rand() % RANDMAX;
        randnumz= RANDMIN + rand() % RANDMAX;
        fprintf(fasteroid, "%c %.2lf %.2lf %.2lf", asteroidletter, randnumx, randnumy, randnumz);
        asteroidletter++;
    }
    //Closes the asteroids.txt file
    fclose (fasteroid);
    return 0;
}

//Function to copy the x,y and z coordinates from asteroids.txt, calculate the distance based off of this, and save info to distance.txt file
int distancefunc (void){
    //Makes pointers to distance.txt and asteroids.txt and opens them for writing and reading, respectively
    FILE *fasteroid;
    fasteroid=fopen("asteroids.txt","r");
    FILE *fdistance;
    fdistance=fopen("distance.txt","w");
    
    //Integer for the while loop counter
    int count=0;
    
    //While loop to
    while (fscanf(fasteroid, "%c %lf %lf %lf", &asteroidletter, &asteroidlocation[count][X], &asteroidlocation[count][Y], &asteroidlocation[count][Z])==4)
    {
        //Calculate the distance
        distance=distancevaluefunc (asteroidlocation[count][X], asteroidlocation[count][Y], asteroidlocation[count][Z]);
        fprintf(fdistance, "%c %.2lf %.2lf %.2lf %.2lf\n", asteroidletter, asteroidlocation[count][X], asteroidlocation[count][Y], asteroidlocation[count][Z], distance);
        //If distance is closest one, save to location for reference by barfunc
        if (distance<closest)
        {
            closest=distance;
            rowofclosest=count;
            asteroidletterclosest=asteroidletter;
        }
        count++;
    }
    //Close the asteroids.txt and distance.txt files
    fclose (fasteroid);
    fclose (fdistance);
    //Reset the letter counter to a before closing the function
    asteroidletter='a';
    return 0;
}

//Function to return the distance of an asteroid from the file
double distancevaluefunc (double x, double y, double z){
    //Sets the retvalue equal to the distance
    double retvalue;
    retvalue=sqrt((pow(x,2))+(pow(y,2))+(pow(z,2)));
    return retvalue;
}

//Function to display warnings, play sound and make the bar chart.
int barfunc (void)
{
    //Makes pointer to distance.txt and opens it for reading
    
    FILE *fdistance;
    fdistance=fopen("distance.txt","r");
    
    //reset loop counter to 0 for the new loop
    int count=0;
    double time=closest/VELOCITY;
    //Prints the warning of the closest asteroid and how far away it is from impact
    printf ("Warning - Warning - Warning\n");
    printf ("Nearest asteroid %c at (%.2lf, %.2lf, %.2lf): %.2lf km away\n", asteroidletterclosest,asteroidlocation[rowofclosest][X], asteroidlocation[rowofclosest][Y],asteroidlocation[rowofclosest][Z], closest);
    printf ("Time to impact %.2lf seconds!\n", time);
    
    while (fscanf(fdistance, "%c %lf %lf %lf %lf", &asteroidletter, &asteroidlocation[count][X], &asteroidlocation[count][Y], &asteroidlocation[count][Z], &distance)!=EOF)
    {
        //Check to see if while loop is running:
        //printf("Ast. %c Distance is %.2lf\n", asteroidletter, distance);
        
        //If distance is less than the warning distance, print a display warning with location and distance
        if (distance<WARNINGDISTANCE)
        {
            printf ("Asteroid %c is at (%.2lf ,%.2lf ,%.2lf): %.2lf km away\n", asteroidletter, asteroidlocation[count][X], asteroidlocation[count][Y], asteroidlocation[count][Z], distance);
        }
        count++;
    }
    
    //Resets the asteroid letter counter to 'a'
    asteroidletter='a';
    
    //Makes the console beep 7 times for an alarm sound
    printf ("\a \a \a \a \a \a \a \n");
    //Prints the two dashed lines to separate alarm message from asteroid position bar graph
    printf ("-------------------------------------\n");
    printf ("-------------------------------------\n");
    
    //For loop to make bar graph with locations of asteroids with appropriate scale
    for (count=0; count<MAXASTEROIDS; count++)
    {
        //fscanf(fident, "%c %lf %lf %lf %lf", &asteroidletter, &
        distance=distancevaluefunc (asteroidlocation[count][X], asteroidlocation[count][Y], asteroidlocation[count][Z]);
        printf ("\n%c |", asteroidletter);
        while ((distance-DASHVALUE)>0)
        {
            //Prints an asterisk for as long as the distance is greater than 40, the value of one dash
            printf ("*");
            distance-=DASHVALUE;
        }
        asteroidletter++;
    }
    
    //Prints the scale for the asteroid positions
    printf("\n  0----|----|----|----|----|----|----|----|---- km");
    printf("\n      200  400  600  800  1000 1200 1400 1600 \n\n");
    fclose (fdistance);
    return 0;
}


