#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <cmath>
#include <bits/stdc++.h>
#include <iomanip>
#include <windows.h>
using namespace std;

// add patch notes and error messages & codes
// get file creation and modification date

string patchnotes = {"almost finished now wow"};


string args;
void Start(bool message = false);
bool canCalculate = false;
enum class CalcException{
    SYNTAXERROR,
    MATHERROR,
    OVERFLOWERROR
};
void throwException(CalcException error, int index = -1, const string message = "");
double trystod(const string str);
void printVstr(vector<string> v);
template <typename T>
string to_string_precise(const T __val, const int __precision = numeric_limits<T>::digits10);
double degrees_to_radians(const double __val);
double radians_to_degrees(const double __val);
const vector<string> operations   {"sin", "cos", "tan", "arcsin", "arccos", "arctan", // 1-arg
                                    "sinh", "cosh", "tanh", "log", "exp"};
const vector<string> commands {"help", "?", "/?", "patchnotes", "whatsnew",};
const double toleranceValue = 0.00000000001;
WIN32_FILE_ATTRIBUTE_DATA fileInfo;


int main()
{
    Start(true);
    LPCWSTR fileName = L"C:\\Users\\nakrt\\Documents\\c++ proj\\std1\\kulator.exe";
    GetFileAttributesExW(fileName,GetFileExInfoStandard,&fileInfo);
}

