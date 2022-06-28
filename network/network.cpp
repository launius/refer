#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class NetworkMonitor
{
public:
    std::string getFileAsStringUsingFp(std::string path);
	std::string getFileAsStringUsingStream(std::string path);
	bool setStaticConfig();
	
private:
	const std::string netConfig = "./interfaces";
	const std::string ip = "192.168.0.100";
	const std::string netmask = "255.255.255.0";
	const std::string gateway = "192.168.0.1";
};

std::string NetworkMonitor::getFileAsStringUsingFp(std::string path)
{
    std::ifstream ifs(path);
    std::string text;

    if (!ifs.is_open())
	{
        std::cout << __func__ << ": file open error!\n";
        return std::string();
    }

    ifs.seekg(0, std::ios::end);
    std::ifstream::pos_type size = ifs.tellg();
    if (size < 0)
    {
        std::cout << __func__ << ": file size error!\n";
        return std::string();
	}

    text.resize(static_cast<size_t>(size));
    ifs.seekg(0, std::ios::beg);
    ifs.read(&text[0], size);

    ifs.close();

    return text;
}

std::string NetworkMonitor::getFileAsStringUsingStream(std::string path)
{
    std::ifstream in(path);
    std::ostringstream sstr;

    sstr << in.rdbuf();
    return sstr.str();
}

bool NetworkMonitor::setStaticConfig()
{
    std::string netFile;

    netFile = getFileAsStringUsingFp(netConfig);
	if (netFile.empty())
		return false;

    std::string from = "eth0 inet dhcp";
    std::string to = "eth0 inet static\n\taddress " + ip
                                   + "\n\tnetmask " + netmask
                                   + "\n\tgateway " + gateway;

    size_t pos = netFile.find(from);
    if (pos == std::string::npos)
    {
        std::cout << __func__ << ": no dhcp settings\n";
        return true;
    }

    netFile.replace(pos, from.length(), to);

    std::cout << __func__ << ": file text (\n" << netFile << ")\n";
	
	return true;
}

int main()
{
	NetworkMonitor mon;
	std::string netFile;

	netFile = mon.getFileAsStringUsingFp("./interfaces");
    std::cout << __func__ << ": file text (\n" << netFile << ")\n";

	netFile = mon.getFileAsStringUsingStream("./interfaces");
    std::cout << __func__ << ": file text (\n" << netFile << ")\n";

	mon.setStaticConfig();

    return EXIT_SUCCESS;
}
