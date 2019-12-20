#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

const regex COMMENT_REGEX_HTML("[\t ]*<!--((?!-->)[^])*-->[\t ]*");
const regex COMMENT_REGEX_C_CPP_H_JAVA_JS("([\t ]*\\/\\*((?!\\*\\/)[^])*\\*\\/[\t ]*|\\n?\\/\\/.*)");
const regex COMMENT_REGEX_PHP("([\t ]*\\/\\*((?!\\*\\/)[^])*\\*\\/[\t ]*|\\n?(\\/\\/|#).*)");
const regex COMMENT_REGEX_CSS("[\t ]*\\/\\*((?!\\*\\/)[^])*\\*\\/[\t ]*");
const regex COMMENT_REGEX_PYTHON("|\\n?#.*");


const map<string, regex> TYPE_TO_REGEX = {
	{"c",COMMENT_REGEX_C_CPP_H_JAVA_JS},
	{"h",COMMENT_REGEX_C_CPP_H_JAVA_JS},
	{"cpp",COMMENT_REGEX_C_CPP_H_JAVA_JS},
	{"java",COMMENT_REGEX_C_CPP_H_JAVA_JS},
	{"js",COMMENT_REGEX_C_CPP_H_JAVA_JS},
	{"css",COMMENT_REGEX_CSS},
	{"html",COMMENT_REGEX_HTML},
	{"php",COMMENT_REGEX_PHP},
	{"py",COMMENT_REGEX_PYTHON},
};


int main(int argc, char* argv[]) {
	cout << "Comments Remover by Uriah Shaul Mandel" << endl << endl;
	if (argc != 2) {
		cout << "CommentsRemover must get one parameter, the file's path" << endl;
		return 1;
	}

	string original_file_path = argv[1];
	string rgx1_str_middle = "";
	for (auto it = TYPE_TO_REGEX.begin(); it != TYPE_TO_REGEX.end(); ++it)
		rgx1_str_middle += it->first + "|";

	rgx1_str_middle = rgx1_str_middle.substr(0, rgx1_str_middle.length() - 1);

	regex rgx1("((" + rgx1_str_middle + ")$)"); smatch sm1;
	regex_search(original_file_path, sm1, rgx1);
	if (sm1.empty()) {
		cout << (original_file_path + " Doesn't end with one of the following: " + rgx1_str_middle) << endl;
		return 1;
	}

	string type = (sm1.end() - 1)->str();
	string new_file_path = original_file_path.substr(0, original_file_path.length() - (type.length() + 1)) + "_comments_removed." + type;
	std::ofstream new_file(new_file_path);

	std::ifstream ifs(original_file_path);
	string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	smatch sm2; regex rgx2 = TYPE_TO_REGEX.at(type);
	regex_search(content, sm2, rgx2);
	const int num_of_replacement = sm2.size();
	string replaced_content = regex_replace(content, rgx2, "");
	new_file << replaced_content;
	new_file.close();


	cout << num_of_replacement + " Comments were removed succesfully! " << endl;
	cout << "New file's path: " << new_file_path << endl;
	return 0;
}
