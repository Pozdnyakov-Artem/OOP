#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include "social_network.hpp"

using namespace std;

Social_network::User::User(const string new_id)
{   
    id = new_id;
}

string Social_network::User:: get_id() const
{
    return id;
}

vector<pair<time_t,string>> Social_network::User::get_posts() const
{
    return posts;
}

void Social_network::User::set_new_post(const int date, const string& text)
{
    this->posts.emplace_back(pair(date, text));
}

void Social_network::User::set_new_friend(shared_ptr<User> new_friend)
{
    this->friends.push_back(new_friend);
}

bool Social_network::User::check_friend(shared_ptr<User> people)
{
    auto shared_friends = this->get_friends();

    return find_if(shared_friends.begin(), shared_friends.end(), [&people](shared_ptr<User> user_ptr){
        return user_ptr->get_id() == people->get_id();
    }) == shared_friends.end();
}

vector<shared_ptr<Social_network::User>> Social_network::User::get_friends() const
{
    vector<shared_ptr<User>> frendi;
    for(const auto i : this->friends)
    {
        if(auto friend_ptr = i.lock())
        {
            frendi.push_back(friend_ptr);
        }
    }

    return frendi;
}
Social_network::Social_network(const vector<vector<string>> &list_of_users, vector <vector <string>> &posts)
{
    for(const auto row : list_of_users)
    {
        for(const auto name : row)
        {
            if(isAlphanumericRegex(name))
            {
                if(!user_exist(unique_id, name))
                {
                    auto new_user = make_shared <User>(name);
                    users.push_back(new_user);
                    unique_id.push_back(name);
                }
            }
        }

        auto main_it = find_if(users.begin(), users.end(),[&row](const shared_ptr<User> user_ptr){
            return user_ptr->get_id()==row[0];
        });

        if(main_it == users.end()) continue;

        for(size_t i = 1; i< row.size(); i++)
        {
            auto friend_it = find_if(users.begin(), users.end(), [&row, &i](shared_ptr<User> user_ptr){
                return user_ptr->get_id() == row[i];
            });
            if(friend_it != users.end() && (*main_it)->check_friend(*friend_it))
            {
                (*friend_it)->set_new_friend(*main_it);
                (*main_it)->set_new_friend(*friend_it);
            }
        }
    }

    for(const auto row : posts)
    {
        auto user_it = find_if(users.begin(), users.end(), [&row](shared_ptr<User> user_ptr){
            return user_ptr->get_id() == row[1];
        });

        if(user_it == users.end()) continue;

        auto time = parse_iso8601(row[0]);

        // cout<<ctime(&time)<<' ';

        string text = row[2];

        for(size_t i = 3; i < row.size(); i++)
        {
            text += ' ' + row[i];
        }

        (*user_it)->set_new_post(time, text);

    }


}
bool Social_network::user_exist(const vector<string> unique_id, const string name_user) 
{
    return (find(unique_id.begin(), unique_id.end(), name_user) != unique_id.end());
}

bool Social_network::isAlphanumericRegex(const string& str) 
{
    regex pattern("^[a-zA-Z0-9]*$");
    return regex_match(str, pattern);
}

time_t Social_network::parse_iso8601(const std::string& date)
{
    std::tm tm = {};
    std::istringstream ss(date);
    
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse ISO8601 date: " + date);
    }
    
    std::time_t time_t_val = std::mktime(&tm);
    return time_t_val;
}

