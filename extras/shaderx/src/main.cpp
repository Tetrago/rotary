#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <argparse/argparse.hpp>
#include <shcc/shcc.hpp>

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("shaderx");

	argparse::ArgumentParser dumpCommand("dump");
	dumpCommand.add_argument("file")
		.help("HLSL source path");

	program.add_subparser(dumpCommand);

	try
	{
		program.parse_args(argc, argv);
	}
	catch(const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << program << std::endl;

		return 1;
	}

	std::shared_ptr<shcc::Runtime> runtime = shcc::runtime([](const std::string& message)
	{
		std::cerr << "[shcc] " << message << std::endl;
	});

	if(program.is_subcommand_used("dump"))
	{
		std::cout << "Reading file...\n";

		std::ifstream file(dumpCommand.get<std::string>("file"), std::ios::ate);
		if(!file.is_open())
		{
			std::cerr << "Failed to open file" << std::endl;

			return 1;
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::string source(size, ' ');
		file.read(source.data(), size);

		std::cout << "Parsing HLSL..." << std::endl;

		try
		{
			shcc::Package package = runtime->package(source, { shcc::Stage::Vertex, shcc::Stage::Fragment });

			std::cout << "#--- Dump -----------------------------------------------#" << std::endl;

			std::cout << "Vertex Inputs" << std::endl;

			for(const shcc::VertexInput& input : package.resources().vertexInputs)
			{
				std::cout << " - " << input.name << " (" << shcc::get_size_of(input.type) << 'x' << input.count << ')' <<  std::endl;
			}
		}
		catch(const std::runtime_error& err)
		{
			std::cerr << "Failed to parse binary:" << std::endl;
			std::cerr << "--> " << err.what() << std::endl;

			return 1;
		}
	}

	return 0;
}
