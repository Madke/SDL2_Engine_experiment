#ifndef CONF_H
#define CONF_H

#include <string>
#include <fstream>


class conf
{
    public:
        conf(std::string path = std::string("./config.ini"));
        ~conf();

        unsigned int    width();
        unsigned int    height();
        std::string     path();
        std::string     icon();

        void addLog(std::string);
        void addLog(std::string, std::string);
        void addLog(int, std::string);
        void warn(std::string);

    private:
        std::ofstream   m_log;
        std::string     m_path;
        std::string     m_icon;
        unsigned int    m_width;
        unsigned int    m_height;
};

#endif // CONF_H
