#include "raylib.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#include <string>
#include <stdlib.h>


// Default screen Width, Height and min widnow size
#define ScreenWidth 1200
#define ScreenHeight 800

#define MinWindowWidth 500
#define MinWindowHeight 800



// Piller's colors:
// TODO:
#define NORMAL 0
#define COMPARING 1
#define SORTED 2
#define SELECTED 3
#define MINIMUM 4
#define PIVOT 5
#define LEFT 6
#define RIGHT 7


#define FPS 120

// Size of Array of numbers
int NumberOfPillers = 50;



// Function Prototypes:
void ShowMenuScreen();
void ShowStartOptions();
void ShowEndingScreen();

void Button(float x, float y, char *Text, Color color, bool &state);

void Start_Button(float size, float font, char Start[]);
void Selection_Sort_Button(float size, char Selection_Sort_Text[]);
void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]);
void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]);
void Merge_Sort_Button(float size, char Merge_Sort_Text[]);
void Quick_Sort_Button(float size, char Quick_Sort_Text[]);


Color FindColorForPiller(int pillerState);
void DrawArray(std::vector<std::pair<int, int>> arr);
void RandomizeArray(std::vector<std::pair<int, int>> &arr);

// Sorting Functions Prototypes:
void SortArray();

void Selection_Sort(std::vector<std::pair<int, int>> &arr);

void Insertion_Sort(std::vector<std::pair<int, int>> &arr);

void Bubble_Sort(std::vector<std::pair<int, int>> &arr);


void Merge_Sort(std::vector<std::pair<int, int>> &arr);
void MergeSort_Finale(std::vector<std::pair<int, int>> &arr,
    int leftIndex, int rightIndex);
void Merge(std::vector<std::pair<int, int>> &arr,
    int leftIndex, int m, int rightIndex);


void Quick_Sort(std::vector<std::pair<int, int>> &arr);
void Quick_Sort_Final(std::vector<std::pair<int, int>> &arr, int low, int high);
int Partition(std::vector<std::pair<int, int>> &arr, int low, int high);


// Changing size and speed
void ChangeSize(char operation, int &value);
void ChangeSpeed(char operation, int &value);

// Game States:
bool ShouldShowMenuScreen = true;
bool ShouldShowStartOptions = false;
bool GameShouldStart = false;
bool ShouldShowEndingScreen = false;

bool MuteSound = false;

bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;
bool NormalSize = false;
bool NormalSpeed = false;


bool SelectionSortPressed = false;
bool InsertionSortPressed = false;
bool BubbleSortPressed = false;
bool MergeSortPressed = false;
bool QuickSortPressed = false;
std::vector<bool *> SortingChoice = {
    &SelectionSortPressed,
    &InsertionSortPressed,
    &BubbleSortPressed,
    &MergeSortPressed,
    &QuickSortPressed,
};

bool ShouldRandomizeArray = true;



// Array of Values and It's States;
std::vector<std::pair<int, int>> arr(NumberOfPillers);

int SortingSpeed = 61;

