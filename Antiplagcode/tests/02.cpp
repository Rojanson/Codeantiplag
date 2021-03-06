#include <iostream>
#include <set>
#include <map>
#include <cstddef>
#include <string>
#include <vector>
#include <list>

//renamed and added some space
class SETTE
{
public:
	SETTE(size_t Ss)
	{
		sette = new std::set<long long>[Ss + 1];
	}
	void adds(size_t index_, long long elem_)
	{
		auto tmp_ = mappe.find(elem_);
		if (tmp_ != mappe.end())
		{
			(*tmp_).second.insert(index_);
			sette[index_].insert(elem_);
		}
		else
		{
			std::set<int> jj;
			jj.insert(index_);
			mappe.emplace(elem_, jj);
			sette[index_].insert(elem_);
		}
	}
	void deleters(size_t index_, long long elem_)
	{
		auto& tmp_ = (*(mappe.find(elem_))).second;
		tmp_.erase(tmp_.find(index_));
		sette[index_].erase(sette[index_].find(elem_));
	}
	void clears(size_t index_)
	{
		for (const long long& tmp_ : sette[index_])
		{
			auto jingo_ = mappe.find(tmp_);
			(*jingo_).second.erase((*jingo_).second.find(index_));
		}
		sette[index_].clear();
	}
	void listsets(size_t index_)
	{
		if (!(sette[index_].empty()))
		{
			for (const long long& tmp_ : sette[index_])
			{
				std::cout << tmp_ << ' ';
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << "-1" << std::endl;
		}
	}
	void listsetofs(long long elem_)
	{
		auto itera_ = mappe.find(elem_);
		if (itera_ != mappe.end())
		{
			auto tmp_ = (*itera_).second;
			if (!(tmp_.empty()))
			{
				for (int vivi_ : tmp_)
				{
					std::cout << vivi_ << ' ';
				}
				std::cout << std::endl;
			}
			else
			{
				std::cout << "-1" << std::endl;
			}
		}
		else
		{
			std::cout << "-1" << std::endl;
		}
	}
private:
	std::set<long long>* sette = nullptr;
	std::map<long long, std::set<int>> mappe;
	size_t size_ = 0;
	long long max_ = 0;
};

int main()
{
	long long N_;
	size_t M_;
	size_t K;
	//gadsasgdadsgasgadsgs
	std::cin >> N_ >> M_ >> K;
	SETTE gorg_(M_);
	std::string str_;
	long long num_;
	int setnum_;//asggadsgaesgsplg;slg;asgl
	for (size_t i = 0; i < K; ++i)
	{
		std::cin >> str_;
		if (str_ == "ADD")
		{
			std::cin >> num_ >> setnum_;
			gorg_.adds(setnum_, num_);
		}
		else if (str_ == "DELETE")
		{
			std::cin >> num_ >> setnum_;
			gorg_.deleters(setnum_, num_);
		}
		else if (str_ == "CLEAR")
		{
			std::cin >> setnum_;
			gorg_.clears(setnum_);
		}
		else if (str_ == "LISTSET")
		{
			/*fgksfkgjskfg
			dfgdfshdsfhds
			hdsfhdshdfs
			hdsfhdsfhdshds
			dsfhhdsfhdsfh
			sdfhdsfhdshdsf
			hdsghdsf
			hds
			h
			hdsdfhdsfhdshsdh*/
			std::cin >> setnum_;
			gorg_.listsets(setnum_);
		}
		else if (str_ == "LISTSETSOF")
		{
			std::cin >> num_;
			gorg_.listsetofs(num_);
		}
	}
}

