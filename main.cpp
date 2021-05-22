#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<deque>
#include<set>
#include<iterator>

int intlen (int x) {
    if(x == 0) return 1;
    int ret = 0;
    while(x) {
        ret++;
        x /= 10;
    }
    return ret;
}


class Entry {
    public:
        std::set<int> indices;
        std::vector<char> chars;
        bool eol;
        int diff(Entry &other) {
            int ret = 0;
            for(int i = 0 ; i < chars.size(); i++) {
                ret += chars[i] != other.chars[i];
            }
            return ret;
        }

        Entry(int x, int padding) {
            indices.insert(x);
            while(x) {
                chars.emplace_back((x % 2) + 48);
                x /= 2;
            }
            while(chars.size() <= padding)
                chars.emplace_back('0');
        }
        void print(int padding = 0) {
            int tmp = 0, size = 2;
            std::cout << "(";
            for(const int &x : indices) {
                size += intlen(x);
                std::cout << x;
                if(tmp < indices.size() - 1) {
                    std::cout << " ";
                    size++;
                }
                tmp++;
            } 
            std::cout << ")";
            for(int i = 0 ; i < padding - size ; i++)
                std::cout << " ";
            std::cout << "   |";
        }

        Entry(){
            eol = true;
        }

        Entry combine(const Entry &other) {
            Entry newEntry;
            newEntry.chars.resize(chars.size());
            for(int i = 0 ; i < chars.size() ; i++) {
                if(chars[i] == other.chars[i]) {
                    newEntry.chars[i] = chars[i];
                }
                else newEntry.chars[i] = 'x';
            }
            for(const int &x : indices)
                newEntry.indices.insert(x);
            for(const int &x : other.indices)
                newEntry.indices.insert(x);
            return newEntry;
        }
};

std::vector<Entry> final;

class Table {
    public:
        std::vector<Entry> list;
        Table(std::vector<int> indices) {
            sort(indices.begin(), indices.end(), std::greater<int>());
            int padding = 0, x = indices.front();
            while(x /= 2)
                padding++;
            for(int &i : indices)
                list.emplace_back(Entry(i, padding));
        }

        Table() {
        }

        Table nextTable() {
            Table newTable;
            for(int i = 0 ; i < list.size() ; i++) {
                for(int j = i + 1 ; j < list.size() ; j++) {
                    if(list[i].diff(list[j]) == 1) {
                        list[i].eol = false;
                        list[j].eol = false;
                        Entry tmp = list[i].combine(list[j]);
                        bool duplicate = false;
                        for(const Entry &x : newTable.list) {
                            if(x.indices == tmp.indices) {
                                duplicate = true;
                                break;
                            }
                        }
                        if(!duplicate)
                            newTable.list.emplace_back(tmp);
                    }
                }
            }

            return newTable;
        }

        bool check() {
            for(int i = 0 ; i < list.size() ; i++) {
                for(int j = i + 1 ; j < list.size() ; j++) {
                    if(list[i].diff(list[j]) == 1) {
                        return true;
                    }
                }
            }
            return false;
        }

};

int main() {
    // Uneti indekse razdvojene razmakom na kojima je funkcija tacna
    std::string input;
    std::cout << "Unesi indekse za koje je funkcija tacna\n";
    getline(std::cin, input);
    std::cout << "Unesi neobavezne ulaze (ako ih nema samo stisnite enter) \n";
    std::string dc;
    getline(std::cin, dc);
    std::vector<int> starting, dontcare, toprow;
    std::string tmp;
    for(char &x : input) {
        if(x == ' ') {
            if(!tmp.empty()) // za slucaj ako stoji vise spaceova jedan do drugog
                starting.emplace_back(std::stoi(tmp.c_str()));
            tmp.clear();
        }
        else tmp += x;
    }
    if(!tmp.empty())
        starting.emplace_back(std::stoi(tmp.c_str()));

    toprow = starting;

    tmp.clear();

    for(char &x : dc) {
        if(x == ' ') {
            if(!tmp.empty()) // za slucaj ako stoji vise spaceova jedan do drugog
                dontcare.emplace_back(std::stoi(tmp.c_str()));
            tmp.clear();
        }
        else tmp += x;
    }
    if(!tmp.empty())
        dontcare.emplace_back(std::stoi(tmp.c_str()));

    for(const int &x : dontcare)
        starting.emplace_back(x);


    std::sort(dontcare.begin(), dontcare.end());

    std::list<Table> tables;
    tables.emplace_back(Table(starting));

    while(tables.back().check()) {
        tables.emplace_back(tables.back().nextTable());
    }

    // for(Table &x : tables) x.print();

    for(Table &x : tables) {
        for(Entry &y : x.list) {
            if(y.eol)
                final.emplace_back(y);
        }
    }

    //print fancy table
    int offset = 0;
    for(Entry &x : final) {
        int tmp = 0;
        for(auto &y : x.indices)
            tmp += intlen(y);
        tmp += x.indices.size() + 1;
        offset = std::max(offset, tmp);
    }

    for(int i = 0 ; i < offset + 5 ; i++) std::cout << " ";
    // std::cout << "R";
    int toprowlongest = 2;
    for(int x: toprow) toprowlongest = std::max(toprowlongest, intlen(x));
    std::string format = "%" + std::to_string(toprowlongest) + "d ";
    for(const int &x : toprow)
        printf(format.c_str(), x);
    std::cout << "\n";

    for(Entry &x : final) {
        x.print(offset);
        for(std::vector<int>::iterator it = toprow.begin() ; it != toprow.end() ; it++) {
            std::string tmp = "%" + std::to_string(toprowlongest + 1) +"s";
            if(x.indices.contains(*it)) {
                printf(tmp.c_str(), "  \u2714");
            }
            else {
                printf(tmp.c_str(), " ");
            }
        }
        std::cout << "\n";

    }

    return 0;
}