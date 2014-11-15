#ifndef VERIFICATION_RESULTS_H
#define VERIFICATION_RESULTS_H

#include <vector>
#include <string>

namespace noises
{
    class ValidationResults
    {
    public:
        ValidationResults();

        void add(const std::string& message);

        operator bool() const;
        const std::vector<std::string> messages() const;

    private:
        std::vector<std::string> messages_;
        bool is_valid_;
    };
}



#endif // VERIFICATION_RESULTS_H
