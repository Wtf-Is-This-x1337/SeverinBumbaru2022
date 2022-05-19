#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace std::chrono;

// Folder
BOOL ExistaFolder(LPCSTR szPath) {
    DWORD dwAttrib = GetFileAttributesA(szPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// ! Variabile globale
bool challangemode;
int scorflappysalvat=0, i, nrjucatori, culoareconsola = 7, culoaresecundara = 7, dificultate; // Culoarea default a meniului este 7 (alb)
string textcolorat;
char numejucatori[10][30];

// X si 0
char patratelexsi0[10] = { 'o','1','2','3','4','5','6','7','8','9' };

// Logo
HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);

/* Lista culori pentru SetConsoleTextAttribute()
1: Blue
2: Green
3: Cyan
4: Red
5: Purple
6: Yellow (Dark)
7: Default white
8: Gray/Grey
9: Bright blue
10: Brigth green
11: Bright cyan
12: Bright red
13: Pink/Magenta
14: Yellow
15: Bright white
*/

void Logo(int culoare1, int culoare2) { // Afiseaza logo-ul pe ecran
    SetConsoleTextAttribute(consola, culoare1);
    // Prima jumatate a titlului
    cout << R"(           __  __              _   _                 ____      _      __  __  U _____ u ____
         U|' \/ '|u   ___     | \ |"|       ___   U /"___|uU  /"\  uU|' \/ '|u\| ___"|// __"| u
         \| |\/| |/  |_"_|   <|  \| |>     |_"_|  \| |  _ / \/ _ \/ \| |\/| |/ |  _|" <\___ \/
          | |  | |    | |    U| |\  |u      | |    | |_| |  / ___ \  | |  | |  | |___  u___) |   )";
    // A 2a jumatate a titlului
    SetConsoleTextAttribute(consola, culoare2);
    cout << R"(
          |_|  |_|  U/| |\u   |_| \_|     U/| |\u   \____| /_/   \_\ |_|  |_|  |_____| |____/>>
         <<,-,,-..-,_|___|_,-.||   \\,-.-,_|___|_,-._)(|_   \\    >><<,-,,-.   <<   >>  )(  (__)
          (./  \.)\_)-' '-(_/ (_")  (_/ \_)-' '-(_/(__)__) (__)  (__)(./  \.) (__) (__)(__)
)";
    SetConsoleTextAttribute(consola, 7); // Reseteaza culorile
}

void CitireDate() {
    ifstream fisiersetari("minigames.conf");
    fisiersetari >> challangemode >> nrjucatori;
    for (i = 1; i <= nrjucatori; i++)
        fisiersetari >> numejucatori[i];
    fisiersetari >> textcolorat;
    if (textcolorat == "true") {
        fisiersetari >> culoareconsola;
        switch (culoareconsola) {
        case 11:
            culoaresecundara = 3;
            break;
        case 12:
            culoaresecundara = 4;
            break;
        case 13:
            culoaresecundara = 5;
            break;
        case 14:
            culoaresecundara = 6;
            break;
        }
    }
    else
        culoaresecundara = culoareconsola = 7;
    fisiersetari >> dificultate;
    fisiersetari.close();
}

string LitereMici(string str) {
    for (auto& c : str)
        c = tolower(c);
    return str;
}

