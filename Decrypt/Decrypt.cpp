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
    vector<string> lower, upper;
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
            int index = -1;
            for (int i = 0; i < lower.size(); i++) {
                if (lower[i] == tmp) {
                    index = i;
                    break;
                }
            }
            MostPopI.push_back(index);
        }
    }
};

LangParams lg[] = { LangParams(), LangParams(0.065, "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIGKLMNOPQRSTUVWXYZ", "etaonris","e"), LangParams(0.07, "абвгдежзийклмнопрстуфхцчшщъыьэюя","АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ","ао", "а") };

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
            if (counts[IofMaxChar] < counts[i]) {
                IofMaxChar = i;
                MaxChar = low[i];
            }
            res += this->counts[this->counts.size()-1] * (this->counts[this->counts.size() - 1]);
        }
        //for (int i = 0; i < this->counts.size(); i++) {
        //    res += this->counts[i];
        //}
        //cout << "RES " << res << endl;
        res /= double(this->init.length() * (this->init.length() ));
        return res;
    }

    void Encrypt() {
        int len = lg[language].len_of_char;
        int alpCount = lg[language].lower.size();
        vector<string> low = lg[language].lower, up = lg[language].upper;
        string tmp;
        to_ret = "";
        for (int i = 0; i < init.length() / len; i += len) {
            tmp = "";
            for (int j = 0; j < len; j++) {
                tmp += init[i + j];
            }
            int NumInLowAlp = -1;
            int NumInUpAlp = -1;
            for (int j = 0; j < alpCount; j++) {
                if (tmp == low[j] ) {
                    NumInLowAlp = j;
                    break;
                }
                if (tmp == up[j]) {
                    NumInUpAlp = j;
                    break;
                }
            }
            if (NumInLowAlp > -1 || NumInUpAlp > -1) {
                if (NumInLowAlp > -1) {
                    to_ret += low[(NumInLowAlp - key) < 0? alpCount  + (NumInLowAlp - key) : NumInLowAlp - key];
                }
                if (NumInUpAlp > -1) {
                    to_ret += up[(NumInUpAlp - key) < 0 ? alpCount + (NumInUpAlp - key) : NumInUpAlp - key];
                }
            }
            else {
                for (int j = 0; j < len; j++) {
                    to_ret += init[i + j];
                }
            }
        }
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
    SubString(string insert, bool light = 1) {
        init = insert;
        language = LNGDetector(init);
        /*for (int i = 0; i < this->counts.size(); i++) {
            cout << i << " " << this->counts[i]<<endl;
        }*/
        MIndex = MatrixIndex();
        if (!light) {
            CesarNextStep();
        }
    }

    void KeyChange(int i) {
        key = i;
        while (key >= lg[language].lower.size()) {
            key -= lg[language].lower.size();
        }
        Encrypt();
    }

    void CesarNextStep() {
        if (++curPopChar >= lg[language].MostPopI.size()) {
            curPopChar = 0;
        }
        key = (IofMaxChar - lg[language].MostPopI[curPopChar] );
        if (key < 0) {
            key += lg[language].lower.size();
        }
        Encrypt();
    }
    void CesarPrevStep() {
        if (--curPopChar < 0) {
            curPopChar = lg[language].MostPopI.size()-1;
        }
        key = (IofMaxChar - lg[language].MostPopI[curPopChar]);
        if (key < 0) {
            key += lg[language].lower.size();
        }
        Encrypt();
    }
    
};

vector<SubString> StringFraction(string input, int count, bool light = 1) {
    string tmp;
    vector<SubString> res;
    for (int i = 0; i < count; i++) {
        tmp = "";
        for (int j = 0; i + j * count < input.length(); j++) {
            int for_show = i + j * count;
            tmp += input[i + j * count];
        }
        if (tmp.length() > 0) {
            res.push_back(SubString(tmp,light));
        }
        else break;
    }
    return res;
}

string FractedStr(vector<SubString> VecSub) {
    string to_ret = "";
    int len = 0;
    int size = VecSub.size();
    for (int i = 0; i < size; i++) {
        len += VecSub[i].to_ret.length();
    }
    for (int i = 0; i < len; i++) {
        to_ret += VecSub[i % size].to_ret[i / size];
    }
    return to_ret;
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
    string txt = "efefefefefefe";
    vector<double> r = FindAllIndex(txt);
    lang cur = LNGDetector(txt);
    // you can loop k higher to see more color choices
    //for (int k = 1; k < 255; k++)
    //{
    //    // pick the colorattribute k you want
    //    SetConsoleTextAttribute(hConsole, k);
    //    cout << k << " I want to be nice today!" << endl;
    //}
    bool ProgWork = true;
    int chosenV = 0;
    int chosenH = 0;
    bool change = 1;
    vector<SubString> VecSub = StringFraction(txt,chosenV + 1,0);
    string decoded;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while (ProgWork) {
        if (change) {
            system("cls");
            decoded = FractedStr(VecSub);
            cout << decoded << endl<<endl<<endl<<"KeyWord:\n";
            for (int i = 0; i < VecSub.size(); i++) {
                cout << lg[cur].lower[VecSub[i].key];
                //cout << VecSub[i].key<<" ";
            }
            cout << endl;
            for (int i = 0; i < chosenH; i++) {
                cout << ' ';
            }
            cout << '^';
            cout << endl << endl << endl;
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
                if (chosenV == i) {
                    cout << ">";
                }
                else {
                    cout << " ";
                }
                cout << "Key len = " << i + 1 << " Match index = " << r[i] << endl;
            }
            SetConsoleTextAttribute(hConsole, 7);
            
            cout << endl << endl << endl <<VecSub[0].IofMaxChar<< "Directions - Arrows\nNext auto - Enter\nPrev auto - alt\nClose - Esc";

            change = 0;
        }

        //0x41 A key
        //0x30 0 key

        for (int i = 65; i <= 90; i++) {
            if (GetAsyncKeyState(i)) {
                VecSub[chosenH].KeyChange(i - 65);
                change = 1;
                if (++chosenH >= VecSub.size()) {
                    chosenH = 0;
                }
            }
        }

        if (GetAsyncKeyState(VK_DOWN))
        {
            change = 1;
            if (++chosenV >= r.size()) {
                chosenV = 0;
            }
            VecSub.clear();
            VecSub = StringFraction(txt, chosenV + 1, 0);
        }

        if (GetAsyncKeyState(VK_UP))
        {
            change = 1;
            if (--chosenV < 0) {
                chosenV = r.size() - 1;
            }
            VecSub.clear();
            VecSub = StringFraction(txt, chosenV + 1, 0);
        }


        if (GetAsyncKeyState(VK_RIGHT) & 0x27)
        {
            change = 1;
            if (++chosenH >= VecSub.size()) {
                chosenH = 0;
            }
        }

        if (GetAsyncKeyState(VK_LEFT) & 0x25)
        {
            change = 1;
            if (--chosenH < 0) {
                chosenH = VecSub.size() - 1;
            }
        }

        if (GetAsyncKeyState(VK_RETURN) & 0x0D)
        {
            VecSub[chosenH].CesarNextStep();
            change = 1;
        }
        if (GetAsyncKeyState(VK_MENU))
        {
            VecSub[chosenH].CesarPrevStep();
            change = 1;
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
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