#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <fstream>
using namespace std;

vector<string> all_files_;

bool isDir(string dir)
{
    struct stat fileInfo;
    stat(dir.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode)) {
        return true;
    } else {
        return false;
    }
}

void listFiles(string baseDir, bool recursive)
{
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(baseDir.c_str())) == NULL) {
        cout << "[ERROR: " << errno << " ] Couldn't open " << baseDir << "." << endl;
        return;
    } else {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name != string(".") && dirp->d_name != string("..")) {
                if (isDir(baseDir + dirp->d_name) == true && recursive == true) {
                    //all_files_.push_back(baseDir + dirp->d_name);
                    listFiles(baseDir + dirp->d_name + "/", true);
                } else {
                    all_files_.push_back(dirp->d_name);
                }
            }
        }
        closedir(dp);
    }
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr<<"argc must be 4   ./transformation DIR_to_tran Transformation_1/2/3 out_DIR"<<endl;
        return -1;
    }

    string DIR = argv[1];
    int transformation = stoi(argv[2]);
    string out_DIR = argv[3];
    listFiles(DIR, true);
    fstream input_file;
    fstream result_file;
    for (auto n:all_files_) {
        cout<<n<<endl;
        input_file.open(DIR+n,ios::in);
        string newfile = out_DIR+n+"_transformatioin"+to_string(transformation);
        result_file.open(newfile,ios::out);
        if (input_file.fail()) {
            cerr << "fail to open input file:" << n << "!\n";
        }
        if (result_file.fail()) {
            cerr << "fail to open result file:" << newfile << "!\n";
        }
        string line;
        int payload_num=0;
            if (transformation==1){
                while(getline(input_file,line)) {
                    if (line[0] == '>'){
                        result_file<<">payload"<<payload_num++<<endl;
                        continue;
                    }
                    for (auto m:line) {
                        if (m=='C')
                            result_file<<'G';
                        else if (m=='G')
                            result_file<<'C';
                        else
                            result_file<<m;
                    }
                    result_file<<endl;
                }
            }
            else if (transformation==2){
                while(getline(input_file,line)) {
                    if (line[0] == '>'){
                        result_file<<">payload"<<payload_num++<<endl;
                        continue;
                    }
                    for (auto m:line) {
                        if (m=='A')
                            result_file<<'C';
                        else if (m=='C')
                            result_file<<'A';
                        else if (m=='G')
                            result_file<<'T';
                        else if (m=='T')
                            result_file<<'G';
                    }
                    result_file<<endl;
                }
            }
            else if (transformation==3){
                while(getline(input_file,line)) {
                    if (line[0] == '>'){
                        result_file<<">payload"<<payload_num++<<endl;
                        continue;
                    }
                    for (auto m:line) {
                        if (m=='A')
                            result_file<<'G';
                        else if (m=='G')
                            result_file<<'T';
                        else if (m=='T')
                            result_file<<'C';
                        else if (m=='C')
                            result_file<<'A';
                    }
                    result_file<<endl;
                }
            } else {
                cout<<"wrong! transformation must be 1/2/3"<<endl;
                return EXIT_FAILURE;
            }

        }

    return 0;
}
