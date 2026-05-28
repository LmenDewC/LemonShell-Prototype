  #include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <filesystem>

using namespace std;



vector<string> run(vector<string>arg);
vector<string> write(vector<string>arg);
vector<string> note(vector<string>arg);

int main(){
    cout<<"[-Lemon_cmd_]\n";
    while(true){
        string input;
        string inputToken;
        vector<string> inputSyntaxContainer;
        map<string, function<vector<string>(vector<string>)>> cmdList={
            {"run",run},
            {"write",write},
            {"note",note}
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
                    cout<<"[!]program_terminated";
                    break;
                }
                else if(inputSyntaxContainer[0]=="clear"){
                    system("cls");
                    //either hardcode clear, or find easier way
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

vector<string> run(vector<string>arg){
        //push help(syntaxKeys);
        if(arg.empty()){
            return {"run","filePath","fileName"};
        }
        //scratch or keep, still in brainstorming phase
        //format: cmd_filePath_fileName;
        string filePath=arg[1];
        string fileName=arg[2];

        for(const auto &file : filesystem::directory_iterator(filePath)){
            if(file.path().stem() == fileName){
                system(file.path().string().c_str());
            }
            else{
                cout<<"[!]run:error_404('"<<fileName<<"')"<<endl;
            }
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
        cout<<"[!]syntax_error(arg[1]@;larp!=write||read)"<<endl;
    }
    return {};
}
