#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

//Junk yard of functions
//-----------------------------------------------------------------

//double MatchIndex(string cryptedText, lang alp = eng) {
//    long int sum = 0;
//    int len = lg[alp].len_of_char;
//    vector<string> low = lg[alp].lower;
//    vector<string> up = lg[alp].upper;
//    for (char i = 0; i < low.size() / len; i++) {
//        int count = subs_str_search(cryptedText, low[i]) + subs_str_search(cryptedText, up[i]);
//        sum += (count * (count - 1));
//    }
//    cout << sum / double(cryptedText.length() / len * (cryptedText.length() - 1) / len) << endl;
//    return sum / double(cryptedText.length() / len * (cryptedText.length() - 1) / len);
//}

//-----------------------------------------------------------------
//Junk yard of functions

const int max_key = 10;
const int max_lang = 2;

//Work with languages start
//-----------------------------------------------------------------

enum lang {
    error,
    eng,
    rus
};

class LangParams {
public:
    double MI;
    int len_of_char;
    string ch;
    vector<string> lower, upper, mostPopular;
    vector<int> MostPopI;
    LangParams() {};
    LangParams(double MatchIndex, string low, string up, string most_popular_characters, string just_one_character) {
        MI = MatchIndex;
        ch = just_one_character;
        len_of_char = just_one_character.length();
        for (int i = 0; i < up.length() / len_of_char; i += len_of_char) {
            string tmp = "";
            for (int j = 0; j < len_of_char; j++) {
                tmp += up[i + j];
            }
            upper.push_back(tmp);
            tmp = "";
            for (int j = 0; j < len_of_char; j++) {
                tmp += low[i + j];
            }
            lower.push_back(tmp);
        }
        for (int i = 0; i < most_popular_characters.length() / len_of_char; i += len_of_char) {
            string tmp = "";
            for (int j = 0; j < len_of_char; j++) {
                tmp += most_popular_characters[i + j];
            }
            mostPopular.push_back(tmp);
        }
    }
};

LangParams lg[] = { LangParams(), LangParams(0.065, "abcdefghigklmnopqrstuvwxyz", "ABCDEFGHIGKLMNOPQRSTUVWXYZ", "eo","e"), LangParams(0.07, "абвгдежзийклмнопрстуфхцчшщъыьэюя","АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","ао", "а") };

//-----------------------------------------------------------------
//Work with languages end

//Work with strings start
//-----------------------------------------------------------------

int subs_str_search(string src, string sub) {
    int start = 0;
    int count = 0;
    int pos = 0;
    for (;;) {
        pos = src.find(sub.c_str(), start);
        if (pos != -1) {
            start = pos + sub.size();
            count++;
        }
        else {
            break;
        }
    }
    return count;
}

lang LNGDetector(string substr) {
    for (int i = 1; i <= max_lang; i++) {
        for (int j = 0; j < lg[i].lower.size(); j++) {
            if (substr.find(lg[i].lower[j]) != -1 || substr.find(lg[i].upper[j]) != -1) {
                return static_cast<lang>(i);
            }
        }
    }
    return error;
}

class SubString {
private:

    double MatrixIndex() {
        double res = 0;
        vector<string> low = lg[language].lower;
        vector<string> up = lg[language].upper;
        IofMaxChar = 0;
        MaxChar = low[0];
        for (int i = 0; i < low.size(); i++) {
            this->counts.push_back(subs_str_search(this->init, low[i]) + subs_str_search(this->init, up[i]));
            if (counts[IofMaxChar] > counts[i]) {
                IofMaxChar = i;
                MaxChar = low[i];
            }
            res += this->counts[this->counts.size()-1] * (this->counts[this->counts.size() - 1] - 1);
        }
        //for (int i = 0; i < this->counts.size(); i++) {
        //    res += this->counts[i];
        //}
        //cout << "RES " << res << endl;
        res /= double(this->init.length() * (this->init.length() - 1));
        return res;
    }

public:
    string init;
    string to_ret = "";
    int key = 0;
    int curPopChar = -1;
    double MIndex;
    lang language;
    string MaxChar;
    vector<int> counts;
    int IofMaxChar;
    SubString(string insert) {
        init = insert;
        language = LNGDetector(init);
        /*for (int i = 0; i < this->counts.size(); i++) {
            cout << i << " " << this->counts[i]<<endl;
        }*/
        MIndex = MatrixIndex();        
    }

