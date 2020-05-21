#include "json.hpp"
std::string JSON::stringListToJson(std::list<std::string> sList)
{
    std::string rtnString{"{"};
    int count=1;
    for(std::string s:sList){
        rtnString += "\"" + std::to_string(count++) +"\":\"" + s + "\",";
    }
    if(rtnString.at(rtnString.size()-1) == ','){
        rtnString.pop_back();
    }
    rtnString += "}";
    return rtnString;
}

std::list<std::string> JSON::jsonToStringList_value(std::string json)
{
    std::map<std::string, std::string> map{jsonToMap(json)};
    std::list<std::string> ls;
    for(auto m:map){
        ls.push_back(m.second);
    }
    return ls;
}
std::map<std::string, std::string> JSON::jsonToMap(std::string json)
{
    eatSpace(json);
    std::map<std::string,std::string> _map;
    std::string::iterator it_beg=json.begin();
    std::string::iterator it_end=json.end();
    std::string key;
    std::string value;
    bool inKey=true;

beginState:
    if(*it_beg!='{'){
        goto errorState;
    }else{
        ++it_beg;
        goto signalState;
    }
keyState:
    while(*it_beg!='"'){
        if(it_beg==it_end){
            //goto errorState;
        }else{
            key.push_back(*it_beg);
            it_beg++;
        }
    }
    if(it_beg==it_end)
        goto errorState;
    inKey=false;
    goto signalState;
valueState:
    while(*it_beg!='"'){
        if(it_beg==it_end){
            goto errorState;
        }else{
            value.push_back(*it_beg);
            ++it_beg;
        }
    }
    if(it_beg==it_end)
        goto errorState;
    _map.insert(std::make_pair(key,value));
    key.clear();
    value.clear();
    inKey=true;
    goto signalState;
signalState:
    if(*it_beg!='"'){
        goto errorState;
    }else{
        if(inKey){
            ++it_beg;
            if(*it_beg=='}'){
                ++it_beg;
                goto endState;
            }
            if(*it_beg==','){
                ++it_beg;
                goto signalState;
            }
            goto keyState;
        }else{
            ++it_beg;
            if(*it_beg=='}'){
                ++it_beg;
                goto endState;
            }
            if(*it_beg==':'){
                ++it_beg;
                goto signalState;
            }
            goto valueState;
        }
    }
endState:
    if(*it_beg!='}'){
        goto errorState;
    }else{
        return _map;
    }
errorState:
    _map.empty();
    return _map;
}

void JSON::eatSpace(std::string &s)
{
    std::string s_copy{s};
    s.clear();
    std::string::iterator it_beg=s_copy.begin();
    std::string::iterator it_end=s_copy.end();
    while (it_beg!=it_end) {
        if(*it_beg!=' '){
            s.push_back(*it_beg);
        }
        it_beg++;
    }
}
