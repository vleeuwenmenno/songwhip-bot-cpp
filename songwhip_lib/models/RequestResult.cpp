#include <string>
#include <vector>

#include "../utils/DateTime.cpp"
#include "Artist.cpp"

using namespace std;

namespace Songwhip
{    
    /**
     * @brief Indicates what kind of RequestResult value has been retrieved.
     * 
     */
    enum ResultType 
    {
        track,
        album,
        artist
    };

    struct RequestResult;
    
    /**
     * @brief Contains details regarding a fetched track or album.
     * 
     */
    struct SongwhipResult
    {
        public:
            int id;

            ResultType type;

            string path;
            string name;
            string imageUrl;

            time_t releaseDate;
            time_t createdAt;
            time_t updatedAt;
            time_t refreshedAt;

            vector<Artist> artists;
            map<string, bool> services;
    };

    /**
     * @brief Contains results of a songwhip fetch request.
     * 
     */
    struct RequestResult
    {
        public:
            SongwhipResult result;
            bool success;

            static RequestResult fromJson(json element);
    };

    RequestResult RequestResult::fromJson(json element)
    {
        Songwhip::SongwhipResult result;

        result.path = (element.at("path").get<string>());
        result.name = (element.at("name").get<string>());
        result.imageUrl = (element.at("image").get<string>());
        
        if (element.at("type").get<string>() == "track")
            result.type = ResultType::track;
        else if (element.at("type").get<string>() == "album")
            result.type = ResultType::album;
        else if (element.at("type").get<string>() == "artist")
            result.type = ResultType::artist;
        
        if (element.contains("artists"))
        {
            for (const auto& element : element["artists"])
                result.artists.push_back(Artist::fromJson(element));
        }
        
        for ( auto &mol : element["links"].get<json::object_t>() )
        {
            result.services.emplace(mol.first, mol.second);
        }

        result.releaseDate = ParseISO8601(element.at("releaseDate").get<string>());
        result.createdAt = ParseISO8601(element.at("createdAt").get<string>());
        result.updatedAt = ParseISO8601(element.at("updatedAt").get<string>());
        result.refreshedAt = ParseISO8601(element.at("refreshedAt").get<string>());

        RequestResult reqRes;

        reqRes.result = result;
        reqRes.success = true;
        
        return reqRes;
    }
}