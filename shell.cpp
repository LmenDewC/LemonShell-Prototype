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
//add colored output and formatting
//arg[0]@::note; implement endl on input;

//ISSUES:
//1.Proper error handling

//CURRENT UPDATES& FIXES:
//arg[0]@::dlt; updated cmd, now deletes directories with content, not just empty directories;
//arg[0]@::fnd; updated cmd, now searches for files in current directory and subdirectories, not just current directory;
//arg[0]@::setdir; updated cmd, now changes current directory, not just checks if directory exists;
//arg[0]@::create; updated cmd, now creates directories, not just checks if directory exists;

vector<string> run(vector<string>arg);
vector<string> write(vector<string>arg);
vector<string> note(vector<string>arg);
vector<string> refresh(vector<string>arg);
vector<string> setdir(vector<string>arg);
vector<string> create(vector<string>arg);
vector<string> dlt(vector<string>arg);
vector<string> fnd(vector<string>arg);


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
            {"refresh",refresh},
            {"setdir",setdir},
            {"create",create},
            {"dlt",dlt},
            {"fnd",fnd}
            
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
                else if(inputSyntaxContainer[0]=="dir"){
                    cout<<"[o]Directory::"<<fs::current_path()<<endl;
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

        int result = system(cmd.c_str());
        system(("./" + fileName).c_str());
        
    }
    else if(!fs::exists(filePath)){
        cout << "[!]No_such_directory::('" << filePath << "')" << endl;
    }else{
        system(("./" + filePath).c_str());
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

vector<string> setdir(vector<string>arg){
    if(arg.size() < 2){
        return {"setdir","directoryPath"};
    }
    string dirPath = arg[1];
    if(fs::exists(dirPath)){
        fs::current_path(dirPath);
    }else{
        cout<<"[!]No_such_directory::('" << dirPath << "')" << endl;
    }
    return {};
}

vector<string> create(vector<string>arg){
    if(arg.size() < 2){
        return {"create","directoryPath"};
    }
    
    string dirPath = arg[1];

    if(fs::exists(dirPath)){
        cout<<"[!]directory_already_exists::('" << dirPath << "')" << endl;
        return {};
    }else{
        fs::create_directory(dirPath);
    }

    return {};
}

vector<string> dlt(vector<string>arg){
    if(arg.size() < 2){
        return {"dlt","directoryPath"};
    }
    
    string dirPath = arg[1];

    if(fs::exists(dirPath)){
        fs::remove_all(dirPath);
    }else{
        cout<<"[!]No_such_directory::('" << dirPath << "')" << endl;
        return {};
    }

    return {};
}

vector<string> fnd(vector<string>arg){
    if(arg.size() < 2){
        return {"fnd","fileName"};
    }
    
    string fileName = arg[1];
    bool found = false;

    for(const auto& entry : fs::recursive_directory_iterator(fs::current_path())){
        if(entry.path().filename() == fileName){
            cout<<"[o]File_found@::'" << entry.path() << "'" << endl;
            found = true;
        }
    }

    if(!found){
        cout<<"[!]No_such_file::('" << fileName << "')" << endl;
    }

    return {};
}