void CreateConfig() { // Creeaza config file-ul pentru a putea incepe jocul
    if (ExistaFolder("scoruri"))
        system("rmdir /s /q scoruri");
    CreateDirectoryA("scoruri", NULL);
    ofstream ConfigFile("minigames.conf");
    string optiuni;
    short int jucatori;
    Logo(13, 5);
    cout << "\n[";
    SetConsoleTextAttribute(consola, 13);
    cout << "+";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Mod Challange (Da / Nu) - ";
    SetConsoleTextAttribute(consola, 13);
    cin >> optiuni;
    optiuni = LitereMici(optiuni);
    if (optiuni == "da") ConfigFile << "0\n";
    else ConfigFile << "0\n";
    SetConsoleTextAttribute(consola, 7);
    cout << "\n[";
    SetConsoleTextAttribute(consola, 13);
    cout << "-";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Numar jucatori - ";
    SetConsoleTextAttribute(consola, 13);
    cin >> jucatori;
    SetConsoleTextAttribute(consola, 7);
    ConfigFile << jucatori << "\n"; // Scrie numarul de jucatori in fisierul creat
    for (short int i = 1; i <= jucatori; i++) {
        SetConsoleTextAttribute(consola, 13);
        cout << R"(    -> )";
        SetConsoleTextAttribute(consola, 7);
        cout << "Nume jucator " << i << " - ";
        SetConsoleTextAttribute(consola, 13);
        cin >> optiuni;
        SetConsoleTextAttribute(consola, 7);
        ConfigFile << optiuni << "\n"; // Scrie numele fiecarui jucator in fisierul creat
        // Creaza un fisier cu numele fiecarui jucator pentru scoruri
        string path = "scoruri/" + optiuni;
        CreateDirectoryA(path.c_str(), NULL);
        string xsi0path = path + "/xsi0.scor", flappybirdpath = path + "/flappybird.scor", snakepath = path + "/snake.scor";
        ofstream xsi0fisier(xsi0path);
        xsi0fisier << "0";
        xsi0fisier.close();
        ofstream flappybirdfisier(flappybirdpath);
        flappybirdfisier << "0";
        flappybirdfisier.close();
        ofstream snakefisier(snakepath);
        snakefisier << "0";
        snakefisier.close();
    }
    cout << "\n[";
    SetConsoleTextAttribute(consola, 13);
    cout << "-";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Text colorat (";
    SetConsoleTextAttribute(consola, 13);
    cout << "Da";
    SetConsoleTextAttribute(consola, 7);
    cout << " / Nu) - ";
    SetConsoleTextAttribute(consola, 13);
    cin >> optiuni;
    SetConsoleTextAttribute(consola, 7);
    optiuni = LitereMici(optiuni); // Elimina posibilele erori cauzate de "DA" si "Da"
    if (optiuni == "da") { // Toate cazurile
        ConfigFile << "true\n";
        string culoare;
        SetConsoleTextAttribute(consola, 13);
        cout << R"(    -> )";
        SetConsoleTextAttribute(consola, 7);
        cout << "Culoarea meniului (";
        SetConsoleTextAttribute(consola, 3);
        cout << "Albastru";
        SetConsoleTextAttribute(consola, 7);
        cout << ", ";
        SetConsoleTextAttribute(consola, 12);
        cout << "Rosu";
        SetConsoleTextAttribute(consola, 7);
        cout << ", ";
        SetConsoleTextAttribute(consola, 13);
        cout << "Magenta";
        SetConsoleTextAttribute(consola, 7);
        cout << " sau ";
        SetConsoleTextAttribute(consola, 14);
        cout << "Galben";
        SetConsoleTextAttribute(consola, 7);
        cout << ") - ";
        SetConsoleTextAttribute(consola, 13);
        cin >> optiuni;
        SetConsoleTextAttribute(consola, 7);
        optiuni = LitereMici(optiuni);
        if (optiuni == "albastru") ConfigFile << "11\n";
        else if (optiuni == "rosu") ConfigFile << "12\n";
        else if (optiuni == "galben") ConfigFile << "14\n";
        else ConfigFile << "13\n"; // In cazul unei erori de scriere, culoarea principala a meniului va fi "magenta"
    }
    else ConfigFile << "false\n";
    cout << "\n[";
    SetConsoleTextAttribute(consola, 13);
    cout << "-";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Dificultatea jocurilor (";
    SetConsoleTextAttribute(consola, 13);
    cout << "Usor";
    SetConsoleTextAttribute(consola, 7);
    cout << " / ";
    SetConsoleTextAttribute(consola, 13);
    cout << "Mediu";
    SetConsoleTextAttribute(consola, 7);
    cout << " / ";
    SetConsoleTextAttribute(consola, 13);
    cout << "Dificil";
    SetConsoleTextAttribute(consola, 7);
    cout << ") - ";
    SetConsoleTextAttribute(consola, 13);
    cin >> optiuni;
    SetConsoleTextAttribute(consola, 7);
    optiuni = LitereMici(optiuni);
    if (optiuni == "mediu") ConfigFile << "2";
    else if (optiuni == "dificil") ConfigFile << "3";
    else ConfigFile << "1"; // In cazul unei erori de scriere, dificultatea va fi pusa pe "usor"
    ConfigFile.close();
}

// Functii x si 0
int verificaxsi0()
{
    if (patratelexsi0[1] == patratelexsi0[2] && patratelexsi0[2] == patratelexsi0[3]) return 1;
    else if (patratelexsi0[4] == patratelexsi0[5] && patratelexsi0[5] == patratelexsi0[6]) return 1;
    else if (patratelexsi0[7] == patratelexsi0[8] && patratelexsi0[8] == patratelexsi0[9]) return 1;
    else if (patratelexsi0[1] == patratelexsi0[4] && patratelexsi0[4] == patratelexsi0[7]) return 1;
    else if (patratelexsi0[2] == patratelexsi0[5] && patratelexsi0[5] == patratelexsi0[8]) return 1;
    else if (patratelexsi0[3] == patratelexsi0[6] && patratelexsi0[6] == patratelexsi0[9]) return 1;
    else if (patratelexsi0[1] == patratelexsi0[5] && patratelexsi0[5] == patratelexsi0[9]) return 1;
    else if (patratelexsi0[3] == patratelexsi0[5] && patratelexsi0[5] == patratelexsi0[7]) return 1;
    else if (patratelexsi0[1] != '1' && patratelexsi0[2] != '2' && patratelexsi0[3] != '3'
        && patratelexsi0[4] != '4' && patratelexsi0[5] != '5' && patratelexsi0[6] != '6'
        && patratelexsi0[7] != '7' && patratelexsi0[8] != '8' && patratelexsi0[9] != '9')
        return 0;
    else
        return -1;
}

void tablaxsi0(string jucator1, string jucator2) {
    system("cls");
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << R"(                  __  __        ____                       ___
                  \ \/"/       / __"| u      ___          / _"\  u
                  /\  /\      <\___ \/      |_"_|        | / U |/
                 U /  \ u      u___) |       | |         | \// |,-. )";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << R"(
                  /_/\_\       |____/>>    U/| |\u        \___/(_/
                ,-,>> \\_       )(  (__).-,_|___|_,-.      //
                 \_)  (__)     (__)      \_)-' '-(_/      (__)

)";
    SetConsoleTextAttribute(consola, 7);
    cout << "Player 1 (";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << jucator1;
    SetConsoleTextAttribute(consola, 7);
    cout << ") (X) - Player 2 (";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << jucator2;
    SetConsoleTextAttribute(consola, 7);
    cout << ") (O)\n";
    cout << "\t      |     |     " << "\n";
    cout << "\t   " << patratelexsi0[1] << "  |  " << patratelexsi0[2] << "  |  " << patratelexsi0[3] << "\n";
    cout << "\t _____|_____|_____\n";
    cout << "\t      |     |     \n";
    cout << "\t   " << patratelexsi0[4] << "  |  " << patratelexsi0[5] << "  |  " << patratelexsi0[6] << "\n";
    cout << "\t _____|_____|_____\n";
    cout << "\t      |     |     \n";
    cout << "\t   " << patratelexsi0[7] << "  |  " << patratelexsi0[8] << "  |  " << patratelexsi0[9] << "\n";
    cout << "\t      |     |     \n\n";
}

