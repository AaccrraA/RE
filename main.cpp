#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
using namespace std;

struct Element {
	char symbol;
	vector<int> dependsOn;

	void addDependsOn(int d) {
		bool isHere = false;
		for (int i = 0; i < dependsOn.size(); i++) {
			if (dependsOn[i] == d) {
				isHere = true;
				break;
			}
		}
		if (!isHere) {
			dependsOn.resize(dependsOn.size() + 1);
			dependsOn[dependsOn.size() - 1] = d;
		}
	}

};

struct Cortege {
private:
	string X;
	vector <Element> R;

	void loadFromFile() {
		ifstream ifs("RE.txt");
		string field;
		string rStr;
		if (ifs.fail()) {
			cerr << "Ошибка открытия файла.";
			exit(-1);
		}
		do {
			getline(ifs, field, '\n');
			if (field == "X:") {
				getline(ifs, field);
				for (int i = 0; i < field.length(); i++)
				if (field[i] == ' ' || field[i] == '\n')
					field.erase(i, 1);
				X = field;
			}
			else if (field == "R:") {
				getline(ifs, field);
				for (int i = 0; i < field.length(); i++)
				if (field[i] == ' ' || field[i] == '\n')
					field.erase(i, 1);
				rStr = field;
			}
		} while (!ifs.eof());

		R.resize(rStr.length()*2 + 1);

		int k = 0;
		for (int i = 0; i < (rStr.length() * 2 + 1); i++) {
			if (i % 2 == 0) {
				R[i].symbol = '_';
			}
			else {
				R[i].symbol = rStr[k];
				k++;
			}
		}
	}
	void defineDependings() {
		firstRule();
		secondRule();
		thirdRule();
//		while (forthRule());
	}
    
	void firstRule() {
		/*œÂ‚ÓÂ Ô‡‚ËÎÓ:
		Õ‡˜‡Î¸Ì˚Â ÏÂÒÚ‡ ‚ÒÂı ÔÂÂÏÂÌÌ˚ı
		ÔÓÏÂ˘ÂÌÌ˚ı ‚ ËÚÂ‡ˆËÓÌÌ˚Â ÒÍÓ·ÍË
		ÔÓ‰˜ËÌÂÌ˚ ÏÂÒÚÛ ‡ÒÔÓÎÓÊÂÌÌÓÏÛ ÒÎÂ‚‡ ÓÚ ÓÚÍ˚‚‡˛˘ÂÈ ÒÍÓ·ÍË*/
		int sLvl = 0;
        int iLvl = 0;
		for (int i = 0; i < R.size(); i++) {
			if (R[i].symbol == '(') {
				sLvl = 1;
				iLvl = 0;
				for (int j = i + 1; (j < R.size() && sLvl != 0); j++) {
					if (X.find(R[j].symbol) != string::npos && sLvl == 1 && iLvl == 0)
						R[j-1].addDependsOn(i - 1);
					else if (R[j].symbol == '(')
						sLvl++;
					else if (R[j].symbol == ')')
						sLvl--;
					else if (R[j].symbol == '<')
						iLvl++;
					else if (R[j].symbol == '>')
						iLvl--;
				}
				continue;
			}
			else if (R[i].symbol == '<') {
				iLvl = 1;
				sLvl = 0;
				for (int j = i + 1; (j < R.size() && iLvl != 0); j++) {
					if (X.find(R[j].symbol) != string::npos && iLvl == 1 && sLvl == 0)
						R[j-1].addDependsOn(i - 1);
					else if (R[j].symbol == '<')
						iLvl++;
					else if (R[j].symbol == '>')
						iLvl--;
					else if (R[j].symbol == '(')
						sLvl++;
					else if (R[j].symbol == ')')
						sLvl--;
				}
				continue;
			}
		}
	}

	void secondRule() {
        /*Второе правило:
         Место расположенное справа от закрывающей скобки
         подчинено конечным местам термов многочлена*/
		int sLvl = 0;
        int iLvl = 0;
        for (int i = R.size(); i > 0; i--) {
			if (R[i].symbol == ')') {
				sLvl = 1;
				iLvl = 0;
				for (int j = i - 1; (j > 0 && sLvl != 0); j--) {
					if (X.find(R[j].symbol) != string::npos && sLvl == 1 && iLvl == 0)
						R[i+1].addDependsOn(j + 1);
					else if (R[j].symbol == ')')
						sLvl++;
					else if (R[j].symbol == '(')
						sLvl--;
					else if (R[j].symbol == '>')
						iLvl++;
					else if (R[j].symbol == '<')
						iLvl--;
				}
				continue;
			}
			else if (R[i].symbol == '>') {
				iLvl = 1;
				sLvl = 0;
				for (int j = i - 1; (j > 0 && iLvl != 0); j--) {
					if (X.find(R[j].symbol) != string::npos && iLvl == 1 && sLvl == 0)
						R[i+1].addDependsOn(j + 1);
					else if (R[j].symbol == '>')
						iLvl++;
					else if (R[j].symbol == '<') {
                        if (iLvl == 1) {
                            R[i+1].addDependsOn(j - 1);
                            iLvl--;
                        }
                    }
					else if (R[j].symbol == ')')
						sLvl++;
					else if (R[j].symbol == '(')
						sLvl--;
				}
				continue;
			}
		}
    }
    
	void thirdRule() {
        int iLvl = 0;
        for (int i = R.size(); i > 0; i--) {
            if (R[i].symbol == '>') {
                iLvl = 1;
                for (int j = i - 1; (j > 0 && iLvl != 0); j--) {
                    if (X.find(R[j].symbol) != string::npos && iLvl == 1) {
                        R[j-1].addDependsOn(i + 1);
                    }
                    else if (R[j].symbol == '<') {
                        iLvl--;
                    }
                }
            }
        }
    }
//	bool forthRule();

public:
	void initDefault() {
		loadFromFile();
		defineDependings();
	}
};

int main() {
	setlocale(LC_ALL, "rus");

	Cortege C;
	C.initDefault();

	return 0;
}