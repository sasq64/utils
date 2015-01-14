#ifndef CHIP_PLUGIN_H
#define CHIP_PLUGIN_H

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>

#include "chipplayer.h"

namespace chipmachine {

class ChipPlugin {
public:

	using PluginConstructor = std::function<std::shared_ptr<ChipPlugin>(const std::string &)>;

	virtual ~ChipPlugin() {};

	virtual std::string name() const = 0; 

	virtual bool canHandle(const std::string &name) = 0;
	virtual ChipPlayer *fromFile(const std::string &fileName) = 0;
	virtual ChipPlayer *fromData(uint8_t *data, int size) {
		FILE *fp = fopen("tmpfile", "wb");
		fwrite(data, size, 1, fp);
		fclose(fp);
		return fromFile("tmpfile");
	}

	virtual int priority() { return 0; }

	static void createPlugins(const std::string &configDir, std::vector<std::shared_ptr<ChipPlugin>> &plugins) {
		for(auto &f : pluginConstructors()) {
			plugins.push_back(f(configDir));
		}

		std::sort(plugins.begin(), plugins.end(), [](std::shared_ptr<ChipPlugin> a, std::shared_ptr<ChipPlugin> b) -> bool {
			return a->priority() > b->priority();
		});
	}

	static void addPluginConstructor(PluginConstructor pc) {
		pluginConstructors().push_back(pc);
	}

	struct RegisterMe {
		RegisterMe(PluginConstructor f) {
			ChipPlugin::addPluginConstructor(f);
		};
	};


private:
	// Small trick to put a static variable in an h-file only
	static std::vector<PluginConstructor> &pluginConstructors() {
		static std::vector<PluginConstructor> constructors;
		return constructors;
	};

};


}

#endif // CHIP_PLUGIN_H