// Variabile flappybird
char c[30][21]; // pentru pixeli
int n[30][21];  // pentru verificare
int contr, tuk = 0, scorflappyjoc = 0, t = 0, bt = 0, birdx = 0, birdy = 0;
bool err; // pentru erori

// Functii flappybird
void stalpi() // functie pentru generarea stalpilor
{
    int x, y, r;
    if (t == 10)   // daca am trecut prin loop de 10 ori generam un nou pipe
    {
        r = (rand() % 11) + 5;  // gaura din mijlocul stalpului
        for (y = 0; y < 20; y++) 
        {
            c[29][y] = '|';  // creeaza stalpul
            n[29][y] = 2;    // n=2 pt a verifica daca pasarea l a lovit sau nu
        }
        c[29][r - 1] = ' ';  // gaura
        c[29][r] = ' ';
        c[29][r + 1] = ' ';
        n[29][r - 1] = 0;
        n[29][r] = 0;
        n[29][r + 1] = 0;
        t = 0;
        goto mv; // misca stalpul
    }
    else goto mv;
mv:
    {
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '|')
                {
                    if (x > 0)
                    {
                        c[x - 1][y] = '|';
                        n[x - 1][y] = 2;
                        c[x][y] = ' ';
                        n[x][y] = 0;
                    }
                    if (x == 0)  // daca iese din afisare (x=0) stalpul va disparea
                    {
                        c[x][y] = ' ';
                        n[x][y] = 0;
                    }
                }
            }
        }
    }
}

void afisareflappy(string jucator)
{
    int x, y;
    system("cls");
    for (y = 0; y < 21; y++)
    {
        for (x = 0; x < 30; x++)
        {
            if (x < 29) cout << c[x][y];
            if (x == 29) cout << c[x][y] << "\n";
        }
    }
    cout << "\n";
    cout << "Jucator: ";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << jucator << "\n";
    SetConsoleTextAttribute(consola, 7);
    cout << "Scor: ";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << scorflappyjoc;
    SetConsoleTextAttribute(consola, 7);
}

bool gameover()  // verifica daca a lovit ceva
{
    int x, y, f = 0;
    if (birdy > 19)
    {
        c[birdx][19] = '*';
        c[birdx][20] = '-';
        f = 1;
        goto quit;
    }
    else
    {     // verifica daca pasarea a lovit vreun stalp, aici avem nevoie de matricea n (coordonatele stalpului)
        if (n[birdx][birdy] > 0 && (c[birdx][birdy] == '|' || c[birdx][birdy] == '*'))
        {
            c[birdx][birdy] = '|';
            c[birdx - 1][19] = '*';
            f = 1;
            goto quit;
        }
    }
quit:
    if (f == 1) return true;
    else return false;
}

