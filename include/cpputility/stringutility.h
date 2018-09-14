#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

std::string concatVectorOfStrings(const std::vector<std::string>& VecStrings, const std::string& delimiter = "\n" ){
	std::string concatenatedString = "";
	for(int i=0;i<VecStrings.size();++i){
		if (i==VecStrings.size()-1)
			concatenatedString = concatenatedString + VecStrings[i];
		else
			concatenatedString = concatenatedString + VecStrings[i] + delimiter;

	}

	return concatenatedString;
}


template <typename K, typename V, typename ...Args>
std::string& stringFormat(std::string& str, K key, V value, Args... args){
		stringFormat(str,std::forward<K>(key),std::forward<V>(value));
		return stringFormat(str, std::forward<Args...>(args) );
	
}

template <typename K, typename V>
std::string& stringFormat(std::string& str, K key, V value){
	std::string keystr = "{"+std::string(key)+"}";
	std::stringstream ss;
	ss << value;
	std::string valuestr(ss.str());

	while(1){
		auto n = str.find(keystr);
		if (n == std::string::npos)
			break;
		else{
			str.replace(n,n+valuestr.size());
		}
	}

	return str;
}
