  #include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace fs=std::filesystem;
using namespace std;

//FUTURE UPDATES:
//lxde format reference
//TERM environment reading
//arg[0]@::note; folder handling and implementation
//implement current file directory for shell
//add colored output and formatting
//arg[0]@::note; implement endl on input;


//ISSUES:
//1.Proper error handling


//CURRENT UPDATES& FIXES:
//arg[0]@::refresh; updated cmd, refreshes program to match changes in code, also fixes issue_2 
//arg[0]@::run; updated cmd, now compiles and runs file in one command, also fixes issue_2;

vector<string> run(vector<string>arg);
vector<string> write(vector<string>arg);
vector<string> note(vector<string>arg);
vector<string> refresh(vector<string>arg);


int main(){
    system("clear");

    string clear;

    const char* termEnv = getenv("TERM");
    if(termEnv != nullptr){
        cout<<"[!]Terminal::"<<termEnv << endl;
        if(string(termEnv) == "xterm-256color"){
            clear = "clear";
        }
        else{
            clear = "cls";
        }
    }
    cout<<"\033[38;2;255;244;79m[-LemonShell-]\033[1m"<<endl;
    bool stop=false;
    while(!stop){
        string input;
        string inputToken;
        vector<string> inputSyntaxContainer;
        map<string, function<vector<string>(vector<string>)>> cmdList={
            {"run",run},
            {"write",write},
            {"note",note},
            {"refresh",refresh}
        };

        cout<<"\n[?]:";
        getline(cin,input);

        if(input.empty()||input.back()!=';'){
                cout<<"[!]i:syntax_error"<<endl;
            }
            else{
                //tokenizer
                for(int i=0;i<input.size();i++){
                    if(input[i]==' '){
                        if(!inputToken.empty()){
                            inputSyntaxContainer.push_back(inputToken);
                            inputToken.clear();
                            continue;
                        }
                    }
                    if(input[i]==';'){
                        inputSyntaxContainer.push_back(inputToken);
                        inputToken.clear();
                        break;
                    }
                    inputToken+=input[i];
                }
                if(cmdList.find(inputSyntaxContainer[0])!=cmdList.end()){
                    cmdList[inputSyntaxContainer[0]](inputSyntaxContainer);
                }
                else if(inputSyntaxContainer[0]=="close"){ 
                    cout<<"[!]program_terminated"<<endl;
                    stop=true;
                    break;
                }
                else if(inputSyntaxContainer[0]=="clear"){
                    system("clear");
                }
                else if(inputSyntaxContainer[0]=="help"){
                    vector<string>argKeys;
                    for(auto &[key,value] : cmdList){
                        vector<string> syntaxKeys;
                        syntaxKeys = value({});
                        for(string item : syntaxKeys){
                            cout<<item<<" ";
                        }
                        cout<<endl;
                    }
                }
                else{
                    for(int i=0;i<inputSyntaxContainer.size();i++){
                        cout<<"[!]error_404(arg["<<i<<"]@;"<<inputSyntaxContainer[i]<<")"<<endl;
                    }
                }
        }
    }
}

vector<string> run(vector<string> arg){

    if(arg.size() < 3){
        return {"run","filePath","fileName"};
    }

    string filePath = arg[1];
    string fileName = arg[2];

    if(fs::exists(filePath)){

        string cmd =
            "g++ -std=c++17 \"" +
            filePath +
            "\" -o \"" +
            fileName +
            "\"";

        cout << cmd << endl;

        int result = system(cmd.c_str());
        system(("./" + fileName).c_str());
        
    }
    else{
        cout << "[!]No_such_directory::('" << filePath << "')" << endl;
    }

    return {};
}

vector<string> write(vector<string>arg){
    //push help(syntaxKeys)
    if(arg.empty()){
        return {"write","input"};
    }
    string write_Output;
    for(int i=1;i<arg.size();i++){
        write_Output+=arg[i]+' ';
    }
    cout<<"[o]"<<write_Output<<endl;
    return {};
}

vector<string> note(vector<string>arg){
    //push note(syntaxKeys)
    if(arg.empty()){
        return {"note","write/read","fileName.fileType"};
    }

    string fileName;
    string inputOutput;
    if(arg[1]=="write"){
        cout<<"[i]"<<arg[2]<<"<<";
        getline(cin,inputOutput);

        ofstream file;
        file.open(arg[2]);
        file<<inputOutput;
        file.close();
        inputOutput.clear();
    }
    else if(arg[1]=="read"){
        ifstream file;
        file.open(arg[2]);
        while(getline(file,inputOutput)){
            cout<<"[o]"<<arg[2]<<">>"<<inputOutput<<endl;
        }
        file.close();
        inputOutput.clear();
    }
    else{
        cout<<"[!]syntax_error(arg[1]@;"<<arg[1]<<"=write||read)"<<endl;
    }
    return {};
}

vector<string> refresh(vector<string>arg){
    //push help(syntaxKeys)
    system("g++ -std=c++17 \"/home/Kai/LemonShell-Prototype/shell.cpp\" -o lemon");
    system("./lemon");
    exit(0);
    return {};
}
