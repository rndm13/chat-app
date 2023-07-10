#include "model.hpp"

Message::Message() {}
Message::Message(const boost::array<char, 256> &_contents,
                 const boost::array<char, 64> &_user_name)
    : contents(_contents), user_name(_user_name) {}
Message::Message(std::string_view _contents, std::string_view _user_name) {
    contents = {};
    user_name = {};
    for (u64 index = 0; index < _contents.size(); index++) {
        contents[index] = _contents[index];
    }
    for (u64 index = 0; index < _user_name.size(); index++) {
        user_name[index] = _user_name[index];
    }
}

Message Message::from_message_string(std::string_view sv) {
    u64 user_start = sv.find('[') + 1;
    u64 user_end = sv.find(']');

    u64 content_start = sv.find('\n') + 1;
    u64 content_end = sv.find('\n', content_start);

    if (user_start == std::string_view::npos ||
        user_end == std::string_view::npos ||
        content_start == std::string_view::npos ||
        content_end == std::string_view::npos) {
        throw std::runtime_error("Invalid message");
    }

    return Message(sv.substr(content_start, content_end - content_start),
                   sv.substr(user_start, user_end - user_start));
}