int main(){

    // Window Configuration:
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithms :)");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);

    InitAudioDevice();

    // Set FPS
    SetTargetFPS(FPS);


    Image SoundOnOff = LoadImage("Images/Sound.png");
    ImageResize(&SoundOnOff, 20, 20);

    Texture2D texture = LoadTextureFromImage(SoundOnOff);
    
    Sound CheelSunsetDream = LoadSound("VideoEffects/Cheel Sunset Dream.mp3");

    while (!WindowShouldClose()){
        {
            float x = ((2.0 * GetScreenWidth()) / 100);
            float y = ((2.0 * GetScreenHeight()) / 100);
            Rectangle r1 = {
                .x = x,
                .y = y,
                .width = (float) x + 20,
                .height = (float) y + 20,
            };

        if (CheckCollisionPointRec(GetMousePosition(), r1))
            if (IsMouseButtonPressed(0))
                MuteSound = !MuteSound;
        }

        if (MuteSound){
            DrawTexture(texture, (2 * GetScreenWidth()) / 100, 
                (2 * GetScreenHeight()) / 100, RED);

            PauseSound(CheelSunsetDream);
            
        } else{
            DrawTexture(texture, (2 * GetScreenWidth()) / 100, 
                (2 * GetScreenHeight()) / 100, WHITE);

            ResumeSound(CheelSunsetDream);
        }
        

        if (!IsSoundPlaying(CheelSunsetDream) && !MuteSound)
            PlaySound(CheelSunsetDream);


        BeginDrawing();
        ClearBackground(PURPLE);

        if (ShouldShowMenuScreen){
            ShowMenuScreen();
        }
        
        
        ShouldShowStartOptions = false;
        for (bool *i : SortingChoice){
            if (*i == true)
                ShouldShowStartOptions = true;
        }

        if (ShouldShowStartOptions){
            ShowStartOptions();
        }
        

        if (GameShouldStart){
            ShouldShowMenuScreen = false;
            ShouldShowStartOptions = false;

            SetTargetFPS(SortingSpeed);
            SortArray();

            GameShouldStart = false;
            ShouldShowEndingScreen = true;
        }

        if (ShouldShowEndingScreen){
            ShowEndingScreen();
        }

        if (ShouldRandomizeArray){
            RandomizeArray(arr);
        }

        EndDrawing();
    }

    UnloadImage(SoundOnOff);
    UnloadTexture(texture);
    UnloadSound(CheelSunsetDream);

    CloseAudioDevice();

    CloseWindow();

    return EXIT_SUCCESS;
}


void ShowMenuScreen(){
    float font = (2.5 * GetScreenWidth()) / 100;
    char Selection_Sort_text[] = "Selection Sort!";            
    float tmp = (GetScreenWidth() * 5)/100;
    Selection_Sort_Button(tmp, Selection_Sort_text);

    char Insertion_Sort_Text[] = "Insertion Sort!";
    tmp += MeasureText(Selection_Sort_text, font) + font;
    Insertion_Sort_Button(tmp, Insertion_Sort_Text);

    
    char Bubble_Sort_Text[] = "Bubble Sort!";
    tmp += MeasureText(Insertion_Sort_Text, font) + font;
    Bubble_Sort_Button(tmp, Bubble_Sort_Text);

    
    char Merge_Sort_Text[] = "Merge Sort!";
    tmp += MeasureText(Bubble_Sort_Text, font) + font;
    Merge_Sort_Button(tmp, Merge_Sort_Text);


    char Quick_Sort_text[] = "Qucik Sort!";
    tmp += MeasureText(Merge_Sort_Text, font) + font;
    Quick_Sort_Button(tmp, Quick_Sort_text);
    

    DrawArray(arr);
}

void ShowStartOptions(){
    float font = (2.5 * GetScreenWidth()) / 100;

    char StartText[] = "Start Game!";
    float tmp = (27*GetScreenWidth()) / 100; 
    Start_Button(tmp, font, StartText);

    tmp += MeasureText(StartText, font) + 75;
    char RandomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight()/20 + font*2,
        RandomizeArrayText, DARKGRAY, ShouldRandomizeArray);

    
    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;

    NormalSize = false;
    NormalSpeed = false;

    tmp = (84.2 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed";

 

    Button(tmp, GetScreenHeight()/20 + font*2,
        TimeButtonText, BLUE, NormalSpeed);

    if (NormalSpeed){
        ChangeSpeed('/', SortingSpeed);
        return;
    }


    tmp += MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SpeedPlusButtonText, ORANGE, addSpeed); 


    if (addSpeed){
        ChangeSpeed('+', SortingSpeed);
        return;
    }

    tmp += MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SpeedMinusButtonText, ORANGE, subSpeed);


    if (subSpeed){
        ChangeSpeed('-', SortingSpeed);
        return;
    }


    tmp = (5 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizeButtonText, BLUE, NormalSize);
    

    if (NormalSize){
        ChangeSize('/', NumberOfPillers);
        return;
    }



    tmp += MeasureText(SizeButtonText, font) + 20;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizePlusButton, ORANGE, addSize);

    if (addSize){
        ChangeSize('+', NumberOfPillers);
        return;
    }

    tmp += MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight()/20 + font*2,
        SizeMinusButton, ORANGE, subSize);


    if (subSize){
        ChangeSize('-', NumberOfPillers);
        return;
    }
}

