#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		NEAREST_SHARK,
		NEAREST_FISHFOOD,
		NEAREST_FULLFISH,
		HIGHEST_ENERGYFISH,
	};
	MessageWRU(GameObject *goValue, SEARCH_TYPE typeValue, float thresholdValue) : go(goValue), type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageWRU() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};

#endif
