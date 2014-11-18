#include <Engine/Utils/InitialSettings.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

using namespace std;

namespace engine
{
    InitialSettings *InitialSettings::instance = 0;

    InitialSettings& InitialSettings::Get()
    {
        if(instance)
            return *instance;
        return *(instance = new InitialSettings("Settings.ini"));
    }

    InitialSettings::InitialSettings(const string &path)
    {    
        format["GL_NEAREST"] = GL_NEAREST;
        format["GL_LINEAR"] = GL_LINEAR;

        ifstream f(path);
        if(f.is_open())
        {
            while(!f.eof())
            {
                string name, value;
                f >> name;
                if(name.substr(0, 1) == "#")
                {
                    f >> value;
                    name = name.substr(1);
                    try
                    {
                        if(value[0] == 'f')
                        {
                            float fl = stof(value.substr(1));
                            values[name] = (void*)*(unsigned int*)&fl;
                        }
                        else
                            values[name] = (void*)stoi(value);
                    }
                    catch(exception &ex)
                    {
                        values[name] = (void*)format[value];
                    }
                }
            }
        }
        else
        {
            cerr << "Cannot open graphics settings" << endl;
            exit(-1);
        }
        f.close();
    }


    void* InitialSettings::operator[](const string &variableName)
    {
        return values[variableName];
    }
}