void ChangeSize(char operation, int &value){
    switch (operation)
    {
    case '-':
        if (value > 5){
            value -= 5;
            for (int i = 0; i < 5; i++)
                arr.pop_back();
        }
        break;
    
    case '+':
        value += 5;
        for (int i = 0; i < 5; i++)
            arr.push_back({GetRandomValue(40, MinWindowHeight-140), NORMAL});
        break;


    default:
        while (NumberOfPillers > 50){
            NumberOfPillers--;
            arr.pop_back();
        }

        while (NumberOfPillers < 50){
            NumberOfPillers++;
            arr.push_back({GetRandomValue(40, MinWindowHeight-140), NORMAL});
        }
        break;
    }

    for (int i = 0; i < NumberOfPillers; i++){
        arr[i].second = NORMAL;
    }


    DrawArray(arr);
}

void ChangeSpeed(char operation, int &value){

    switch (operation)
    {
    case '-':
        value = value - 10 > 0 ? value - 10 : value;
        break;
    
    case '+':
        value += 10;
        break;

    default:
        value = 61;
        break;
    
    }
}



void ShowEndingScreen(){
    DrawArray(arr);

    ShouldShowMenuScreen = true;
    ShouldShowEndingScreen = false;
}


void Start_Button(float size, float font, char Start[]){
    Button(size, GetScreenHeight()/20 + font*2,
        Start, DARKGRAY, GameShouldStart);

    return;
}

void Selection_Sort_Button(float size, char Selection_Sort_Text[]){
    Color color;
    if (SelectionSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/20, Selection_Sort_Text, color, 
        SelectionSortPressed);
}

void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]){
    Color color;
    if (InsertionSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/20, Insertion_Sort_Text, color,
        InsertionSortPressed);
}

void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]){
    Color color;
    if (BubbleSortPressed)
        color = BLUE;
    else
        color = GREEN;
    
    Button(size, GetScreenHeight()/20, Bubble_Sort_Text, color,
        BubbleSortPressed);
}

void Merge_Sort_Button(float size, char Merge_Sort_Text[]){
    Color color;
    if (MergeSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/20, Merge_Sort_Text, color, 
        MergeSortPressed);
}

void Quick_Sort_Button(float size, char Quick_Sort_Text[]){
    Color color;
    if (QuickSortPressed)
        color = BLUE;
    else
        color = GREEN;
    
    Button(size, GetScreenHeight()/20, Quick_Sort_Text, color,
        QuickSortPressed);
}

void Button(float x, float y, char *Text, Color color, bool &state){
    float font = (2.5 * GetScreenWidth()) / 100;
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float) MeasureText(Text, font),
        .height = (float) font,
    };

    
    if (CheckCollisionPointRec(GetMousePosition(), r1)){
        
        DrawText(Text, x, y, font, RED);

        if (IsMouseButtonPressed(0)){
            if (state == true)
                state = false;
            else 
                state = true;
                

            if (state != ShouldRandomizeArray && state != GameShouldStart
            && (state != addSize && state != subSize && state != addSpeed
            && state != subSpeed && state != NormalSize && state != NormalSpeed))

                for (bool *i : SortingChoice)
                    if (i != &state)
                        *i = false;
            return;
        }   
        

    } else {
        DrawText(Text, x, y, font, color);
    }

    return;
}


void DrawArray(std::vector<std::pair<int, int>> arr){
    // Drawing the array / pillers:
    float BarWidth = (float) GetScreenWidth() / NumberOfPillers;


    for (int i = 0; i < NumberOfPillers; i++){ 
        Color color = FindColorForPiller(arr[i].second);

        DrawRectangleV (Vector2{(float) i*BarWidth, 
            (float) GetScreenHeight() - arr[i].first},
            Vector2{BarWidth, (float) arr[i].first}, color);
    }
}

void RandomizeArray(std::vector<std::pair<int, int >> &arr){
    // RandomizeArray
    if (ShouldRandomizeArray){
        for (int i = 0; i < NumberOfPillers; i++)
            arr[i] = {GetRandomValue(40, MinWindowHeight-140), NORMAL};
            
        ShouldRandomizeArray = false;
    }
}



