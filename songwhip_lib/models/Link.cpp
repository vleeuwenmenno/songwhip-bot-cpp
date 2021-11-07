#include <string>
#include <vector>
#include <map>

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace Songwhip
{
    /**
     * @brief Link struct containing the service link and an array with available countries it's parent is listed in.
     * 
     */
    struct Link
    {
        public:
            string service;
            string link;
            vector<string> countries;

            static Link fromJson(string service, json el);
    };

    Link Link::fromJson(string service, json element)
    {
        Link l;

        l.service = service;

        if (element.is_array())
        {
            l.link = element[0].at("link").get<string>();

            if (!element[0].at("countries").is_null())
                l.countries = element[0].at("countries").get<vector<string>>();
        }

        return l;
    }
}