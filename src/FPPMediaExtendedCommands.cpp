#include <fpp-pch.h>
#include "Plugin.h"
#include "Plugins.h"
#include "commands/Commands.h"
#include "mediadetails.h"

class FPPMediaExtendedCommands : public FPPPlugins::Plugin
{
public:
  FPPMediaExtendedCommands() : FPPPlugins::Plugin("fpp-media-extended-commands")
  {
    registerCommand();
  }
  virtual ~FPPMediaExtendedCommands() {}

  void registerCommand()
  {
    CommandManager::INSTANCE.addCommand(new TriggerPresetCommandWithMetadata(this));
  }

  void getMediaMetadata(std::map<std::string, std::string> &metadata)
  {
    metadata["ARTIST"] = MediaDetails::INSTANCE.artist;
    metadata["TITLE"] = MediaDetails::INSTANCE.title;
    metadata["ALBUM"] = MediaDetails::INSTANCE.album;
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
      plugin->getMediaMetadata(metadata);
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
  FPPPlugins::Plugin *createPlugin()
  {
    return new FPPMediaExtendedCommands();
  }
}