void Start(bool message)
{
    cout << "\e[0m"; //reset formatting
    if(message){cout << "\nkulator c++\n" << "\e[1;32m" << ">" << "\e[0m";}
    else{cout << "\e[1;32m" << "\n>" << "\e[0m";}
    getline(cin, args);
    canCalculate = true;

    //argument stream reading
    stringstream ss(args);
    vector<string> vstrnumbers{};
    vector<vector<string>> vstrbrackets{};
    string potentialCmd = "";
    string strtoken = "";
    const string operators = "+-*/^";
    
    if(find(commands.begin(),commands.end(),args) != commands.end())
    {
        if(args == "help" || args == "?" || args == "/?")
        {
            for (string cmd: commands)
            {
                cout << cmd << "\n";
            }
            for (string op: operations)
            {
                cout << op << "\n";
            }
        }
        if(args == "patchnotes" || args == "whatsnew")
        {
            cout << "kulator c++";
            cout << "\n" << patchnotes;
            cout << "Mohamedamin Nuur";
            
            DWORD lowPart = fileInfo.ftCreationTime.dwLowDateTime;
            DWORD highPart = fileInfo.ftCreationTime.dwHighDateTime;

            // Print the FILETIME as a 64-bit integer
            ULONGLONG combinedValue = static_cast<ULONGLONG>(lowPart) | (static_cast<ULONGLONG>(highPart) << 32);
            
            cout << combinedValue;
        }
        Start();
    }


    //is a lambda function because of bracket support which will use another set of args and vectors
    auto pushToVstr = [&](const string arguments, vector<string>& vstr)
    {
        //identification of numbers and operators in order
        vstrnumbers.resize(1);
        
        bool currentlyIteratingNumber, currentlyIteratingOperator, currentlyIteratingLetter, currentlyIteratingNegative;
        currentlyIteratingNumber = currentlyIteratingOperator = currentlyIteratingLetter = currentlyIteratingNegative = false;
        // ^^ WHY WASNT I TOLD YOU HAD TO DO THIS??? setting to false as doing "bool a,b = false" will set a to undefined and b to false
        size_t j = 0;
        int number_of_decimal_points = 0;
        int currently_iterated_negative_index = 0;
        for (size_t i = 0; i < arguments.length(); i++)
        {
            if(isdigit(arguments[i]) || arguments[i] == '.')
            {
                if((currentlyIteratingOperator || currentlyIteratingLetter) && !currentlyIteratingNegative){j++; number_of_decimal_points = 0;};
                if(potentialCmd != ""){throwException(CalcException::SYNTAXERROR,i - potentialCmd.size());break;}
                if(arguments[i] == '.'){number_of_decimal_points++;}
                if(number_of_decimal_points > 1){throwException(CalcException::SYNTAXERROR,i);break;}
                vstr.resize(j+1);
                vstr[j] += arguments[i];
                potentialCmd = "";
                currentlyIteratingNumber = true;
                currentlyIteratingOperator = false;
                currentlyIteratingLetter = false;
            }
            else if(operators.find(arguments[i]) != string::npos)
            {
                currentlyIteratingNegative = (i == 0 && arguments[i] == '-') || (arguments[i] == '-' && arguments[i-1] == '-');
                if(currentlyIteratingNegative){currently_iterated_negative_index++;}
                else{currently_iterated_negative_index = 0;}
                if(currentlyIteratingOperator && !currentlyIteratingNegative){throwException(CalcException::SYNTAXERROR,i);break;}
                if(potentialCmd != ""){throwException(CalcException::SYNTAXERROR,i - potentialCmd.size());break;}
                if(i+1 == arguments.size()){throwException(CalcException::SYNTAXERROR,i+1);Start(false);break;}
                if(i == 0 && arguments[i] != '-'){
                    throwException(CalcException::SYNTAXERROR,i);break;
                }
                currentlyIteratingNumber = false;
                currentlyIteratingOperator = true;
                currentlyIteratingLetter = false;
                if(currently_iterated_negative_index == 1 && i != 0){j++;}
                if(!currentlyIteratingNegative){j++;}
                vstr.resize(j+1);
                vstr[j] += arguments[i];
            }
            else if(arguments[i] == '(' || arguments[i] == ')' || arguments[i] == '[' || arguments[i] == ']' || arguments[i] == ' '){}
            else if(isalpha(arguments[i]))
            {
                potentialCmd += arguments[i];
                if(find(operations.begin(),operations.end(),potentialCmd) != operations.end() && !isalpha(arguments[i+1]))
                {
                    if(i+1 == arguments.size() || (arguments[i+1] != '(' && arguments[i+1] != '[')){;throwException(CalcException::SYNTAXERROR,i);break;}
                    if(isdigit(arguments[i-potentialCmd.size()]))
                    {
                        j++;
                        vstr.resize(j+1);
                        vstr[j] += "*";
                    }
                    if(currentlyIteratingNumber || currentlyIteratingOperator){j++;}
                    vstr.resize(j+1);
                    vstr[j] += potentialCmd;
                    potentialCmd = "";
                }
                else
                {
                    //this does not execute if the potentialCmd is a true command
                    if(!isalpha(arguments[i+1])){throwException(CalcException::SYNTAXERROR,i);break;}
                }
                currentlyIteratingNumber = false;
                currentlyIteratingOperator = false;
                currentlyIteratingLetter = true;
            }
            else
            {
                currentlyIteratingNumber = false;
                currentlyIteratingOperator = false;
                currentlyIteratingLetter = false;
                potentialCmd = "";
                throwException(CalcException::SYNTAXERROR,i);               
                break;
            }
            
        }
    };
    auto calculate = [&](vector<string>& vstr)
    {
        //calculation
        //i think it's IN ORDER OF OPERATIONS
        //brackets are done seperately

        


        //then operations
        while(find_first_of(vstr.begin(),vstr.end(),operations.begin(),operations.end()) != vstr.end()
        && canCalculate)
        {
            int cmdIndex = distance(vstr.begin(),find_first_of(vstr.begin(),vstr.end(),operations.begin(),operations.end()));
            string command = vstr[cmdIndex];

            // 1-arg
            if(command == "sin" || command == "cos" || command == "tan" 
            || command == "arcsin" || command == "arccos" || command == "arctan"
            || command == "sinh" || command == "cosh" || command == "tanh"
            || command == "log" || command == "exp")
            {
                double operand = trystod(vstr[cmdIndex + 1]);
                if(operand == INT_MAX){break;}
                double ans;
                if(command == "sin"){ans = sin(degrees_to_radians(operand));}
                else if(command == "cos"){ans = cos(degrees_to_radians(operand));}
                else if(command == "tan"){ans = tan(degrees_to_radians(operand));}
                else if(command == "arcsin"){ans = radians_to_degrees(asin(operand));}
                else if(command == "arccos"){ans = radians_to_degrees(acos(operand));}
                else if(command == "arctan"){ans = radians_to_degrees(atan(operand));}
                else if(command == "sinh"){ans = sinh(operand);}
                else if(command == "cosh"){ans = cosh(operand);}
                else if(command == "tanh"){ans = tanh(operand);}
                else if(command == "log"){ans = log(operand);}
                else if(command == "exp"){ans = exp(operand);}
                if(isnan(ans)){throwException(CalcException::MATHERROR);break;}
                vstr.insert(vstr.begin() + cmdIndex + 2,to_string_precise(ans));
                vstr.erase(vstr.begin() + cmdIndex,vstr.begin() + cmdIndex + 2);
            }
        }

        while (find(vstr.begin(),vstr.end(),"^") != vstr.end())
        {
            for (size_t i = 0; i < vstr.size(); i++)
            {
                if(vstr[i] == "^")
                {
                    double before = trystod(vstr[i-1]);
                    double after = trystod(vstr[i+1]);
                    if(before >= INT_MAX || after >= INT_MAX){break;}
                    double ans = pow(before, after);
                    vstr.insert(vstr.begin()+i+2,to_string_precise(ans));
                    vstr.erase(vstr.begin()+i-1,vstr.begin()+i+2);
                    i=0;
                }
            }
        }
        

        //then divison and multiplication
        while((find(vstr.begin(),vstr.end(),"*") != vstr.end() 
        || find(vstr.begin(),vstr.end(),"/") != vstr.end())
        && canCalculate){
        for (size_t i = 0; i < vstr.size(); i++)
        {
            //cant use switch here
            if(vstr[i] == "*")
            {
                double before = trystod(vstr[i-1]);
                double after = trystod(vstr[i+1]);
                if(before == INT_MAX || after == INT_MAX){break;}
                double ans = before * after;
                vstr.insert(vstr.begin()+i+2,to_string_precise(ans));
                vstr.erase(vstr.begin()+i-1,vstr.begin()+i+2);
                //^^^ "the range includes all the elements between first and last,
                //     including the element pointed by first but not the one 
                //     pointed by last."
                i=0; // have to reset i to continue right-to-left iteration.
            }
            else if(vstr[i] == "/")
            {
                double before = trystod(vstr[i-1]);
                double after = trystod(vstr[i+1]);
                if(before == INT_MAX || after == INT_MAX){break;}
                if(after == 0){throwException(CalcException::MATHERROR,i+1);break;}
                double ans = before / after;
                vstr.insert(vstr.begin()+i+2,to_string_precise(ans));
                vstr.erase(vstr.begin()+i-1,vstr.begin()+i+2);
                i=0;
            }
            
        }
        }

        //then addition and subtraction
        while((find(vstr.begin(),vstr.end(),"+") != vstr.end() 
        || find(vstr.begin(),vstr.end(),"-") != vstr.end())
        && canCalculate){
        for (size_t i = 0; i < vstr.size(); i++)
        {
            if(vstr[i] == "+")
            {
                double before = trystod(vstr[i-1]);
                double after = trystod(vstr[i+1]);
                if(before == INT_MAX || after == INT_MAX){break;}
                double ans = before + after;
                vstr.insert(vstr.begin()+i+2,to_string_precise(ans));
                vstr.erase(vstr.begin()+i-1,vstr.begin()+i+2);
                i=0;
            }
            else if(vstr[i] == "-")
            {
                double before = trystod(vstr[i-1]);
                double after = trystod(vstr[i+1]);
                if(before == INT_MAX || after == INT_MAX){break;}
                double ans = before - after;
                vstr.insert(vstr.begin()+i+2,to_string_precise(ans));
                vstr.erase(vstr.begin()+i-1,vstr.begin()+i+2);
                i=0;
            }
            
        }
        }

        
    };

    
    /*bracket function*/{
    int iter = 0;
    while(args.find_last_of("(") != string::npos)
    {
        //these are each other's
        size_t indexLastOpen = args.find_last_of("(");
        
        int offset = 0; // if a "*" is inserted before the last open bracket,
                        // it takes its index. to compensate there is an offset added.
                        // (added when the item before the last open bracket is a closed bracket or a number)
        int offset2 = 0; // offset that is added when a command uses brackets.
        string fromLastOpenOnwards = args.substr(indexLastOpen);
        if(fromLastOpenOnwards.find_first_of(")") != string::npos){
            size_t indexFirstClosed = fromLastOpenOnwards.find_first_of(')') + indexLastOpen;
            if(isdigit(args[indexLastOpen-1]) || args[indexLastOpen-1] == ')'){args.insert(indexLastOpen,"*"); offset = 1;}
            if(isalpha(args[indexLastOpen-1])){offset2 = 1;}
            indexFirstClosed += offset;
            indexLastOpen += offset;
            string pseudoArgs = args.substr(indexLastOpen+1,indexFirstClosed-indexLastOpen-1);
            vstrbrackets.resize(iter+1);
            if(pseudoArgs == ""){pseudoArgs = "0";}
            pushToVstr(pseudoArgs,vstrbrackets[iter]);
            calculate(vstrbrackets[iter]);
            if(isalpha(args[indexLastOpen-1])){args.replace(indexLastOpen,1,"[");args.replace(indexFirstClosed,1,"]");}
            string s(vstrbrackets[iter][0]);
            args.insert(indexFirstClosed+1-offset2,s);
            args.erase(indexLastOpen + offset2,indexFirstClosed-indexLastOpen+1-(2*offset2));
            iter++;
        }
        else
        {
            throwException(CalcException::SYNTAXERROR,indexLastOpen);
            break;
        }
        
    }
    if(args.find(')') != string::npos){throwException(CalcException::SYNTAXERROR,args.find(')'));} //it is only ever alone if found
    }

    pushToVstr(args,vstrnumbers);
    calculate(vstrnumbers);

    if(canCalculate)
    {
        double _ans;
        _ans = trystod(vstrnumbers[0]);

        // if the difference between the answer and the nearest integer is less than "toleranceValue" round it
        double toRound;
        bool toAdd;
        if(fmod(_ans,1) > 0.5){toRound = 1-fmod(_ans,1);}
        else{toRound = fmod(_ans,1);}

        if(toRound <= toleranceValue){_ans = round(_ans);}
        
        //if it is empty print 0, if not then answer
        (vstrnumbers[0] != "") ? vstrnumbers[0] = to_string_precise(_ans) : vstrnumbers[0] = "0";
        cout << "\e[1;32m" << " =" << "\e[0m" << vstrnumbers[0];
    }

    cout << "\n";
    //system("pause");
    Start(false);
}


