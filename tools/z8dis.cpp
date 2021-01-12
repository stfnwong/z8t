/*
 * Z8TDIS
 * Disassembler entry point for z80 source 
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <getopt.h>

#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "Program.hpp"
#include "Source.hpp"


/*
 * Options
 */
struct DisOpts
{
    std::string infile;
    std::string outfile;
    std::string literal;
    bool verbose;
    bool print_adr;

    public:
        DisOpts() : infile("\0"), 
                    outfile("\0"),
                    literal("\0"),
                    verbose(false),
                    print_adr(false) {} 
};


int main(int argc, char *argv[])
{
    DisOpts dis_opts;
    const char* const short_opts = "avhi:o:l:";
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
            case 'v':   // verbose mode
                dis_opts.verbose = true;
                break;

            case 'a':   // print addresses
                dis_opts.print_adr = true;
                break;
                
            case 'i':   // input file 
                dis_opts.infile = std::string(optarg);
                break;

            case 'o':   // output file
                dis_opts.outfile = std::string(optarg);
                break;

            case 'l':
                dis_opts.literal = std::string(optarg);
                break;

            default:
                std::cerr << "Uknown option " << std::string(optarg) << 
                    "(arg " << argn << ")" << std::endl;
                exit(-1);
                break;
        }
        argn++;
    }

    // we can disassemble a uint32_t literal from the command line 
    if(dis_opts.literal != "\0")
    {
        uint32_t instr_literal = std::strtoul(dis_opts.literal.c_str(), nullptr, 16);
        std::vector<uint8_t> code_buf;

        uint32_t instr_mask = 0xFF000000;
        uint8_t buf_elem;
        for(int i = 0; i < 4; ++i)
        {
            if((instr_literal & instr_mask) > 0)
            {
                buf_elem = (instr_literal & instr_mask) >> ((3-i) * 8);
                code_buf.push_back(buf_elem);
            }
            instr_mask = instr_mask >> 8;
        }

        std::string dis_repr = dis_instr_to_repr(code_buf);
        std::cout << dis_repr << std::endl;

        //if(dis_opts.print_adr)
        //{
        //    std::cout << "[0x" << std::hex << std::setw(8) << std::setfill('0')
        //        << dis_out.addr << "] ";
        //}
        //std::cout << dis_out.toInstrString() << std::endl;

        return 0;
    }
    // otherwise, check that we have a valid infile 
    else if(dis_opts.infile == "\0")
    {
        std::cerr << "Invalid input filename" << std::endl;
        std::cerr << "Use -i <filename> to specify" << std::endl;
        exit(-1);
    }

    Program prog;
    status = prog.load(dis_opts.infile);
    if(status < 0)
    {
        std::cerr << "Failed to load input file [" << 
            dis_opts.infile << "]" << std::endl;
        exit(-1);
    }

    if(dis_opts.verbose)
    {
        std::cout << "Found " << prog.length() << " instructions in program [" 
            << dis_opts.infile << "] (" << std::dec << prog.numBytes() << " bytes)" << std::endl;
        std::cout << std::endl;
    }

    // Dump representation to console
    // TODO: data segment
    //for(unsigned int idx = 0; idx < prog.length(); ++idx)
    //{
    //    Instr cur_instr = prog.get(idx);
    //    LineInfo dis_out = dis_instr(cur_instr.ins, cur_instr.adr);

    //    if(dis_opts.print_adr)
    //    {
    //        std::cout << "[0x" << std::hex << std::setw(8) << std::setfill('0')
    //            << dis_out.addr << "] ";
    //    }
    //    std::cout << dis_out.toInstrString() << std::endl;
    //}

    return 0;
}
