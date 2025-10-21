#include <vector>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Graph
{
private:
    class Point
    {
    private:
        int x;
        int y;
        int id;
        int ind;

    public:
        Point(const int & new_ind, const tuple <int, int, int>& parametrs)
        {
            x = get<1>(parametrs);
            y = get<2>(parametrs);
            id = get<0>(parametrs);
            ind = new_ind; 

        }
        int get_ind() const
        {
            return this->ind;
        }

        int get_x() const
        {
            return this->x;
        }

        int get_y() const
        {
            return this->y;
        }

        int get_id() const
        {
            return this->id;
        }
    };

    class Edge
    {
    private:
        int ind_start;
        int ind_end;
        double len;
    public:
        Edge(const Point& start, const Point& end)
        {
            ind_start = start.get_ind();
            ind_end = end.get_ind();
            len = sqrt( pow((end.get_x() - start.get_x()), 2) + pow((end.get_y() - start.get_y()), 2) );

        }

        double get_len() const
        {
            return this->len;
        }

        int get_ind_start()
        {
            return this->ind_start;
        }

        int get_ind_end()
        {
            return this->ind_end;
        }

    };

    int find_parent(const int& ind, int *mas)
    {
        if(mas[ind] != ind)
        {
            mas[ind] = find_parent(mas[ind], mas);
            return mas[ind];
        }

        return mas[ind];
    }

    vector <Point> arr_of_points;
    vector <Edge> arr_of_edges;

public:
    Graph(const vector <tuple <int, int, int>> & create_mas)
    {

        for(const auto& i : create_mas)
        {
            arr_of_points.push_back( Point(arr_of_points.size() ,i));
        }
        
        for(int i = 0; i < arr_of_points.size(); i++)
        {
            for(int j = i+1; j < arr_of_points.size(); j++)
            {
                arr_of_edges.push_back(Edge(arr_of_points[i],arr_of_points[j]));
            }
        }

        sort(arr_of_edges.begin(), arr_of_edges.end(),[](const Edge &a, const Edge& b)
        {
            return a.get_len() < b.get_len();
        });

        // for(auto& i : arr_of_edges)
        // {
        //     cout << i.get_ind_start() << ' ' << i.get_ind_end()<<endl;
        // }

    }

    vector <pair<int,int>> create_backbone_tree()
    {
        vector <pair<int, int>> backbone_tree;

        int parent[arr_of_points.size()];

        for(int i = 0; i < arr_of_points.size(); i++)
        {
            parent[i] = i;
        }

        int i = 0;
        // for(int i = 0; i < arr_of_edges.size(); i++)
        while(backbone_tree.size() != arr_of_points.size()-1 && i < arr_of_edges.size())
        {

            int ind_start = arr_of_edges[i].get_ind_start();
            int ind_end = arr_of_edges[i].get_ind_end();

            int root_start = find_parent(ind_start, parent);
            int root_end = find_parent(ind_end, parent);

            if (root_start != root_end)
            {
                int id_start = arr_of_points[ind_start].get_id();
                int id_end = arr_of_points[ind_end].get_id();

                backbone_tree.push_back({id_start,id_end});

                parent[root_end] = root_start;
            }

            i++;
        }

        return backbone_tree;
    }

};


vector< tuple<int, int, int>> read_file(const string& file_path)
{
    ifstream file;
    
    file.open(file_path);

    string new_str;

    vector <tuple <int, int, int>> arr_point;

    while(getline(file,new_str)){

        for(char& i : new_str)
        {
            if (i == ',')
            {
                i = ' ';
            }
        }

        int id, x, y;

        stringstream ss(new_str);
        
        if(!(ss >> id >> x >> y))
        {
            continue;
        }

        else
        {
            // cout<<"pppppp"<<endl;
            arr_point.push_back({id, x, y});
        }

    }

    file.close();

    return arr_point;
}


int main()
{
    
    vector < tuple< int, int, int>> mas = read_file("file.txt");

    // for(auto& i:mas)
    // {
    //     cout<<get<1>(i)<<endl;
    // }

    Graph fst = Graph(mas);

    vector <pair<int,int>> ans = fst.create_backbone_tree();

    // cout<<ans.size()<<endl;

    for(auto& i : ans)
    {
        cout << i.first <<"\t"<<i.second<<endl;
    }
    // cout << ans;
    return 0;
}