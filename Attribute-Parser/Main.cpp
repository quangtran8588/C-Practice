#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

//	Function print out the result
//	If a Tag is matched, then print out its value
//	Otherwise, print "Not Found!"
void print_result(vector<pair<string, string>> tag_value_list, string attribute) {
	vector<pair<string, string>>::iterator it;
	for (it = tag_value_list.begin(); it != tag_value_list.end(); it++) {
		if (it->first == attribute) {
			cout << it->second << endl;
			return;
		}
	}
	cout << "Not Found!" << endl;
}

//	This function is to remove special characters out of string 
//	Special characters: '<', '>', '"', ' '
string remove_special_character(string data) {
	for (int i = 0; i < data.size(); i++) {
		if (data[i] == '<' || data[i] == '"' || data[i] == '>' || data[i] == ' ' || data[i] == '~') {
			data.erase(i, 1);
			i--;
		}
	}
	return data;
}


//	Split data in a line of input using delimiter
//	store all data in a vector
vector<string> split_data_using_delimiter(string data, char delimiter) {
	vector<string> split;
	string temp;
	stringstream line(data);
	while (getline(line, temp, delimiter)) {
		split.push_back(temp);
	}
	return split;
}

//	This function will find if value exist in the HRML line input data
//	For exampe: <tag1 value = "Hello">
//	=> value does exist
//	<tag1>
//	=> value does not exist because there is no attribute.

bool isValueExist(string data, string delimiter) {
	size_t found = data.find(delimiter);
	if (found != string::npos)
		return true;
	return false;
}

//	Function check if a line of string data is a closing tag
//	It checks the first and the second character are '<' and '/'
//	then, it should be a closing tag.
//	For example: <tag1> => not a closing tag
//	Another example: </tag1> => is a closing tag
bool isClosingTag(string data) {
	if (data[0] == '<' && data[1] == '/')
		return true;
	return false;
}

//	Function spit two numbers in the fist line of input data
//	One of them represents a number of line in the HRML source
//	The other one represents a number of queried line
vector<string> split_two_numbers(vector<string> data, int *N, int *Q) {
	vector<string> temp;
	string line;	// string value of line
					//	Read the first line and split two numbers
					//	to save numb_of_HRML and numb_of_queries
	line = data.at(0);
	temp = split_data_using_delimiter(line, ' ');
	*N = stoi(temp.at(0));
	*Q = stoi(temp.at(1));
	data.erase(data.begin());
	return data;
}

//	Read all input data in a file or command line interface
vector<string> readInput() {
	string line;
	vector<string> inputData;
	while (getline(cin, line)) {
		inputData.push_back(line);
	}
	return inputData;
}

//	Function assigns a pair of Tag and its value into list
//	A Tag contains a tag_name + attribute_name
//	For example: tag_list = {tag1, tag2, tag3}, attribute = name1; value = "Hello"
//	First, create a tag name as tag1.tag2.tag3 => Tag = tag_name + attribute_name = tag1.tag2.tag3~name1
//	Then, assign into list as <"tag1.tag2.tag3~name1","Hello">

void assign_tag_value_list(vector<pair<string, string>> &list, vector<string> tag_list, string attribute, string value) {
	vector<string>::iterator it;
	string temp = "";
	for (int i = 0; i < tag_list.size(); i++) {
		if (i == 0)
			temp = tag_list.at(i);
		else
			temp = temp + "." + tag_list.at(i);
	}
	temp = temp + "~" + attribute;
	value = remove_special_character(value);
	list.push_back(make_pair(temp, value));
}

int main() {
	//	Declare all variables to be used
	vector<pair<string, string>> tag_value;	// vector stores pair of tag and its value
	vector<string> inputFile;	// vector stores input data reading from command line/input file, etc....
	vector<string> tag_list; // vector stores all tags in the input file.
	
	int numb_of_HRML = 0;	// number of line in the HRML source
	int numb_of_queries = 0; // number of queried lines

	//	Call function to read input data 
	//	and push data into list
	inputFile = readInput();
	//	Read the first line of input to assign values
	//	into numb_of_HRML and numb_of_queries
	//	and update inputFile data (erase first line of input)
	vector<string> temp = inputFile;
	inputFile.clear();
	inputFile = split_two_numbers(temp, &numb_of_HRML, &numb_of_queries);

	//	Create a loop to assign a pair of Tag and its value
	for (unsigned int i = 0; i < numb_of_HRML; i++) {
		//	Checking whether an input line data contains value
		//	If YES, split all values and assign those into tag_value pair list
		//	If NOT contains a value (tag ONLY), then push this tag into tag_list
		//	If it is a closing tag, then erase the last element of tag_list
		if (isValueExist(inputFile.at(i), "=")) {
			vector<string> temp = split_data_using_delimiter(inputFile.at(i), ' ');	// split all using delimiter ' '
			// the first element is always tag, then remove special character '<' and push into tag_list
			string tag = remove_special_character(temp.at(0)); 
			tag_list.push_back(tag);
			//	erase first element, then we have only a string contains attribute, "=", and value
			temp.erase(temp.begin());
			//	The remaining string has the form as:
			//	attribute_1 + "=" + value_1 + .... + attribute_n + "=" + value_n
			//	Read a first three elements and assign into tag_value list
			//	Erase these three element, and continue until vector is empty
			while (temp.size() != 0) {
				assign_tag_value_list(tag_value, tag_list, temp.at(0), temp.at(2));
				temp.erase(temp.begin(), temp.begin() + 3);
			}
		}
		else if (!isValueExist(inputFile.at(i), "=") && !isClosingTag(inputFile.at(i))) {
			string tag = remove_special_character(inputFile.at(i));
			tag_list.push_back(tag);
		}
		if (isClosingTag(inputFile.at(i))) {
			tag_list.erase(tag_list.begin() + (tag_list.size() - 1));
		}
	}
	inputFile.erase(inputFile.begin(), inputFile.begin() + numb_of_HRML);	// erase inputFile data after using
	
	//	Read all queries and print out corresponding result if matched
	for (unsigned int i = 0; i < numb_of_queries; i++) {
		print_result(tag_value, inputFile.at(i));
	}

	return 0;
}
