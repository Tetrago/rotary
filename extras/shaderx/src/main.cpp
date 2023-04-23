#include <stdexcept>
#include <iostream>
#include <fstream>
#include <argparse/argparse.hpp>
#include <shcc/shcc.hpp>

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("shaderx");

	argparse::ArgumentParser dumpCommand("dump");
	dumpCommand.add_argument("file")
		.help("Shader SPIR-V binary");

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

	if(program.is_subcommand_used("dump"))
	{
		std::cout << "Reading file...\n";

		std::ifstream file(dumpCommand.get<std::string>("file"), std::ios::ate | std::ios::binary);
		if(!file.is_open())
		{
			std::cerr << "Failed to open file" << std::endl;

			return 1;
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> bytes(size);
		file.read(reinterpret_cast<char*>(bytes.data()), size);

		std::cout << "Parsing SPIR-V..." << std::endl;

		shcc::ReflectionData data;
		try
		{
			data = shcc::compile_reflection_data(bytes);
		}
		catch(const std::runtime_error& err)
		{
			std::cerr << "Failed to parse binary" << std::endl;
			std::cerr << err.what() << std::endl;

			return 1;
		}

		std::cout << "#--- Dump -----------------------------------------------#" << std::endl;

		std::cout << "Vertex Inputs" << std::endl;

		for(const shcc::VertexInput& input : data.vertexInputs)
		{
			std::cout << " - " << input.name << " (" << shcc::get_size_of(input.type) << 'x' << input.count << ')' <<  std::endl;
		}
	}

	return 0;
}