double trystod(const string str)
{
    double returnValue;
    try
    {
        int MINUS_COUNT = 0; // its gobsmacking how i have to set this to 0 because '++;' has undefined behaviour otherwise
        for (int i = 0; i < str.size(); i++)
        {
            char ch = str[i];
            if(ch == '-')
            {
                MINUS_COUNT++;
            }
            else
            {
                break;
            }
        }
        if(str == "."){returnValue = 0;}else{
        returnValue = stod(str.substr(MINUS_COUNT));}
        (MINUS_COUNT % 2 == 0)? returnValue = returnValue: returnValue = -returnValue;
    }
    catch(out_of_range)
    {
        returnValue = INT_MAX;
        throwException(CalcException::OVERFLOWERROR);
    }
    return returnValue;
}


template <typename T>
string to_string_precise(const T __val, const int __precision)
{
    ostringstream oss;
    oss.precision(__precision);
    oss << fixed << __val;
    string ossstr = oss.str();
    size_t decimal_index = ossstr.find('.');
    string no_decimal_ossstr = ossstr;
    no_decimal_ossstr.erase(no_decimal_ossstr.begin() + decimal_index);
    size_t last_non_zero = no_decimal_ossstr.find_last_not_of('0');
    int offset;
    (decimal_index == last_non_zero + 1) ? offset = 1: offset = 2;
    ossstr.resize(last_non_zero + offset);
    return ossstr;
}

