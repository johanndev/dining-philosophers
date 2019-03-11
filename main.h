#pragma once

std::tuple<int, int, int> ParseCmdLine(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Naive dining philosophers", ' ', "0.9");

	TCLAP::ValueArg<int> numberArg("n", "number", "Number of Philosophers", true, 0, "int");
	TCLAP::ValueArg<int> thinkingTimeArg("t", "thinkingTime", "Maximal thinking time", true, 0, "int");
	TCLAP::ValueArg<int> eatingTimeArg("e", "eatingTime", "Maximal eating time", true, 0, "int");

	cmd.add(numberArg);
	cmd.add(thinkingTimeArg);
	cmd.add(eatingTimeArg);

	cmd.parse(argc, argv);

	auto number = numberArg.getValue();
	auto thinkingTime = thinkingTimeArg.getValue();
	auto eatingTime = eatingTimeArg.getValue();

	return std::make_tuple(number, thinkingTime, eatingTime);
}
