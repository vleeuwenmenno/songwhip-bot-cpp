#include <string>
#include <vector>
#include <map>

#include <nlohmann/json.hpp>

#include "Link.cpp"

using namespace std;
using json = nlohmann::json;

namespace Songwhip
{
    /**
     * @brief Artist details related to a parent RequestResult.
     * 
     */
    struct Artist
    {
        public:
            int id;

            string path;
            string name;
            string description;
            string url;
            string imageUrl;

            time_t createdAt;
            time_t updatedAt;
            time_t refreshedAt;

            vector<string> linksCountries;
            vector<Link> links;

            static Artist fromJson(json element);
    };

    Artist Artist::fromJson(json element)
    {
        Artist a;
        a.path = element.at("path").get<string>();
        a.name = element.at("name").get<string>();
        a.description = element.at("description").get<string>();
        a.url = element.at("url").get<string>();
        a.imageUrl = element.at("image").get<string>();

        a.createdAt = ParseISO8601(element.at("createdAt").get<string>());
        a.updatedAt = ParseISO8601(element.at("updatedAt").get<string>());
        a.refreshedAt = ParseISO8601(element.at("refreshedAt").get<string>());

        for ( auto &mol : element["links"].get<json::object_t>() )
        {
            a.links.push_back(Link::fromJson(mol.first, mol.second));
        }
        
        return a;
    }
}