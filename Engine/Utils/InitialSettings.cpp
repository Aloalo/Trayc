#include <Engine/Utils/InitialSettings.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

using namespace std;

namespace engine
{
    InitialSettings::InitialSettings()
    {    
        const string path("Settings.ini");

        mFormat["GL_NEAREST"] = GL_NEAREST;
        mFormat["GL_LINEAR"] = GL_LINEAR;

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
                    if(value[0] == 'f')
                    {
                        float fl = stof(value.substr(1));
                        mValues[name] = (void*)*(unsigned int*)&fl;
                    }
                    else if(value.substr(0, 2) == "GL")
                        mValues[name] = (void*)mFormat[value];
                    else
                        mValues[name] = (void*)stoi(value);
                 
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
}
