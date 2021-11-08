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

        void onMessage(SleepyDiscord::Message msg) override 
        {
            if (msg.startsWith("https://www.spotify.com/") ||
                msg.startsWith("https://spotify.com/") ||
                msg.startsWith("https://open.spotify.com/") ||
                msg.startsWith("https://deezer.com/") ||
                msg.startsWith("https://www.deezer.com/") ||
                msg.startsWith("https://youtube.com/") ||
                msg.startsWith("https://www.youtube.com/") ||
                msg.startsWith("https://music.youtube.com/") ||
                msg.startsWith("https://music.apple.com/") ||
                msg.startsWith("https://tidal.com/") ||
                msg.startsWith("https://www.tidal.com/"))
            {
                Songwhip::RequestResult requestResult = Songwhip::Requester::fetch(msg.content);
                if (requestResult.success)
                {
                    Songwhip::SongwhipResult result = requestResult.result;
                    SleepyDiscord::Embed embed;
                    SleepyDiscord::EmbedImage image;
                    SleepyDiscord::EmbedAuthor author;
                    
                    author.iconUrl = result.artists[0].imageUrl;
                    image.url = result.imageUrl;

                    embed.author = author;
                    embed.title = "Title";
                    embed.description = "Description";
                    embed.url = result.imageUrl;
                    embed.image = image;

                    embed.footer.text = "Shared by " + msg.author.username + "#" + msg.author.discriminator;
                    // embed.footer.iconUrl = msg.author.getJSONStructure();

                    sendMessage(msg.channelID, " ", embed);
                }
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