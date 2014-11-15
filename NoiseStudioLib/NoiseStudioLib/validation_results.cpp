#include "validation_results.h"

namespace noises
{
    ValidationResults::ValidationResults() : is_valid_(true)
    {

    }

    void ValidationResults::add(const std::string &message)
    {
        is_valid_ = false;
        messages_.push_back(message);
    }

    ValidationResults::operator bool() const
    {
        return is_valid_;
    }

    const std::vector<std::string> ValidationResults::messages() const
    {
        return messages_;
    }
}
