#pragma once

#include <string>

class CanToggleObjects {
public:
	virtual void enableObject(const std::string& label, bool enable) = 0;
	virtual bool isObjectEnabled(const std::string& label) const = 0;

	virtual std::vector<std::string> getObjectLabels() const = 0;
};