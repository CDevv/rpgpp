#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

void flattenJson(const json& j,
				 std::map<std::string, std::string>& out,
				 const std::string& prefix = "")
{
	for (auto it = j.begin(); it != j.end(); ++it) {
		std::string key = prefix.empty() ? it.key() : prefix + "." + it.key();

		if (it->is_object()) { flattenJson(*it, out, key); }
		else if (it->is_string()) { out[key] = it->get<std::string>(); }
		else { out[key] = it->dump(); }
	}
}

std::map<std::string, std::string> loadAndFlatten(const fs::path& path) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open " + path.string());

	json parsed;
	file >> parsed;

	std::map<std::string, std::string> flattened;
	flattenJson(parsed, flattened);
	return flattened;
}

void compareTranslations(
	const std::map<std::string, std::string>& base,
	const std::map<std::string, std::string>& target,
	const std::string& filename)
{
	std::vector<std::string> missing;
	std::vector<std::string> extra;
	std::vector<std::string> untranslated;

	for (const auto& [key, value] : base) {
		if (target.find(key) == target.end()) {
			missing.push_back(key);
		}
		else if (target.at(key) == value) {
			untranslated.push_back(key);
		}
	}

	for (const auto& [key, _] : target) {
		if (base.find(key) == base.end())
			extra.push_back(key);
	}

	size_t translatedCount =
		base.size() - missing.size() - untranslated.size();

	double percent =
		base.empty() ? 0.0 :
		(double)translatedCount / base.size() * 100.0;

	std::cout << "\n=== " << filename << " ===\n";
	std::cout << "Completion: " << percent << "%\n";

	if (!missing.empty()) {
		std::cout << "\nMissing keys:\n";
		for (const auto& key : missing)
			std::cout << "  - " << key << "\n";
	}

	if (!untranslated.empty()) {
		std::cout << "\nUntranslated keys (same as base):\n";
		for (const auto& key : untranslated)
			std::cout << "  - " << key << "\n";
	}

	if (!extra.empty()) {
		std::cout << "\nExtra keys:\n";
		for (const auto& key : extra)
			std::cout << "  - " << key << "\n";
	}

	if (missing.empty() && untranslated.empty() && extra.empty())
		std::cout << "Fully synced and translated!\n";
}

int main() {
	fs::path translationsDir = "resources/translations";
	fs::path baseFile = translationsDir / "en_us.json";

	try {
		auto base = loadAndFlatten(baseFile);

		for (const auto& entry : fs::directory_iterator(translationsDir)) {
			if (!entry.is_regular_file())
				continue;

			if (entry.path() == baseFile)
				continue;

			if (entry.path().extension() != ".json")
				continue;

			auto target = loadAndFlatten(entry.path());

			compareTranslations(
				base,
				target,
				entry.path().filename().string());
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
