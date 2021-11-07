#include <nlohmann/json.hpp>
#include <iostream>
#include "songwhip_lib/songwhip.cpp"

using json = nlohmann::json;
using namespace std;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        Songwhip::RequestResult result = Songwhip::Requester::fetch(argv[1]);
        
        if (result.type == Songwhip::ResultType::album)
            cout << "Album";
        else if (result.type == Songwhip::ResultType::track)
            cout << "Track";

        string s;
        cout << " '" << result.name << "' by ";

        for (Songwhip::Artist a : result.artists)
            s += a.name + ", ";

        s = s.substr(0, s.length()-2);
        cout << s << endl;
    }
    else
    {
        cout << "No arguments given!" << endl;
    }
}