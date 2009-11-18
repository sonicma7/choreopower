#include <string>
#include <map>
#include <fstream>

using std::string;
using std::map;
using std::pair;
using std::istringstream;
using std::ifstream;

class GameConfig {
public:
	GameConfig() {}
	~GameConfig() {}

	void loadConfigFile(const string &file) {
		ifstream inFile(file.c_str());
		string readBuf;

		while(getline(inFile, readBuf, '\n')) {
			if(readBuf[0] != '#' && readBuf.size() > 0) {
				string varName, temp, varData;
				istringstream stm(readBuf);
				stm >> varName >> temp >> varData;
				mGameData[varName] = varData;
			}
		}

		inFile.close();
	}
	
	template <class T>
	bool getData(const string &varName, T &data) {
		string val = mGameData[varName];
		istringstream stm(val);
		return !(stm >> data).fail();
	}

public:
	map<string, string> mGameData;
};