#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <functional>

class choice {
    using branch_f = std::function<void()>;
    
    // std::vector<choice_branch> _branches;
    std::vector<std::tuple<std::string, branch_f>> _branches;
  public:
    choice() = default;
    choice(const choice &) = delete;
    choice(choice &&) = delete;
    ~choice() = default;

    choice &add_branch(std::string choice, branch_f branch) noexcept;
    void branch(const std::string& message);
};