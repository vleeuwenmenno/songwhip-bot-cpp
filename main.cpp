#include <nlohmann/json.hpp>
#include <iostream>
#include <websocketpp/client.hpp>

#include "songwhip_lib/songwhip.cpp"
#include "sleepy_discord/sleepy_discord.h"

using json = nlohmann::json;
using namespace std;

// Get current date/time, format is YYYY-MM-DD HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue() {
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

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
                    cout << "[" << currentDateTime() << "] "<< msg.author.username << "#" << msg.author.discriminator << " requested " << msg.content << " in " << msg.channelID.string() << " (VmRSS: " << (getValue() / 1024) << " MB)" << endl;
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