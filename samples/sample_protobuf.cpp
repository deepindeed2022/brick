#include <iostream>
#include <fstream>
#include <string>
#include "protobuf/addressbook.pb.h"

void PromptForAddress(tutorial::Person* person){
    std::cout << "Enter persion name:" << std::endl;
    std::string name;
    std::cin >> name;
    person->set_name(name);

    std::string email;
    std::cin >> email;
    person->set_email(email);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " ADDRESS_BOOL_FILE" << std::endl;
        return -1;
    }

    tutorial::AddressBook address_book;
    {
        std::fstream input(argv[1], std::ios::in | std::ios::binary);
        if (!input) {
            std::cout << argv[1] << ": File not found. Creating a new file." << std::endl;
        } else if (!address_book.ParseFromIstream(&input)) {
            std::cerr << "Filed to parse address book." << std::endl;
            return -1;
        }
    }
    // Add an address
    PromptForAddress(address_book.add_people());
    {
        std::fstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
        if (!address_book.SerializeToOstream(&output)) {
            std::cerr << "Failed to write address book." << std::endl;
            return -1;
        }
    }
    // Optional: Delete all global objects allocated by libprotobuf.
    //google::protobuf::ShutdownProtobufLibrary();
    return 0;
}