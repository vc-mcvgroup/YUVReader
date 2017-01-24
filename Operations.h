/*
 * Operations.h
 *
 *  Created on: Jan 24, 2017
 *      Author: augusto
 */

#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "TComPicYuv.h"

class Operations {

public:

	Void grayScale(TComPicYuv* pic);
	Operations();
	virtual ~Operations();
};

#endif /* OPERATIONS_H_ */
