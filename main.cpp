#include <nlohmann/json.hpp>
#include <iostream>
#include <websocketpp/client.hpp>

#include "songwhip_lib/songwhip.cpp"
#include "sleepy_discord/sleepy_discord.h"

using json = nlohmann::json;
using namespace std;

class DiscordClient : public SleepyDiscord::DiscordClient 
{
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;

        void onMessage(SleepyDiscord::Message message) override 
        {
            if (message.startsWith("https://"))
            {
                //sendMessage(message.channelID, "Hello " + message.author.username);
                string s;
                Songwhip::RequestResult result = Songwhip::Requester::fetch(message.content);
                
                if (result.type == Songwhip::ResultType::album)
                    s += "Album";
                else if (result.type == Songwhip::ResultType::track)
                    s += "Track";

                s += " '" + result.name + "' by ";

                for (Songwhip::Artist a : result.artists)
                    s += a.name + ", ";

                s = s.substr(0, s.length()-2);
                sendMessage(message.channelID, s);
            }
        }
};

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        DiscordClient client(getenv("DISCORD_TOKEN"), SleepyDiscord::USER_CONTROLED_THREADS);
        client.setIntents(SleepyDiscord::Intent::SERVER_MESSAGES);
        client.run();
    }
    else
    {
        cout << "No arguments given!" << endl;
    }
}