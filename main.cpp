/* Submitted by
 * Surya Pratap Singh
 * Avinash Gurjar
 * Arun Nainwal
 * Harshit Chauhan
 * Om Satyam Swarnkar
 * Kamal Sharma
 * Vivek Ranjan
 */

#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
string PATH = "";
string DATABASE = "";

string charStarToString(char *A) {
    string s = "";
    for (int i = 0; i < strlen(A); ++i) {
        s += A[i];
    }
    return s;
}

vector<string> querry_vector;

void showCompilationError(string s) {
    cout << "compilation Error: " << s << endl;
}

void showExicutionError(string s) {
    cout << "Execution Error: " << s << endl;
}

bool isDatabaseSelected() {
    return DATABASE.length() != 0;
}

bool isTableExist(string s){
    DIR *dir = opendir(DATABASE.c_str());
    bool yes = false;
    if(dir!= NULL){
        while (auto ep = readdir(dir)){
            string tbl;
            tbl = ep->d_name;
            if(tbl == (s+".txt")){
                yes = true;
            }
        }
        closedir(dir);
    }else{
        showExicutionError("Unable to open directory");
    }
    return yes;
}

bool isDatabaseExist(string s){
    DIR *dir = opendir(PATH.c_str());
    bool yes = false;
    if(dir!= NULL){
        while (auto ep = readdir(dir)){
            string db;
            db = ep->d_name;
            if(db == s){
                yes = true;
            }
        }
        closedir(dir);
    }else{
        showExicutionError("Unable to open directory");
    }
    return yes;
}


bool compile(string querry) {
    string s = "";
    querry_vector.clear();

    for (int i = 0; i < querry.length(); ++i) {
        if (isalnum(querry[i])) {
            s += (char) tolower(querry[i]);
        } else {
            if (s.length()) { querry_vector.push_back(s); }
            s = "";
        }
    }
    if (s != "") {
        querry_vector.push_back(s);
    }
    if (!querry_vector.size()) {
        showCompilationError("Error in querry");
        return false;
    }

    if (querry_vector[0] == "create") {
        if (querry_vector[1] == "database") {
            return querry_vector.size() == 3;
        } else if (querry_vector[1] == "table") {
            if (querry_vector.size() == 3) {
                showCompilationError("Table can't have 0 column");
                return false;
            }
            if (querry_vector.size() % 2 == 0) {
                cout << querry_vector.size() << endl;
                showCompilationError("Error in column and their Datatype");
                return false;
            }
            int idx = 3;
            set<string> st;

            while (idx < querry_vector.size()) {

                if (idx & 1) {
                    st.insert(querry_vector[idx]);
                } else {
                    if (querry_vector[idx] != "varchar" and querry_vector[idx] != "integer" and
                        querry_vector[idx] != "longinteger" and querry_vector[idx] != "boolean") {
                        showCompilationError("Again see the instruction for the datatype allowed");
                        return false;
                    }
                }
                idx++;
            }

            if ((querry_vector.size() - 3) / 2 != st.size()) {
                showCompilationError("Error: table column should be different");
                return false;
            }
        } else {
            showCompilationError("Database says what should I create.");
            return false;
        }
    } else if (querry_vector[0] == "use") {
        if (querry_vector.size() != 2) {
            showCompilationError("Please write proper querry");
            return false;
        }
    }else if(querry_vector[0] == "drop"){
        if(querry_vector.size() != 3){
            return false;
        }
        if(querry_vector[1]!="database" and querry_vector[1] != "table"){
            return false;
        }
    } else if (querry_vector[0] == "select"){
        if(querry_vector.size() != 4){ return false; }
        if(querry_vector[1] != "from"){
            return false;
        }
    } else if (querry_vector[0] == "insert") {
        if(querry_vector[1]!= "into"){ return false; }
        if(querry_vector.size()<=3){
            return false;
        }
    } else if (querry_vector[0] == "show") {
        if(querry_vector.size() != 2){ return false; }
        if(querry_vector[1] != "databases" and querry_vector[1] != "tables"){
            return false;
        }
    } else if (querry_vector[0] == "quit") {
        return true;
    } else {
        return false;
    }

    return true;
}

