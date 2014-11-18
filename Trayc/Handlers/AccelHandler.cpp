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
    AccelHandler::AccelHandler(void) :
        accel_cache_loaded(false)
    {
    }

    void AccelHandler::LoadAccelCache(const std::string &filename, GeometryGroup geometry_group)
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

            cerr << "acceleration cache file found: '" << cachefile << "' (" << size << " bytes)\n";

            if(sizeof(size_t) <= 4 && size >= 0x80000000ULL)
            {
                cerr << "[WARNING] acceleration cache file too large for 32-bit application.\n";
                accel_cache_loaded = false;
                return;
            }

            char *data = new char[static_cast<size_t>(size)];
            in.read(data, static_cast<streamsize>(size));

            // Load data into accel
            Acceleration accel = geometry_group->getAcceleration();
            try
            {
                accel->setData(data, static_cast<RTsize>(size));
                accel_cache_loaded = true;
            }
            catch(Exception &e)
            {
                cerr << "[WARNING] could not use acceleration cache, reason: " << e.getErrorString() << endl;
                accel_cache_loaded = false;
            }

            delete[] data;

        }
        else
        {
            accel_cache_loaded = false;
            cerr << "no acceleration cache file found\n";
        }
        in.close();
    }

    void AccelHandler::SaveAccelCache(const std::string &filename, GeometryGroup geometry_group)
    {
        // If accel caching on, marshallize the accel 
        if(!accel_cache_loaded)
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
                cerr << "could not open acceleration cache file '" << cachefile << "'" << endl;
                return;
            }
            out.write(data, size);
            delete[] data;
            cerr << "acceleration cache written: '" << cachefile << "'" << endl;
        }
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
