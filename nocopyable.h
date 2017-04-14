#ifndef _COMMON_H_
#define _COMMON_H_

namespace http
{
	
class nocopyable 
{
	private:
		nocopyable(const nocopyable &other);
		nocopyable & operator=(const nocopyable &other);
	protected:
		nocopyable(){}
};

}



#endif