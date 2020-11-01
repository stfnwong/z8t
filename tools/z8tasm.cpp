/*
 * Z8TASM
 * Assembler entry point for z80
 *
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <getopt.h>

#include "Assembler.hpp"
#include "Program.hpp"
#include "Source.hpp"


/*
 * Options
 */
struct AsmOpts
{
    std::string infile;
    std::string outfile;
    std::string literal;
    bool verbose;

    public:
        AsmOpts() : infile("\0"), 
                    outfile("\0"),
                    literal("\0"),
                    verbose(false) {} 
};


/*
 * Assembler
 */
int main(int argc, char *argv[])
{
    AsmOpts asm_opts;
    const char* const short_opts = "vhi:o:c:";
    const option long_opts[] = {};
    int argn = 0;
    int status;

    // get args
    while(1) 
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if(opt == -1)
            break;

        switch(opt)
        {
            case 'v':   // verbose
                asm_opts.verbose = true;
                break;
                
            case 'i':   // input file
                asm_opts.infile = std::string(optarg);
                break;

            case 'o':   // output file
                asm_opts.outfile = std::string(optarg);
                break;

            case 'c':   // literal
                asm_opts.literal = std::string(optarg);
                break;

            default:
                std::cerr << "Unknown option " << std::string(optarg) << 
                    "(arg " << argn << ")" << std::endl;
                exit(-1);
                break;
        }
        argn++;
    }
    // Get an assembler 
    Assembler assem;
    Program program;

    assem.setVerbose(asm_opts.verbose);

    if(asm_opts.literal != "\0")
    {
        assem.loadSource(asm_opts.literal);
    }
    // check that what we got was valid
    else if(asm_opts.infile != "\0")
    {
        status = assem.read(asm_opts.infile);
        if(status < 0)
        {
            std::cerr << "Failed to load input file [" << 
                asm_opts.infile << "]" << std::endl;
            exit(-1);
        }
    }
    else
    {
        std::cerr << "Invalid input filename" << std::endl;
        std::cerr << "Use -i <filename> to specify" << std::endl;
        exit(-1);
    }

    assem.assemble();

    // Run through source and print error strings
    // TODO : need to handle data sections as well..
    SourceInfo lexed_source;
    TextLine line;

    lexed_source = assem.getSourceInfo();
    for(unsigned int idx = 0; idx < lexed_source.getNumLines(); ++idx)
    {
        // TODO: is there a data section for z80 programs?
        line = lexed_source.get(idx);
        if(line.error)
        {
            std::cerr << "[ERROR] line " << line.line_num << std::endl;
            std::cerr << line.errstr << std::endl;
        }
    }

    // After (possibly) printing error strings, exit
    if(lexed_source.hasError())
        exit(-1);

    // TODO: maybe make this a super verbose option
    //if(asm_opts.verbose)
    //    std::cout << lexed_source.toString() << std::endl;

    // Save the program to disk
    program = assem.getProgram();

    if(asm_opts.outfile != "\0")
    {
        status = program.save(asm_opts.outfile);
        if(status < 0)
        {
            std::cerr << "[ERROR] writing program to file [" <<
                asm_opts.outfile << "]" << std::endl;
            exit(-1);
        }
    }
    
    if(asm_opts.verbose)
        std::cout << program.toString() << std::endl;

    return 0;
}