void pasare()
{
    int x, y;
    if (tuk > 0) // daca vreo tasta este apasata
    {
        bt = 0;
        for (y = 0; y < 20; y++) // loop pentru coordonate
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                    if (y > 0)
                    {
                        c[x][y - 1] = '*';  // pasarea se misca cu 1px;
                        c[x][y] = ' ';
                        birdx = x;        // seteaza coordonata x
                        birdy = y - 1;      // seteaza coordonata y
                        return;         // retuns to game func
                    }
            }
        }
    }
    else // Daca nicio tasta nu este apasata, pasarea cade
    {
        bt++;
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == '*')
                {
                    if (y < 20)  //if bird is not on the ground
                    {
                        if (bt < 3)   //if bird time is lower that 3, it falls 1 pixel
                        {
                            c[x][y + 1] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 1;
                            return;
                        }
                        else if (bt > 2 && bt < 5)  //more time has passed, faster bird falls (acceleration)
                        {
                            c[x][y + 2] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 2;
                            return;
                        }
                        else if (bt > 4)
                        {
                            c[x][y + 3] = '*';
                            c[x][y] = ' ';
                            birdx = x;
                            birdy = y + 3;
                            return;
                        }
                    }
                    else return; // daca pasarea este deja pe pamant, verifica starea jocului
                }
            }
        }
    }
}

void verificascor()  // verifica daca a crescut scorul
{
    int y;
    for (y = 0; y < 20; y++)
    {
        if (c[birdx][y] == '|')  //if bird x coord is equal to pipe's coord, you get 1 point
        {
            scorflappyjoc++;
            return;
        }
    }
}

void game(string jucator)  //function for playing game
{
    int x, y;
    char s;
    for (y = 0; y < 21; y++)  //setting afisareflappy
    {
        for (x = 0; x < 30; x++)
        {
            if (y < 20)
            {
                c[x][y] = ' ';
                n[x][y] = 0;
            }
            if (y == 20)
            {
                c[x][y] = '-';
                n[x][y] = 2;
            }
        }
    }
    c[10][10] = '*'; // pasarea
    afisareflappy(jucator);
    while (1)
    {
        s = '~';
        Sleep(0.2 * 500);  // cat de rapid se misca jocul
        t++; // counter
        if (_kbhit())
        {
            s = _getch();
            if (s != '~') tuk = 1;
        }
        for (x = 0; x < 30; x++) // pamantul
        {
            c[x][20] = '-';
            n[x][20] = 2;
        }
        pasare();
        verificascor(); // verifica scorul
        if (gameover() == true) goto gameEnd;   //checks if bird hits pipes, if yes, game ends
        stalpi(); // creeaza pipe uri
        afisareflappy(jucator);
        if (tuk > 0) tuk++;           //if key is pressed, bird will fly up 2 times.
        if (tuk == 3) tuk = 0;          //after that, bird falls
    }
gameEnd:   // sfarsit
    {
        afisareflappy(jucator);
        return;
    }
}

// Functii snake
bool snaketerminat;
const int latime = 20;
const int inaltime = 20;
int x, y, punctX, punctY, scorsnakejoc;
int CoadaX[100], CoadaY[100];
int nCoada;
enum Directii { STOP = 0, LEFT, RIGHT, UP, DOWN };
Directii dir;

