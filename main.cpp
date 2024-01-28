#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<algorithm>
#include <cstdlib>

using namespace std;

const int word_length = 5;
const int number_of_guess = 6;
const int Not_Matched = 0;
const int Parital_Matched = 2;
const int Matched = 1;



string getword(){
   std::string url = "https://random-word-api.herokuapp.com/word?length=5";

    // Make an HTTP GET request using the system command
    std::string command = "curl -s \"" + url + "\"";  

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing command." << std::endl;
        return "N";
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

string newword(){
    string myword = getword();
    string bestword;
    for(auto i : myword){
        if(i != 34 && i != 91 && i != 93){
            bestword += i;
        }
    }
    return "kappa";
}


void toupperCase(string &word){
   for (char &c : word) {
       // Convert each character to uppercase
       c = toupper(static_cast<unsigned char>(c));
   }
}

void setTextColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void printword(vector<vector<int>> &status,vector<string> Guess, int currentguess){
   
    system("cls");

   cout<<"---------------------------------------------------------"<<endl;
   cout<<"//                                  _   _              //"<<endl; 
   cout<<"//                                 | | | |             //"<<endl; 
   cout<<"//                                 | | | |             //"<<endl; 
   cout<<"//   __      __   ___    _ __    __| | | |   ___       //"<<endl; 
   cout<<"//   \\ \\ /\\ / /  / _ \\  | '__|  / _` | | |  / _ \\      //"<<endl; 
   cout<<"//    \\ V  V /  | (_) | | |    | (_| | | | |  __/      //"<<endl; 
   cout<<"//     \\_/\\_/    \\___/  |_|     \\__,_| |_|  \\___|      //"<<endl;
   cout<<"---------------------------------------------------------"<<endl;

   for(int i = 0 ; i<=currentguess && i<Guess.size();i++){
      string seprater = "-";
      string padding = "|";
      string text = "|";
        for(int j=0;j<Guess[i].size();j++){
            seprater += "------";
            padding += "     |";
            char ch = (Guess[i][j]);
            text += "  ";
            if(status[i][j] == 2){
                text += "\033[33m";
            }
            else if(status[i][j] == 1){
                text += "\033[32m";
            }
            text += ch;
    
            if(status[i][j] == 2 || status[i][j] == 1){
                text+="\033[0m";
            }
            text +="  |";
    
        }
    if( currentguess == 0){
        cout<<seprater<<endl;
    }
    cout<<padding<<endl;
    cout<<text<<endl;
    cout<<padding<<endl;
    cout<<seprater<<endl;
   }

}

bool isvalidword(string &word){
    return word.length() == word_length; 
}

void markstatus(vector<vector<int>> &status, int currentguess,string targetword,string input){
    for(int i=0;i<input.size();i++){
        status[currentguess][i] = 0;
    }
    vector<int> visited(word_length,0);
    for(int i=0;i<input.size();i++){
        int cnt = 0;
        for(int j=0;j<targetword.size();j++){
            if(input[i] == targetword[j] ){
                if (i!=j && visited[j] != 1 ){
                    status[currentguess][i] = 2;
                    if(cnt == 0){
                      visited[j] = 1;
                      cnt++;
                    }  
                }
                if(i==j && visited[j] != 1 ){
                    status[currentguess][i] = 1;
                    visited[j] = 1;
                    break;
                }
            }
        }
    }
}

bool isAllMatched(string input, string targetword){
    for(int i=0;i<word_length;i++){
        if(input[i] != targetword[i]){
            return false;
        }
    }
    return true;
}

int main(){
    vector<string> Guess(number_of_guess);
    vector<vector<int>> status(number_of_guess,vector<int>(word_length));
    string targetword = newword();
    bool flag = true;
    toupperCase(targetword);
    if(targetword == ""){
        flag = false;
    }
    printword(status,Guess,-1);

    string input ;
    int currentguess = 0;
    while(currentguess < number_of_guess){
        if(flag == false){
            cout<<"Unable to establish a connection to the internet. Please check your network settings and try again."<<endl;
            break;
        }
        do{
            cout << "Take a guess! (Word length should be " + to_string(word_length) + ") or type Q to quit: ";
            getline(cin,input);
            toupperCase(input);

        }while(input != "Q"  && !isvalidword(input));
   
        if(input == "Q"){
            cout<<endl;
            cout << "The target word is: " << "\033[32m" << targetword <<"\033[0m"<< endl;
            cout << "Thank you for playing. Exiting the game." << endl;
            break;
        }
        
        //need to code for if input is not alphabet  
        Guess[currentguess] = input;
        // need to make funtion to check is world matching or not or partial matched
        markstatus(status,currentguess,targetword,input);

        printword(status,Guess,currentguess);

        if(isAllMatched(input,targetword)){

            cout << "Congratulations! You've completed the guess. Well done!" << endl;
            break;
        }
        // print word nicely and by colur
        currentguess++;

    }
        if(currentguess == number_of_guess){
            cout << "The target word is: " << "\033[32m" << targetword <<"\033[0m"<< endl;
            cout<< "The maximum number of guesses has been reached" <<endl;
        }
    return 0;
}
