#include "utils.h"

#include "folder.h"
#include "disk.h"
#include "engine.h"

void utf8toWStr(wstring & dest, const string & src){
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'�';
	for (size_t i = 0; i < src.size(); i++){
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
}

void wstrToUtf8(string & dest, const wstring & src){
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07));
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

string fileSizeToStr(long long sz)
{
	double dSz = sz;

	string sign[5] = {"B", "kiB", "MiB", "GiB", "TiB"};
	int i = 0;

	for(; i < 5; i++)
		if(dSz < 1024) break;
		else dSz /= 1024;

	char buf[32];
	sprintf(buf, "%.2f %s", dSz, sign[i].c_str());

	return buf;
}

FilePath getFilePath(File * f)
{
	FilePath p;

	Folder * parent = f->getFolder();
	p.path.push_back(f->getName());

	if(!parent) return p;
	p.disk = parent->getDisk();

	while(parent)
	{
		p.path.push_back(parent->getName());
		parent = parent->getParent();
	}

	p.path.pop_back();
	p.valid = true;

	return p;
}

File * getFileFromPath(FilePath p)
{
	if(!p.valid) return NULL;

	Folder * f = p.disk->getRootFolder();

	if(f)
	{
		while(f->getFolder(p.path.back()))
		{
			f = f->getFolder(p.path.back());
			p.path.pop_back();
		}

		return f->getFile(p.path.back());
	}

	return NULL;
}

File * findRandomFile(Folder * folder)
{
	vector<File*> files;
	folder->getAllFilesRecursively(files);

	if(files.size() == 0) return NULL;

	int randFile = Engine::random(0, (int)files.size() - 1);
	return files[randFile];
}

Folder * findRandomFolder(Folder * folder)
{
	vector<pair<Folder*, int>> folders;
	folder->getAllFoldersRecursively(folders);

	if(folders.size() == 0) return NULL;

	int randFolder = Engine::random(0, (int)folders.size() - 1);
	return folders[randFolder].first;
}
