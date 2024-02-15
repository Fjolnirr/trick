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
    class MongoDbHandler
    {
// TODO This class might be deleted and functions here be used in DRMongo class. Decide it later.
    public:
        MongoDbHandler(const std::string mongoDbUri, const std::string databaseName, const std::string collectionName)
            : uri(mongocxx::uri(mongoDbUri)),
              client(mongocxx::client(uri)),
              db(client[databaseName]),
          _collectionName(collectionName) {}
                
        bool add(bsoncxx::builder::stream::document& builder) {
            mongocxx::collection collection = db[_collectionName];
            bsoncxx::document::value doc = builder << bsoncxx::builder::stream::finalize;
            collection.insert_one(doc.view());
            return true;
        }
        bool add(bsoncxx::document::view& docView) {
            mongocxx::collection collection = db[_collectionName];
            collection.insert_one(docView);
            return true;
        }

    private:
        mongocxx::uri uri;
        mongocxx::client client;
        mongocxx::database db;
        std::string _collectionName;
    };
}