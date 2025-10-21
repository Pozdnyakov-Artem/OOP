#include <string>
#include <vector>
#include <regex>

class Social_network{

private:
    class User{

    private:
        std::string id;
        std::vector <std::weak_ptr <User>> friends;
        std::vector <std::pair <std::time_t, std::string>> posts;

    public:
        User(const std::string new_id);
        std::string get_id() const;
        std::vector<std::pair <std::time_t, std::string>> get_posts() const;
        std::vector<std::shared_ptr<User>> get_friends() const;
        void set_new_post(int date, const std::string&text);
        void set_new_friend(std::shared_ptr<User> new_friend);
        bool check_friend(std::shared_ptr<User> people);
    };

    std::vector<std::shared_ptr<User>> users;
    std::vector<std::string> unique_id;

public:

    enum class Type_of_lenta{
        FRIENDS,
        FRIENDS_OF_FRIENDS
    };

    Social_network(const std::vector<std::vector<std::string>> &list_of_users, std::vector<std::vector<std::string>> &posts);
    void check_users();
    static bool user_exist(const std::vector<std::string> unique_id, const std::string name_user);
    static bool isAlphanumericRegex(const std::string& str);
    static std::time_t parse_iso8601(const std::string & date);
    std::vector <std::pair<std::time_t, std::string>> posts_of_friends(const std::string& name_user, Type_of_lenta type_lenta);
};

std::vector<std::string> splitBySpaces(const std::string& str);
auto read_users_and_posts(const std::string file_path);