int exicute() {

    if (querry_vector[0] == "create") {
        if (querry_vector[1] == "database") {
            DIR *dir = opendir(PATH.c_str());
            if (dir != NULL) {

                while (auto ep = readdir(dir)) {
                    string db;
                    db = ep->d_name;
                    if (db == querry_vector[2]) {
                        showExicutionError("Database already exits");
                        return 2;
                    }
                }
                closedir(dir);
            } else {
                cout << "Sorry we have some issues try after they fixed" << endl;
                return 2;
            }

            string folder = PATH + '/' + querry_vector[2];
            int response = mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (response == -1) {
                showExicutionError("problem in creating directory");
                return 2;
            }
        } else if (querry_vector[1] == "table") {
            if (!isDatabaseSelected()) {
                showExicutionError("Database is not selected");
                return 2;
            }

            DIR *dir = opendir(DATABASE.c_str());
            if (dir != NULL) {

                while (auto ep = readdir(dir)) {
                    string db;
                    db = ep->d_name;
                    if (db == querry_vector[2]) {
                        showExicutionError("Table already exits");
                        return 2;
                    }
                }
                closedir(dir);
            } else {
                cout << "Sorry we have some issues try after they fixed" << endl;
                return 2;
            }

            string file = DATABASE + '/' + querry_vector[2] + ".txt";

            ofstream myfile;
            myfile.open(file);

            int idx = 3;
            while (idx < querry_vector.size()) {
                if (idx & 1) {
                    myfile << querry_vector[idx] << " ";
                }
                idx++;
            }
            myfile << endl;
            myfile.close();
        }
    } else if (querry_vector[0] == "use") {
        if (isDatabaseExist(querry_vector[1])) {
            DATABASE = PATH + '/' + querry_vector[1];
            cout << DATABASE << endl;
            cout << "Database changed" << endl;
        } else {
            showExicutionError("no such database name" + querry_vector[1]);
            return 2;
        }
    } else if(querry_vector[0] == "drop"){
        if(querry_vector[1] == "database"){
            if(isDatabaseExist(querry_vector[2])){
                string database = PATH + '/' + querry_vector[2];
                remove(database.c_str());
                DATABASE = "";
            }
        }else{
            if(!isDatabaseSelected()){
                showExicutionError("select the database first");
                return 2;
            }
            if(isTableExist(querry_vector[2])){
                string file = DATABASE + '/' + querry_vector[2] + ".txt";
                remove(file.c_str());
            }
        }
    } else if (querry_vector[0] == "select") {
        if(!isDatabaseSelected()){
            showExicutionError("Database not selected");
            return 2;
        }
        string table;
        if(querry_vector[1] == "from") {
            table = querry_vector[2];
            if (!isTableExist(table)) {
                showExicutionError("table not exist");
                return 2;
            }
            string file = DATABASE + '/' + table + ".txt";
            ifstream myfile;
            myfile.open(file);
            int col = 0;
            for (int i = 0; i <querry_vector[3].size(); ++i) {
                col  = col*10 + querry_vector[3][i]-'0';
            }
            vector< string > vec[col];
            while (!myfile.eof()){
                for (int i = 0; i <col; ++i) {
                    string s;
                    myfile>>s;
                    vec[i].push_back(s);
                }
            }

            for (int j = 0; j <vec[0].size(); ++j) {
                for (int i = 0; i <col; ++i) {
                    cout<<vec[i][j]<<" ";
                }
                cout<<endl;
            }
        }

    } else if (querry_vector[0] == "insert") {
        if(!isDatabaseSelected()){
            showCompilationError("Database not selected");
            return 2;
        }
        if(!isTableExist(querry_vector[2])){
            showExicutionError("table not exist");
            return 2;
        }
        string file = DATABASE + '/' + querry_vector[2] + ".txt";
        ofstream myfile;
        myfile.open(file.c_str(),std::ios::app);
        for (int i = 4; i <querry_vector.size(); ++i) {
            myfile<<querry_vector[i]<<" ";
        }
        myfile<<endl;
        myfile.close();

    } else if (querry_vector[0] == "show") {
        if(querry_vector[1] == "databases"){
           DIR *dir = opendir(PATH.c_str());
           if(dir != NULL){
               cout<<"********Databases********"<<endl;
               while (auto ep = readdir(dir)) {
                   string db;
                   db = ep->d_name;
                   if(db.size()){cout<<db<<endl; }
               }
               closedir(dir);
           }else{
               cout<<"Unable to fix some issues try after they fixed"<<endl;
               return 2;
           }
        }else{
            if(!isDatabaseSelected()){
                showExicutionError("Please select the database first");
                return 2;
            }

            DIR *dir = opendir(DATABASE.c_str());
            if(dir != NULL){
                cout<<"********Tables********"<<endl;
                while(auto ep = readdir(dir)){
                    string tbl;
                    cout<<tbl<<endl;
                    tbl = ep->d_name;
                    for (int i = 0; i <tbl.length(); ++i) {
                        if(tbl[i] == '.'){
                            break;
                        }
                        cout<<tbl[i];
                    }
                    cout<<endl;
                }
                closedir(dir);
            }else{
                cout<<"Unable to fix some issues try after they fixed"<<endl;
                return 2;
            }
        }
    } else if (querry_vector[0] == "quit") {
        return 0;
    }
    return 1;
}

int main() {

    cout << "*********Instructions*********" << endl;
    cout << "Write only simple mysql querry as create table,create database,select from table,insert into table" << endl;
    cout << "update table and showtables, no complex querry will exicute. " << endl;
    cout << "Only datatype for tables is varchar" << endl;
    cout << "enter quit to terminate the program" << endl;
    cout << "Select statement 'Select from table integer(colums in the table)'" << endl;
    cout << "Select is simple select to show whole table " <<endl;
    cout << "Insert statement 'Insert into table val1, val2, val3 ...' " << endl;
    cout << "Create statement 'Create table tableNmae(column1 varchar, column2 varchar.....)" << endl;
    cout << "You can delete databse by 'drop database databaseName' and table by 'drop table tableName' " << endl;
    cout << "'show databases' to see the databases and 'show tables' to see the tables list" << endl;
    cout << " 'use databaseName' for using the database " << endl;

    char cCurrentPath[FILENAME_MAX];

    if (!getcwd(cCurrentPath, sizeof(cCurrentPath))) {
        return errno;
    }

    PATH = string(cCurrentPath);

    while (true) {
        cout << "Enter a querry" << endl;
        string querry;
        getline(cin, querry);
        //cout<<querry<<endl;
        if (compile(querry)) {
            int status = exicute();
            if (status == 0) {
                cout << "Thank you for using Our database" << endl;
                break;
            } else if (status == 1) {
                cout << "Querry exicuted successfully" << endl;
            }
        } else {
            showCompilationError("please write proper querry. You have some compilation errors on your querry.");
        }
    }
    return 0;
}