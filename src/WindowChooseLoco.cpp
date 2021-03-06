/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco choice window>
*************************************************************/

#ifndef NANOCONTROLER
#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"

byte WindowChooseLoco::GetChoiceTextNumber() const
{
	byte slot = 0;

	while (EngineShed::LocoSlots[slot] != 255)
		slot++;

	return slot;
}

char *WindowChooseLoco::GetChoiceTextValue(byte indexValue, LcdScreen *apScreen) const
{
	return EngineShed::GetName(indexValue);
}

void WindowChooseLoco::Event(byte inEventType, LcdUi *inpLcd)
{
	unsigned int lastSelected = *this->pValue;

	WindowChoiceText::Event(inEventType, inpLcd);

	if ((this->state == STATE_NONE || this->state == STATE_INITIALIZE) && lastSelected != 255)
	{
		bool redo, redone = false;

		this->lockScreen = true;

		if (this->state == STATE_INITIALIZE)
			inEventType = EVENT_MORE;
		do {
			redo = false;
			switch (this->canSelect)
			{
			case AllLocos:
				return;
			case InRollingStock:
				if (!RollingStock::IsInStock(*this->pValue))
					redo = true;
				break;
			case NotInRollingStock:
				if (RollingStock::IsInStock(*this->pValue))
					redo = true;
				break;
			}
			if (redo)
			{
				redone = true;
				WindowChoiceText::Event(inEventType, inpLcd);
			}

		} while (redo && lastSelected != *this->pValue);

		this->lockScreen = false;
		if (redone)
		{
			this->state = STATE_INITIALIZE;
			WindowChoiceText::Event(EVENT_NONE, inpLcd);
		}
	}
}
#endif