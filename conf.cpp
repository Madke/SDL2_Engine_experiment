#include "conf.h"
#include <string>

#include <fstream>
#include <sstream>

using namespace std;

conf::conf(string path) : m_path(path), m_width(640), m_height(480)
{
    ifstream configFile;
    string configLine;

    m_log.open("./lemon.log", ios::out);

    m_log   << "Reading settings:" << endl
            << "(path: \"" << path.c_str() << "\")" << endl;

    configFile.open(path.c_str(), ios::in);
        while(getline(configFile, configLine)) {
            istringstream sin(configLine.substr(configLine.find(":") + 1));
            if (configLine.find("width") != string::npos) {
                sin >> m_width;
                addLog(m_width, "width");
            }
            else if (configLine.find("height") != string::npos) {
                sin >> m_height;
                addLog(m_height, "height");
            }
            else if (configLine.find("icon") != string::npos) {
                sin >> m_icon;
                addLog(m_icon, "icon");
            }
            else if (configLine.find("FPS") != string::npos) {
                sin >> m_FPS;
                addLog(m_FPS, "FPS");
            }
        }
    configFile.close();
}

conf::~conf()
{
    m_log.close();
}

unsigned int conf::height() {
    return this->m_height;
}

unsigned int conf::width() {
    return this->m_width;
}

unsigned int conf::fps() {
    return this->m_FPS;
}

string conf::path() {
    return this->m_path;
}

string conf::icon() {
    return this->m_icon;
}

void conf::warn(string warning) {
    addLog(warning, string("Warning"));
}

void conf::addLog(string msg, string type) {
    m_log << type << ": " << msg << endl;
}

void conf::addLog(int msg, string type) {
    m_log << type << ": " << msg << endl;
}

void conf::addLog(unsigned int msg, string type) {
    m_log << type << ": " << msg << endl;
}

void conf::addLog(string msg) {
    m_log << msg << endl;
}
