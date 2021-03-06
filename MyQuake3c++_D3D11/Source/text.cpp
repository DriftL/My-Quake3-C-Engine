#include <windows.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text.h"

text::text()
{
}

text::text(const char *name)
{
	FILE *f;
	struct stat stbuffer;

	stat(name,&stbuffer);
	fopen_s(&f, name,"rb");
	size=stbuffer.st_size;
	data=new char[size];
	sl=0;
	fread(data,size,1,f);
	fclose(f);
}

void text::create(char *name)
{
	FILE *f;
	struct stat stbuffer;

	stat(name,&stbuffer);
	fopen_s(&f, name,"rb");
	if (f==NULL)
	{
		char *s=new char[256];
		sprintf_s(s, 256, "The resource file %s does not exist\n",name);
		MessageBox(NULL,s,"SR",MB_OK);
		delete s;
		exit(-1);
	}
	size=stbuffer.st_size;
	data=new char[size];
	sl=0;
	fread(data,size,1,f);
	fclose(f);
}

int legal(char c)
{
	int res;

	res=(c>32);
	return res;
}

char *text::getword()
{
	int p0,p1,i;
	char *res;

	p0=sl;
	while (!legal(data[p0]) && p0<(int)size) p0++;
	if (p0>(int)size) return NULL;
	p1=p0+1;
	while (legal(data[p1])) 
		p1++;
	res=new char[(1+p1-p0)];
	for (i=p0;i<p1;i++)
	{
		if ((data[i]<='z') && (data[i]>='a')) data[i]+=('A'-'a');
		res[i-p0]=data[i];
	}
	res[p1-p0]='\0';
	sl=p1;
	_strupr_s(res, 1+p1-p0);
	return res;
}

char *text::getcommaword()
{
	int p0,p1,i;
	char *res;

	p0=sl;
	while (data[p0]!='"')
		p0++;
	p0++;
	p1=p0+1;
	while (data[p1]!='"')
		p1++;
	res=new char[(1+p1-p0)];
	for (i=p0;i<p1;i++)
	{
		res[i-p0]=data[i];
	}
	res[p1-p0]='\0';
	sl=p1+1;
	return res;
}

int text::getint()
{
	int res;
	char *tmp;

	tmp=getword();
	res=atoi(tmp);
	delete tmp;
	return res;
}

double text::getfloat()
{
	double res;
	char *tmp;

	tmp=getword();
	res=atof(tmp);
	delete tmp;
	return res;
}

void text::goback()
{
	int p0,p1;

	p0=sl;
	while (!legal(data[p0]))
		p0--;
	p1=p0-1;
	while (legal(data[p1]))
		p1--;
	sl=p1;
}

int text::countchar(char c)
{
	int res;
	unsigned int i;

	res=0;
	for (i=0;i<size;i++)
	{
		if (data[i]==c)
			res++;
	}
	return res;
}

void text::reset()
{
	sl=0;
}

void text::destroy()
{
	if (data!=NULL) 
		delete data;
}

int text::countword(char *s)
{
	int res;
	unsigned int i;
	int final;
	unsigned int si;

	res=0;
	final=0;
	i=0;
	while (!final)
	{
		si=0;
		while (toupper(data[i])==toupper(s[si]))
		{
			i++;
			si++;
		}
		res+=(si==strlen(s));
		i+=si;
		i++;
		final=(i>=size);
	}
	return res;
}

int text::countwordfromhere(char *s)
{
	int res;
	unsigned int i;
	int final;
	unsigned int si;

	res=0;
	final=0;
	i=sl;
	while (!final)
	{
		si=0;
		while (toupper(data[i])==toupper(s[si]))
		{
			i++;
			si++;
		}
		res+=(si==strlen(s));
		i+=si;
		i++;
		final=(i>=size);
	}
	return res;
}

int text::eof()
{
	return (sl>size);
}

bool text::seek(char *token)
{
	char *dummy=getword();

	if(sl>size) 
		return false;
	while (strcmp(dummy,token) && (sl<size))
	{	
		delete dummy;
		dummy=getword();
	}
	delete dummy;
	if (sl<size) 
		return true;
	else
		return false;
}

text::~text()
{
	if (data!=NULL)
		delete data;
}

unsigned int text::getPos()
{
	return sl;
}

BOOL text::setPos(unsigned int posicion)
{
	if(posicion>=size) 
		return FALSE;
	sl=posicion;
	return TRUE;
}
