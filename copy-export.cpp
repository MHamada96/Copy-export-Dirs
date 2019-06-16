#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

// directory includes
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

void Copy(_Folder_ *curr, string path)
{

	int sz = 0;

	DIR *dir;
	struct dirent *entry;
	struct stat info;

	dir = opendir(path.c_str());
	if (dir == NULL)
	{
		//cout << "Invalid Path" << endl;
	}
	else
	{
		while (true)
		{
			entry = readdir(dir);
			if (entry == NULL)
				break;

			string nm = entry->d_name;
			if (nm[0] == '.' || nm[0] == '$')
				continue;

			// if this is a directory => keep exploring
			if (entry->d_type == S_IFDIR)
			{
				createFolder(curr, entry->d_name);
				Copy(curr->inFolders.back(), path + "\\" + entry->d_name);
			}

			// is this is file ==> get the size
			else
			{
				FILE *F;
				F = fopen((path + "\\" + entry->d_name).c_str(), "rb");
				fseek(F, 0, SEEK_END);
				sz += ftell(F);
				rewind(F);
				char *buffer = new char[sz];
				fread(buffer, 1, sz, F);
				fclose(F);

				createFile(curr, entry->d_name);
				curr->inFilse.back()->Data = vector<char>(buffer, buffer + sz);
			}
		}
	}

	closedir(dir);
}

void Export(_Folder_ *curr, string path)
{
	string com = "";
	com += "mkdir ";
	com += path;
	com += "\\";
	com += curr->name;


	system(com.c_str());
	string pcd = "cd ";
	pcd += path;
	pcd += "\\";
	pcd += curr->name;
	system(pcd.c_str());

	for (int i = 0; i < curr->inFilse.size(); i++)
	{
		string nm = curr->inFilse[i]->name;
		string comm = "";
		comm += path;
		comm += "\\";
		comm += curr->name;
		comm += "\\";
		comm += nm;

		FILE * pFile;
		pFile = fopen(comm.c_str(), "w");
		if (pFile != NULL)
		{
			int SZ = curr->inFilse[i]->Data.size();
			char *arr = new char[SZ];
			for (int j = 0; j < SZ; j++)
				arr[j] = curr->inFilse[i]->Data[j];
			fwrite(arr, 1, SZ, pFile);

			fclose(pFile);
		}
		//cout << "### " << comm << endl; Copy E:\ export
	}

	for (int i = 0; i < curr->inFolders.size(); i++)
	{
		if (curr->name.back() != '.')
			Export(curr->inFolders[i], path + "\\" + curr->name);
	}
}

int main()
{

	cout << myExplorFunction("E:\\");
	cin.get();
}
