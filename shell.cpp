  #include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <filesystem>

namespace fs=std::filesystem;
using namespace std;

//FUTURE UPDATES:
//lxde format reference
//TERM environment reading
//arg[0]@::note; transition to filesystem_lib

//ISSUES:
//1.Proper error handling
//2.arg[0]@::close; doesnt break loop
//3.arg[0]@::run; file handling

//CURRENT UPDATES& FIXES:
//+arg[0]@::refresh; easy way to edit and test code


vector<string> run(vector<string>arg);
vector<string> write(vector<string>arg);
vector<string> note(vector<string>arg);
vector<string> refresh(vector<string>arg);


int main(){
    system("clear");

    cout<<"[-Lemon_cmd-]\n";

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

        cout<<"[?]:";
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
    return 0;
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

        if(result == 0){
            system(("./" + fileName).c_str());
        }
    }
    else{
        cout << "[!]No_such_directory::('" << filePath << "')" << endl;
    }

    return {};
}

vector<string> refresh(vector<string>arg){
    //push help(syntaxKeys)
    system("g++ -std=c++17 \"/home/Kai/LemonShell-Prototype/shell.cpp\" -o lemon");
    system("./lemon");
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
