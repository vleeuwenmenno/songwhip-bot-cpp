#include <string>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "exceptions/SongwhipException.cpp"
#include "models/RequestResult.cpp"

using namespace std;
using json = nlohmann::json;

namespace Songwhip
{
    class Requester
    {
        public:
            static Songwhip::RequestResult fetch(string url);

        private:
            static size_t curlWriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
    };

    size_t Requester::curlWriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    Songwhip::RequestResult Requester::fetch(string url) 
    {
        std::map<std::string, std::string> body = { { "url", url } };
        string readBuffer;
        json j = body;
        string jsonBody = j.dump();
        CURL *hnd = curl_easy_init();

        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(hnd, CURLOPT_URL, "https://songwhip.com/");

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, jsonBody.c_str());
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, curlWriteCallback);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);
        int curl_code = curl_easy_perform(hnd);
        long http_code = 0;
        curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);

        // Make sure it resulted in something success
        if (http_code == 200 && curl_code != CURLE_ABORTED_BY_CALLBACK)
        {
            // Parse the buffer to a JSON object and cleanup curl
            json j = json::parse(readBuffer);
            curl_easy_cleanup( hnd );

            return RequestResult::fromJson(j);
        }
        else
        {
            RequestResult result;
            result.success = false;
            return result;
        }
    } 
}