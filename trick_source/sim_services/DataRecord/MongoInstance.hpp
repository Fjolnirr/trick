#include <mongocxx/instance.hpp>

class MongoInstance {
public:
    // Static method to get the single instance
    static mongocxx::instance& get_instance() {
        static mongocxx::instance instance;  // This will be created only once
        return instance;
    }
};