void printVstr(vector<string> v)
{
    cout << "\n";
    cout << "{";
    for (size_t i = 0; i < v.size(); i++)
    {
        if(i+1 == v.size())
        {
            cout << v[i];
        }
        else
        {
            cout << v[i] << ", ";
        }
    }
    cout << "}";
}


double degrees_to_radians(const double __val)
{
    return __val * (M_PI/180.0);
}
double radians_to_degrees(const double __val)
{
    return __val * (180/M_PI);
}

void throwException(CalcException error, int const index, const string message)
{
    //stops it from calculating a clear error
    canCalculate = false;

    // places a caret where the error occured
    if(index!=-1)
    {
        for (int i = -1; i < index; i++){cout << " ";}
        cout << "\e[1;31m" << "^" << "\e[0m" << "\n";
    }

    switch (error)
    {
    case CalcException::SYNTAXERROR:
        cout << "\e[1;31m" << "SYNTAX ERROR";
        if(message!=""){cout << ":" << message;}
        cout << "\e[0m";
        break;
    case CalcException::MATHERROR:
        cout << "\e[1;31m" << "MATH ERROR";
        if(message!=""){cout << ":" << message;}
        cout << "\e[0m";
        break;
    case CalcException::OVERFLOWERROR:
        cout << "\e[1;31m" << "OVERFLOW ERROR";
        if(message!=""){cout << ":" << message;}
        cout << "\e[0m";
        break;
    default:
        break;
    }
}