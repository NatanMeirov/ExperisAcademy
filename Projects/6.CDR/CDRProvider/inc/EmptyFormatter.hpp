#include "IFormatter.hpp"

// An example formatter (testing reasons)
class EmptyFormatter : public nm::cdr::IFormatter {
public:
    virtual void Format(const std::string& a_file) override { }
};