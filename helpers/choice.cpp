#include "choice.hpp"

choice& choice::add_branch(std::string choice, choice::branch_f branch) noexcept {
    _branches.emplace_back(std::move(choice), branch);
    return *this;
}

void choice::branch(const std::string& message) {
    printf("%s\n", message.c_str());
    for (int i = 0; i < _branches.size(); ++i) {
        auto choice_tup = _branches[i];
        printf("%d. %s\n", i + 1, std::get<std::string>(choice_tup).c_str());
    }

    int n;
    do {
        printf("enter a number: ");
    }
    while (scanf_s("%d", &n) != 1 || n <= 0 || n > _branches.size());
    putchar('\n');
    std::get<choice::branch_f>(_branches[n - 1])();
}