#include<SDL.h> 
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
#include<chrono> 

using namespace std::chrono; 
using namespace std;
#define SDL_MAIN_HANDLED //This function is defined in SDL_main.h, along with the preprocessor rule to redefine main() as SDL_main().
// void SDL_SetMainReady(void);

const int arraysize = 65 ;  
const int rectanglesize = 10; //in pixels ,

int arr[arraysize]; // intializing array
int Randomarr[arraysize]; //

bool complete = false;

//SDL_Window* window = NULL;
//SDL_Renderer* renderer = NULL;
//float delay;

class visualfunctions {
    const int SCREEN_WIDTH = 660; 
    const int SCREEN_HEIGHT = 375;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

public:
    float delay;
    // SDL INTIALIZING / CLOSING / HANDLING /VISUALIZING 
    bool init()   //initailizing SDL2 visulaizer and catching errors 
    {
        bool outcome = true;
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
            outcome = false;
        }
        else
        {
            if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
            {
                cout << "Warning: Linear Texture Filtering not enabled.\n";
            }

            window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (window == NULL)
            {
                cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
                outcome = false;
            }
            else
            {
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                if (renderer == NULL)
                {
                    cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                    outcome = false;
                }
            }
        }

        return outcome;
    }

    void close()  //closing visulaizer
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;

        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }

    void visualize(int x = -1, int y = -1, int z = -1)  // =======IMPORTANT=======
    {
        //x = largest value bar highlighted(green) 
        //y = currently comparing values with other values(red)
        //z = (only used for bubble sort) largest bar highlighted green
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        int j = 0;
        for (int i = 0; i <= SCREEN_WIDTH - rectanglesize; i += rectanglesize) //loops continously till screen width creating array sized bars 
        {
            SDL_PumpEvents(); //Pumps the event loop, gathering events from the input devices.

            SDL_Rect rect = { i, 0, rectanglesize, arr[j] }; //creates the individual array sized bars
            if (complete)     // in switch case once the function is completed , the value becomes true . and everthing turns into green
            {
                SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0); //goes green after sorting is completed
                SDL_RenderDrawRect(renderer, &rect);
                // cout << "complete\n";
            }
            //compares the intertion number with j 
            //j value increase everytime visulize function is called 
            else if (j == x || j == z) 
            {
                SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0); //green
                SDL_RenderFillRect(renderer, &rect);
                //cout << "green\n";
                //SDL_Delay(1000);
            }
            else if (j == y)
            {
                SDL_SetRenderDrawColor(renderer, 188, 80, 80, 0); //red 
                SDL_RenderFillRect(renderer, &rect);
                //cout << "red\n";
                //SDL_Delay(1000);
            }
            else
            {
                //cout << "else\n";
                SDL_SetRenderDrawColor(renderer, 0, 0, 205, 0); //blue- unsolvved
                SDL_RenderDrawRect(renderer, &rect);
                //SDL_RenderFillRect(renderer, &rect);
            }
            j++;
        }
        SDL_RenderPresent(renderer); //update the screen
    }

    //MERGE SORT 
    void merge2sortedarrays(int a[], int si, int ei)
    {
        int size_output = (ei - si) + 1;
        int* output = new int[size_output];

        int mid = (si + ei) / 2;
        int i = si, j = mid + 1, k = 0;
        while (i <= mid && j <= ei)
        {
            if (a[i] <= a[j])
            {
                output[k] = a[i];
                visualize(i, j);
                i++;
                k++;
            }
            else
            {
                output[k] = a[j];
                visualize(i, j);
                j++;
                k++;
            }

        }
        while (i <= mid)
        {
            output[k] = a[i];
            visualize(-1, i);
            i++;
            k++;
        }
        while (j <= ei)
        {
            output[k] = a[j];
            visualize(-1, j);
            j++;
            k++;
        }
        int x = 0;
        for (int l = si; l <= ei; l++)
        {
            a[l] = output[x];
            visualize(l);
            SDL_Delay(delay*2);
            x++;
        }
        delete[]output;
    }

    void mergesort(int a[], int si, int ei)
    {

        if (si >= ei)
        {
            return;
        }
        int mid = (si + ei) / 2;
       // SDL_Delay(delay * 2);
        mergesort(a, si, mid);
       // SDL_Delay(delay * 2);
        mergesort(a, mid + 1, ei);
       // SDL_Delay(delay * 2);
        merge2sortedarrays(a, si, ei);

    }

    void calculateMergesort(int a[], int si, int ei) {
        auto start = high_resolution_clock::now();
        mergesort(a, si, ei);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: "
            << duration.count() / 1000 << " milliseconds" << endl;
    }
    
    //INSERTION SORT
    void insertionSort(float delay)
    {
        auto start = high_resolution_clock::now();
        for (int i = 1; i < arraysize; i++)
        {
            int j = i - 1;
            int temp = arr[i];
            while (j >= 0 && arr[j] > temp)
            {
                arr[j + 1] = arr[j];
                j--;

                visualize(i, j + 1);
                // (green)i is paased as x as it it as barrier between sorted and unsorted sublist
                // (red) j+1 comparison between sorted values 
                SDL_Delay(delay); 
            }
            arr[j + 1] = temp;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: "
            << duration.count() / 1000 << " milliseconds" << endl;
    }
    
    //SELECTION SORT 
    void selectionSort(float delay)
    {
        auto start = high_resolution_clock::now();
        int minIndex;
        for (int i = 0; i < arraysize - 1; i++)
        {
            minIndex = i;
            for (int j = i + 1; j < arraysize; j++)
            {
                if (arr[j] < arr[minIndex])
                {
                    minIndex = j;
                    visualize(i, minIndex);
                }
                SDL_Delay(delay);
            }
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: "
            << duration.count() / 1000 << " milliseconds" << endl;
    }

    //QUICK SORT 

    int partition_array(int a[], int si, int ei)  ///quicksort
    { 
        int count_small = 0;

        for (int i = (si + 1); i <= ei; i++)
        {
            if (a[i] <= a[si])
            {
                count_small++;
            }
        }

        int c = si + count_small;
        int temp = a[c];
        a[c] = a[si];
        a[si] = temp;
        visualize(c, si);
        //green = pivot element
        //lowerbound = red 
        int i = si, j = ei;

        while (i<c && j>c)
        {
            if (a[i] <= a[c])
            {
                i++;
            }
            else if (a[j] > a[c])
            {
                j--;
            }
            else
            {
                //swap
                int temp_1 = a[j];
                a[j] = a[i];
                a[i] = temp_1;
                visualize(i, j);
                SDL_Delay(delay*20);
                i++;
                j--;
            }
        }
        return c;
    }

    void quickSort(int a[], int low, int high)
    {
        //si= lowerbound / start = 0
        //ei= upperbound  / end = array[arraysize -1]
        if (low < high)
        {
            int pi = partition_array(arr, low, high);
            // Separately sort elements before
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }

    }

    void calculatequickSort(int a[], int si, int ei) {
        auto start = high_resolution_clock::now();
        quickSort(a, si, ei);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: "
            << duration.count() / 1000 << " milliseconds" << endl;
    }

    //BUBBLE SORT

    void bubbleSort(float delay)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < arraysize - 1; i++)
        {
            for (int j = 0; j < arraysize - 1 - i; j++)
            {
                if (arr[j + 1] < arr[j])
                {
                    int temp = arr[j];        //swapping 
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    visualize(j + 1, j, arraysize - i); 
                    //visualize();
                    //x=j+1
                    //y=j
                    //z=arraysize -i (largest number)
                }
                SDL_Delay(delay); //delay in the sorting 
            }
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << endl;
    }

    //LOADING FUNCTIONS
    void loadArr()
    {
        memcpy(arr, Randomarr, sizeof(int) * arraysize); //Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.

    }

    void randomizeAndSaveArray()   //generating a list of randomized array 
    {
        unsigned int seed = (unsigned)time(NULL);
        srand(seed); //pseudo random number is generated Seeds the pseudo-random number generator used by rand() with the value seed.
        for (int i = 0; i < arraysize; i++)
        {
            int random = rand() % (SCREEN_HEIGHT);  //remainder is store , so that number does not exceed  screen height
            Randomarr[i] = random;
        }
    }

    float Speed() {
        float speed;
        bool again = true;
        cout << "PRESS the following numbers\n"
            << "0.5 = 0.5x speed \t"
            << "1 = 1x speed \t"
            << "2 = 2x speed \t"
            << "4 = 4x speed \t"
            << "10 = 10x speed \t\n";
        while (again) {
            while (!(cin >> speed)) {
                cout << "Must be a number: " << endl;
                cin.clear();
                cin.ignore(100, '\n');
            }
            if (speed > 0 && speed <= 10) {
                again = false;
                return 10/speed; 
            }
            else {
                cout << "INVALID number please enter again : ";
            }
        }
    }

    //EXECUTION

    void executevisulization()
    {
        if (!init())
        {
            cout << "SDL Initialization Failed.\n";
        }
        else
        {
            randomizeAndSaveArray();
            loadArr();
            SDL_Event e;// created SDL event
            bool quit = false;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0) // poll until all events are handled!
                                            // decide what to do with this event. 
                {
                    if (e.type == SDL_QUIT) //if event type is quit
                    {
                        quit = true;
                        complete = false;
                    }
                    else if (e.type == SDL_KEYDOWN) // if event is a key press
                    {
                        switch (e.key.keysym.sym)
                        {
                        case(SDLK_x):  // x for quiting
                            quit = true;
                            complete = false;
                            cout << "\nexiting visualise ......\n";
                            break;
                        case(SDLK_0):  // 0 for creating new array list
                            randomizeAndSaveArray();
                            complete = false;
                            loadArr();
                            cout << "\nrandom array generated\n";
                            break;
                        case(SDLK_1):  // bubble
                            loadArr();
                            cout << "\nbubble sort started.\n";
                            complete = false;
                            bubbleSort(delay);
                            complete = true;
                            cout << "bubble sort completed.\n";
                            break;
                        case(SDLK_2):  // quick
                            loadArr();
                            cout << "\nquick sort started.\n";
                            complete = false;
                            calculatequickSort(arr, 0, arraysize - 1);
                            complete = true;
                            cout << "quick sort complete.\n";
                            break;
                        case(SDLK_3): // selection
                            loadArr();
                            cout << "\nselection sort started\n";
                            complete = false;
                            selectionSort(delay);
                            complete = true;
                            cout << "selection sort completed\n";
                            break;
                        case(SDLK_4): //insertion
                            loadArr();
                            cout << "\ninsertion sort started.\n";
                            complete = false;
                            insertionSort(delay);
                            complete = true;
                            cout << "insertion sort completed\n";
                            break;
                        case(SDLK_5): //merge
                            loadArr();
                            cout << "\nmerge sort started\n";
                            complete = false;
                            calculateMergesort(arr, 0, arraysize - 1);
                            complete = true;
                            cout << "merge sort completed.\n";
                            break;
                        }
                    }
                }
                visualize(); /// this continues rendering of the window until it is not closed 
            }
            close();
        }
    }

    bool options()
    {

        cout << "next command should be provided after the first is complete.\n\n"
            << "Press the desired number:-\n"
            << "0 = generate a random array\n"
            << "1 = bubble sort\n"
            << "2 = quick sort\n"
            << "3 = selection sort\n"
            << "4 = insertion sort \n"
            << "5 = merge sort\n"
            << "x = exiting visulizer\n"
            << "-1 = Quit the program";

        string s;
        getline(cin, s);
        if (s == "-1")
        {
            return false;
        }
        return true;
    }

    void opening()
    {
        cout << "SORTING VISUALIZER\n\n"
            << "The list size is fixed to 65 numbers\n\n"
            << "Press ENTER to procces . . . . . .\n ";
        string s;
        getline(cin, s);
        if (s == "\n")
        {
            return;
        }
    }
};

//MAIN
int main(int argc, char* args[])
{
    visualfunctions visualizer;
    visualizer.opening();
    visualizer.delay = visualizer.Speed();

    while (1)
    {
        cout << '\n';
        if (visualizer.options())
            visualizer.executevisulization();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }
    return 0;
}

