/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - event.h
 * Definition of Events, classes that inherit Power
 * Events can take place at the end of a round in a
 * Battle. Both players can potentially be affected by
 * whatever happens
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "power.h"

class EventElephantStampede : public Power
{
    public:
        EventElephantStampede();
        EventElephantStampede(const EventElephantStampede & source);

        void apply_effect(Character & char1, Character & char2) const override;
};

class EventEarthquake : public Power
{
    public:
        EventEarthquake();
        EventEarthquake(const EventEarthquake & source);

        void apply_effect(Character & char1, Character & char2) const override;
};

#endif