#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

// "/usr/local/include/bsoncxx/v_noabi" include this to  include path
#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

namespace trick
{
    
    enum class CharacterSize
    {
        Small,
        Medium,
        Large
    };

    // map size enums to string(or char*)
    std::unordered_map<CharacterSize, const std::string> character_size_to_string({{CharacterSize::Small, "small"},
                                                                              {CharacterSize::Medium, "medium"},
                                                                              {CharacterSize::Large, "large"}});
    

    constexpr char mongoDbUri[] = "mongodb://rootuser:rootpass@10.41.74.77:27017"; //TODO this constant shall be request from user by input.py

    constexpr char databaseName[] = "trickDre"; //TODO this constant shall be request from user by input.py
    constexpr char collectionName[] = "CannonAnalytic"; //TODO this constant shall be request from user by input.py

    class MongoDbHandler
    {
// TODO This class might be deleted and functions here be used in DRMongo class. Decide it later.
    public:
        MongoDbHandler()
            : uri(mongocxx::uri(mongoDbUri)),
              client(mongocxx::client(uri)),
              db(client[databaseName]) {}

        bool AddCharacterToDb(const std::string &character_name,
                              const CharacterSize &size, const int16_t &wins)
        {
            mongocxx::collection collection = db[collectionName];
            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::document::value doc_to_add =
                builder << "characterName" << character_name
                        << "size" << character_size_to_string.find(size)->second
                        << "wins" << wins << bsoncxx::builder::stream::finalize;

            collection.insert_one(doc_to_add.view());

            return true;
        }

        bool add(bsoncxx::builder::stream::document& builder) {
            mongocxx::collection collection = db[collectionName];
            bsoncxx::document::value doc = builder << bsoncxx::builder::stream::finalize;
            collection.insert_one(doc.view());
            return true;
        }

        bool add(const std::string &param_names, const std::string &param_values) {
            
            mongocxx::collection collection = db[collectionName];
            auto builder = bsoncxx::builder::stream::document{};
            // Check if the sizes of param_names and param_values are the same
            // if (param_names.size() != param_values.size()) {
            //     std::cout << "ERROR: Sizes don't match\n";
            //     return false;
            // }

            // for (size_t i = 0; i < param_names.size(); ++i) {
            //     // Use "concat" or "append" to add key-value pairs to the BSON document
            //     builder = builder << param_names[i] << param_values[i];

            // }

            builder << param_names << param_values;

            bsoncxx::document::value doc_to_add = builder << bsoncxx::builder::stream::finalize;
            collection.insert_one(doc_to_add.view());
            return true;
        }

    private:
        mongocxx::uri uri;
        mongocxx::client client;
        mongocxx::database db;
    };
}