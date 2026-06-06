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
//TERM environment reading
//add colored output and formatting
//add git as cmd

//ISSUES:
//1.Proper error handling

//CURRENT UPDATES& FIXES:
//arg[0]::ct; fixed bugs on directory handling;
//arg[0]::cpy; fixed bugs on directory handling;
//arg[0]::dlt: added confirmation before execution;


vector<string> run(vector<string>arg);
vector<string> write(vector<string>arg);
vector<string> note(vector<string>arg);
vector<string> refresh(vector<string>arg);
vector<string> setdir(vector<string>arg);
vector<string> cdir(vector<string>arg);
vector<string> dlt(vector<string>arg);
vector<string> fnd(vector<string>arg);
vector<string> cpy(vector<string>arg);
vector<string> ct(vector<string>arg);
vector<string> rname(vector<string>arg);
vector<string> lc(vector<string>arg);
vector<string> git(vector<string>arg);


string currentDirectory=fs::current_path();
string mainDir="/data/data/com.termux/files/home";

int main(){
    system("clear");
    
    cout<<"[d]"<<currentDirectory<<endl;


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
            {"cdir",cdir},
            {"dlt",dlt},
            {"fnd",fnd},
            {"cpy",cpy},
            {"ct",ct},
	    {"rname",rname},
            {"lc",lc},
	    {"git",git}
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
    vector<string> noteBuffer;
    if(arg[1]=="write"){
        while(true){
        cout<<"[i]"<<"<<";
        getline(cin,inputOutput);
        if(inputOutput=="_end"){
            break;
        }
        noteBuffer.push_back(inputOutput);
        }

        ofstream file;
        file.open(arg[2]);
        for(const auto& line : noteBuffer){
            file << line << endl;
        }
        file.close();
        inputOutput.clear();
    }
    else if(arg[1]=="read"){
        int lineCount = 0;
        ifstream file;
        file.open(arg[2]);
        cout<<"\033[37m["<<arg[2]<<"]\033[0m"<<endl;
        cout<<"\033[32m--------------------"<<endl;
        while(getline(file,inputOutput)){
            lineCount++;
            cout<<"["<<lineCount<<"]"<<">>"<<inputOutput<<endl;
        }
        cout<<"--------------------\033[38;2;255;244;79m"<<endl;
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
    system("g++ -std=c++17 \"/data/data/com.termux/files/home/kai/editables/shell.cpp\" -o lemon");
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
    }else
    if(dirPath=="main"){
        fs::current_path(mainDir);
    }
    else{
        	cout<<"[!]No_such_directory::('" << dirPath << "')" << endl;
    }
    return {};
}

vector<string> cdir(vector<string>arg){
    if(arg.size() < 2){
        return {"cdir","directoryPath"};
    }
    string dirPath=arg[1];

    if(fs::exists(dirPath)){
	cout<<"[!]directory_already_exists::('"<<dirPath<<"')"<<endl;
    }else{
	fs::create_directory(dirPath);
	cout<<"[!]dir_created"<<endl;
    }
    return {};
}

vector<string> dlt(vector<string>arg){
    if(arg.size() < 2){
        return {"dlt","directoryPath"};
    }
    
    string dirPath = arg[1];
    string yn;
    if(fs::exists(dirPath)){
	cout<<"[!]delete_file?_[y/n]:";
	cin>>yn;
	if(yn=="y"||yn=="Y"){
        	fs::remove_all(dirPath);
		cout<<"[!]deleted"<<endl;
	}
	if(yn=="n"||yn=="N"){
		cout<<"[!]aborted"<<endl;
	}
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

vector<string> cpy(vector<string>arg){
    if(arg.size() < 3){
        return {"copy","sourcePath","destinationPath"};
    }
    
    string sourcePath = arg[1];
    string destinationPath = arg[2];

    if(fs::exists(sourcePath)){
        try{
            fs::copy(sourcePath, destinationPath, fs::copy_options::recursive);
        }catch(const fs::filesystem_error& e){
            cout<<"[!]Error_copying_file::(" << e.what() << ")" << endl;
        }
    }else{
        cout<<"[!]No_such_file_or_directory::('" << sourcePath << "')" << endl;
        return {};
    }

    return {};
}

vector<string> ct(vector<string>arg){
    if(arg.size() < 3){
        return {"ct","sourceFile","destinationPath"};
    }
    
    string sourceFile = arg[1];
    string destinationPath = arg[2];

    if(fs::exists(sourceFile)){
        try{
               fs::copy(sourceFile,destinationPath+"/"+sourceFile,fs::copy_options::recursive|fs::copy_options::overwrite_existing);
	       fs::remove_all(sourceFile);
	       cout<<"[!]file_'"<<sourceFile<<"'_moved"<<endl;
        }catch(const fs::filesystem_error& e){
            cout<<"[!]Error_copying_file::(" << e.what() << ")" << endl;
        }
    }
    return {};
}

vector<string> rname(vector<string>arg){
	if(arg.size()<3){
		return{"rname","fileName.dirName","newName"};
	}
	string originalName=arg[1];
	string newName=arg[2];
	
	if(!fs::exists(originalName)){
		cout<<"[!]error_404("<<originalName<<")"<<endl;		
	}
	else{
	fs::rename(originalName,newName);
	}
	return {};
}

vector<string> lc(vector<string>arg){
	if(arg.size()<2){
		return{"lc","dirPath"};
	}
	string dirPath=arg[2];

	if(arg[1]=="files"){
		for(const auto& content:fs::directory_iterator(dirPath)){
			if(content.is_regular_file()){
				cout<<"[.]"<<content<<endl;
			}
		}
	}
	else if(arg[1]=="dir"){
		for(const auto& content:fs::directory_iterator(dirPath)){
			if(content.is_directory()){
				cout<<"[/]"<<content<<endl;
			}
		}
	}
	else if(arg[1]=="all"){
		for(const auto& content:fs::directory_iterator(dirPath)){
			if(content.is_directory()){
				cout<<"[/]"<<content<<endl;
			}
			else if(content.is_regular_file()){
				cout<<"[.]"<<content<<endl;
			}
			else{
				cout<<"[!]error(unreadable_file/path)"<<endl;
			}
		}
	}
	else{
		cout<<"[!]error_404(arg[1]@::"<<arg[1]<<"...files/dir)"<<endl;
	}
	return {};
}

vector<string>git(vector<string>arg){
    if(arg.size()<2){
         return{"git","args"};
    }

    string header=arg[0];
    string gcmd;
    for(int i=1;i<arg.size();i++){
	 gcmd+=" "+arg[i];
    }
    system((header+gcmd).c_str());

    return {};
}
