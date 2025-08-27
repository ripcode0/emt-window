#pragma once

namespace emt
{
class application
{
public:
    application(int args, char* argv[]);
    ~application();
    // executions
    int exec();

    bool m_is_running{true};
};
} // namespace emt
