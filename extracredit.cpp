#include <string>
#include <map>
#include <iostream>
class InMemoryDB{
    private:
    std::map<std::string, int> tempMap;
    std::map<std::string, int> officialMap;
    bool exists;
    public:
    int get(std::string key);
    void put(std::string key, int value);
    void begin_transaction();
    void commit();
    void rollback();
};

int InMemoryDB::get(std::string key){
    if(!exists && officialMap.size() < 1){
        return NULL;
    }
    for(auto i = officialMap.begin(); i != officialMap.end(); i++){
        if(i->first == key){
            
            return i->second;
        }
    }

    
    if (officialMap.find(key) == officialMap.end()) {
        
        return NULL;
    } else {
        return officialMap[key];
    }
}

void InMemoryDB::put(std::string key, int value){
    if(!exists && tempMap.size() < 1){
        throw std::invalid_argument( "received negative value" );
    }
    tempMap[key] = value; 
}

void InMemoryDB::begin_transaction(){
    if(!exists){
        exists = true;
    }
    else{
        throw std::invalid_argument( "received negative value" );
    }
    return;
}

void InMemoryDB::commit(){
    if(!exists){
        throw std::invalid_argument( "received negative value" );
    }
    exists = false;
    officialMap.insert(tempMap.begin(), tempMap.end());
    tempMap.clear();
//    std::cout << officialMap["A"] << std::endl;
}

void InMemoryDB::rollback(){
    if(!exists){
        throw std::invalid_argument( "received negative value" );
    }
    tempMap.clear();
    exists = false;
}

int main(){
    InMemoryDB inmemoryDB; //= new InMemoryDB();

    // should return null, because A doesn’t exist in the DB yet
    std::cout << "test1" << std::endl;
    if(inmemoryDB.get("A") == NULL){

    }
    else{
        std::cout << "failed" << std::endl;
    }

    // should throw an error because a transaction is not in progress
    std::cout << "test2" << std::endl;
    try {
        inmemoryDB.put("A", 5);
    }
    catch( const std::invalid_argument& e ) {

    }

    // starts a new transaction
    inmemoryDB.begin_transaction();
    std::cout << "test3" << std::endl;


    // set’s value of A to 5, but its not committed yet
    std::cout << "test4" << std::endl;
    try {
        inmemoryDB.put("A", 5);
    }
    catch( const std::invalid_argument& e ) {
        std::cout << "failed" << std::endl;
    }

    // should return null, because updates to A are not committed yet
    std::cout << "test5" << std::endl;
    if(inmemoryDB.get("A") == NULL){

    }
    else{
        std::cout << "failed" << std::endl;
    }

    // update A’s value to 6 within the transaction
    std::cout << "test6" << std::endl;
    try {
        inmemoryDB.put("A", 6);
    }
    catch( const std::invalid_argument& e ) {
        std::cout << "failed" << std::endl;
    }

    // commits the open transaction
    std::cout << "test7" << std::endl;
    try {
        inmemoryDB.commit();
    }
    catch( const std::invalid_argument& e ) {
        std::cout << "failed" << std::endl;
    }

    // should return 6, that was the last value of A to be committed
    std::cout << "test8" << std::endl;
    if(inmemoryDB.get("A") == 6){

    }
    else{
        std::cout << "failed" << std::endl;
    }

    // throws an error, because there is no open transaction
    std::cout << "test9" << std::endl;
    try {
        inmemoryDB.commit();
    }
    catch( const std::invalid_argument& e ) {

    }

    // throws an error because there is no ongoing transaction
    std::cout << "test10" << std::endl;
    try {
        inmemoryDB.rollback();
        
    }
    catch( const std::invalid_argument& e ) {

    }

    // should return null because B does not exist in the database
    std::cout << "test11" << std::endl;
    if(inmemoryDB.get("B") == NULL){

    }
    else{
        std::cout << "failed" << std::endl;
    }

    // starts a new transaction
    inmemoryDB.begin_transaction();
    std::cout << "test12" << std::endl;

    // Set key B’s value to 10 within the transaction
    std::cout << "test13" << std::endl;
    try {
        inmemoryDB.put("B", 10);
    }
    catch( const std::invalid_argument& e ) {
        std::cout << "failed" << std::endl;
    }

    // Rollback the transaction - revert any changes made to B
    std::cout << "test14" << std::endl;
    try {
        inmemoryDB.rollback();
    }
    catch( const std::invalid_argument& e ) {
        std::cout << "failed" << std::endl;
    }

    // Should return null because changes to B were rolled back
    std::cout << "test15" << std::endl;
    if(inmemoryDB.get("B") == NULL){

    }
    else{
        std::cout << "failed" << std::endl;
    }
}