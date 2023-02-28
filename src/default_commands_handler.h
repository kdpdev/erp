#pragma once

#include <functional>
#include <memory>

namespace Erp { class Enterprise; }
using CommandsHandler = std::function<void(const std::string)>;
CommandsHandler CreateDefaultCommandsHandler(std::shared_ptr<Erp::Enterprise> enterprise);
