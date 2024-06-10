/*
PURPOSE:
    (Data record to MongoDB in json format.)
PROGRAMMERS:
     (((Robert W. Bailey) (LinCom Corp) (3/96) (SES upgrades)
     ((Alex Lin) (NASA) (April 2009) (--) (c++ port)))
     ((Yusuf Can Anar) (TAI))
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <chrono>

#include "trick/mongodb_handler.hh"
#include "trick/MongoInstance.hh"

#include "trick/DRMongo.hh"
#include "trick/command_line_protos.h"
#include "trick/memorymanager_c_intf.h"
#include "trick/message_proto.h"
#include "trick/message_type.h"
#include "trick/bitfield_proto.h"

Trick::DRMongo::DRMongo( std::string in_name ) : Trick::DataRecordGroup( in_name ) {

    ascii_float_format = "%20.8g" ;
    ascii_double_format = "%20.16g" ;
    delimiter = ",";

    mongoDbUri = "";
    databaseName = "trick"; 
    collectionName = in_name; 

    // DataRecord permanent fields
    groupId = "groupName";

    register_group_with_mm(this, "Trick::DRMongo") ;
}

int Trick::DRMongo::format_specific_header( std::fstream & out_st ) {
    out_st << " is in ASCII & connected to the MongoDB" << std::endl ;
    return(0) ;
}

/**
@details
-# If the #delimiter is not empty and not a comma then set the file extension to ".txt"
-# Else set the file extension to ".csv"
-# Allocate enough memory to hold #record_size of records in memory
-# Open the log file
   -# Return an error if the open failed.
-# Write out the title line of the log file.  The title line includes the names of
   recorded variables and the units of measurement separated by the #delimiter
-# Declare the recording group to the memory manager so that the group can be checkpointed
   and restored
*/
int Trick::DRMongo::format_specific_init() {
    
     mongocxx::instance& mongo_instance = MongoInstance::get_instance(); //Consider moving this another place: multiple inits keeps calling this although this is a singleton class so it is safe to use.

    unsigned int jj ;
    std::streampos before_write;

    /* Calculate a "worst case" for space used for 1 record. */
    writer_buff_size = record_size * rec_buffer.size();
    writer_buff = (char *)calloc(1 , writer_buff_size) ;

    /* This loop touches all of the memory locations in the allocation forcing the
       system to actually do the allocation */
    for ( jj= 0 ; jj < record_size * rec_buffer.size() ; jj += 1024 ) {
        writer_buff[jj] = 1 ;
    }
    writer_buff[record_size * rec_buffer.size() - 1] = 1 ;

    return(0) ;
}

/**
@details
-# While there is data in memory that has not been written to disk
   -# Write out the time to a temporary #writer_buff
   -# Write out each of the other parameter values preceded by the delimiter to the temporary #writer_buff
   -# Write #writer_buff to the output file
-# Flush the output file stream
-# Return the number of bytes written
*/
int Trick::DRMongo::format_specific_write_data(unsigned int writer_offset) {
    #ifndef SWIG

    trick::MongoDbHandler mongoDbHandler(mongoDbUri, databaseName, collectionName); //TODO move this to the constructor later

    unsigned int ii ;
    char *buf;

    buf = writer_buff ;
    nlohmann::json json;
    
    /* Write out the first parameters (time) */
    copy_data_ascii_item(rec_buffer[0], writer_offset, buf );
    json = variable_string_to_json(json, std::string(rec_buffer[0]->ref->reference), std::string(writer_buff));

    /* Write out all other parameters */
    for (ii = 1; ii < rec_buffer.size() ; ii++) {
        copy_data_ascii_item(rec_buffer[ii], writer_offset, buf ); // This function is responsible for copying the value of the current parameter to the buf
        json = variable_string_to_json(json, std::string(rec_buffer[ii]->ref->reference), std::string(writer_buff));
    }

    // -- Add the fixed fields --
    // Group Name
    json = variable_string_to_json(json, groupId, collectionName);

    // current Date Time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

     std::tm* local_time = std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%dT%H:%M:%S");
    // Append milliseconds
    oss << '.' << std::setfill('0') << std::setw(3) << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000 << "+00:00";

    std::string creationDate = oss.str();

    json["currentEpochTime"] = now_time_t;
    json["creationDate"] = creationDate;
    
    // serialize it to BSON 
    std::vector<std::uint8_t> v = nlohmann::json::to_bson(json);

    // create a bsoncxx::document::view from the data
    auto fullDocument = bsoncxx::document::view(v.data(), v.size());
    mongoDbHandler.add(fullDocument);

    #endif

    #ifdef SWIG
    std::cout << "WARNING : SWIG usage blocking DRMongo format_specific_write_data" << endl;
    #endif

    /*! +1 for endl */
    return(strlen(writer_buff) + 1) ;
}
/**
@details
-# Close the output file stream
*/
int Trick::DRMongo::format_specific_shutdown() {

    if ( inited ) {
        // std::cout << "MONGODB shutdown" << std::endl;
    }
    return(0) ;
}