Color FindColorForPiller(int pillerState){
    switch (pillerState)
    {
    case SELECTED:
        return LIGHTGRAY;
        break;

    case COMPARING:
        return LIGHTGRAY;
        break;
    
    case SORTED:
        return GREEN;
        break;

    case MINIMUM:
        return SKYBLUE;
        break;


    case LEFT:
        return SKYBLUE;
        break;

    case RIGHT:
        return BLUE;
        break;

    case PIVOT:
        return GRAY;
        break;

    default:
        return GOLD;
        break;
    }
}


void SortArray(){
    for (bool *state : SortingChoice){
        if (*state == true){
            if (state == &InsertionSortPressed)
                Insertion_Sort(arr);

            else if (state == &SelectionSortPressed)
                Selection_Sort(arr);

            else if (state == &BubbleSortPressed)
                Bubble_Sort(arr);

            else if (state == &MergeSortPressed)
                Merge_Sort(arr);

            else if (state == &QuickSortPressed)
                Quick_Sort(arr); 

                                 
        }

        *state = false;
    }
    
    return;
}



// Selection Sort:
void Selection_Sort(std::vector<std::pair<int, int>> &arr){

    for (int i = 0; i < NumberOfPillers; i++){
        arr[i].second = SELECTED;

        std::pair<int, int> minNum = {arr[i].first, i};
        for (int j = i; j < NumberOfPillers; j++){
            arr[j].second = COMPARING;
            arr[minNum.second].second = MINIMUM;


            if (minNum.first > arr[j].first){

                if (minNum.second == i)
                    arr[minNum.second].second = SELECTED;
                else if (minNum.second == j)
                    arr[minNum.second].second = COMPARING;
                else 
                    arr[minNum.second].second = NORMAL;


                minNum = {arr[j].first, j};

            }


            BeginDrawing();
            ClearBackground(PURPLE);

            DrawArray(arr);


            if (j == i)
                arr[j].second = SELECTED;
            else 
                arr[j].second = NORMAL;

            EndDrawing();
        }

        std::swap(arr[i], arr[minNum.second]);
        arr[i].second = SORTED;

        for (int j = i+1; j < NumberOfPillers; j++){
            arr[j].second = NORMAL;
        }
    }
}

// Insertion Sort:
void Insertion_Sort(std::vector<std::pair<int, int>> &arr){

    arr[0].second = SORTED;
    for (int i = 1; i < arr.size(); i++){
        BeginDrawing();

        ClearBackground(PURPLE);

        arr[i].second = SELECTED;

            Color color = GREEN;

            int key = arr[i].first;


            // Sorting the arr
            int tmp = 0;
            int j = i-1;
            while (arr[j].first > key && j >= 0){

                // Swap the two pillers
                std::swap(arr[j+1].first, arr[j].first);


                // Make the comparing piller
                arr[j].second = COMPARING;
                

                BeginDrawing();
                ClearBackground(PURPLE);


                DrawArray(arr);

                // Make the Selected piller sorted again (it already was)
                arr[j].second = SORTED;

                EndDrawing();

                j--;
                tmp++;
            }


            if (tmp == 0){
                BeginDrawing();


                ClearBackground(PURPLE);

                DrawArray(arr);

                EndDrawing();
            }


            // DE-Select the unnecesserry pillers
            for (int k = 0; k < NumberOfPillers; k++)
                if (arr[k].second == SELECTED)
                    arr[k].second = NORMAL;


            // Color the sorted pillers                
            for (int k = i-1; k >= 0; k--)
                arr[k].second = SORTED;


            arr[j+1].first = key;
            arr[j+1].second = SORTED;

        EndDrawing();
    }

    arr[NumberOfPillers-1].second = SORTED;
}


