#include <fpp-pch.h>
#include "Plugin.h"
#include "commands/Commands.h"
#include "common.h"
#include "mediadetails.h"
#include "settings.h"

class FPPMediaExtendedCommands : public FPPPlugin
{
public:
  FPPMediaExtendedCommands() : FPPPlugin("fpp-media-extended-commands")
  {
    if (settings.find("Preset Name") == settings.end())
    {
      settings["Preset Name"] = "Media Details Handler";
    }
    CommandManager::INSTANCE.addCommand(new TriggerPresetCommandWithMetadata(this));
  }
  virtual ~FPPMediaExtendedCommands() {}

  virtual void mediaCallback(const Json::Value &playlist, const MediaDetails &mediaDetails)
  {
    std::map<std::string, std::string> metadata;
    metadata["ARTIST"] = mediaDetails.artist;
    metadata["TITLE"] = mediaDetails.title;
    metadata["ALBUM"] = mediaDetails.album;
    if (CommandManager::INSTANCE.TriggerPreset(settings["Preset Name"], metadata))
      std::make_unique<Command::Result>("Preset Triggered");
    else
      std::make_unique<Command::ErrorResult>("Not found");
  }

  class TriggerPresetCommandWithMetadata : public Command
  {

  public:
    TriggerPresetCommandWithMetadata(FPPMediaExtendedCommands *p) : Command("Trigger Command Preset with Media Metadata"), plugin(p)
    {
      args.push_back(CommandArg("name", "datalist", "Preset Name").setContentListUrl("api/commandPresets?names=true"));
    }

    std::unique_ptr<Command::Result> run(const std::vector<std::string> &args)
    {
      if (args.size() != 1)
      {
        return std::make_unique<Command::ErrorResult>("Not found");
      }

      std::map<std::string, std::string> metadata;
      metadata["ARTIST"] = MediaDetails::INSTANCE.artist;
      metadata["TITLE"] = MediaDetails::INSTANCE.title;
      metadata["ALBUM"] = MediaDetails::INSTANCE.album;
      if (CommandManager::INSTANCE.TriggerPreset(args[0], metadata))
        return std::make_unique<Command::Result>("Preset Triggered");
      else
        return std::make_unique<Command::ErrorResult>("Not found");
    }

    FPPMediaExtendedCommands *plugin;
  };
};

extern "C"
{
  FPPPlugin *createPlugin()
  {
    return new FPPMediaExtendedCommands();
  }
}
