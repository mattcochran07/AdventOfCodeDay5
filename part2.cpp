#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Rule
{
	int before;
	int after;
	Rule(const std::vector<int>& inp)
	{
		before = inp[0];
		after = inp[1];
	}
	std::string toString() const
	{
		std::stringstream ss;
		ss << "before = " << before << " after = " << after << std::endl;
		return ss.str();
	}
};

bool passes(std::vector<int>& update, const struct Rule& rule)
{
	bool tf = true;

	//does the update line have the left num of rule
	std::vector<int>::iterator itrL = std::find(update.begin(), update.end(), rule.before);
	if (itrL != update.end())
	{
		//check to see if the right side of the rule is in the update
		std::vector<int>::iterator itrR = std::find(update.begin(), itrL, rule.after);
		if (itrR != itrL) // we found it before we got to the left number, we broke the rule!
		{
			tf = false;
			//std::cout << "rule broken!" << std::endl;
		}
	}

	return tf;
}

void fixOrder(std::vector<int>& update, std::vector<struct Rule>& rules)
{
	//std::cout << "update before:\n";
	//for (int i = 0; i < update.size(); i++)
		//std::cout << update[i] << " ";
	//std::cout << std::endl;
	for (int i = 0; i < rules.size(); i++)
	{
		struct Rule& rule = rules[i];
		//does the update line have the left num of rule
		std::vector<int>::iterator itrL = std::find(update.begin(), update.end(), rule.before);
		if (itrL != update.end())
		{
			//check to see if the right side of the rule is in the update
			std::vector<int>::iterator itrR = std::find(update.begin(), itrL, rule.after);
			if (itrR != itrL) // we found it before we got to the left number, we broke the rule!
			{
				int rVal = *itrR;
				update.erase(itrR);
				//update.push_back(rVal); // shove it at the end
				update.insert(itrL, rVal);
			}
		}
	}
	//std::cout << "update after:\n";
	//for (int i = 0; i < update.size(); i++)
		//std::cout << update[i] << " ";
	//std::cout << std::endl;
	//std::cout << std::endl;
}

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		std::ifstream ifs(argv[1]);
		std::string line;
		std::vector<struct Rule> rules;
		std::vector<std::vector<int>> updates;
		while (std::getline(ifs, line))
		{
			if (line.find("|") != std::string::npos)
			{
				std::stringstream ss(line.c_str());
				std::string arg;
				std::vector<int> inp;
				while (std::getline(ss, arg, '|'))
				{
					inp.push_back(atoi(arg.c_str()));
				}
				struct Rule rule(inp);
				rules.push_back(rule);
			}
			else
			{
				std::stringstream ss(line.c_str());
				std::string arg;
				std::vector<int> intLine;
				while (std::getline(ss, arg, ','))
				{
					intLine.push_back(atoi(arg.c_str()));
				}
				if (intLine.size())
					updates.push_back(intLine);
			}
		}
		// now we have all the rules and updates
		// loop through updates to see if they are correct or not
		int sum = 0;
		for (unsigned i = 0; i < updates.size(); i++)
		{
			std::vector<int>& update = updates[i];
			bool pass = true;
			for (unsigned j = 0; j < rules.size(); j++)
			{
				struct Rule& rule = rules[j];
				pass &= passes(update, rule);
			}
			if (!pass)
			{
				//didnt work after 1 pass but worked after i ran it 100 times lol
				for (int j = 0; j < 100; j++)
					fixOrder(update, rules);
				int idx = update.size() / 2;
				sum += update[idx];
			}
		}
		std::cout << "sum = " << sum << std::endl;
	}
	return 0;
}