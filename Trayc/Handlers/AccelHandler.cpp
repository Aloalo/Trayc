/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Handlers/AccelHandler.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace optix;

namespace trayc
{
    bool AccelHandler::LoadAccelCache(const std::string &filename, GeometryGroup geometry_group)
    {
        const string cachefile = GetCacheFileName(filename);
        ifstream in(cachefile.c_str(), ifstream::in | ifstream::binary);
        if(in)
        {
            unsigned long long int size = 0ull;

            // Read data from file
            in.seekg(0, ios::end);
            const ifstream::pos_type szp = in.tellg();
            in.seekg(0, ios::beg);
            size = static_cast<unsigned long long int>(szp);

            cout << "Acceleration cache file found: '" << cachefile << "' (" << size << " bytes)" << endl;

            if(sizeof(size_t) <= 4 && size >= 0x80000000ULL)
            {
                cerr << "[WARNING] Acceleration cache file too large for 32-bit application." << endl;
                return false;
            }

            char *data = new char[static_cast<size_t>(size)];
            in.read(data, static_cast<streamsize>(size));

            // Load data into accel
            try
            {
                geometry_group->getAcceleration()->setData(data, static_cast<RTsize>(size));
            }
            catch(Exception &e)
            {
                cerr << "[WARNING] could not use acceleration cache, reason: " << e.getErrorString() << endl;
                return false;
            }

            delete[] data;

        }
        else
        {
            cerr << "No acceleration cache file found" << endl;
            return false;
        }
        in.close();
        return true;
    }

    void AccelHandler::SaveAccelCache(const std::string &filename, GeometryGroup geometry_group)
    {
        const string cachefile = GetCacheFileName(filename);

        // Get data from accel
        Acceleration accel = geometry_group->getAcceleration();
        const RTsize size = accel->getDataSize();
        char* data  = new char[size];
        accel->getData(data);

        // Write to file
        ofstream out(cachefile.c_str(), ofstream::out | ofstream::binary);
        if(!out)
        {
            delete[] data;
            cerr << "Could not open acceleration cache file '" << cachefile << "'" << endl;
            return;
        }
        out.write(data, size);
        delete[] data;
        cerr << "Acceleration cache written: '" << cachefile << "'" << endl;
    }

    string AccelHandler::GetCacheFileName(const std::string &filename) const
    {
        string cachefile = filename;
        size_t idx = cachefile.find_last_of('.');
        cachefile.erase(idx);
        cachefile.append(".accelcache");
        return cachefile;
    }
}
