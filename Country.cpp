#include "Country.h"

string Country::GetCountryName() {
    return countryName;
}
void Country::SetCountryName(string inCountryName){
    countryName = inCountryName;
}
string Country:: GetContinentName(){
    return continentName;
}
void Country::SetContinentName(string inContinentName){
    continentName = inContinentName;
}
string Country::GetCountryColour(){
    return countryColour;
}
void Country::SetCountryColour(string inCountryColour){
    countryColour = inCountryColour;
}
string Country::GetOwnerName(){
    return ownerName;
}
void Country::SetCountryOwner(string inOwnerName){
    ownerName = inOwnerName;
}