// Bubble Sort:
void Bubble_Sort(std::vector<std::pair<int, int>> &arr){

    int endingPoint = NumberOfPillers;

    bool swapped;
    do {
        swapped = false;

        for (int i = 0; i < endingPoint-1; i++){

            arr[i].second = SELECTED;

            if (arr[i] > arr[i+1]){
                // arr[i+1].second = SELECTED;

                std::swap(arr[i], arr[i+1]);
                
                swapped = true;

            }         
        


        BeginDrawing();

        ClearBackground(PURPLE);


            for (int k = NumberOfPillers-1; k >= endingPoint; k--)
                arr[k].second = SORTED;
            
            DrawArray(arr);

            for (int k = i; k >= 0; k--)
                arr[k].second = NORMAL;




        EndDrawing();

        }


        endingPoint--;

    } while (swapped);

    for (int k = NumberOfPillers-1; k >= 0; k--)
        arr[k].second = SORTED;
}

// Merge Sort:
// TODO;
void Merge_Sort(std::vector<std::pair<int, int>> &arr){
    MergeSort_Finale(arr, 0, NumberOfPillers-1);
}

void MergeSort_Finale(std::vector<std::pair<int, int>> &arr,
    int leftIndex, int rightIndex){

    if (leftIndex >= rightIndex)
        return;
    
    int m = (leftIndex + rightIndex) / 2;

    MergeSort_Finale(arr, leftIndex, m);
    MergeSort_Finale(arr, m+1, rightIndex);

    Merge(arr, leftIndex, m, rightIndex);
}

void Merge(std::vector<std::pair<int, int>> &arr,
    int leftIndex, int m, int rightIndex){

    int arrSizeLeft = m - leftIndex + 1;
    int arrSizeRight = rightIndex - m;



    int left[arrSizeLeft];
    for (int i = 0; i < arrSizeLeft; i++){
        left[i] = arr[leftIndex + i].first;
        arr[leftIndex + i].second = LEFT;
    }
    

    int right[arrSizeRight];
    for (int i = 0; i < arrSizeRight; i++){
        right[i] = arr[m + 1 + i].first;
        arr[m + 1 + i].second = RIGHT;
    }



    BeginDrawing();
        ClearBackground(PURPLE);


        DrawArray(arr);

    EndDrawing();
    




    int i = 0; // Left index
    int j = 0; // Right index
    
    int k = leftIndex; // New array index;


    while (i < arrSizeLeft && j < arrSizeRight){
        if (left[i] <= right[j]){
            arr[k].first = left[i];
            i++;
        } else{
            arr[k].first = right[j];
            j++;
        }
        arr[k].second = SORTED;
        
        k++;
    }


    while(i < arrSizeLeft){
        arr[k].first = left[i];
        arr[k].second = SORTED;
        
        i++;
        k++;
    }


    while(j < arrSizeRight){
        arr[k].first = right[j];
        arr[k].second = SORTED;
        
        j++;
        k++;
    }
}



// Quick Sort:
void Quick_Sort(std::vector<std::pair<int, int>> &arr){
    Quick_Sort_Final(arr, 0, NumberOfPillers-1);
}

void Quick_Sort_Final(std::vector<std::pair<int, int>> &arr, int low, int high){
    if (low < high){
        int pi = Partition(arr, low, high);


        Quick_Sort_Final(arr, low, pi-1);
        Quick_Sort_Final(arr, pi+1, high);
    } else {
        arr[low].second = SORTED;
        arr[high].second = SORTED;
    }
}

int Partition(std::vector<std::pair<int, int>> &arr, int low, int high){
    int pivot = arr[high].first;
    arr[high].second = PIVOT;

    int i = low-1;


    for (int j = low; j < high; j++){
        arr[j].second = COMPARING;

        if (arr[j].first < pivot){
            i++;
            std::swap(arr[i], arr[j]);

            arr[i].second = LEFT;
        } else {
            arr[j].second = RIGHT;
        }


        BeginDrawing();
            ClearBackground(PURPLE);

            DrawArray(arr);

        EndDrawing();

        for (int k = 0; k < NumberOfPillers; k++){
            /* if (arr[k].second != SORTED && arr[k].second != PIVOT)
                arr[k].second = NORMAL; */

            if (arr[k].second == COMPARING)
                arr[k].second = NORMAL;
        }
    }


    arr[high].second = SORTED;

    i++;
    std::swap(arr[i], arr[high]);

    for (int k = 0; k < NumberOfPillers; k++){
        if (arr[k].second != SORTED && arr[k].second != PIVOT)
            arr[k].second = NORMAL;      
    }


    return i;
}

