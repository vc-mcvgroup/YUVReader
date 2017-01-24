/*
 * Operations.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: augusto
 */

#include "Operations.h"

Operations::Operations() {
	// TODO Auto-generated constructor stub

}

Operations::~Operations() {
	// TODO Auto-generated destructor stub
}

Void Operations::grayScale(TComPicYuv* pic)
{
	for(UInt component = 1; component<pic->getNumberValidComponents();component++)
	    {
	    	const ComponentID ch=ComponentID(component);
	    	Int size = pic->getStride(ch) * pic->getHeight(ch);
	    	printf("Size: %d\n", size);
	    	for(Int pi = 0; pi< size ;pi++)
	    	{
	    		pic->getAddr(ch)[pi] = 127;
	    	}

	    }

}
