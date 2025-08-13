#pragma once

namespace emt
{
class application
{
public:
    application() = default;
    // executions
    int exec();

    bool m_is_running{true};
};
} // namespace emt