int Trick::DRMongo::set_ascii_float_format( std::string in_float_format ) {
    ascii_float_format = in_float_format ;
    return(0) ;
}

int Trick::DRMongo::set_ascii_double_format( std::string in_double_format ) {
    ascii_double_format = in_double_format ;
    return(0) ;
}

int Trick::DRMongo::set_delimiter( std::string in_delimiter ) {
    delimiter = in_delimiter ;
    return(0) ;
}

int Trick::DRMongo::set_single_prec_only( bool in_single_prec_only ) {
    Trick::DataRecordGroup::set_single_prec_only(in_single_prec_only) ;
    if( single_prec_only ) {
        ascii_double_format = "%20.8g";
    }
    else {
        ascii_double_format = "%20.16g";
    }
    return(0) ;
}

int Trick::DRMongo::copy_data_ascii_item( Trick::DataRecordBuffer * DI, int item_num, char *buf ) {

    char *address = 0;

    unsigned long bf;
    int sbf;

    address = DI->buffer + (item_num * DI->ref->attr->size) ;

    size_t writer_buf_spare = writer_buff + writer_buff_size - buf;

    switch (DI->ref->attr->type) {
        case TRICK_CHARACTER:
            snprintf(buf, writer_buf_spare, "%c", *((char *) address));
            break;

        case TRICK_UNSIGNED_CHARACTER:
#if ( __linux | __sgi )
        case TRICK_BOOLEAN:
#endif
            snprintf(buf, writer_buf_spare, "%u", *((unsigned char *) address));
            break;

        case TRICK_STRING:
            snprintf(buf, writer_buf_spare, "%s", *((char **) address));
            break;

        case TRICK_SHORT:
            snprintf(buf, writer_buf_spare, "%d", *((short *) address));
            break;

        case TRICK_UNSIGNED_SHORT:
            snprintf(buf, writer_buf_spare, "%u", *((unsigned short *) address));
            break;

        case TRICK_ENUMERATED:
        case TRICK_INTEGER:
#if ( __sun | __APPLE__ )
        case TRICK_BOOLEAN:
#endif
            snprintf(buf, writer_buf_spare, "%d", *((int *) address));
            break;

        case TRICK_UNSIGNED_INTEGER:
            snprintf(buf, writer_buf_spare, "%u", *((unsigned int *) address));
            break;

        case TRICK_LONG:
            snprintf(buf, writer_buf_spare, "%ld", *((long *) address));
            break;

        case TRICK_UNSIGNED_LONG:
            snprintf(buf, writer_buf_spare, "%lu", *((unsigned long *) address));
            break;

        case TRICK_FLOAT:
            snprintf(buf, writer_buf_spare, ascii_float_format.c_str() , *((float *) address));
            break;

        case TRICK_DOUBLE:
            snprintf(buf, writer_buf_spare, ascii_double_format.c_str() , *((double *) address));
            break;

        case TRICK_BITFIELD:
            sbf = GET_BITFIELD(address, DI->ref->attr->size, DI->ref->attr->index[0].start, DI->ref->attr->index[0].size);
            snprintf(buf, writer_buf_spare, "%d", sbf);
            break;

        case TRICK_UNSIGNED_BITFIELD:
            bf = GET_UNSIGNED_BITFIELD(address, DI->ref->attr->size, DI->ref->attr->index[0].start, DI->ref->attr->index[0].size);
            snprintf(buf, writer_buf_spare, "%lu", bf);
            break;

        case TRICK_LONG_LONG:
            snprintf(buf, writer_buf_spare, "%lld", *((long long *) address));
            break;

        case TRICK_UNSIGNED_LONG_LONG:
            snprintf(buf, writer_buf_spare, "%llu", *((unsigned long long *) address));
            break;
        default:
            break;
    }

    return(0) ;
}

int Trick::DRMongo::set_mongoDbUri( std::string in_mongoDbUri ) {
    mongoDbUri = in_mongoDbUri;
    return(0);
}

int Trick::DRMongo::set_databaseName( std::string in_databaseName ) {
    databaseName = in_databaseName;
    return(0);
}

int Trick::DRMongo::set_collectionName( std::string in_collectionName ) {
    collectionName = in_collectionName;
    return(0);
}
#ifndef SWIG
nlohmann::json Trick::DRMongo::variable_string_to_json(nlohmann::json j, std::string str, std::string value){
    replace(str.begin(), str.end(), '.', '/');
    replace(str.begin(), str.end(), '[', '/');
    str.erase(remove(str.begin(), str.end(), ']'), str.end());

    j[nlohmann::json::json_pointer('/' + str)] = value;
    return j;
}
#endif