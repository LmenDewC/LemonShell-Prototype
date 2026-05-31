//FUTURE UPDATES:
//lxde format reference
//TERM environment reading
//arg[0]@::note; folder handling and implementation
//implement current file directory for shell
//add colored output and formatting
//arg[0]@::note; implement newline on input;
//Add dir as variable for easy access
//implement arg[0]::rsetdir; to reset directory to original TERM directory


//ISSUES:
//1.Proper error handling
//2.run should pull data then run it on the same instance of the program, not compile and run in a new instance, also should pull data from current file directory, not just absolute path

//CURRENT UPDATES& FIXES:
//arg[0]@::refresh; updated cmd, refreshes program to match changes in code, also fixes issue_2 
//arg[0]@::run; updated cmd, now compiles and runs file in one command, also fixes issue_2;
//use fstream for arg[0]::run...; to get data;
