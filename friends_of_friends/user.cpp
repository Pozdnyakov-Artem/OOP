#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iostream>

using namespace std;

class Social_network{

private:
    class User{

    private:
        string id;
        vector <weak_ptr <User>> friends;
        vector <pair<int,string>> posts;

    public:
        User(const string new_id)
        {   
            id = new_id;
        }

        string get_id() const
        {
            return id;
        }

        vector<pair<int,string>> get_posts() const
        {
            return posts;
        }

        vector<weak_ptr <User>> get_friends() const
        {
            return friends;
        }

        void set_new_post(const int date, const string text)
        {
            this->posts.emplace_back(pair(date, text));
        }

        void set_new_friend(weak_ptr <User> new_friend)
        {
            cout<<"size "<< this->friends.size() << ' ';
            this->friends.push_back(new_friend);
            // shared_ptr<User> resstored = new_friend.lock();
            cout << this->friends.size()<<endl;
            // if (resstored)
            // {
            //     this->friends.push_back(resstored);
            // }
        }
    };

    int number_of_users = 0;
    vector <User> users;
    vector <shared_ptr<User>> ptr_users;
    vector <string> unique_id;

public:
    Social_network(const vector<vector <string>>& start_users)
    {
        for(auto row : start_users)
        {   
            int ind_of_main_user = -1;
            int count = 0;
            for(auto id_user : row)
            {

                if(isAlphanumericRegex(id_user))
                {
                    if(!user_exist(unique_id, id_user))
                    {
                        users.emplace_back(User(id_user));
                        unique_id.emplace_back(id_user);
                        ptr_users.emplace_back(make_shared<User>(users[number_of_users]));
                        cout<<ind_of_main_user<<' '<< count << endl;
                        if(ind_of_main_user == -1 && count == 0){
                            ind_of_main_user = number_of_users;
                            cout<<id_user<<endl;
                        }
                        number_of_users++;
                        cout<<ind_of_main_user << ' '<<number_of_users<<endl;
                        if(ind_of_main_user>=0)
                        {
                            cout<<users[ind_of_main_user].get_id()<<endl;
                            ptr_users[ind_of_main_user]->set_new_friend(ptr_users[number_of_users-1]);
                            ptr_users[number_of_users-1]->set_new_friend(ptr_users[ind_of_main_user]);

                            // shared_ptr <User> shared_user = make_shared<User>(users[number_of_users]);
                            // weak_ptr<User> weak_ptr = shared_user;
                            // users[ind_of_main_user].set_new_friend(weak_ptr);

                            // shared_ptr <User> shared_main_user = make_shared<User>(users[ind_of_main_user]);
                            // weak_ptr<User> weak_main_ptr = shared_main_user;
                            // users[number_of_users].set_new_friend(weak_main_ptr);

                        }
                    }
                    else if(count == 0 && ind_of_main_user == -1)
                    {
                        auto it = find(unique_id.begin(), unique_id.end(), id_user);
                        if(it != unique_id.end())
                        {
                            ind_of_main_user = distance(unique_id.begin(), it);
                        }
                    }

                }
                count++;
            }
        }
    }

    void check_users()
    {
        for(auto i : this->users)
        {
            cout<< i.get_id();
            // shared_ptr<User>resstored = i.get_friends()
            cout << ' '<< i.get_friends().size();
            // for(auto j : i.get_friends())
            // {
            //     cout<<"ooooo";
            //     // shared_ptr<User> resstored = j.lock();
            //     // cout << "--" << resstored->get_id();
            // }
            cout<<endl;
        }
    }

    static bool user_exist(const vector<string> unique_id, const string name_user) 
    {
        return (find(unique_id.begin(), unique_id.end(), name_user) != unique_id.end());
    }

    static bool isAlphanumericRegex(const string& str) 
    {
        regex pattern("^[a-zA-Z0-9]*$");
        return regex_match(str, pattern);
    }
};

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


int main(){

    auto [users, posts] = read_users_and_posts("file.txt");

    // for(auto i:users)
    // {
    //     for(auto j:i)
    //     {
    //         cout<<j<<',';
    //     }
    //     cout<<endl;
    // }

    auto SOC = Social_network(users);
    SOC.check_users();

    return 0;
}