vector<pair<time_t, string>> Social_network::posts_of_friends(const string & name_user, Type_of_lenta type_lenta)
{
    auto user_it = find_if(users.begin(), users.end(), [&name_user](shared_ptr<User> user_ptr){
        return user_ptr->get_id() == name_user;
    });

    if(user_it == users.end())
    {
        throw ("Пользователя с именем" + name_user + "не существует");
    }

    vector <pair<time_t, string>> posts;

    auto frendi = (*user_it)->get_friends();

    for(const auto i : frendi)
    {
        for(const auto j : i->get_posts())
        {
            auto post_it = find_if(posts.begin(), posts.end(), [&j](pair<time_t, string> post){
                return post.first == j.first && post.second == j.second;
            });

            if(post_it == posts.end())
            {
                posts.push_back(j);
            }
        }

        if(type_lenta == Type_of_lenta::FRIENDS_OF_FRIENDS)
        {

            for(const auto k : i->get_friends())
            {
                if(k->get_id() == name_user) continue;

                for(const auto j : k->get_posts())
                {
                    auto post_it = find_if(posts.begin(), posts.end(), [&j](pair<time_t, string> post){
                        return post.first == j.first && post.second == j.second;
                    });

                    if(post_it == posts.end())
                    {
                        posts.push_back(j);
                    }
                }
            }
        }
    }

    sort(posts.begin(), posts.end(), [](pair<time_t, string> post_1, pair<time_t, string> post_2){
        return post_1.first > post_2.first;
    });

    cout << "Lenta for  " + name_user << endl;

    for(auto i : posts)
    {
        cout<< ctime(&i.first) << i.second << "\n\n";
    }

    return posts;
}

int Social_network::find_way(const string& name1, const string& name2)
{

    auto name1_it = find_if(users.begin(), users.end(), [&name1](shared_ptr<User> user_ptr){
        return user_ptr->get_id() == name1;
    });

    if(name1_it == users.end())
    {
        throw("Пользователя "+name1+" не существует");
    }

    auto name2_it = find_if(users.begin(), users.end(), [&name2](shared_ptr<User> user_ptr){
        return user_ptr->get_id() == name2;
    });

    if(name2_it == users.end())
    {
        throw("Пользователя "+name2+" не существует");
    }

    vector <shared_ptr<User>> queue;

    unordered_map <string, string> parents;

    parents.emplace(name1, "000");

    for(const auto i : (*name1_it)->get_friends())
    {
        queue.push_back(i);
        parents.emplace(i->get_id(), name1);
    }

    while(queue.size()>0)
    {
        auto i = queue.front();
        queue.erase(queue.begin());
        
        if(i->get_id() == name2)
            break;

        for(const auto j : i->get_friends())
        {
            if(parents.count(j->get_id()) == 0)
            {
                queue.push_back(j);
                parents.emplace(j->get_id(), i->get_id());
            }
        }        
    }

    if(parents.count(name2) == 0)
    {
        cout<<"Пути не существует"<<endl;
        return -1;
    }
    else
    {
        string nex = name2;
        int count = 0;
        while(parents[nex] != "000")
        {
            nex = parents[nex];
            count++;
        }
        cout<<count<<endl;
        return count;
    }

}

void Social_network::check_users()
{
    for(auto i : this->users)
    {
        cout<<i->get_id()<<": ";
        for(const auto j : i->get_friends())
        {
            cout<<j->get_id()<<' ';
        }
        cout<<endl;
        for(const auto j : i->get_posts())
        {
            cout<<ctime(&(j.first))<<j.second<<' ';
        }
        cout<<endl;
    }
}

vector<string> splitBySpaces(const string& str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

auto read_users_and_posts(const string file_path)
{
    ifstream file;
    file.open(file_path);

    string str;
    vector<vector<string>> users;

    while(getline(file,str))
    {
        if(str=="")
        {
            break;
        }

        users.emplace_back(splitBySpaces(str));
    }

    vector <vector<string>> posts;

    if(!file.eof())
    {
        while(getline(file,str))
        {
            posts.emplace_back(splitBySpaces(str));
        }
    }    

    return make_pair(users,posts);
}



int main()
{
    // setlocale(LC_ALL, "Rus");

    auto [users, posts] = read_users_and_posts("file.txt");

    auto Soc = Social_network(users, posts);

    // Soc.check_users();
    Soc.posts_of_friends("sasha",Social_network::Type_of_lenta::FRIENDS_OF_FRIENDS);

    Soc.find_way("vasya", "bolik");

    return 0;
}