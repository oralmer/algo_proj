#include <string>

const std::string or_in = R"JSON({
    "type":"or",
    "sub_generators":[
        {
            "type":"character",
            "characters":"1234567890"
        },
        {
            "type":"character",
            "characters":"abcdefghi"
        }
    ]
})JSON";