void Afiseaza(string jucator)
{
    system("cls");
    for (int i = 0; i < latime + 2; i++)
        cout << "#";
    cout << "\n";

    for (int i = 0; i < inaltime; i++)
    {
        for (int j = 0; j < latime; j++)
        {
            if (j == 0) cout << "#";
            if (i == y && j == x) cout << "O";
            else if (i == punctY && j == punctX) {
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << "W";
                SetConsoleTextAttribute(consola, 7);
            }
            else
            {
                bool print = false;
                for (int k = 0; k < nCoada; k++)
                {
                    if (CoadaX[k] == j && CoadaY[k] == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }
            if (j == latime - 1) cout << "#";
        }
        cout << "\n";
    }
    for (int i = 0; i < latime + 2; i++)
        cout << "#";
    cout << "\n\nJucator: ";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << jucator;
    SetConsoleTextAttribute(consola, 7);
    cout << "\nScor: ";
    SetConsoleTextAttribute(consola, culoareconsola);
    cout << scorsnakejoc << "\n";
    SetConsoleTextAttribute(consola, 7);
}

void Logica()
{
    int prevX = CoadaX[0];
    int prevY = CoadaY[0];
    int prev2X, prev2Y;
    CoadaX[0] = x;
    CoadaY[0] = y;
    for (int i = 1; i < nCoada; i++)
    {
        prev2X = CoadaX[i];
        prev2Y = CoadaY[i];
        CoadaX[i] = prevX;
        CoadaY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= latime) x = 0; else if (x < 0) x = latime - 1;
    if (y >= inaltime) y = 0; else if (y < 0) y = inaltime - 1;

    for (int i = 0; i < nCoada; i++)
        if (CoadaX[i] == x && CoadaY[i] == y)
            snaketerminat = true;

    if (x == punctX && y == punctY)
    {
        scorsnakejoc += 10;
        punctX = rand() % latime;
        punctY = rand() % inaltime;
        nCoada++;
    }
}

void Meniu(int culoareconsola, int culoaresecundara) {
    system("cls"); // Sterge consola
    Logo(culoareconsola, culoaresecundara); // Afiseaza un nou logo cu tema din config
    cout << "\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "1";
    SetConsoleTextAttribute(consola, 7);
    cout << "] X si 0\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "2";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Flappy Bird\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "3";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Snake\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "4";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Spaceship Battle\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "5";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Reconfigurare setari\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << "6";
    SetConsoleTextAttribute(consola, 7);
    cout << "] Afisare scoruri\n\n[";
    SetConsoleTextAttribute(consola, culoaresecundara);
    cout << ">";
    SetConsoleTextAttribute(consola, 7);
    cout << "] ";
    unsigned short int alegere;
    cin >> alegere;
    if (challangemode) {
        bool joc2 = false, joc3 = false, joc4 = false;
        if (alegere == 2 && !joc2)
            cout << "[!] Flappy Bird nu a fost deblocat, castiga in X si 0 prima data!";
        if (alegere == 3 && !joc3)
            cout << "[!] Snake nu a fost deblocat, acumuleaza 10 puncte in Flappy Bird prima data!";
        if (alegere == 4 && !joc4)
            cout << "[!] SpaceshipBattle nu a fost deblocat, acumuleaza 100 de puncte in Snake prima data!";
    }
    switch (alegere) {
    case 1: {
        system("cls");
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << R"(                  __  __        ____                       ___
                  \ \/"/       / __"| u      ___          / _"\  u
                  /\  /\      <\___ \/      |_"_|        | / U |/
                 U /  \ u      u___) |       | |         | \// |,-. )";
        SetConsoleTextAttribute(consola, culoaresecundara);
        cout << R"(
                  /_/\_\       |____/>>    U/| |\u        \___/(_/
                ,-,>> \\_       )(  (__).-,_|___|_,-.      //
                 \_)  (__)     (__)      \_)-' '-(_/      (__)

)";
        SetConsoleTextAttribute(consola, 7);
        bool al2leajucatoresteinregistrat = true;
        string jucator1 = numejucatori[1], jucator2 = numejucatori[2];
        int alegerexsi0;
        // Afla numele jucatorului nr 2 in cazul in care este trecut un singur nume
        if (nrjucatori == 1) {
            al2leajucatoresteinregistrat = false;
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] X si 0 trebuie jucat alaturi de un prieten!\n\nIntrodu numele jucatorului numarul 2: ";
            SetConsoleTextAttribute(consola, culoareconsola);
            cin >> jucator2;
            SetConsoleTextAttribute(consola, 7);
        }
        // Afla numele jucatorilor in cazul in care sunt mai multe nume trecute in config file
        else if (nrjucatori > 2) {
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] X si 0 trebuie jucat alaturi un prieten!\n\nAlege jucatorul numarul 1:";
            for (int k = 1; k <= nrjucatori; k++) {
                cout << "\n[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << k;
                SetConsoleTextAttribute(consola, 7);
                cout << "] " << numejucatori[k];
            }
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            cin >> alegerexsi0;
            jucator1 = numejucatori[alegerexsi0];
            cout << "\nPrimul jucator a fost selectat cu succes! Alege jucatorul numarul 2:\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            cin >> alegerexsi0;
            jucator2 = numejucatori[alegerexsi0];
        }
        // X si 0
        int activxsi0 = 1, verificarexsi0;
        char marcarexsi0;
        do
        {
            tablaxsi0(jucator1, jucator2);
            if (activxsi0 % 2) {
                activxsi0 = 1;
                cout << "Jucatorul 1 (";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << jucator1;
                SetConsoleTextAttribute(consola, 7);
                cout << "), alege un numar:  ";
            }
            else {
                activxsi0 = 2;
                cout << "Jucatorul 2 (";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << jucator2;
                SetConsoleTextAttribute(consola, 7);
                cout << "), alege un numar:  ";
            }
            cin >> alegerexsi0;

            marcarexsi0 = (activxsi0 == 1) ? 'X' : 'O';

            if (alegerexsi0 == 1 && patratelexsi0[1] == '1') patratelexsi0[1] = marcarexsi0;
            else if (alegerexsi0 == 2 && patratelexsi0[2] == '2') patratelexsi0[2] = marcarexsi0;
            else if (alegerexsi0 == 3 && patratelexsi0[3] == '3') patratelexsi0[3] = marcarexsi0;
            else if (alegerexsi0 == 4 && patratelexsi0[4] == '4') patratelexsi0[4] = marcarexsi0;
            else if (alegerexsi0 == 5 && patratelexsi0[5] == '5') patratelexsi0[5] = marcarexsi0;
            else if (alegerexsi0 == 6 && patratelexsi0[6] == '6') patratelexsi0[6] = marcarexsi0;
            else if (alegerexsi0 == 7 && patratelexsi0[7] == '7') patratelexsi0[7] = marcarexsi0;
            else if (alegerexsi0 == 8 && patratelexsi0[8] == '8') patratelexsi0[8] = marcarexsi0;
            else if (alegerexsi0 == 9 && patratelexsi0[9] == '9') patratelexsi0[9] = marcarexsi0;
            else
            {
                cout << "[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << "!";
                SetConsoleTextAttribute(consola, 7);
                cout << "] Mutare interzisa! Apasa Enter pentru a reveni la joc.";
                activxsi0--;
                cin.ignore();
                cin.get();
            }
            verificarexsi0 = verificaxsi0();

            activxsi0++;
        } while (verificarexsi0 == -1);
        tablaxsi0(jucator1, jucator2);
        if (verificarexsi0 == 1) {
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            SetConsoleTextAttribute(consola, culoareconsola);
            int scorxsi0;
            if (--activxsi0 == 1) {
                // +1 pentru jucator1
                string highscor1 = "scoruri/" + jucator1 + "/xsi0.scor";
                ifstream citirexsi0(highscor1);
                citirexsi0 >> scorxsi0;
                scorxsi0++;
                citirexsi0.close();
                ofstream scriexsi0(highscor1);
                scriexsi0 << scorxsi0;
                scriexsi0.close();
                // Afisare castigator
                cout << jucator1;
            }
            else {
                // +1 pentru jucator2
                if (al2leajucatoresteinregistrat) {
                    string highscor2 = "scoruri/" + jucator2 + "/xsi0.scor";
                    ifstream citirexsi0(highscor2);
                    citirexsi0 >> scorxsi0;
                    scorxsi0++;
                    citirexsi0.close();
                    ofstream scriexsi0(highscor2);
                    scriexsi0 << scorxsi0;
                    scriexsi0.close();
                }
                // Afisare castigator
                cout << jucator2;
            }
            SetConsoleTextAttribute(consola, 7);
            cout << " a castigat! Felicitari!\n";
        }
        else {
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Remiza!\n";

        }
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Apasa Enter pentru a reveni la meniul principal.";
        cin.ignore();
        cin.get();
        break;
    }
    case 2: {
        system("cls");
        int alegereflappy;
        scorflappyjoc = 0;
        string jucator = numejucatori[1];
        if (nrjucatori > 1) {
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << R"(              _____    _         _       ____     ____   __   __       ____               ____     ____    
	     |" ___|  |"|    U  /"\  u U|  _"\ uU|  _"\ u\ \ / /    U | __")u    ___   U |  _"\ u |  _"\   
	    U| |_  uU | | u   \/ _ \/  \| |_) |/\| |_) |/ \ V /      \|  _ \/   |_"_|   \| |_) |//| | | |  
	    \|  _|/  \| |/__  / ___ \   |  __/   |  __/  U_|"|_u      | |_) |    | |     |  _ <  U| |_| |\ )";
            SetConsoleTextAttribute(consola, culoaresecundara);
            cout << R"(
	     |_|      |_____|/_/   \_\  |_|      |_|       |_|        |____/   U/| |\u   |_| \_\  |____/ u 
	     )(\\,-   //  \\  \\    >>  ||>>_    ||>>_ .-,//|(_      _|| \\_.-,_|___|_,-.//   \\_  |||_    
	    (__)(_/  (_")("_)(__)  (__)(__)__)  (__)__) \_) (__)    (__) (__)\_)-' '-(_/(__)  (__)(__)_)   

)";
            SetConsoleTextAttribute(consola, 7);
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Flappy Bird nu poate fi jucat in 2 / mai multi playeri!\n\nAlege un singur jucator:";
            for (int k = 1; k <= nrjucatori; k++) {
                cout << "\n[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << k;
                SetConsoleTextAttribute(consola, 7);
                cout << "] " << numejucatori[k];
            }
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            cin >> alegereflappy;
            jucator = numejucatori[alegereflappy];
        }
        // Flappy Bird
        srand(time(0)); // Ajuta la randomizarea numerelor
        game(jucator);
        // Highscore
        int scorflappy = 0;
        string flappyhighscore = "scoruri/" + jucator + "/flappybird.scor";
        ifstream citireflappy(flappyhighscore);
        citireflappy >> scorflappy;
        citireflappy.close();
        if (scorflappyjoc > scorflappy) {
            // Highscore nou pentru jucatorul respectiv
            ofstream scrieflappy(flappyhighscore);
            scrieflappy << scorflappyjoc;
            scrieflappy.close();
        }
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "!";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Ai pierdut cu un scor total de: ";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << scorflappyjoc;
        SetConsoleTextAttribute(consola, 7);
        cout << " puncte!";
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Apasa Enter pentru a reveni la meniul principal.";
        cin.ignore();
        cin.get();
        break;
    }
    case 3: {
        // Snake
        system("cls");
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << R"(          ____     _   _       _       _  __  U _____ u
         / __"| u | \ |"|  U  /"\  u  |"|/ /  \| ___"|/
        <\___ \/ <|  \| |>  \/ _ \/   | ' /    |  _|"
         u___) | U| |\  |u  / ___ \ U/| . \\u  | |___   )";
        SetConsoleTextAttribute(consola, culoaresecundara);
        cout << R"(
         |____/>> |_| \_|  /_/   \_\  |_|\_\   |_____|
          )(  (__)||   \\,-.\\    >>,-,>> \\,-.<<   >>
         (__)     (_")  (_/(__)  (__)\.)   (_/(__) (__)

)";
        SetConsoleTextAttribute(consola, 7);
        int alegeresnake;
        string jucator = numejucatori[1];
        if (nrjucatori > 1) {
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Snake nu poate fi jucat in 2 / mai multi playeri!\n\nAlege un singur jucator:";
            for (int k = 1; k <= nrjucatori; k++) {
                cout << "\n[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << k;
                SetConsoleTextAttribute(consola, 7);
                cout << "] " << numejucatori[k];
            }
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            cin >> alegeresnake;
            jucator = numejucatori[alegeresnake];
        }
        // Setup-ul
        memset(CoadaX, 0, sizeof(CoadaX)); // resetam tot vectorul CoadaX
        memset(CoadaY, 0, sizeof(CoadaY)); // resetam tot vectorul CoadaY
        nCoada = 0;
        snaketerminat = false;
        dir = STOP;
        x = latime / 2;
        y = inaltime / 2;
        punctX = rand() % latime;
        punctY = rand() % inaltime;
        scorsnakejoc = 0;
        while (!snaketerminat)
        {
            Afiseaza(jucator);
            if (_kbhit())
            {
                switch (_getch())
                {
                case 'a':
                    dir = LEFT;
                    break;
                case 'd':
                    dir = RIGHT;
                    break;
                case 'w':
                    dir = UP;
                    break;
                case 's':
                    dir = DOWN;
                    break;
                case 'x':
                    snaketerminat = true;
                    break;
                }
            }
            Logica();
            Sleep(10);
        }
        // Highscore
        int scorsnake = 0;
        string highscore = "scoruri/" + jucator + "/snake.scor";
        ifstream citirsnake(highscore);
        citirsnake >> scorsnake;
        citirsnake.close();
        if (scorsnakejoc > scorsnake) {
            // Highscore nou pentru jucatorul respectiv
            ofstream scriesnake(highscore);
            scriesnake << scorsnakejoc;
            scriesnake.close();
        }
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "!";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Ai pierdut cu un scor total de: ";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << scorsnakejoc;
        SetConsoleTextAttribute(consola, 7);
        cout << " puncte!";
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Apasa Enter pentru a reveni la meniul principal.";
        cin.ignore();
        cin.get();
        break;
    }
    case 4: {
        // SpaceshipBattle
        system("cls");
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << R"(		  ____     ____   U _____ u   ____                _       _      
		 / __"| uU|  _"\ u\| ___"|/U /"___|    ___    U  /"\  u  |"|     
		<\___ \/ \| |_) |/ |  _|"  \| | u     |_"_|    \/ _ \/ U | | u   
		 u___) |  |  __/   | |___   | |/__     | |     / ___ \  \| |/__  )";
        SetConsoleTextAttribute(consola, culoaresecundara);
        cout << R"(
		 |____/>> |_|      |_____|   \____|  U/| |\u  /_/   \_\  |_____| 
		  )(  (__)||>>_    <<   >>  _// \\.-,_|___|_,-.\\    >>  //  \\  
		 (__)    (__)__)  (__) (__)(__)(__)\_)-' '-(_/(__)  (__)(_")("_) 

)";
        SetConsoleTextAttribute(consola, 7);
        int alegereSpaceshipBattle;
        string jucator = numejucatori[1];
        if (nrjucatori > 1) {
            cout << "[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Spaceship Battle nu poate fi jucat in 2 / mai multi playeri!\n\nAlege un singur jucator:";
            for (int k = 1; k <= nrjucatori; k++) {
                cout << "\n[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << k;
                SetConsoleTextAttribute(consola, 7);
                cout << "] " << numejucatori[k];
            }
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            cin >> alegereSpaceshipBattle;
            jucator = numejucatori[alegereSpaceshipBattle];
        }
        break;
    }
    case 5: {
        // Reconfigurare setari
        system("cls");
        // Sterge continutul fisierului minigames.conf
        std::ofstream stergere("minigames.conf", std::ofstream::out | std::ofstream::trunc);
        stergere.close();
        CreateConfig(); // Creeaza un fisier nou
        cout << "\n\n[";
        SetConsoleTextAttribute(consola, 13);
        cout << "!";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Setarile au fost salvate - Jocul va reveni la meniul principal in 3 secunde";
        CitireDate();
        Sleep(940);
        cout << ".";
        Sleep(940);
        cout << ".";
        Sleep(900);
        cout << ".";
        Sleep(150);
        Meniu(culoareconsola, culoaresecundara);
        break;
    }
    case 6: {
        // Afisare scoruri
        if (ExistaFolder("scoruri")) // Verifica existenta fisierului "./scoruri.conf" si marimea acestuia
        {
            system("cls");
            Logo(culoareconsola, culoaresecundara);
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Poti verifica scorurile pentru urmatorii jucatori:\n";
            for (int k = 1; k <= nrjucatori; k++) {
                cout << "\n[";
                SetConsoleTextAttribute(consola, culoareconsola);
                cout << k;
                SetConsoleTextAttribute(consola, 7);
                cout << "] " << numejucatori[k];
            }
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << ">";
            SetConsoleTextAttribute(consola, 7);
            cout << "] ";
            int numehighscore;
            cin >> numehighscore;
            system("cls");
            SetConsoleTextAttribute(consola, culoareconsola);
            cout << R"(          _   _               ____   _   _    ____      ____   U  ___ u   ____    U _____ u ____     
         |'| |'|     ___   U /"___|u|'| |'|  / __"| uU /"___|   \/"_ \/U |  _"\ u \| ___"|// __"| u  
        /| |_| |\   |_"_|  \| |  _ /| |_| |\<\___ \/ \| | u     | | | | \| |_) |/  |  _|" <\___ \/   
        U|  _  |u    | |    | |_| |U|  _  |u u___) |  | |/__.-,_| |_| |  |  _ <    | |___  u___) |   )";
            SetConsoleTextAttribute(consola, culoaresecundara);
            cout << R"(
         |_| |_|   U/| |\u   \____| |_| |_|  |____/>>  \____|\_)-\___/   |_| \_\   |_____| |____/>>  
         //   \\.-,_|___|_,-._)(|_  //   \\   )(  (__)_// \\      \\     //   \\_  <<   >>  )(  (__) 
        (_") ("_)\_)-' '-(_/(__)__)(_") ("_) (__)    (__)(__)    (__)   (__)  (__)(__) (__)(__)      

)";
        SetConsoleTextAttribute(consola, 7);
        string rezultate, jucator = numejucatori[numehighscore], xsi0path = "scoruri/" + jucator + "/xsi0.scor", flappybirdpath = "scoruri/" + jucator + "/flappybird.scor", snakepath = "scoruri/" + jucator + "/snake.scor";
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "!";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Jucator: ";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << jucator;
        SetConsoleTextAttribute(consola, 7);
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Jocuri castigate de X si 0: ";
        SetConsoleTextAttribute(consola, culoareconsola);
        ifstream xsi0fisier(xsi0path);
        xsi0fisier >> rezultate;
        cout << rezultate;
        xsi0fisier.close();
        SetConsoleTextAttribute(consola, 7);
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Cel mai mare scor pe Flappy Bird: ";
        SetConsoleTextAttribute(consola, culoareconsola);
        ifstream flappybirdfisier(flappybirdpath);
        flappybirdfisier >> rezultate;
        cout << rezultate;
        flappybirdfisier.close();
        SetConsoleTextAttribute(consola, 7);
        cout << " puncte\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Cel mai mare scor de pe Snake : ";
        SetConsoleTextAttribute(consola, culoareconsola);
        ifstream snakefisier(snakepath);
        snakefisier >> rezultate;
        cout << rezultate;
        snakefisier.close();
        SetConsoleTextAttribute(consola, 7);
        cout << "\n\n[";
        SetConsoleTextAttribute(consola, culoareconsola);
        cout << "+";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Apasa Enter pentru a reveni la meniul principal.";
        cin.ignore();
        cin.get();
        }
        else {
            // Niciun folder nu a fost gasit || fisierul este gol
            cout << "\n[";
            SetConsoleTextAttribute(consola, culoaresecundara);
            cout << "!";
            SetConsoleTextAttribute(consola, 7);
            cout << "] Nu exista niciun highscore salvat - Jocul va reveni la meniul principal in 3 secunde";
            Sleep(950);
            cout << ".";
            Sleep(950);
            cout << ".";
            Sleep(900);
            cout << ".";
            Sleep(150);
            Meniu(culoareconsola, culoaresecundara);
        }
        break;
    }
    default: {
        cout << "\n[";
        SetConsoleTextAttribute(consola, culoaresecundara);
        cout << "!";
        SetConsoleTextAttribute(consola, 7);
        cout << "] Selectie invalida - Jocul va reveni la meniul principal in 3 secunde";
        Sleep(950);
        cout << ".";
        Sleep(950);
        cout << ".";
        Sleep(900);
        cout << ".";
        Sleep(150);
        Meniu(culoareconsola, culoaresecundara);
        break;
    }
    }
    Meniu(culoareconsola, culoaresecundara);
}

int main() {
    ifstream config("minigames.conf", std::ifstream::ate | std::ifstream::binary);
    if (!config.good()) { // Verifica existenta fisierului "./minigames.conf"
        config.close();
        // Daca acesta nu exista, creeaza un config nou, apeland functia CreateConfig()
        CreateConfig();
    }
    else { // Verifica daca fisierul "minigames.conf" nu este corupt / gol
        unsigned int filesize = config.tellg();
        config.close();
        if (filesize == 0) CreateConfig(); // Marimea fisierului este de 0 bytes => Creeaza un config file nou
    }

    // Citeste datele din config
    CitireDate();
    Meniu(culoareconsola, culoaresecundara);
    system("pause");
    return 0;
}