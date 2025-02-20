#include <iostream> // Include input-output stream library
#include <fstream>  // Include file stream library
#include <string>

int main(int argc, char **argv) {
    // Specify the name of the file to write to
    std::string filename = argv[1];

    // Create an output file stream object
    std::ofstream outfile(filename);

    // Check if the file was opened successfully
    if (!outfile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 1; // Return an error code if opening fails
    }

    // Write content to the file
    outfile << "Hello, World!" << std::endl;
    outfile << "This is a simple file writing example." << std::endl;
    outfile << "File operations in C++ are quite easy." << std::endl;

    // Close the file stream
    outfile.close();

    // Notify the user that the content has been successfully written
    std::cout << "Content has been successfully written to file: " << filename << std::endl;

    return 0; // Return a success code
}