    void CesarNextStep() {
        curPopChar++;
        if (++curPopChar >= lg[language].mostPopular.size()) {
            curPopChar = 0;
        }
    }
    
};

vector<SubString> StringFraction(string input, int count) {
    string tmp;
    vector<SubString> res;
    for (int i = 0; i < count; i++) {
        tmp = "";
        for (int j = 0; i + j * count < input.length(); j++) {
            int for_show = i + j * count;
            tmp += input[i + j * count];
        }
        if (tmp.length() > 0) {
            res.push_back(SubString(tmp));
        }
        else break;
    }
    return res;
}
//-----------------------------------------------------------------
//Work with strings end


//Finding Key length start
//-----------------------------------------------------------------

vector<double> FindAllIndex(string crypted) {
    lang cur = LNGDetector(crypted);
    int len = lg[cur].len_of_char;
    vector<double> res;
    if (cur != error) {
        for (int i = 1; i <= max_key; i++) {
            string sub = "";
            vector<SubString> tmp = StringFraction(crypted,i);
            double MidleIndex = 0;
            for (int j = 0; j < i; j++) {
                MidleIndex += tmp[j].MIndex;
            }
            MidleIndex /= i;
            //cout << i << " " << tmp[0].init.length() << endl;
            //res.push_back(tmp[0].MIndex);
            res.push_back(MidleIndex);
        }
    }
    return res;
}

//-----------------------------------------------------------------
//Finding Key length end


int main()
{
    string txt = "JGRMQOYGHMVBJWRWQFPWHGFFDQGFPFZRKBEEBJIZQQOCIBZKLFAFGQVFZFWWE";
    vector<double> r = FindAllIndex(txt);
    lang cur = LNGDetector(txt);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // you can loop k higher to see more color choices
    //for (int k = 1; k < 255; k++)
    //{
    //    // pick the colorattribute k you want
    //    SetConsoleTextAttribute(hConsole, k);
    //    cout << k << " I want to be nice today!" << endl;
    //}
    bool ProgWork = true;
    int chosen = 0;
    bool change = 1;
    while (ProgWork) {
        if (change) {
            system("cls");

            for (int i = 0; i < r.size(); i++) {
                SetConsoleTextAttribute(hConsole, 14);
                if (r[i] > lg[cur].MI - 0.006 && r[i] < lg[cur].MI + 0.006) {
                    SetConsoleTextAttribute(hConsole, 2);
                }
                if (r[i] > lg[cur].MI - 0.003 && r[i] < lg[cur].MI + 0.003) {
                    SetConsoleTextAttribute(hConsole, 10);
                }
                if (r[i] < lg[cur].MI - 0.02 || r[i] > lg[cur].MI + 0.02) {
                    SetConsoleTextAttribute(hConsole, 4);
                }
                if (r[i] < lg[cur].MI - 0.01 || r[i] > lg[cur].MI + 0.01) {
                    SetConsoleTextAttribute(hConsole, 12);
                }
                if (chosen == i) {
                    cout << ">";
                }
                else {
                    cout << " ";
                }
                cout << "Key len = " << i + 1 << " Match index = " << r[i] << endl;
            }
            SetConsoleTextAttribute(hConsole, 7);
            change = 0;
        }
        if (GetKeyState('S') & 0x8000)
        {
            change = 1;
            if (++chosen >= r.size()) {
                chosen = 0;
            }
        }
        if (GetKeyState('W') & 0x8000)
        {
            change = 1;
            if (--chosen < 0) {
                chosen = r.size()-1;
            }
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x0D) 
        {
            ProgWork = 0;
        }
        Sleep(200);

    }
    // you can loop k higher to see more color choices
    //for (int k = 1; k < 255; k++)
    //{
    //    // pick the colorattribute k you want
    //    SetConsoleTextAttribute(hConsole, k);
    //    cout << k << " I want to be nice today!" << endl;
    //}
}